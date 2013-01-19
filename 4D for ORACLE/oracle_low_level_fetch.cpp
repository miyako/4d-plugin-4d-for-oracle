#include "oracle_low_level_fetch.h"
#include "oracle_preferences.h"

/*
sb4 _read_lob(dvoid *ctxp, CONST dvoid*bufxp, ub4 len, ub1 piece)
{
	std::vector<uint8_t> *dataBuf = (std::vector<uint8_t> *)ctxp;
	size_t pos = dataBuf->size();
	
	switch(piece)
	{
		case OCI_LAST_PIECE:
		case OCI_NEXT_PIECE:
		case OCI_FIRST_PIECE:
			dataBuf->resize(pos + len);
			memcpy(&dataBuf->at(pos), bufxp, len);
			break;
			
		default:
			return OCI_ERROR;
	}
	//no possibility of OCI_ONE_PIECE
	//http://docs.oracle.com/cd/A97630_01/appdev.920/a96584/oci16m40.htm
	
	return OCI_CONTINUE;	
}
*/

sword _checkNumberOfRecordsToFetch(ORACLE_SQL_CURSOR *cursor)
{
	sword err = 0;
		
	if(cursor->rowCount != 0) 
	{
		//not first call after sql execution
		
		sword status = 0;
		
		status = OCIStmtFetch2(cursor->stmtp, cursor->errhp, cursor->itemCount, OCI_DEFAULT, 0, OCI_DEFAULT);
		//http://docs.oracle.com/cd/A97630_01/appdev.920/a96584/oci16ms4.htm
		
		switch (status)
		{
			case OCI_NO_DATA:
				OCIAttrGet(cursor->stmtp, OCI_HTYPE_STMT, &cursor->rowsFetched, 0, OCI_ATTR_ROWS_FETCHED, cursor->errhp);
				OCIAttrGet(cursor->stmtp, OCI_HTYPE_STMT, &cursor->rowCount, 0, OCI_ATTR_ROW_COUNT, cursor->errhp);
				cursor->isEndOfSelection = true;
				break;
				
			case 0:
				OCIAttrGet(cursor->stmtp, OCI_HTYPE_STMT, &cursor->rowsFetched, 0, OCI_ATTR_ROWS_FETCHED, cursor->errhp);
				OCIAttrGet(cursor->stmtp, OCI_HTYPE_STMT, &cursor->rowCount, 0, OCI_ATTR_ROW_COUNT, cursor->errhp);
				break;	
				
			default:
				err = -1;//used as return code for error
				cursor->isEndOfSelection = true;
				break;//error
		}
		
	}else{
		
		//first call after sql execution
		
		OCIAttrGet(cursor->stmtp, OCI_HTYPE_STMT, &cursor->rowsFetched, 0, OCI_ATTR_ROWS_FETCHED, cursor->errhp);
		OCIAttrGet(cursor->stmtp, OCI_HTYPE_STMT, &cursor->rowCount, 0, OCI_ATTR_ROW_COUNT, cursor->errhp);
	}
		
	return err;
	
}

#pragma mark -

sword _fetchDataIntoDateArray(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{	
	if(cursor->rowsFetched)
	{		
		unsigned int itemCount = cursor->rowsFetched < cursor->itemCount ? cursor->rowsFetched : cursor->itemCount;
		
		//to clear the content of element #0, if any
		PA_ResizeArray(&variable, 0);
		PA_ResizeArray(&variable, itemCount);
		
		short d; 
		short m; 
		short y;
		
		for(unsigned int i = 0; i < itemCount; ++i)
		{	
			PA_YieldAbsolute();
			
			if(cursor->indicatorLists.at(pos).at(i) != -1)
			{		
				OCIDateGetDate(&cursor->arrayOfDates.at(pos).at(i), &y, &m, &d);
				PA_SetDateInArray(variable, i + 1, d, m, y);
				
			}else{
				setDateArrayValueNull(variable, i + 1);
			}
			
		}
		
		PA_SetPointerValue(&cursor->pointers.at(pos), variable);	
		
	}
		
	return cursor->rowsFetched;	
}

sword _fetchDataIntoRealArray(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{	
	if(cursor->rowsFetched)
	{		
		unsigned int itemCount = cursor->rowsFetched < cursor->itemCount ? cursor->rowsFetched : cursor->itemCount;
		
		//to clear the content of element #0, if any
		PA_ResizeArray(&variable, 0);
		PA_ResizeArray(&variable, itemCount);
		
		double realValue;
		
		for(unsigned int i = 0; i < itemCount; ++i)
		{	
			PA_YieldAbsolute();
			
			if(cursor->indicatorLists.at(pos).at(i) != -1)
			{		
				OCINumberToReal(cursor->errhp, &cursor->arrayOfNumbers.at(pos).at(i), sizeof(double), &realValue);
				PA_SetRealInArray(variable, i + 1, realValue);
				
			}else{
				setRealArrayValueNull(variable, i + 1);
			}
			
		}
		
		PA_SetPointerValue(&cursor->pointers.at(pos), variable);	
		
	}
	
	return cursor->rowsFetched;	
}

sword _fetchDataIntoBooleanArray(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{		
	if(cursor->rowsFetched) 
	{			
		unsigned int itemCount = cursor->rowsFetched < cursor->itemCount ? cursor->rowsFetched : cursor->itemCount;
		
		//to clear the content of element #0, if any
		PA_ResizeArray(&variable, 0);
		PA_ResizeArray(&variable, itemCount);
		
		int booleanValue;
		
		for(unsigned int i = 0; i < itemCount; ++i)
		{	
			PA_YieldAbsolute();
			
			if(cursor->indicatorLists.at(pos).at(i) != -1)
			{		
				OCINumberToInt(cursor->errhp, &cursor->arrayOfNumbers.at(pos).at(i), sizeof(int), OCI_NUMBER_UNSIGNED, &booleanValue);
				PA_SetBooleanInArray(variable, i + 1, booleanValue);
				
			}else{
				setBooleanArrayValueNull(variable, i + 1);
			}
			
		}
		
		PA_SetPointerValue(&cursor->pointers.at(pos), variable);	
		
	}
	
	return cursor->rowsFetched;	
}

sword _fetchDataIntoLongintArray(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{		
	if(cursor->rowsFetched)
	{			
		unsigned int itemCount = cursor->rowsFetched < cursor->itemCount ? cursor->rowsFetched : cursor->itemCount;
		
		//to clear the content of element #0, if any
		PA_ResizeArray(&variable, 0);
		PA_ResizeArray(&variable, itemCount);
		
		int intValue;
		
		for(unsigned int i = 0; i < itemCount; ++i)
		{	
			PA_YieldAbsolute();

			if(cursor->indicatorLists.at(pos).at(i) != -1)
			{		
				OCINumberToInt(cursor->errhp, &cursor->arrayOfNumbers.at(pos).at(i), sizeof(int), OCI_NUMBER_SIGNED, &intValue);
				PA_SetLongintInArray(variable, i + 1, intValue);
									
			}else{
				setLongintArrayValueNull(variable, i + 1);
			}
			
		}
		
		PA_SetPointerValue(&cursor->pointers.at(pos), variable);	
		
	}
		
	return cursor->rowsFetched;	
}

sword _fetchDataIntoTextArray(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos, sessionInfo *session)
{			
	if(cursor->rowsFetched)
	{	
		unsigned int itemCount = cursor->rowsFetched < cursor->itemCount ? cursor->rowsFetched : cursor->itemCount;
					
		//to clear the content of element #0, if any
		PA_ResizeArray(&variable, 0);
		PA_ResizeArray(&variable, itemCount);
		
		for(unsigned int i = 0; i < itemCount; ++i)
		{	
			PA_YieldAbsolute();
			
			if(cursor->indicatorLists.at(pos).at(i) != -1)
			{	
				PA_Unistring u = PA_CreateUnistring((PA_Unichar *)OCIStringPtr(session->envhp, cursor->arrayOfTexts.at(pos).at(i)));
				PA_SetStringInArray(variable, i + 1, &u);
				
			}else{
				setTextArrayValueNull(variable, i + 1);
			}
			
			//prepare buffer of next fetch
			memset(OCIStringPtr(session->envhp, cursor->arrayOfTexts.at(pos).at(i)), 0, BUFFER_SIZE_TEXT_ARRAY_ELEMENT);
			
		}
		
		PA_SetPointerValue(&cursor->pointers.at(pos), variable);	
		
	}
					
	return cursor->rowsFetched;
		
}

#pragma mark -

sword _fetchDataIntoTextVariable(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos, sessionInfo *session)
{	
	if(cursor->rowsFetched)
	{
		
		if(cursor->indicators.at(pos) != -1)
		{		
			PA_Unistring u = PA_GetStringVariable(variable);
			PA_SetUnistring(&u, (PA_Unichar *)&cursor->texts.at(pos).at(0));
			PA_SetStringVariable(&variable, &u);
			
		}else{
			setTextVariableValueNull(variable);
		}
		
		PA_SetPointerValue(&cursor->pointers.at(pos), variable);		
	}
	
	return cursor->rowsFetched;
}

sword _fetchDataIntoLongintVariable(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{		
	if(cursor->rowsFetched)
	{
		int intValue = 0;
		
		if(cursor->indicators.at(pos) != -1)
		{		
			OCINumberToInt(cursor->errhp, &cursor->numbers.at(pos), sizeof(int), OCI_NUMBER_SIGNED, &intValue);
			PA_SetLongintVariable(&variable, intValue);	
			
		}else{
			setLongintVariableValueNull(variable);
		}
		
		PA_SetPointerValue(&cursor->pointers.at(pos), variable);
	}
		
	return cursor->rowsFetched;
}

sword _fetchDataIntoBooleanVariable(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{		
	if(cursor->rowsFetched)
	{
		int intValue = 0;
		
		if(cursor->indicators.at(pos) != -1)
		{		
			OCINumberToInt(cursor->errhp, &cursor->numbers.at(pos), sizeof(int), OCI_NUMBER_UNSIGNED, &intValue);
			PA_SetBooleanVariable(&variable, intValue);	
			
		}else{
			setBooleanVariableValueNull(variable);
		}
		
		PA_SetPointerValue(&cursor->pointers.at(pos), variable);		
	}
		
	return cursor->rowsFetched;
}

sword _fetchDataIntoRealVariable(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{		
	if(cursor->rowsFetched)
	{
		double realValue;
		
		if(cursor->indicators.at(pos) != -1)
		{	
			OCINumberToReal(cursor->errhp, &cursor->numbers.at(pos), sizeof(double), &realValue);
			PA_SetRealVariable(&variable, realValue);	
			
		}else{
			setRealVariableValueNull(variable);
		}
		
		PA_SetPointerValue(&cursor->pointers.at(pos), variable);		
	}
	
	return cursor->rowsFetched;
}

sword _fetchDataIntoDateVariable(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{			
	if(cursor->rowsFetched)
	{
		short d; 
		short m; 
		short y;
		
		if(cursor->indicators.at(pos) != -1)
		{		
			OCIDateGetDate(&cursor->dates.at(pos), &y, &m, &d);
			PA_SetDateVariable(&variable, d, m, y);
			
		}else{
			setDateVariableValueNull(variable);
		}
		
		PA_SetPointerValue(&cursor->pointers.at(pos), variable);		
	}
		
	return cursor->rowsFetched;
}

sword _fetchDataIntoTimeVariable(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{		
	if(cursor->rowsFetched)
	{
		unsigned int h; 
		unsigned int m; 
		unsigned int s;
		
		if(cursor->indicators.at(pos) != -1)
		{		
			OCIDateGetTime(&cursor->dates.at(pos), &h, &m, &s);
			PA_SetTimeVariable(&variable, s + (m * 60) + (h * 3600));
			
		}else{
			setTimeVariableValueNull(variable);
		}

		PA_SetPointerValue(&cursor->pointers.at(pos), variable);
	}
	
	return cursor->rowsFetched;
}

sword _fetchDataIntoBlobVariable(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos, sessionInfo *session)
{
	
	if(cursor->rowsFetched)
	{
		ub4 len = cursor->bytes.at(pos).len;
		ub4 dif = BUFFER_SIZE_BLOB_VARIABLE - len;
		ub4 size = cursor->bytes.at(pos).buf.size();
		
		cursor->bytes.at(pos).buf.resize(size - dif);
		
		PA_SetBlobVariable(&variable, &cursor->bytes.at(pos).buf[0], cursor->bytes.at(pos).buf.size());
		PA_SetPointerValue(&cursor->pointers.at(pos), variable);		
	}
	
	/*
	if(cursor->rowsFetched)
	{
		std::vector<uint8_t> dataBuf;
		ub4 offset = 1;
		ub4 amtp = 0;
		ub1 buf[BUFFER_SIZE_BLOB_VARIABLE];
		
		OCILobRead(session->svchp, cursor->errhp, cursor->locators.at(pos), &amtp, offset, buf, BUFFER_SIZE_BLOB_VARIABLE, &dataBuf, _read_lob, 0, SQLCS_IMPLICIT);
		//http://docs.oracle.com/cd/A97630_01/appdev.920/a96584/oci16m40.htm
		
		PA_SetBlobVariable(&variable, &dataBuf[0], dataBuf.size());
		PA_SetPointerValue(&cursor->pointers.at(pos), variable);
	}
	*/
	
	return cursor->rowsFetched;		
}

#pragma mark -
sword _fetchDataIntoAlphaField(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos, sessionInfo *session)
{		
	if(cursor->rowsFetched)
	{
		
		if(cursor->indicators.at(pos) != -1)
		{	
			PA_Unistring u = PA_CreateUnistring((PA_Unichar *)&cursor->texts.at(pos).at(0));
			PA_SetStringField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber, &u);
			
		}else{
			setAlphaFieldValueNull(variable);
		}
			
	}
	
	return cursor->rowsFetched;	
}

sword _fetchDataIntoTextField(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos, sessionInfo *session)
{
	return _fetchDataIntoAlphaField(cursor, variable, pos, session);
}

sword _fetchDataIntoLongintField(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{		
	if(cursor->rowsFetched)
	{
		int intValue = 0;
		
		if(cursor->indicators.at(pos) != -1)
		{		
			OCINumberToInt(cursor->errhp, &cursor->numbers.at(pos), sizeof(int), OCI_NUMBER_SIGNED, &intValue);
			PA_SetLongintField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber, intValue);	
			
		}else{
			setLongintFieldValueNull(variable);
		}
	
	}

	return cursor->rowsFetched;	
}

sword _fetchDataIntoIntegerField(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{		
	if(cursor->rowsFetched)
	{
		short intValue = 0;
		
		if(cursor->indicators.at(pos) != -1)
		{		
			OCINumberToInt(cursor->errhp, &cursor->numbers.at(pos), sizeof(short), OCI_NUMBER_SIGNED, &intValue);
			PA_SetIntegerField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber, intValue);	
			
		}else{
			setIntegerFieldValueNull(variable);
		}
			
	}
		
	return cursor->rowsFetched;	
}

sword _fetchDataIntoBooleanField(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{		
	if(cursor->rowsFetched)
	{
		short intValue = 0;
		
		if(cursor->indicators.at(pos) != -1)
		{		
			OCINumberToInt(cursor->errhp, &cursor->numbers.at(pos), sizeof(short), OCI_NUMBER_SIGNED, &intValue);
			PA_SetBooleanField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber, intValue);	
			
		}else{
			setBooleanFieldValueNull(variable);
		}
			
	}
		
	return cursor->rowsFetched;	
}

sword _fetchDataIntoRealField(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{		
	if(cursor->rowsFetched)
	{
		double realValue;
		
		if(cursor->indicators.at(pos) != -1)
		{	
			OCINumberToReal(cursor->errhp, &cursor->numbers.at(pos), sizeof(double), &realValue);
			PA_SetRealField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber, realValue);	
			
		}else{
			setRealFieldValueNull(variable);
		}
			
	}
	
	return cursor->rowsFetched;
}

sword _fetchDataIntoDateField(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{		
	if(cursor->rowsFetched)
	{
		short d; 
		short m; 
		short y;
		
		if(cursor->indicators.at(pos) != -1)
		{		
			OCIDateGetDate(&cursor->dates.at(pos), &y, &m, &d);
			PA_SetDateField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber, d, m, y);
			
		}else{
			setDateFieldValueNull(variable);
		}
			
	}
	
	return cursor->rowsFetched;
}

sword _fetchDataIntoTimeField(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{		
	if(cursor->rowsFetched)
	{
		unsigned int h; 
		unsigned int m; 
		unsigned int s;
		
		if(cursor->indicators.at(pos) != -1)
		{		
			OCIDateGetTime(&cursor->dates.at(pos), &h, &m, &s);
			PA_SetTimeField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber, s + (m * 60) + (h * 3600));
			
		}else{
			setTimeFieldValueNull(variable);
		}
			
	}
	
	return cursor->rowsFetched;
}


sword _fetchDataIntoBlobField(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos, sessionInfo *session)
{
	
	if(cursor->rowsFetched)
	{
		ub4 len = cursor->bytes.at(pos).len;
		ub4 dif = BUFFER_SIZE_BLOB_VARIABLE - len;
		ub4 size = cursor->bytes.at(pos).buf.size();
		
		cursor->bytes.at(pos).buf.resize(size - dif);
		
		if(cursor->indicators.at(pos) != -1)
		{		
			PA_SetBlobField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber, &cursor->bytes.at(pos).buf[0], cursor->bytes.at(pos).buf.size());
			
		}else{
			setBlobFieldValueNull(variable);
		}			
			
	}
	
	/*
	if(cursor->rowsFetched)
	{
		std::vector<uint8_t> dataBuf;
		ub4 offset = 1;
		ub4 amtp = 0;
		ub1 buf[BUFFER_SIZE_BLOB_VARIABLE];
		
		OCILobRead(session->svchp, cursor->errhp, cursor->locators.at(pos), &amtp, offset, buf, BUFFER_SIZE_BLOB_VARIABLE, &dataBuf, _read_lob, 0, SQLCS_IMPLICIT);
		
		if(cursor->indicators.at(pos) != -1)
		{		
			PA_SetBlobField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber, &dataBuf[0], dataBuf.size());
			
		}else{
			setBlobFieldValueNull(variable);
		}		

	}
	*/
	
	return cursor->rowsFetched;		
}
