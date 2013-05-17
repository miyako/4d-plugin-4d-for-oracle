#include "oracle_low_level.h"
#include "oracle_low_level_bind_i.h"
#include "oracle_low_level_bind_o.h"
#include "oracle_low_level_fetch.h"
#include "oracle_control.h"
#include "oracle_preferences.h"

#pragma mark -
#pragma mark cursor management
#pragma mark -

std::map<uint32_t, ORACLE_SQL_CURSOR*> _cursors;

ORACLE_SQL_CURSOR *_cursorCreate(unsigned int *index, uint32_t sessionId, OCIStmt *stmtp, OCIError* errhp)
{
	ORACLE_SQL_CURSOR *obj = NULL;
	
	unsigned int i = 1000;
	
	while ((_cursors.find(i) != _cursors.end()) && (i < 10000))
	{
		i++;
	}
	
	if(i != 10000)
	{
		obj = new(ORACLE_SQL_CURSOR);

		obj->isActive = false;
		obj->sessionId = sessionId;
		obj->stmtp = stmtp;
		obj->errhp = errhp;
		obj->itemCount = 0;	
		obj->sql.clear();
		obj->sql_type = 0;
		obj->rowCount = 0;
		obj->rowsFetched = 0;
		obj->isEndOfSelection = false;
		obj->isEndOfSelectionInFirstCall = false;

		_cursors.insert(std::map<uint32_t, ORACLE_SQL_CURSOR*>::value_type(i, obj));	
		*index = i;
		
	}else{
		*index = 0;
		_errorInfoSet(0, 0, 0, 0, true, PA_GetCurrentProcessNumber(), PLUGIN_ERROR_NOT_ENOUGH_MEMORY);
	}
	
	return obj;	
}

void _cursorDelete(unsigned int i)
{	
	std::map<uint32_t, ORACLE_SQL_CURSOR*>::iterator pos = _cursors.find(i);
	
	if(pos != _cursors.end()) 
	{
		
		ORACLE_SQL_CURSOR *obj = pos->second;
		
		OCIHandleFree(obj->stmtp, OCI_HTYPE_STMT);
		OCIHandleFree(obj->errhp, OCI_HTYPE_ERROR);
		
		obj->stmtp = 0;
		obj->errhp = 0;
		
		delete obj;
		
		_cursors.erase(pos);
	}
}

ORACLE_SQL_CURSOR *_cursorGet(unsigned int i)
{	
	ORACLE_SQL_CURSOR *obj = NULL;
	
	std::map<uint32_t, ORACLE_SQL_CURSOR*>::iterator pos = _cursors.find(i);
	
	if(pos != _cursors.end()) 
	{
		obj = pos->second;
	}else{
		_errorInfoSet(0, 0, 0, i, true, PA_GetCurrentProcessNumber(), PLUGIN_ERROR_CURSOR_INVALID);
	}
	
	return obj;
}

void _cursorResize(sessionInfo *session, ORACLE_SQL_CURSOR *cursor, size_t size)
{	
	/*
	 because of the way OD_Set_SQL_in_cursor works,
	 [substitutions, isByName, names] can be larger than other members at this point
	 */
	
	size_t oldSize = cursor->substitutions.size();
	
	if(size < oldSize)
	{//we need to release own created objects 
		for(unsigned int i = size; i < oldSize; ++i)
		{
			PA_YieldAbsolute();
			if(cursor->isObjectValid.at(i))
			{
				for(unsigned j = 0; j < cursor->isObjectElementValid.at(i).size(); ++j)
				{
					if(cursor->isObjectElementValid.at(i).at(j))
						OCIObjectFree(session->envhp, session->errhp, cursor->arrayOfTexts.at(i).at(j), OCI_OBJECTFREE_FORCE);	
				}
				
			}
			
			if(cursor->isRawObjectValid.at(i))			
				PA_UnlockHandle(cursor->blobs.at(i));

//			if(cursor->isLocatorValid.at(i))			
//				OCIDescriptorFree(cursor->locators.at(i), OCI_DTYPE_LOB);
			
		}
				
	}

	cursor->substitutions.resize(size);
	cursor->pointers.resize(size);
	cursor->pointers.resize(size);
	cursor->isTowardsSQL.resize(size);	
//	cursor->isByName.resize(size);	
	cursor->indicators.resize(size);	
	cursor->names.resize(size);	
	cursor->binds.resize(size);
	cursor->defines.resize(size);		
	cursor->dates.resize(size);	
	cursor->numbers.resize(size);
	cursor->texts.resize(size);
	cursor->blobs.resize(size);	
//	cursor->blobLengths.resize(size);
	cursor->bytes.resize(size);	
//	cursor->locators.resize(size);	
	cursor->isObjectElementValid.resize(size);	
	cursor->isObjectValid.resize(size);
	cursor->isRawObjectValid.resize(size);
//	cursor->isLocatorValid.resize(size);
	cursor->arrayOfDates.resize(size);	
	cursor->arrayOfNumbers.resize(size);	
	cursor->arrayOfTexts.resize(size);
	cursor->indicatorLists.resize(size);	
	cursor->lengths.resize(size);
}

void _cursorClearBind(sessionInfo *session, ORACLE_SQL_CURSOR *cursor)
{
	cursor->isActive = false;
	//keep sessionId, stmtp, bindp, errhp
	cursor->itemCount = 0;	
	cursor->sql.clear();
	cursor->sql_type = 0;
	cursor->rowCount = 0;
	cursor->rowsFetched = 0;
	cursor->isEndOfSelection = false;
	cursor->isEndOfSelectionInFirstCall = false;
	
	_cursorResize(session, cursor, 0);		
}

ORACLE_SQL_CURSOR * _cursorGetAndCheckActive(uint32_t cursorId)
{
	ORACLE_SQL_CURSOR * cursor = _cursorGet(cursorId);
	
	if(cursor)
	{		
		if(!cursor->isActive)
		{
			_errorInfoSet(0, cursor->errhp, cursor->sessionId, cursorId, true, PA_GetCurrentProcessNumber(), PLUGIN_ERROR_CURSOR_NOT_ACTIVE);
			cursor = 0;
		}
		
	}
	
	return cursor;		
	
}

ORACLE_SQL_CURSOR * _cursorGetAndCheckInactive(uint32_t cursorId)
{
	ORACLE_SQL_CURSOR * cursor = _cursorGet(cursorId);
	
	if(cursor)
	{		
		if(cursor->isActive)
		{
			_errorInfoSet(0, cursor->errhp, cursor->sessionId, cursorId, true, PA_GetCurrentProcessNumber(), PLUGIN_ERROR_CURSOR_ALREADY_ACTIVE);
			cursor = 0;
		}
		
	}
	
	return cursor;		
	
}

ORACLE_SQL_CURSOR * _cursorGetAndParse(uint32_t cursorId)
{
	ORACLE_SQL_CURSOR * cursor = _cursorGet(cursorId);
	
	if(cursor)
	{
		sessionInfo *session = _sessionGet(cursor->sessionId);
		
		if(session)
		{
			if(!cursor->isActive)
			{
				
				sword err = 0;
				
				err = OCIStmtExecute(session->svchp, cursor->stmtp, cursor->errhp, 1, 0, 0, 0, OCI_PARSE_ONLY);
				
				if(err)
				{
					_errorInfoSet(session->envhp, cursor->errhp, cursor->sessionId, cursorId, false, PA_GetCurrentProcessNumber(), 0);
					cursor = 0;
				}
				
			}	
			
		}
		
	}
	
	return cursor;
}

ORACLE_SQL_CURSOR * _cursorGetAndParseAndCountColumns(uint32_t cursorId, uint32_t pos)
{
	ORACLE_SQL_CURSOR * cursor = _cursorGetAndParse(cursorId);
	
	if(cursor)
	{
		ub4 columnCount = 0;
		sword err = 0;
		
		err = OCIAttrGet(cursor->stmtp, OCI_HTYPE_STMT, &columnCount, 0, OCI_ATTR_PARAM_COUNT, cursor->errhp);
		
		if(err)
		{
			
			_errorInfoSet(0, cursor->errhp, cursor->sessionId, cursorId, false, PA_GetCurrentProcessNumber(), 0);
			cursor = 0;
			
		}else{
			
			if(columnCount > pos)
			{
				_errorInfoSet(0, 0, cursor->sessionId, cursorId, true, PA_GetCurrentProcessNumber(), PLUGIN_ERROR_COLUMN_INVALID);
				cursor = 0;
			}
			
		}
		
	}
	
	return cursor;
}

// --------------------------------- OD: Low Level --------------------------------

#pragma mark -
#pragma mark commands
#pragma mark -

void OD_Load_rows_cursor(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	uint32_t cursorId = Param1.getIntValue();
	
	sword err = 0;
	
	ORACLE_SQL_CURSOR *cursor = _cursorGetAndCheckActive(cursorId);
	
	if(cursor)
	{
		
		sessionInfo *session = _sessionGet(cursor->sessionId);
	
		if(session)
		{

			PA_FieldKind kind;
			short stringLength;
			char indexed;
			long attributes;
			
			if(!cursor->isEndOfSelection)
			{
				err = _checkNumberOfRecordsToFetch(cursor);
			
				if(err != -1)
				{
					
					for(unsigned int i = 0; i < cursor->substitutions.size(); ++i)
					{
						
						if(!cursor->isTowardsSQL.at(i))
						{
							switch (PA_GetVariableKind(cursor->substitutions.at(i)))
							{
								case eVK_ArrayUnicode:
									err = _fetchDataIntoTextArray(cursor, cursor->substitutions.at(i), i, session);
									break;
									
								case eVK_ArrayLongint:
									err = _fetchDataIntoLongintArray(cursor, cursor->substitutions.at(i), i);
									break;
									
								case eVK_ArrayBoolean:
									err = _fetchDataIntoBooleanArray(cursor, cursor->substitutions.at(i), i);
									break;
									
								case eVK_ArrayReal:
									err = _fetchDataIntoRealArray(cursor, cursor->substitutions.at(i), i);
									break;
									
								case eVK_ArrayDate:
									err = _fetchDataIntoDateArray(cursor, cursor->substitutions.at(i), i);
									break;
									
								case eVK_Unistring:
									err = _fetchDataIntoTextVariable(cursor, cursor->substitutions.at(i), i, session);
									break;							
									
								case eVK_Longint:
									err = _fetchDataIntoLongintVariable(cursor, cursor->substitutions.at(i), i);
									break;		
									
								case eVK_Boolean:
									err = _fetchDataIntoBooleanVariable(cursor, cursor->substitutions.at(i), i);
									break;		
									
								case eVK_Real:
									err = _fetchDataIntoRealVariable(cursor, cursor->substitutions.at(i), i);
									break;	
									
								case eVK_Date:
									err = _fetchDataIntoDateVariable(cursor, cursor->substitutions.at(i), i);
									break;	
									
								case eVK_Time:
									err = _fetchDataIntoTimeVariable(cursor, cursor->substitutions.at(i), i);
									break;	

								case eVK_Blob:
									err = _fetchDataIntoBlobVariable(cursor, cursor->substitutions.at(i), i, session);
									break;										

								case eVK_PointerToField:							
									PA_GetFieldProperties(cursor->substitutions.at(i).uValue.fTableFieldDefinition.fTableNumber, 
														  cursor->substitutions.at(i).uValue.fTableFieldDefinition.fFieldNumber, 
														  &kind, 
														  &stringLength, 
														  &indexed, 
														  &attributes);
									
									if(eER_NoErr == PA_GetLastError())
									{
										unsigned int recordsInSelection = PA_RecordsInSelection(cursor->substitutions.at(i).uValue.fTableFieldDefinition.fTableNumber);
										
										switch (recordsInSelection)
										{
											case 0:
												break;
												
											case 1:
												switch(kind)
											{
												case eFK_AlphaField:
													err = _fetchDataIntoAlphaField(cursor, cursor->substitutions.at(i), i, session);
													break;
													
												case eFK_TextField:
													err = _fetchDataIntoTextField(cursor, cursor->substitutions.at(i), i, session);
													break;		
													
												case eFK_LongintField:
													err = _fetchDataIntoLongintField(cursor, cursor->substitutions.at(i), i);
													break;	
													
												case eFK_IntegerField:
													err = _fetchDataIntoIntegerField(cursor, cursor->substitutions.at(i), i);
													break;	
													
												case eFK_BooleanField:
													err = _fetchDataIntoBooleanField(cursor, cursor->substitutions.at(i), i);
													break;			
													
												case eFK_RealField:
													err = _fetchDataIntoRealField(cursor, cursor->substitutions.at(i), i);
													break;		
													
												case eFK_DateField:
													err = _fetchDataIntoDateField(cursor, cursor->substitutions.at(i), i);
													break;		
													
												case eFK_TimeField:
													err = _fetchDataIntoTimeField(cursor, cursor->substitutions.at(i), i);
													break;	
													
												case eFK_BlobField:
													err = _fetchDataIntoBlobField(cursor, cursor->substitutions.at(i), i, session);
													break;	
													
												default:
													break;
													
											}
												
										}
										
									}
									break;
									
								default:
									break;							
							}
							
						}
						
					}
					
				}
			
				if(cursor->isEndOfSelectionInFirstCall)
					cursor->isEndOfSelection = true;
				
			}
			
			returnValue.setIntValue(err);

		}
			
	}
	
	returnValue.setReturn(pResult);
}

void OD_EXECUTE_CURSOR(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT Param2;	
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	
	uint32_t cursorId = Param1.getIntValue();
	uint32_t itemCount = Param2.getIntValue();
	
	if(!itemCount)
		itemCount = 20;//default value in old plugin

	ORACLE_SQL_CURSOR *cursor = _cursorGetAndCheckInactive(cursorId);
	
	if(cursor)
	{		
		sessionInfo *session = _sessionGet(cursor->sessionId);
		
		if(session)
		{
			sword status = 0;
			
			PA_FieldKind kind;
			short stringLength;
			char indexed;
			long attributes;
			
			cursor->itemCount = itemCount;
			
			for(unsigned int i = 0; i < cursor->substitutions.size(); ++i)
			{				
				if(cursor->isTowardsSQL.at(i))
				{
					switch (PA_GetVariableKind(cursor->substitutions.at(i)))
					{
						case eVK_ArrayUnicode:							
							_bindTextArrayTowardsSQL(cursor, cursor->substitutions.at(i), i);
							break;	
							
						case eVK_ArrayLongint:							
							_bindLongintArrayTowardsSQL(cursor, cursor->substitutions.at(i), i);
							break;
							
						case eVK_ArrayBoolean:							
							_bindBooleanArrayTowardsSQL(cursor, cursor->substitutions.at(i), i);
							break;	
							
						case eVK_ArrayReal:							
							_bindRealArrayTowardsSQL(cursor, cursor->substitutions.at(i), i);
							break;	
							
						case eVK_ArrayDate:							
							_bindDateArrayTowardsSQL(cursor, cursor->substitutions.at(i), i);
							break;		
							
						case eVK_Unistring:
							_bindTextVariableTowardsSQL(cursor, cursor->substitutions.at(i), i);
							break;
							
						case eVK_Longint:							
							_bindLongintVariableTowardsSQL(cursor, cursor->substitutions.at(i), i);
							break;	
							
						case eVK_Boolean:
							_bindBooleanVariableTowardsSQL(cursor, cursor->substitutions.at(i), i);
							break;	
	
						case eVK_Real:							
							_bindRealVariableTowardsSQL(cursor, cursor->substitutions.at(i), i);
							break;	

						case eVK_Date:							
							_bindDateVariableTowardsSQL(cursor, cursor->substitutions.at(i), i);
							break;
							
						case eVK_Time:							
							_bindTimeVariableTowardsSQL(cursor, cursor->substitutions.at(i), i);
							break;
							
						case eVK_Blob:							
							_bindBlobVariableTowardsSQL(cursor, cursor->substitutions.at(i), i);
							break;
							
						case eVK_PointerToField:							
							PA_GetFieldProperties(cursor->substitutions.at(i).uValue.fTableFieldDefinition.fTableNumber, 
												  cursor->substitutions.at(i).uValue.fTableFieldDefinition.fFieldNumber, 
												  &kind, 
												  &stringLength, 
												  &indexed, 
												  &attributes);
							
							if(eER_NoErr == PA_GetLastError())
							{
								unsigned int recordsInSelection = PA_RecordsInSelection(cursor->substitutions.at(i).uValue.fTableFieldDefinition.fTableNumber);
	
								switch (recordsInSelection)
								{
									case 0:
										break;
										
									case 1:
										switch(kind)
										{
											case eFK_AlphaField:
												_bindAlphaFieldTowardsSQL(cursor, cursor->substitutions.at(i), i);
												break;
												
											case eFK_TextField:
												_bindTextFieldTowardsSQL(cursor, cursor->substitutions.at(i), i);
												break;
												
											case eFK_LongintField:
												_bindLongintFieldTowardsSQL(cursor, cursor->substitutions.at(i), i);
												break;	

											case eFK_IntegerField:
												_bindIntegerFieldTowardsSQL(cursor, cursor->substitutions.at(i), i);
												break;	
												
											case eFK_BooleanField:
												_bindBooleanFieldTowardsSQL(cursor, cursor->substitutions.at(i), i);
												break;	
												
											case eFK_RealField:
												_bindRealFieldTowardsSQL(cursor, cursor->substitutions.at(i), i);
												break;	
												
											case eFK_DateField:
												_bindDateFieldTowardsSQL(cursor, cursor->substitutions.at(i), i);
												break;		
												
											case eFK_TimeField:
												_bindTimeFieldTowardsSQL(cursor, cursor->substitutions.at(i), i);
												break;	
												
											case eFK_BlobField:
												_bindBlobFieldTowardsSQL(cursor, cursor->substitutions.at(i), i);
												break;	
												
											default:
												break;
										}
										break;
									
									default:
										break;
								}
							}							
							break;	
							
						default:
							break;
					}
					   
				}else{
				   
				   switch (PA_GetVariableKind(cursor->substitutions.at(i)))
				   {
						   
					   case eVK_ArrayUnicode:
						   _bindTextArrayTowards4D(cursor, i);
						   break;
						
					   case eVK_ArrayLongint:
						   _bindLongintArrayTowards4D(cursor, i);
						   break;

					   case eVK_ArrayBoolean:
						   _bindBooleanArrayTowards4D(cursor, i);
						   break;
						   
					   case eVK_ArrayReal:
						   _bindRealArrayTowards4D(cursor, i);
						   break;		
						   
					   case eVK_ArrayDate:
						   _bindDateArrayTowards4D(cursor, i);
						   break;
						   
					   case eVK_Unistring:
						   _bindTextVariableTowards4D(cursor, i);
						   break;		
						   
					   case eVK_Longint:
						   _bindLongintVariableTowards4D(cursor, i);
						   break;		
						   
					   case eVK_Boolean:
						   _bindBooleanVariableTowards4D(cursor, i);
						   break;	
						   
					   case eVK_Real:
						   _bindRealVariableTowards4D(cursor, i);
						   break;	

					   case eVK_Date:
						   _bindDateVariableTowards4D(cursor, i);
						   break;
						   
					   case eVK_Time:
						   _bindTimeVariableTowards4D(cursor, i);
						   break;	
						   
					   case eVK_Blob:							
						   _bindBlobVariableTowards4D(cursor, i);
						   break;
   
					   case eVK_PointerToField:							
						   PA_GetFieldProperties(cursor->substitutions.at(i).uValue.fTableFieldDefinition.fTableNumber, 
												 cursor->substitutions.at(i).uValue.fTableFieldDefinition.fFieldNumber, 
												 &kind, 
												 &stringLength, 
												 &indexed, 
												 &attributes);
						   
						   if(eER_NoErr == PA_GetLastError())
						   {
							   unsigned int recordsInSelection = PA_RecordsInSelection(cursor->substitutions.at(i).uValue.fTableFieldDefinition.fTableNumber);
							   
							   switch (recordsInSelection)
							   {
								   case 0:
									   break;
									   
								   case 1:
									   switch(kind)
								   {
									   case eFK_AlphaField:
										   _bindAlphaFieldTowards4D(cursor, i);
										   break;
										   
									   case eFK_TextField:
										   _bindTextFieldTowards4D(cursor, i);
										   break;	
										   
									   case eFK_LongintField:
										   _bindLongintFieldTowards4D(cursor, i);
										   break;			
										   
									   case eFK_IntegerField:
										   _bindIntegerFieldTowards4D(cursor, i);
										   break;	
										   
									   case eFK_BooleanField:
										   _bindBooleanFieldTowards4D(cursor, i);
										   break;	
										   
									   case eFK_RealField:
										   _bindRealFieldTowards4D(cursor, i);
										   break;	

									   case eFK_DateField:
										   _bindDateFieldTowards4D(cursor, i);
										   break;		
										   
									   case eFK_TimeField:
										   _bindTimeFieldTowards4D(cursor, i);
										   break;
										   
									   default:
										   break;   
								   }
								   
								   default:
									   break;   
							   }
							   
						   }
						   break;						   
						   
					   default:
						   break;
				   }
								   
				   //todo
				   //If a statement with RETURNING clause is used, a call to OCIBindDynamic() must follow this call.
			   }

			}

			status = OCIStmtExecute(session->svchp, cursor->stmtp, cursor->errhp, cursor->itemCount, 0, 0, 0, OCI_DEFAULT);
			//http://docs.oracle.com/cd/B10500_01/appdev.920/a96584/oci16ms2.htm
			
			switch (status)
			{
				case OCI_NO_DATA:
				case OCI_SUCCESS:
				case OCI_SUCCESS_WITH_INFO:	
					switch (cursor->sql_type)
					{
					case OCI_STMT_SELECT:
						//we only want to increment the counter after a call to load cursor
						break;
					default:
						OCIAttrGet(cursor->stmtp, OCI_HTYPE_STMT, &cursor->rowCount, 0, OCI_ATTR_ROW_COUNT, cursor->errhp);
						break;
					}
					cursor->isActive = true;
					cursor->isEndOfSelectionInFirstCall = (status == OCI_NO_DATA);
					break;
					
				default:
					_errorInfoSet(session->envhp, cursor->errhp, cursor->sessionId, cursorId, false, PA_GetCurrentProcessNumber(), 0);					
					break;
			}
			//clear any pointer blocks created using PA_ClearVariable
			for(unsigned int i = 0; i < cursor->substitutions.size(); ++i)
			{				
				if(cursor->isTowardsSQL.at(i))
				{
					if(PA_GetVariableKind(cursor->substitutions.at(i)) != eVK_PointerToField) 
						PA_ClearVariable(&cursor->substitutions.at(i));
				}
			}
		
		}
					
	}
	
}

#pragma mark -

void OD_BIND_TOWARDS_SQL(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT Param2;
	C_POINTER Param3;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
		
	size_t cursorId = Param1.getIntValue();
	unsigned int pos = Param2.getIntValue();
	
	ORACLE_SQL_CURSOR *cursor = _cursorGet(cursorId);
	
	if(cursor)
	{
		sessionInfo *session = _sessionGet(cursor->sessionId);
		
		if(session)
		{
			cursor->isActive  = false;
			
			unsigned int i = pos - 1;
			
			if(pos > cursor->substitutions.size())
			{
				_cursorResize(session, cursor, pos);
			}
			
			Param3.getVariable(&cursor->substitutions.at(i));
			cursor->isTowardsSQL.at(i) = true;
			
		}
		
	}
	
}

void OD_BIND_TOWARDS_4D(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT Param2;
	C_POINTER Param3;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
	
	size_t cursorId = Param1.getIntValue();
	unsigned int pos = Param2.getIntValue();
	
	ORACLE_SQL_CURSOR *cursor = _cursorGet(cursorId);
	
	if(cursor)
	{
		sessionInfo *session = _sessionGet(cursor->sessionId);
		
		if(session)
		{		
			cursor->isActive  = false;
			
			unsigned int i = pos - 1;
			
			if(pos > cursor->substitutions.size())
			{
				_cursorResize(session, cursor, pos);
			}
				
			Param3.getPointerBlock(&cursor->pointers.at(i));
			Param3.getVariable(&cursor->substitutions.at(i));
			cursor->isTowardsSQL.at(i) = false;
		
		}
		
	}		
	
}

void OD_GET_COLUMN_ATTRIBUTES(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT Param2;
	C_LONGINT Param3;
	C_LONGINT Param4;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	
	uint32_t cursorId = Param1.getIntValue();
	uint32_t pos = Param2.getIntValue();
	
	ORACLE_SQL_CURSOR *cursor = _cursorGetAndParseAndCountColumns(cursorId, pos);
	
	OCIParam *param = 0;
	
	sword err = 0;
	
	if(cursor)
	{
		err = OCIParamGet(cursor->stmtp, OCI_HTYPE_STMT, cursor->errhp, (dvoid **)&param, pos);
		//http://docs.oracle.com/cd/A97630_01/appdev.920/a96584/oci15r26.htm
		
		if(!err)
		{
			ub2 dataType = 0;
			ub2 dataSize = 0;
			
			err = OCIAttrGet(param, OCI_DTYPE_PARAM, &dataType, 0, OCI_ATTR_DATA_TYPE, cursor->errhp);
			err = err|OCIAttrGet(param, OCI_DTYPE_PARAM, &dataSize, 0, OCI_ATTR_DATA_SIZE, cursor->errhp);
			//http://docs.oracle.com/cd/A97630_01/appdev.920/a96584/oci15r20.htm
			
			Param3.setIntValue(dataType);
			Param4.setIntValue(dataSize);	
			
			OCIDescriptorFree(param, OCI_DTYPE_PARAM);
			//http://docs.oracle.com/cd/A97630_01/appdev.920/a96584/oci15r23.htm
			
		}
		
		if(err)
			_errorInfoSet(0, cursor->errhp, cursor->sessionId, cursorId, false, PA_GetCurrentProcessNumber(), 0);	
		
	}
	
	Param3.toParamAtIndex(pParams, 3);
	Param4.toParamAtIndex(pParams, 4);	
}

void OD_Get_column_title(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT Param2;
	C_TEXT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	
	uint32_t cursorId = Param1.getIntValue();
	uint32_t pos = Param2.getIntValue();
	
	ORACLE_SQL_CURSOR *cursor = _cursorGetAndParseAndCountColumns(cursorId, pos);
	
	OCIParam *param = 0;
	
	sword err = 0;
	
	if(cursor)
	{
		err = OCIParamGet(cursor->stmtp, OCI_HTYPE_STMT, cursor->errhp, (dvoid **)&param, pos);
		//http://docs.oracle.com/cd/A97630_01/appdev.920/a96584/oci15r26.htm
		
		if(!err)
		{
			text *col_name;
			ub4 col_name_len;
			
			err = OCIAttrGet(param, OCI_DTYPE_PARAM, &col_name, &col_name_len, OCI_ATTR_NAME, cursor->errhp);
			//http://docs.oracle.com/cd/A97630_01/appdev.920/a96584/oci15r20.htm			
			
			if(!err)
			{
				CUTF16String u((const PA_Unichar *)col_name, col_name_len/sizeof(PA_Unichar));
				returnValue.setUTF16String(&u);
			}
			
			OCIDescriptorFree(param, OCI_DTYPE_PARAM);
			//http://docs.oracle.com/cd/A97630_01/appdev.920/a96584/oci15r23.htm
			
		}

		if(err)
			_errorInfoSet(0, cursor->errhp, cursor->sessionId, cursorId, false, PA_GetCurrentProcessNumber(), 0);	

	}
			
	returnValue.setReturn(pResult);
}

void OD_Number_rows_processed(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	returnValue.setIntValue(-1);
	
	uint32_t cursorId = Param1.getIntValue();
	ORACLE_SQL_CURSOR *cursor = _cursorGetAndParse(cursorId);
	
	if(cursor)
	{
		if(cursor->isActive)
		{
			returnValue.setIntValue(cursor->rowCount);
		}
		
	}
	
	returnValue.setReturn(pResult);
}

void OD_Number_of_columns(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	returnValue.setIntValue(-1);
	
	uint32_t cursorId = Param1.getIntValue();
	ORACLE_SQL_CURSOR *cursor = _cursorGetAndParse(cursorId);
	
	if(cursor)
	{
		ub4 columnCount;
		
		if(!OCIAttrGet(cursor->stmtp, OCI_HTYPE_STMT, &columnCount, 0, OCI_ATTR_PARAM_COUNT, cursor->errhp))
		{
			returnValue.setIntValue(columnCount);
		}else{
			_errorInfoSet(0, cursor->errhp, cursor->sessionId, cursorId, false, PA_GetCurrentProcessNumber(), 0);	
		}
		
	}
	
	returnValue.setReturn(pResult);
}

void OD_Cursor_state(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	uint32_t cursorId = Param1.getIntValue();
	ORACLE_SQL_CURSOR *cursor = _cursorGet(cursorId);
	
	if(cursor)
	{
		
		if(cursor->isActive)
		{
			returnValue.setIntValue(2);
		}else{
			returnValue.setIntValue(1);
		}
		
	}
	
	returnValue.setReturn(pResult);
}

void OD_Create_cursor(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	uint32_t sessionId = Param1.getIntValue();
	sessionInfo *session = _sessionGet(sessionId);
	
	if(session)
	{
		unsigned int i = 0;
		
		OCIStmt		*stmtp = 0;	
		OCIError	*errhp = 0;
		
		OCIEnv *envhp = session->envhp;
		
		OCIHandleAlloc((dvoid *)envhp, (dvoid **)&stmtp, OCI_HTYPE_STMT,	0, 0);	
		OCIHandleAlloc((dvoid *)envhp, (dvoid **)&errhp, OCI_HTYPE_ERROR,	0, 0);
		
		_cursorCreate(&i, sessionId, stmtp, errhp);
		returnValue.setIntValue(i);	
		
		if(!i)
		{
			OCIHandleFree(stmtp, OCI_HTYPE_STMT);
			OCIHandleFree(errhp, OCI_HTYPE_ERROR);
			
			returnValue.setIntValue(-1);	
		}
		
	}
	
	returnValue.setReturn(pResult);	
}

void OD_DROP_CURSOR(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	
	Param1.fromParamAtIndex(pParams, 1);
	uint32_t cursorId = Param1.getIntValue();
	ORACLE_SQL_CURSOR *cursor = _cursorGet(cursorId);
	
	if(cursor)
	{
		sessionInfo *session = _sessionGet(cursor->sessionId);
		
		if(session)
		{
			_cursorClearBind(session, cursor);
			_cursorDelete(cursorId);	
			
		}
		
	}
	
}

void OD_Set_SQL_in_cursor(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_TEXT Param2;
	C_LONGINT Param3;	
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
	
	uint32_t cursorId = Param1.getIntValue();
	
	sword err = 0;
	
	ORACLE_SQL_CURSOR *cursor = _cursorGetAndCheckInactive(cursorId);
	
	if(cursor)
	{
		sessionInfo *session = _sessionGet(cursor->sessionId);
		
		if(session)
		{
			_cursorClearBind(session, cursor);
			
			//convert 4D bindings to oracle bindings
			CUTF8String sql, substr;
			Param2.copyUTF8String(&sql);
			
			size_t start, end, pos; 
			end = 0;
			pos = 0;
			
			C_TEXT variableName;
			PA_Variable variable;
			
			CUTF8String parsedSql;
			
			ORACLE_SQL_SUBSTITUTION_LIST substitutions;
			//ORACLE_SQL_BIND_TYPE_LIST isByName;
			ORACLE_SQL_BIND_NAME_LIST names;
			
			C_TEXT temp;
			CUTF16String u16;
			
			for(start = sql.find((const uint8_t *)"<<"); start != CUTF8String::npos; start = sql.find((const uint8_t *)"<<", end))
			{
				end = sql.find((const uint8_t *)">>", start);
				if(end != CUTF8String::npos)
				{
					start += 2;//length of "<<"
					substr = sql.substr(start, end-start);
					
					variableName.setUTF8String(&substr);
					variable = PA_GetVariable((PA_Unichar *)variableName.getUTF16StringPtr());
					
					parsedSql += sql.substr(pos, start - pos - 2);
					parsedSql += (const uint8_t *)":";
					
					if(substr.length()){
						
						parsedSql += substr;
						//isByName.push_back(true);
						temp.setUTF8String(&substr);
						temp.copyUTF16String(&u16);
						
					}else{
						
						char buf[10];
						size_t len;
						
						len = sprintf(buf,"%d", (int)substitutions.size() + 1);
						parsedSql += CUTF8String((const uint8_t *)buf, len);
						//isByName.push_back(false);
						u16.clear();
						
					}
					
					pos = end + 2;//length of ">>"
					
					names.push_back(u16);
					substitutions.push_back(variable);	
					
				}
				
			}
			
			//remaining text
			parsedSql += sql.substr(pos);			

			temp.setUTF8String(&parsedSql);
			temp.copyUTF16String(&cursor->sql);
			
			cursor->substitutions = substitutions;
//			cursor->isByName = isByName;
			cursor->names = names;
			
			size_t count = substitutions.size();
			
			_cursorResize(session, cursor, count);

			unsigned int sql_type = 0;
			
			ub4 language = OCI_NTV_SYNTAX;
			
			switch (Param3.getIntValue())
			{
				case 2:
					language = OCI_V7_SYNTAX;
					break;

				case 3:
					language = OCI_V8_SYNTAX;
					break;					
			}
			
			err = OCIStmtPrepare(cursor->stmtp, cursor->errhp, 
						   (CONST text *)cursor->sql.c_str(), 
						   (ub4)cursor->sql.length() * sizeof(PA_Unichar),								 
						   language,
						   OCI_DEFAULT);
			
			//in characters or in number of bytes, depending on the encoding
			//http://docs.oracle.com/cd/B10500_01/appdev.920/a96584/oci16ms6.htm
			
			if(!err)
			{
				err = OCIAttrGet(cursor->stmtp, OCI_HTYPE_STMT, (dvoid *)&sql_type, 0, OCI_ATTR_STMT_TYPE, cursor->errhp);
				//http://docs.oracle.com/cd/B10500_01/appdev.920/a96584/oci15r20.htm#443771
				
				if(!err)
				{
					cursor->sql_type = sql_type;
					//http://docs.oracle.com/cd/E14072_01/appdev.112/e10646/oci04sql.htm#CIHEHCEJ	
					
					switch (sql_type)
					{
						case OCI_STMT_SELECT:
							cursor->isTowardsSQL.assign(count, false);//SELECT is towards 4D
							break;					
						default:
							cursor->isTowardsSQL.assign(count, true);//INSERT is towards SQL
							break;
					}
					
					returnValue.setIntValue(1);

				}

			}
			
			if(err)
			{
				_errorInfoSet(0, cursor->errhp, cursor->sessionId, cursorId, false, PA_GetCurrentProcessNumber(), 0);	
				returnValue.setIntValue(err);
			}
			
		}

	}
		
	returnValue.setReturn(pResult);	
	
}