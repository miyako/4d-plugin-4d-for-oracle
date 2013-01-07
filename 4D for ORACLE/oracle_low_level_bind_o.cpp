#include "oracle_low_level_bind_o.h"
#include "oracle_control.h"
#include "oracle_preferences.h"

sword _bindTextArrayTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos, sessionInfo *session)
{	
	cursor->defines.at(pos) = 0;
	cursor->isObjectValid.at(pos)  = false;
	
	cursor->arrayOfTexts.at(pos).resize(cursor->itemCount);
	cursor->indicatorLists.at(pos).resize(cursor->itemCount);	
	cursor->lengths.at(pos).resize(cursor->itemCount);
	cursor->isObjectElementValid.at(pos).resize(cursor->itemCount);
	
	for(unsigned int i = 0; i < cursor->itemCount; ++i)
	{
		PA_YieldAbsolute();
		
		cursor->arrayOfTexts.at(pos).at(i) = 0;
		cursor->isObjectElementValid.at(pos).at(i) = false;
		
		sword err = 0;
		
		err = OCIObjectNew(session->envhp, session->errhp, session->svchp, OCI_TYPECODE_VARCHAR2, 0, 0, OCI_DURATION_SESSION, true, (dvoid **)&cursor->arrayOfTexts.at(pos).at(i));	
		
		if(!err)
		{
			err = OCIStringResize(session->envhp, session->errhp, BUFFER_SIZE_TEXT_ARRAY_ELEMENT, &cursor->arrayOfTexts.at(pos).at(i));
			
			if(!err)
			{
				memset(OCIStringPtr(session->envhp, cursor->arrayOfTexts.at(pos).at(i)), 0, BUFFER_SIZE_TEXT_ARRAY_ELEMENT);
				
				cursor->isObjectElementValid.at(pos).at(i) = true;	
				cursor->isObjectValid.at(pos)  = true;
				cursor->lengths.at(pos).at(i) = BUFFER_SIZE_TEXT_ARRAY_ELEMENT;				
			}
			
		}
		
	}
	
	OCIDefineByPos(cursor->stmtp, 
				   &cursor->defines.at(pos),
				   cursor->errhp, 
				   pos + 1, 
				   &cursor->arrayOfTexts.at(pos).at(0), 
				   BUFFER_SIZE_TEXT_ARRAY_ELEMENT,
				   SQLT_VST,
				   &cursor->indicatorLists.at(pos).at(0),  
				   &cursor->lengths.at(pos).at(0),
				   0, 
				   OCI_DEFAULT);	
	//http://docs.oracle.com/cd/A97630_01/appdev.920/a96584/oci15r34.htm
	
	return OCIDefineArrayOfStruct(cursor->defines.at(pos), 
								  cursor->errhp,
								  sizeof(cursor->arrayOfTexts.at(pos).at(0)),
								  sizeof(cursor->indicatorLists.at(pos).at(0)),
								  sizeof(cursor->lengths.at(pos).at(0)),
								  0);
	//http://docs.oracle.com/cd/A97630_01/appdev.920/a96584/oci15r33.htm#498629	
}

sword _bindLongintArrayTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos)
{
	cursor->defines.at(pos) = 0;
	cursor->isObjectValid.at(pos)  = false;
	
	cursor->arrayOfNumbers.at(pos).resize(cursor->itemCount);
	cursor->indicatorLists.at(pos).resize(cursor->itemCount);	
	cursor->lengths.at(pos).resize(cursor->itemCount);
	
	for(unsigned int i = 0; i < cursor->itemCount; ++i)
	{
		PA_YieldAbsolute();		
		cursor->lengths.at(pos).at(i) = sizeof(cursor->arrayOfNumbers.at(pos).at(0));
	}
	
	OCIDefineByPos(cursor->stmtp, 
				   &cursor->defines.at(pos),
				   cursor->errhp, 
				   pos + 1, 
				   &cursor->arrayOfNumbers.at(pos).at(0), 
				   sizeof(cursor->arrayOfNumbers.at(pos).at(0)), 
				   SQLT_VNU,
				   &cursor->indicatorLists.at(pos).at(0),  
				   &cursor->lengths.at(pos).at(0),
				   0, 
				   OCI_DEFAULT);	
	//http://docs.oracle.com/cd/A97630_01/appdev.920/a96584/oci15r34.htm
	
	return OCIDefineArrayOfStruct(cursor->defines.at(pos), 
								  cursor->errhp,
								  sizeof(cursor->arrayOfNumbers.at(pos).at(0)),
								  sizeof(cursor->indicatorLists.at(pos).at(0)),
								  sizeof(cursor->lengths.at(pos).at(0)),
								  0);
	//http://docs.oracle.com/cd/A97630_01/appdev.920/a96584/oci15r33.htm#498629	
}

sword _bindBooleanArrayTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos)
{
	return _bindLongintArrayTowards4D(cursor, pos);
}

sword _bindRealArrayTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos)
{
	return _bindLongintArrayTowards4D(cursor, pos);
}

sword _bindDateArrayTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos)
{
	cursor->defines.at(pos) = 0;
	cursor->isObjectValid.at(pos)  = false;
	
	cursor->arrayOfNumbers.at(pos).resize(cursor->itemCount);
	cursor->indicatorLists.at(pos).resize(cursor->itemCount);	
	cursor->lengths.at(pos).resize(cursor->itemCount);
	
	for(unsigned int i = 0; i < cursor->itemCount; ++i)
	{
		PA_YieldAbsolute();
		cursor->lengths.at(pos).at(i) = sizeof(cursor->arrayOfDates.at(pos).at(0));
	}
	
	OCIDefineByPos(cursor->stmtp, 
				   &cursor->defines.at(pos),
				   cursor->errhp, 
				   pos + 1, 
				   &cursor->arrayOfDates.at(pos).at(0), 
				   sizeof(cursor->arrayOfDates.at(pos).at(0)), 
				   SQLT_ODT,
				   &cursor->indicatorLists.at(pos).at(0),  
				   &cursor->lengths.at(pos).at(0),
				   0, 
				   OCI_DEFAULT);	
	//http://docs.oracle.com/cd/A97630_01/appdev.920/a96584/oci15r34.htm
	
	return OCIDefineArrayOfStruct(cursor->defines.at(pos), 
								  cursor->errhp,
								  sizeof(cursor->arrayOfNumbers.at(pos).at(0)),
								  sizeof(cursor->indicatorLists.at(pos).at(0)),
								  sizeof(cursor->lengths.at(pos).at(0)),
								  0);
	//http://docs.oracle.com/cd/A97630_01/appdev.920/a96584/oci15r33.htm#498629	
}

#pragma mark -

sword _bindTextVariableTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos)
{
	cursor->defines.at(pos) = 0;
	cursor->itemCount = 1;//this will over write whatever size given for arrays
	
	cursor->texts.at(pos).resize(BUFFER_SIZE_TEXT_VARIABLE);
	memset(&cursor->texts.at(pos).at(0), 0, BUFFER_SIZE_TEXT_VARIABLE);
	
	return OCIDefineByPos(cursor->stmtp, 
				   &cursor->defines.at(pos),
				   cursor->errhp, 
				   pos + 1, 
				   &cursor->texts.at(pos).at(0), 
				   BUFFER_SIZE_TEXT_VARIABLE,
				   SQLT_STR,
				   &cursor->indicators.at(pos),  
				   0,
				   0, 
				   OCI_DEFAULT);	
	//http://docs.oracle.com/cd/A97630_01/appdev.920/a96584/oci15r34.htm
}

sword _bindLongintVariableTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos)
{
	cursor->defines.at(pos) = 0;
	cursor->itemCount = 1;//this will over write whatever size given for arrays
	
	return OCIDefineByPos(cursor->stmtp, 
						  &cursor->defines.at(pos),
						  cursor->errhp, 
						  pos + 1, 
						  &cursor->numbers.at(pos), 
						  sizeof(OCINumber),
						  SQLT_VNU,
						  &cursor->indicators.at(pos),  
						  0,
						  0, 
						  OCI_DEFAULT);	
	//http://docs.oracle.com/cd/A97630_01/appdev.920/a96584/oci15r34.htm
}

sword _bindBooleanVariableTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos)
{
	return _bindLongintVariableTowards4D(cursor, pos);
}

sword _bindRealVariableTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos)
{
	return _bindLongintVariableTowards4D(cursor, pos);
}

sword _bindDateVariableTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos)
{
	cursor->defines.at(pos) = 0;
	cursor->itemCount = 1;//this will over write whatever size given for arrays
	
	return OCIDefineByPos(cursor->stmtp, 
						  &cursor->defines.at(pos),
						  cursor->errhp, 
						  pos + 1, 
						  &cursor->dates.at(pos), 
						  sizeof(OCIDate),
						  SQLT_ODT,
						  &cursor->indicators.at(pos),  
						  0,
						  0, 
						  OCI_DEFAULT);	
	//http://docs.oracle.com/cd/A97630_01/appdev.920/a96584/oci15r34.htm
}

sword _bindTimeVariableTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos)
{
	return _bindDateVariableTowards4D(cursor, pos);
}

#pragma mark -

sword _bindAlphaFieldTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos)
{
	return _bindTextVariableTowards4D(cursor, pos);
}

sword _bindTextFieldTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos)
{
	return _bindTextVariableTowards4D(cursor, pos);
}

sword _bindLongintFieldTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos)
{
	return _bindLongintVariableTowards4D(cursor, pos);
}

sword _bindIntegerFieldTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos)
{
	return _bindLongintVariableTowards4D(cursor, pos);
}

sword _bindBooleanFieldTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos)
{
	return _bindLongintVariableTowards4D(cursor, pos);
}

sword _bindRealFieldTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos)
{
	return _bindLongintVariableTowards4D(cursor, pos);
}

sword _bindDateFieldTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos)
{
	return _bindDateVariableTowards4D(cursor, pos);
}

sword _bindTimeFieldTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos)
{
	return _bindDateVariableTowards4D(cursor, pos);
}
