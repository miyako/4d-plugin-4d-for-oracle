#include "oracle_low_level_bind_i.h"
#include "oracle_control.h"
#include "oracle_preferences.h"

#pragma mark -
#pragma mark bind towards sql
#pragma mark -

sword _bindTextArrayTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos, sessionInfo *session)
{	
	cursor->binds.at(pos) = 0;
	cursor->itemCount = PA_GetArrayNbElements(variable);
	
	cursor->isObjectValid.at(pos)  = false;
	
	cursor->arrayOfTexts.at(pos).resize(cursor->itemCount);
	cursor->indicatorLists.at(pos).resize(cursor->itemCount);	
	cursor->lengths.at(pos).resize(cursor->itemCount);
	cursor->isObjectElementValid.at(pos).resize(cursor->itemCount);
	
	sb4 value_sz = 0;
	
	for(int i = 0; i < PA_GetArrayNbElements(variable); ++i)
	{
		PA_YieldAbsolute();
		if(isTextArrayElementValueNull(variable, i + 1))
		{
			cursor->indicatorLists.at(pos).at(i) = -1;
		}else{
			cursor->indicatorLists.at(pos).at(i) = 1;
		}	
		
		PA_Unistring u = PA_GetStringInArray(variable, i + 1);
		
		cursor->arrayOfTexts.at(pos).at(i) = 0;
		cursor->isObjectElementValid.at(pos).at(i) = false;
		
		sword err = 0;
		
		err = OCIObjectNew(session->envhp, session->errhp, session->svchp, OCI_TYPECODE_VARCHAR2, 0, 0, OCI_DURATION_SESSION, true, (dvoid **)&cursor->arrayOfTexts.at(pos).at(i));
		
		if(!err)
		{
			err = OCIStringAssignText(session->envhp, session->errhp, (CONST oratext *)u.fString, u.fLength * sizeof(PA_Unichar), &cursor->arrayOfTexts.at(pos).at(i));
			
			if(!err)
			{
				cursor->isObjectElementValid.at(pos).at(i) = true;	
				cursor->isObjectValid.at(pos)  = true;
				
				//seems ok to measure in code points...
				//cursor->lengths.at(pos).at(i) = u.fLength * sizeof(PA_Unichar) * sizeof(PA_Unichar);
				cursor->lengths.at(pos).at(i) = u.fLength + 1;
				
				if(value_sz < cursor->lengths.at(pos).at(i))
					value_sz = cursor->lengths.at(pos).at(i);
				
			}
			
		}
		
	}
	
	OCIBindByPos(cursor->stmtp, 
				 &cursor->binds.at(pos),
				 cursor->errhp, 
				 pos + 1, 
				 &cursor->arrayOfTexts.at(pos).at(0), 
				 value_sz,
				 SQLT_VST,
				 &cursor->indicatorLists.at(pos).at(0),  
				 &cursor->lengths.at(pos).at(0),
				 0, 
				 0, 
				 0,
				 OCI_DEFAULT);	
	
	return OCIBindArrayOfStruct(cursor->binds.at(pos), 
								cursor->errhp,
								sizeof(cursor->arrayOfTexts.at(pos).at(0)),
								sizeof(cursor->indicatorLists.at(pos).at(0)),
								sizeof(cursor->lengths.at(pos).at(0)),
								0);
}

sword _bindLongintArrayTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{	
	cursor->binds.at(pos) = 0;
	cursor->itemCount = PA_GetArrayNbElements(variable);
	
	cursor->arrayOfNumbers.at(pos).resize(cursor->itemCount);
	cursor->indicatorLists.at(pos).resize(cursor->itemCount);	
	cursor->lengths.at(pos).resize(cursor->itemCount);
	
	for(int i = 0; i < PA_GetArrayNbElements(variable); ++i)
	{
		PA_YieldAbsolute();
		if(isLongintArrayElementValueNull(variable, i + 1))
		{
			cursor->indicatorLists.at(pos).at(i) = -1;
		}else{
			cursor->indicatorLists.at(pos).at(i) = 1;
		}	
		
		int intValue = (int)PA_GetLongintInArray(variable, i + 1);
		
		OCINumberFromInt(cursor->errhp, &intValue, sizeof(int), OCI_NUMBER_SIGNED, &cursor->arrayOfNumbers.at(pos).at(i));
		
		cursor->lengths.at(pos).at(i) = sizeof(cursor->arrayOfNumbers.at(pos).at(0));
	}
	
	OCIBindByPos(cursor->stmtp, 
				 &cursor->binds.at(pos),
				 cursor->errhp, 
				 pos + 1, 
				 &cursor->arrayOfNumbers.at(pos).at(0), 
				 sizeof(cursor->arrayOfNumbers.at(pos).at(0)), 
				 SQLT_VNU,
				 &cursor->indicatorLists.at(pos).at(0),  
				 &cursor->lengths.at(pos).at(0),
				 0, 
				 0, 
				 0,
				 OCI_DEFAULT);	
	
	return OCIBindArrayOfStruct(cursor->binds.at(pos), 
								cursor->errhp,
								sizeof(cursor->arrayOfNumbers.at(pos).at(0)),
								sizeof(cursor->indicatorLists.at(pos).at(0)),
								sizeof(cursor->lengths.at(pos).at(0)),
								0);
	
}

sword _bindBooleanArrayTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{	
	cursor->binds.at(pos) = 0;
	cursor->itemCount = PA_GetArrayNbElements(variable);
	
	cursor->arrayOfNumbers.at(pos).resize(cursor->itemCount);
	cursor->indicatorLists.at(pos).resize(cursor->itemCount);	
	cursor->lengths.at(pos).resize(cursor->itemCount);
	
	for(int i = 0; i < PA_GetArrayNbElements(variable); ++i)
	{
		PA_YieldAbsolute();
		if(isBooleanArrayElementValueNull(variable, i + 1))
		{
			cursor->indicatorLists.at(pos).at(i) = -1;
		}else{
			cursor->indicatorLists.at(pos).at(i) = 1;
		}	
		
		int boolValue = (int)PA_GetBooleanInArray(variable, i + 1);
		
		OCINumberFromInt(cursor->errhp, &boolValue, sizeof(int), OCI_NUMBER_UNSIGNED, &cursor->arrayOfNumbers.at(pos).at(i));
		
		cursor->lengths.at(pos).at(i) = sizeof(cursor->arrayOfNumbers.at(pos).at(0));
	}
	
	OCIBindByPos(cursor->stmtp, 
				 &cursor->binds.at(pos),
				 cursor->errhp, 
				 pos + 1, 
				 &cursor->arrayOfNumbers.at(pos).at(0), 
				 sizeof(cursor->arrayOfNumbers.at(pos).at(0)), 
				 SQLT_VNU,
				 &cursor->indicatorLists.at(pos).at(0),  
				 &cursor->lengths.at(pos).at(0),
				 0, 
				 0, 
				 0,
				 OCI_DEFAULT);	
	
	return OCIBindArrayOfStruct(cursor->binds.at(pos), 
								cursor->errhp,
								sizeof(cursor->arrayOfNumbers.at(pos).at(0)),
								sizeof(cursor->indicatorLists.at(pos).at(0)),
								sizeof(cursor->lengths.at(pos).at(0)),
								0);
	
}

sword _bindRealArrayTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{	
	cursor->binds.at(pos) = 0;
	cursor->itemCount = PA_GetArrayNbElements(variable);
	
	cursor->arrayOfNumbers.at(pos).resize(cursor->itemCount);
	cursor->indicatorLists.at(pos).resize(cursor->itemCount);	
	cursor->lengths.at(pos).resize(cursor->itemCount);
	
	for(int i = 0; i < PA_GetArrayNbElements(variable); ++i)
	{
		PA_YieldAbsolute();
		if(isRealArrayElementValueNull(variable, i + 1))
		{
			cursor->indicatorLists.at(pos).at(i) = -1;
		}else{
			cursor->indicatorLists.at(pos).at(i) = 1;
		}	
		
		double realValue = (double)PA_GetRealInArray(variable, i + 1);
		
		OCINumberFromReal(cursor->errhp, &realValue, sizeof(double), &cursor->arrayOfNumbers.at(pos).at(i));
		
		cursor->lengths.at(pos).at(i) = sizeof(cursor->arrayOfNumbers.at(pos).at(0));
	}
	
	OCIBindByPos(cursor->stmtp, 
				 &cursor->binds.at(pos),
				 cursor->errhp, 
				 pos + 1, 
				 &cursor->arrayOfNumbers.at(pos).at(0), 
				 sizeof(cursor->arrayOfNumbers.at(pos).at(0)), 
				 SQLT_VNU,
				 &cursor->indicatorLists.at(pos).at(0),  
				 &cursor->lengths.at(pos).at(0),
				 0, 
				 0, 
				 0,
				 OCI_DEFAULT);	
	
	return OCIBindArrayOfStruct(cursor->binds.at(pos), 
								cursor->errhp,
								sizeof(cursor->arrayOfNumbers.at(pos).at(0)),
								sizeof(cursor->indicatorLists.at(pos).at(0)),
								sizeof(cursor->lengths.at(pos).at(0)),
								0);
	
}

sword _bindDateArrayTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{	
	cursor->binds.at(pos) = 0;
	cursor->itemCount = PA_GetArrayNbElements(variable);
	
	short d; 
	short m; 
	short y;	
	
	cursor->arrayOfDates.at(pos).resize(cursor->itemCount);
	cursor->indicatorLists.at(pos).resize(cursor->itemCount);	
	cursor->lengths.at(pos).resize(cursor->itemCount);
	
	for(int i = 0; i < PA_GetArrayNbElements(variable); ++i)
	{
		PA_YieldAbsolute();
		if(isDateArrayElementValueNull(variable, i + 1))
		{
			cursor->indicatorLists.at(pos).at(i) = -1;
		}else{
			cursor->indicatorLists.at(pos).at(i) = 1;
		}	
		
		PA_GetDateInArray(variable, i + 1, &d, &m, &y);
		
		OCIDateSetDate(&cursor->arrayOfDates.at(pos).at(i), y, m, d);
		
		cursor->lengths.at(pos).at(i) = sizeof(cursor->arrayOfDates.at(pos).at(0));
	}
	
	OCIBindByPos(cursor->stmtp, 
				 &cursor->binds.at(pos),
				 cursor->errhp, 
				 pos + 1, 
				 &cursor->arrayOfDates.at(pos).at(0), 
				 sizeof(cursor->arrayOfDates.at(pos).at(0)), 
				 SQLT_ODT,
				 &cursor->indicatorLists.at(pos).at(0),  
				 &cursor->lengths.at(pos).at(0),
				 0, 
				 0, 
				 0,
				 OCI_DEFAULT);	
	
	return OCIBindArrayOfStruct(cursor->binds.at(pos), 
								cursor->errhp,
								sizeof(cursor->arrayOfDates.at(pos).at(0)),
								sizeof(cursor->indicatorLists.at(pos).at(0)),
								sizeof(cursor->lengths.at(pos).at(0)),
								0);
	
}

#pragma mark -

sword _bindAlphaFieldTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{
	cursor->binds.at(pos) = 0;
	cursor->itemCount = 1;
	
	PA_Unistring u = PA_GetStringField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber);
	
	if(isAlphaFieldValueNull(variable))
	{
		cursor->indicators.at(pos) = -1;
	}else{
		cursor->indicators.at(pos) = 1;
	}
	
	return OCIBindByPos(cursor->stmtp, 
						&cursor->binds.at(pos),
						cursor->errhp, 
						pos + 1, 
						u.fString, 
						u.fLength * sizeof(PA_Unichar) + sizeof(PA_Unichar), 
						SQLT_STR,
						&cursor->indicators.at(pos), 
						0,
						0, 
						0, 
						0,
						OCI_DEFAULT);
	
	//http://docs.oracle.com/cd/B10500_01/appdev.920/a96584/oci15r30.htm	
}

sword _bindTextFieldTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{
	cursor->binds.at(pos) = 0;
	cursor->itemCount = 1;
	
	PA_Unistring u = PA_GetStringField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber);
	
	if(isTextFieldValueNull(variable))
	{
		cursor->indicators.at(pos) = -1;
	}else{
		cursor->indicators.at(pos) = 1;
	}
	
	return OCIBindByPos(cursor->stmtp, 
						&cursor->binds.at(pos),
						cursor->errhp, 
						pos + 1, 
						u.fString, 
						u.fLength * sizeof(PA_Unichar) + sizeof(PA_Unichar), 
						SQLT_STR,
						&cursor->indicators.at(pos), 
						0,
						0, 
						0, 
						0,
						OCI_DEFAULT);
	
	//http://docs.oracle.com/cd/B10500_01/appdev.920/a96584/oci15r30.htm	
}

sword _bindLongintFieldTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{	
	cursor->binds.at(pos) = 0;
	cursor->itemCount = 1;
	
	if(isLongintFieldValueNull(variable))
	{
		cursor->indicators.at(pos) = -1;
	}else{
		cursor->indicators.at(pos) = 1;
	}
	
	int intValue = (int)PA_GetLongintField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber);
	
	OCINumberFromInt(cursor->errhp, &intValue, sizeof(int), OCI_NUMBER_SIGNED, &cursor->numbers.at(pos));
	//http://docs.oracle.com/cd/B28359_01/appdev.111/b28395/oci19map003.htm
	
	return OCIBindByPos(cursor->stmtp, 
						&cursor->binds.at(pos),
						cursor->errhp, 
						pos + 1, 
						&cursor->numbers.at(pos), 
						sizeof(OCINumber), 
						SQLT_VNU,
						&cursor->indicators.at(pos), 
						0,
						0, 
						0, 
						0,
						OCI_DEFAULT);
	
	//http://docs.oracle.com/cd/B10500_01/appdev.920/a96584/oci15r30.htm
}

sword _bindIntegerFieldTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{	
	cursor->binds.at(pos) = 0;
	cursor->itemCount = 1;
	
	if(isIntegerFieldValueNull(variable))
	{
		cursor->indicators.at(pos) = -1;
	}else{
		cursor->indicators.at(pos) = 1;
	}
	
	int intValue = (int)PA_GetIntegerField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber);
	
	OCINumberFromInt(cursor->errhp, &intValue, sizeof(int), OCI_NUMBER_SIGNED, &cursor->numbers.at(pos));
	//http://docs.oracle.com/cd/B28359_01/appdev.111/b28395/oci19map003.htm
	
	return OCIBindByPos(cursor->stmtp, 
						&cursor->binds.at(pos),
						cursor->errhp, 
						pos + 1, 
						&cursor->numbers.at(pos), 
						sizeof(OCINumber), 
						SQLT_VNU,
						&cursor->indicators.at(pos), 
						0,
						0, 
						0, 
						0,
						OCI_DEFAULT);
	
	//http://docs.oracle.com/cd/B10500_01/appdev.920/a96584/oci15r30.htm
}

sword _bindBooleanFieldTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{		
	cursor->binds.at(pos) = 0;
	cursor->itemCount = 1;
	
	if(isBooleanFieldValueNull(variable))
	{
		cursor->indicators.at(pos) = -1;
	}else{
		cursor->indicators.at(pos) = 1;
	}
	
	int boolValue = (int)PA_GetBooleanField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber);
	
	OCINumberFromInt(cursor->errhp, &boolValue, sizeof(int), OCI_NUMBER_UNSIGNED, &cursor->numbers.at(pos));
	//http://docs.oracle.com/cd/B28359_01/appdev.111/b28395/oci19map003.htm
	
	return OCIBindByPos(cursor->stmtp, 
						&cursor->binds.at(pos),
						cursor->errhp, 
						pos + 1, 
						&cursor->numbers.at(pos), 
						sizeof(OCINumber), 
						SQLT_VNU,
						&cursor->indicators.at(pos), 
						0,
						0, 
						0, 
						0,
						OCI_DEFAULT);
	
	//http://docs.oracle.com/cd/B10500_01/appdev.920/a96584/oci15r30.htm
}

sword _bindRealFieldTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{		
	cursor->binds.at(pos) = 0;
	cursor->itemCount = 1;
	
	if(isRealFieldValueNull(variable))
	{
		cursor->indicators.at(pos) = -1;
	}else{
		cursor->indicators.at(pos) = 1;
	}
	
	double realValue = (double)PA_GetRealField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber);
	
	OCINumberFromInt(cursor->errhp, &realValue, sizeof(double), OCI_NUMBER_SIGNED, &cursor->numbers.at(pos));
	//http://docs.oracle.com/cd/B28359_01/appdev.111/b28395/oci19map003.htm
	
	return OCIBindByPos(cursor->stmtp, 
						&cursor->binds.at(pos),
						cursor->errhp, 
						pos + 1, 
						&cursor->numbers.at(pos), 
						sizeof(OCINumber), 
						SQLT_VNU,
						&cursor->indicators.at(pos), 
						0,
						0, 
						0, 
						0,
						OCI_DEFAULT);
	
	//http://docs.oracle.com/cd/B10500_01/appdev.920/a96584/oci15r30.htm
}

sword _bindDateFieldTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{	
	cursor->binds.at(pos) = 0;
	cursor->itemCount = 1;
	
	if(isDateFieldValueNull(variable))
	{
		cursor->indicators.at(pos) = -1;
	}else{
		cursor->indicators.at(pos) = 1;
	}
	
	short day; 
	short month; 
	short year;
	
	PA_GetDateField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber, &day, &month, &year);
	
	OCIDateSetDate(&cursor->dates.at(pos), year, month, day);
	
	return OCIBindByPos(cursor->stmtp, 
						&cursor->binds.at(pos),
						cursor->errhp, 
						pos + 1, 
						&cursor->dates.at(pos), 
						sizeof(OCIDate), 
						SQLT_ODT,
						&cursor->indicators.at(pos),  
						0,
						0, 
						0, 
						0,
						OCI_DEFAULT);
	
	//http://docs.oracle.com/cd/B10500_01/appdev.920/a96584/oci15r30.htm
}

sword _bindTimeFieldTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{	
	cursor->binds.at(pos) = 0;
	cursor->itemCount = 1;
	
	if(isTimeFieldValueNull(variable))
	{
		cursor->indicators.at(pos) = -1;
	}else{
		cursor->indicators.at(pos) = 1;
	}
	
	unsigned int seconds = PA_GetTimeField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber);
	
	unsigned int h = seconds / 3600;
	unsigned int m = (seconds % 3600) / 60;	
	unsigned int s = (seconds % 3600) % 60;
	
	OCIDateSetTime(&cursor->dates.at(pos), h, m, s);
	
	return OCIBindByPos(cursor->stmtp, 
						&cursor->binds.at(pos),
						cursor->errhp, 
						pos + 1, 
						&cursor->dates.at(pos), 
						sizeof(OCIDate), 
						SQLT_ODT,
						&cursor->indicators.at(pos),  
						0,
						0, 
						0, 
						0,
						OCI_DEFAULT);
	
	//http://docs.oracle.com/cd/B10500_01/appdev.920/a96584/oci15r30.htm
}

sword _bindBlobFieldTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{
	cursor->binds.at(pos) = 0;
	cursor->itemCount = 1;
	
	if(isBlobFieldValueNull(variable))
	{
		cursor->indicators.at(pos) = -1;
	}else{
		cursor->indicators.at(pos) = 1;
	}
	
	cursor->isRawObjectValid.at(pos) = true;
	cursor->blobs.at(pos) = PA_GetBlobHandleField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber);
	
	return OCIBindByPos(cursor->stmtp, 
						&cursor->binds.at(pos),
						cursor->errhp, 
						pos + 1, 
						PA_LockHandle(cursor->blobs.at(pos)), 
						PA_GetHandleSize(cursor->blobs.at(pos)), 
						SQLT_BIN,
						&cursor->indicators.at(pos), 
						0,
						0, 
						0, 
						0,
						OCI_DEFAULT);
	
	//http://docs.oracle.com/cd/B10500_01/appdev.920/a96584/oci15r30.htm	
}

#pragma mark -

sword _bindTextVariableTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{	
	cursor->binds.at(pos) = 0;
	cursor->itemCount = 1;
	
	PA_Unistring u = PA_GetStringVariable(variable);
	
	if(isTextVariableValueNull(variable))
	{
		cursor->indicators.at(pos) = -1;
	}else{
		cursor->indicators.at(pos) = 1;
	}
	
	return OCIBindByPos(cursor->stmtp, 
						&cursor->binds.at(pos),
						cursor->errhp, 
						pos + 1, 
						u.fString, 
						//must be bytes, documentation wrong... 
						//u.fLength + 1,
						u.fLength * sizeof(PA_Unichar) + sizeof(PA_Unichar), 
						SQLT_STR,
						&cursor->indicators.at(pos), 
						0,
						0, 
						0, 
						0,
						OCI_DEFAULT);
	
	//http://docs.oracle.com/cd/B10500_01/appdev.920/a96584/oci15r30.htm
}

sword _bindLongintVariableTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{
	cursor->binds.at(pos) = 0;
	cursor->itemCount = 1;
	
	if(isLongintVariableValueNull(variable))
	{
		cursor->indicators.at(pos) = -1;
	}else{
		cursor->indicators.at(pos) = 1;
	}
	
	int intValue = (int)PA_GetLongintVariable(variable);
	
	OCINumberFromInt(cursor->errhp, &intValue, sizeof(int), OCI_NUMBER_SIGNED, &cursor->numbers.at(pos));
	//http://docs.oracle.com/cd/B28359_01/appdev.111/b28395/oci19map003.htm
	
	return OCIBindByPos(cursor->stmtp, 
						&cursor->binds.at(pos),
						cursor->errhp, 
						pos + 1, 
						&cursor->numbers.at(pos), 
						sizeof(OCINumber), 
						SQLT_VNU,
						&cursor->indicators.at(pos), 
						0,
						0, 
						0, 
						0,
						OCI_DEFAULT);	
}

sword _bindBooleanVariableTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{	
	cursor->binds.at(pos) = 0;
	cursor->itemCount = 1;
	
	if(isBooleanVariableValueNull(variable))
	{
		cursor->indicators.at(pos) = -1;
	}else{
		cursor->indicators.at(pos) = 1;
	}
	
	int boolValue = (int)PA_GetBooleanVariable(variable);
	
	OCINumberFromInt(cursor->errhp, &boolValue, sizeof(int), OCI_NUMBER_UNSIGNED, &cursor->numbers.at(pos));
	//http://docs.oracle.com/cd/B28359_01/appdev.111/b28395/oci19map003.htm
	
	return OCIBindByPos(cursor->stmtp, 
						&cursor->binds.at(pos),
						cursor->errhp, 
						pos + 1, 
						&cursor->numbers.at(pos), 
						sizeof(OCINumber), 
						SQLT_VNU,
						&cursor->indicators.at(pos), 
						0,
						0, 
						0, 
						0,
						OCI_DEFAULT);
}

sword _bindRealVariableTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{
	cursor->binds.at(pos) = 0;
	cursor->itemCount = 1;
	
	if(isRealVariableValueNull(variable))
	{
		cursor->indicators.at(pos) = -1;
	}else{
		cursor->indicators.at(pos) = 1;
	}
	
	double realValue = (double)PA_GetRealVariable(variable);
	
	OCINumberFromReal(cursor->errhp, &realValue, sizeof(double), &cursor->numbers.at(pos));
	//http://docs.oracle.com/cd/B28359_01/appdev.111/b28395/oci19map003.htm
	
	return OCIBindByPos(cursor->stmtp, 
						&cursor->binds.at(pos),
						cursor->errhp, 
						pos + 1, 
						&cursor->numbers.at(pos), 
						sizeof(OCINumber), 
						SQLT_VNU,
						&cursor->indicators.at(pos),  
						0,
						0, 
						0, 
						0,
						OCI_DEFAULT);	
}

sword _bindDateVariableTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{
	cursor->binds.at(pos) = 0;
	cursor->itemCount = 1;
	
	if(isDateVariableValueNull(variable))
	{
		cursor->indicators.at(pos) = -1;
	}else{
		
		cursor->indicators.at(pos) = 1;
	}
	
	OCIDateSetDate(&cursor->dates.at(pos), variable.uValue.fDate.fYear, variable.uValue.fDate.fMonth, variable.uValue.fDate.fDay);
	
	return OCIBindByPos(cursor->stmtp, 
						&cursor->binds.at(pos),
						cursor->errhp, 
						pos + 1, 
						&cursor->dates.at(pos), 
						sizeof(OCIDate), 
						SQLT_ODT,
						&cursor->indicators.at(pos),  
						0,
						0, 
						0, 
						0,
						OCI_DEFAULT);	
}

sword _bindTimeVariableTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{
	cursor->binds.at(pos) = 0;
	cursor->itemCount = 1;
	
	if(isTimeVariableValueNull(variable))
	{
		cursor->indicators.at(pos) = -1;
	}else{
		
		cursor->indicators.at(pos) = 1;
	}
	
	unsigned int seconds = (unsigned int)variable.uValue.fTime;
	
	unsigned int h = seconds / 3600;
	unsigned int m = (seconds % 3600) / 60;	
	unsigned int s = (seconds % 3600) % 60;
	
	OCIDateSetTime(&cursor->dates.at(pos), h, m, s);
	
	return OCIBindByPos(cursor->stmtp, 
						&cursor->binds.at(pos),
						cursor->errhp, 
						pos + 1, 
						&cursor->dates.at(pos), 
						sizeof(OCIDate), 
						SQLT_ODT,
						&cursor->indicators.at(pos),  
						0,
						0, 
						0, 
						0,
						OCI_DEFAULT);	
}

sword _bindBlobVariableTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos)
{
	cursor->binds.at(pos) = 0;
	cursor->itemCount = 1;
	
	cursor->indicators.at(pos) = 1;
	
	cursor->isRawObjectValid.at(pos) = true;
	cursor->blobs.at(pos) = PA_GetBlobHandleVariable(variable);

	return OCIBindByPos(cursor->stmtp, 
						&cursor->binds.at(pos),
						cursor->errhp, 
						pos + 1, 
						PA_LockHandle(cursor->blobs.at(pos)), 
						PA_GetHandleSize(cursor->blobs.at(pos)), 
						SQLT_BIN,
						&cursor->indicators.at(pos), 
						0,
						0, 
						0, 
						0,
						OCI_DEFAULT);
	
	//http://docs.oracle.com/cd/B10500_01/appdev.920/a96584/oci15r30.htm	
}
