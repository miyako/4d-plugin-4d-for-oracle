#include "oracle_low_level_extra.h"
#include "oracle_low_level.h"

#pragma mark -
#pragma mark constants
#pragma mark -

CUTF8String VARIABLE_TYPE_TEXT((const uint8_t *)"C_TEXT", strlen("C_TEXT"));
CUTF8String VARIABLE_TYPE_REAL((const uint8_t *)"C_REAL", strlen("C_REAL"));
CUTF8String VARIABLE_TYPE_DATE((const uint8_t *)"C_DATE", strlen("C_DATE"));
CUTF8String VARIABLE_TYPE_TIME((const uint8_t *)"C_TIME", strlen("C_TIME"));
CUTF8String VARIABLE_TYPE_BLOB((const uint8_t *)"C_BLOB", strlen("C_BLOB"));
CUTF8String VARIABLE_TYPE_PICTURE((const uint8_t *)"C_PICTURE", strlen("C_PICTURE"));
CUTF8String VARIABLE_TYPE_LONGINT((const uint8_t *)"C_LONGINT", strlen("C_LONGINT"));
CUTF8String VARIABLE_TYPE_BOOLEAN((const uint8_t *)"C_BOOLEAN", strlen("C_BOOLEAN"));
CUTF8String VARIABLE_TYPE_ARRAY_TEXT((const uint8_t *)"ARRAY_TEXT", strlen("ARRAY_TEXT"));
CUTF8String VARIABLE_TYPE_ARRAY_REAL((const uint8_t *)"ARRAY_REAL", strlen("ARRAY_REAL"));
CUTF8String VARIABLE_TYPE_ARRAY_DATE((const uint8_t *)"ARRAY_DATE", strlen("ARRAY_DATE"));
CUTF8String VARIABLE_TYPE_ARRAY_PICTURE((const uint8_t *)"ARRAY_PICTURE", strlen("ARRAY_PICTURE"));
CUTF8String VARIABLE_TYPE_ARRAY_LONGINT((const uint8_t *)"ARRAY_LONGINT", strlen("ARRAY_LONGINT"));
CUTF8String VARIABLE_TYPE_ARRAY_BOOLEAN((const uint8_t *)"ARRAY_BOOLEAN", strlen("ARRAY_BOOLEAN"));

CUTF8String VARIABLE_TYPE_FIELD((const uint8_t *)"[FIELD]", strlen("[FIELD]"));
CUTF8String VARIABLE_TYPE_ERROR((const uint8_t *)"[ERROR]", strlen("[ERROR]"));

CUTF8String VARIABLE_VALUE_NULL((const uint8_t *)"", strlen(""));

CUTF8String BIND_TOWARDS_4D((const uint8_t *)"PARAM_TOWARDS_4D", strlen("PARAM_TOWARDS_4D"));
CUTF8String BIND_TOWARDS_SQL((const uint8_t *)"PARAM_TOWARDS_SQL", strlen("PARAM_TOWARDS_SQL"));

#pragma mark -
#pragma mark messages
#pragma mark -

void _toArrayString(PA_Variable variable, CUTF8String &arrayString)
{	
	char buf[64];
	size_t len;
	
	if(variable.uValue.fArray.fNbElements == 1)
	{
		len = sprintf(buf,"1 element");	
	}else{
		len = sprintf(buf,"%u elements", (unsigned int)variable.uValue.fArray.fNbElements);
	}	
	
	arrayString.assign((const uint8_t *)buf, len);	
}

void _toBooleanString(PA_Variable variable, CUTF8String &booleanString)
{	
	char buf[20];
	size_t len;
	
	len = sprintf(buf,"%u", PA_GetBooleanVariable(variable));
	booleanString.assign((const uint8_t *)buf, len);	
}

void _toLongintString(PA_Variable variable, CUTF8String &longintString)
{
	char buf[20];
	size_t len;
	
	len = sprintf(buf,"%d", (int)PA_GetLongintVariable(variable));
	longintString.assign((const uint8_t *)buf, len);	
}

void _toRealString(PA_Variable variable, CUTF8String &realString)
{
	char buf[20];
	size_t len;
	
	len = sprintf(buf,"%f", PA_GetRealVariable(variable));
	realString.assign((const uint8_t *)buf, len);	
}

void _toTimeString(PA_Variable variable, CUTF8String &timeString)
{
	timeString.clear();
	
	char buf[10];
	size_t len;
	
	unsigned int seconds = (unsigned int)variable.uValue.fTime;
	
	unsigned int hh = seconds / 3600;
	unsigned int mm = (seconds % 3600) / 60;	
	unsigned int ss = (seconds % 3600) % 60;
	
	len = sprintf(buf,"%02u", hh);
	timeString.append((const uint8_t *)buf, len);
	timeString += (const uint8_t *)":";
	
	len = sprintf(buf,"%02u", mm);
	timeString.append((const uint8_t *)buf, len);
	timeString += (const uint8_t *)":";					  
	
	len = sprintf(buf,"%02u", ss);
	timeString.append((const uint8_t *)buf, len);			
}

void _toPictureString(PA_Variable variable, CUTF8String &pictureString)
{
	pictureString.clear();
	
	char buf[64];
	size_t len;
	
	uint32_t i = 0;
	PA_ErrorCode err = eER_NoErr;
	
	while (err == eER_NoErr) 
	{
		PA_Handle h = PA_NewHandle(0);
		PA_Unistring u = PA_GetPictureData(variable.uValue.fPicture, ++i, h);
		err = PA_GetLastError();
		
		if(err == eER_NoErr)
		{
			C_TEXT t;
			t.setUTF16String(&u);
			
			CUTF8String format, formatList;
			t.copyUTF8String(&formatList);
			
			size_t pos = formatList.find(';');
			
			if(pos == CUTF8String::npos)
			{
				format = formatList;
			}else{
				format = formatList.substr(0, pos);
			}
			
			if(pictureString.length())
				pictureString.append((const uint8_t *)", ", 2);
			
			if(PA_GetHandleSize(h) == 1)
			{
				len = sprintf(buf,"1 byte (%s)", format.c_str());	
			}else{
				len = sprintf(buf,"%u bytes (%s)", (unsigned int)PA_GetHandleSize(h), format.c_str());
			}
			
			pictureString.append((const uint8_t *)buf, len);				
		}
		
		PA_DisposeHandle(h);
		
	}
	
}

void _toBlobString(PA_Variable variable, CUTF8String &blobString)
{
	char buf[64];
	size_t len;
	
	if(variable.uValue.fBlob.fSize == 1)
	{
		len = sprintf(buf,"1 byte");	
	}else{
		len = sprintf(buf,"%u bytes", (unsigned int)variable.uValue.fBlob.fSize);
	}
	
	blobString.assign((const uint8_t *)buf, len);
}

void _toFieldString(PA_Variable variable, CUTF8String &fieldString)
{
	char buf[64];
	size_t len;
	
	len = sprintf(buf,"Table %u, Field %u", 
				  variable.uValue.fTableFieldDefinition.fTableNumber,
				  variable.uValue.fTableFieldDefinition.fFieldNumber);	
	
	fieldString.assign((const uint8_t *)buf, len);
	
}

void _toDateString(PA_Variable variable, CUTF8String &dateString)
{
	dateString.clear();
	
	char buf[10];
	size_t len;
	
	len = sprintf(buf,"%02u", (unsigned int)variable.uValue.fDate.fDay);
	dateString.append((const uint8_t *)buf, len);
	dateString += (const uint8_t *)"-";
	
	switch (variable.uValue.fDate.fMonth)
	{
		case 2:
			dateString += (const uint8_t *)"FEB-";
			break;
		case 3:
			dateString += (const uint8_t *)"MAR-";
			break;				
		case 4:
			dateString += (const uint8_t *)"APR-";
			break;					
		case 5:
			dateString += (const uint8_t *)"MAY-";
			break;	
		case 6:
			dateString += (const uint8_t *)"JUN-";
			break;	
		case 7:
			dateString += (const uint8_t *)"JUL-";
			break;					
		case 8:
			dateString += (const uint8_t *)"AUG-";
			break;	
		case 9:
			dateString += (const uint8_t *)"SEP-";
			break;
		case 10:
			dateString += (const uint8_t *)"OCT-";
			break;				
		case 11:
			dateString += (const uint8_t *)"NOV-";
			break;		
		case 12:
			dateString += (const uint8_t *)"DEC-";
			break;						
		default:
			dateString += (const uint8_t *)"JAN-";
			break;
	}
	
	len = sprintf(buf,"%u", (unsigned int)variable.uValue.fDate.fYear);
	dateString.append((const uint8_t *)buf, len);
}

#pragma mark -
#pragma mark command
#pragma mark -

void OD_INTROSPECT_CURSOR(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_TEXT Param2;
	ARRAY_TEXT Param3;
	ARRAY_TEXT Param4;
	ARRAY_TEXT Param5;
	ARRAY_TEXT Param6;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	ORACLE_SQL_CURSOR *cursor = _cursorGet(Param1.getIntValue());
	
	if(cursor)
	{
		Param2.setUTF16String(&cursor->sql);
		Param3.setSize(1);
		Param4.setSize(1);
		Param5.setSize(1);
		Param6.setSize(1);
		
		uint32_t i;
		PA_Unistring u;
		CUTF8String stringValue;
		
		for(i = 0; i < cursor->substitutions.size(); i++)
		{
			PA_Variable variable = cursor->substitutions.at(i);
			
			CUTF16String name = cursor->names.at(i);
			Param6.appendUTF16String(&name);
			
			switch (PA_GetVariableKind(variable))
			{	
				case eVK_PointerToField:
					Param3.appendUTF8String(&VARIABLE_TYPE_FIELD);
					_toFieldString(variable, stringValue);
					Param4.appendUTF8String(&stringValue);
					break;					
					
				case eVK_Integer:						
				case eVK_Longint:
					Param3.appendUTF8String(&VARIABLE_TYPE_LONGINT);
					_toLongintString(variable, stringValue);
					Param4.appendUTF8String(&stringValue);
					break;
					
				case eVK_Real:
					Param3.appendUTF8String(&VARIABLE_TYPE_REAL);
					_toRealString(variable, stringValue);
					Param4.appendUTF8String(&stringValue);				
					break;					
					
				case eVK_Unistring:
					Param3.appendUTF8String(&VARIABLE_TYPE_TEXT);
					u = PA_GetStringVariable(variable);
					Param4.appendUTF16String(&u);				
					break;
					
				case eVK_Boolean:
					Param3.appendUTF8String(&VARIABLE_TYPE_BOOLEAN);
					_toBooleanString(variable, stringValue);
					Param4.appendUTF8String(&stringValue);					
					break;	
					
				case eVK_Date:
					Param3.appendUTF8String(&VARIABLE_TYPE_DATE);					
					_toDateString(variable, stringValue);
					Param4.appendUTF8String(&stringValue);					
					break;	
					
				case eVK_Time:
					Param3.appendUTF8String(&VARIABLE_TYPE_TIME);					
					_toTimeString(variable, stringValue);
					Param4.appendUTF8String(&stringValue);
					break;
					
				case eVK_Picture:
					Param3.appendUTF8String(&VARIABLE_TYPE_PICTURE);	
					_toPictureString(variable, stringValue);
					Param4.appendUTF8String(&stringValue);
					break;
					
				case eVK_Blob:	
					Param3.appendUTF8String(&VARIABLE_TYPE_BLOB);	
					_toBlobString(variable, stringValue);
					Param4.appendUTF8String(&stringValue);					
					break;
					
				case eVK_ArrayUnicode:	
					Param3.appendUTF8String(&VARIABLE_TYPE_ARRAY_TEXT);	
					_toArrayString(variable, stringValue);
					Param4.appendUTF8String(&stringValue);					
					break;
					
				case eVK_ArrayBoolean:	
					Param3.appendUTF8String(&VARIABLE_TYPE_ARRAY_BOOLEAN);	
					_toArrayString(variable, stringValue);
					Param4.appendUTF8String(&stringValue);					
					break;					
					
				case eVK_ArrayPicture:	
					Param3.appendUTF8String(&VARIABLE_TYPE_ARRAY_PICTURE);	
					_toArrayString(variable, stringValue);
					Param4.appendUTF8String(&stringValue);					
					break;						
					
				case eVK_ArrayDate:	
					Param3.appendUTF8String(&VARIABLE_TYPE_ARRAY_DATE);	
					_toArrayString(variable, stringValue);
					Param4.appendUTF8String(&stringValue);					
					break;							
					
				case eVK_ArrayLongint:	
				case eVK_ArrayInteger:						
					Param3.appendUTF8String(&VARIABLE_TYPE_ARRAY_LONGINT);	
					_toArrayString(variable, stringValue);
					Param4.appendUTF8String(&stringValue);					
					break;							
					
				case eVK_ArrayReal:						
					Param3.appendUTF8String(&VARIABLE_TYPE_ARRAY_REAL);	
					_toArrayString(variable, stringValue);
					Param4.appendUTF8String(&stringValue);					
					break;	
					
				default:	
					Param3.appendUTF8String(&VARIABLE_TYPE_ERROR);	
					Param4.appendUTF8String(&VARIABLE_VALUE_NULL);	
					break;	
					
			}
			
			if(cursor->isTowardsSQL.at(i))
			{
				Param5.appendUTF8String(&BIND_TOWARDS_SQL);						
			}else{
				Param5.appendUTF8String(&BIND_TOWARDS_4D);
			}
			
		}		
		
	}
	
	Param2.toParamAtIndex(pParams, 2);
	Param3.toParamAtIndex(pParams, 3);
	Param4.toParamAtIndex(pParams, 4);
	Param5.toParamAtIndex(pParams, 5);	
	Param6.toParamAtIndex(pParams, 6);		
}
