#include "oracle_preferences.h"


// -------------------------------- OD: Preferences -------------------------------


#pragma mark -
#pragma mark typedef
#pragma mark -

CUTF8String DATA_TYPE_ALPHA((const uint8_t *)"ALPHA", strlen("ALPHA"));
CUTF8String DATA_TYPE_REAL((const uint8_t *)"REAL", strlen("REAL"));
CUTF8String DATA_TYPE_TEXT((const uint8_t *)"TEXT", strlen("TEXT"));
CUTF8String DATA_TYPE_DATE((const uint8_t *)"DATE", strlen("DATE"));
CUTF8String DATA_TYPE_BOOLEAN((const uint8_t *)"BOOLEAN", strlen("BOOLEAN"));
CUTF8String DATA_TYPE_INTEGER((const uint8_t *)"INTEGER", strlen("INTEGER"));
CUTF8String DATA_TYPE_LONGINT((const uint8_t *)"LONGINT", strlen("LONGINT"));
CUTF8String DATA_TYPE_TIME((const uint8_t *)"TIME", strlen("TIME"));

CUTF8String NULL_TYPE_ALPHA((const uint8_t *)"", strlen(""));
CUTF8String NULL_TYPE_REAL((const uint8_t *)"0", strlen("0"));
CUTF8String NULL_TYPE_TEXT((const uint8_t *)"", strlen(""));
CUTF8String NULL_TYPE_DATE((const uint8_t *)"0000.00.00", strlen("0000.00.00"));
CUTF8String NULL_TYPE_BOOLEAN((const uint8_t *)"0", strlen("0"));
CUTF8String NULL_TYPE_INTEGER((const uint8_t *)"0", strlen("0"));
CUTF8String NULL_TYPE_LONGINT((const uint8_t *)"0", strlen("0"));
CUTF8String NULL_TYPE_TIME((const uint8_t *)"00:00:00", strlen("00:00:00"));

bool _isNullMapAlphaUsed = false;
bool _isNullMapRealUsed = false;
bool _isNullMapTextUsed = false;
bool _isNullMapDateUsed = false;
bool _isNullMapBooleanUsed = false;
bool _isNullMapIntegerUsed = false;
bool _isNullMapLongintUsed = false;
bool _isNullMapTimeUsed = false;

#pragma mark -
#pragma mark null value management
#pragma mark -

bool isTextArrayElementValueNull(PA_Variable variable, unsigned int i)
{
	bool isNull = false;
	
	if(_isNullMapTextUsed)
	{	
		PA_Unistring u = PA_GetStringInArray(variable, i);
		
		C_TEXT temp;
		temp.setUTF16String(&u);
		
		CUTF8String str;
		temp.copyUTF8String(&str);
		
		if(str == NULL_TYPE_TEXT)
			isNull = true;
	}
	
	return isNull;	

}

bool isLongintArrayElementValueNull(PA_Variable variable, unsigned int i)
{
	bool isNull = false;
	
	if(_isNullMapLongintUsed)
	{	
		char buf[20];
		size_t len;
		
		len = sprintf(buf,"%d", (int)PA_GetLongintInArray(variable, i));
		
		CUTF8String str((const uint8_t *)buf, len);
		
		if(str == NULL_TYPE_LONGINT)
			isNull = true;
	}
	
	return isNull;	
}

bool isBooleanArrayElementValueNull(PA_Variable variable, unsigned int i)
{
	bool isNull = false;
	
	if(_isNullMapBooleanUsed)
	{	
		char buf[20];
		size_t len;
		
		len = sprintf(buf,"%u", (int)PA_GetBooleanInArray(variable, i));
		
		CUTF8String str((const uint8_t *)buf, len);
		
		if(str == NULL_TYPE_BOOLEAN)
			isNull = true;
	}
	
	return isNull;		
}

bool isDateArrayElementValueNull(PA_Variable variable, unsigned int i)
{
	bool isNull = false;
	
	if(_isNullMapDateUsed)
	{	
		char buf[20];
		size_t len;
		
		short d; 
		short m; 
		short y;	
		
		PA_GetDateInArray(variable, i, &d, &m, &y);
	
		len = sprintf(buf,"%04u", y);
		CUTF8String str((const uint8_t *)buf, len);
		str += (const uint8_t *)".";
		
		len = sprintf(buf,"%02u", m);
		str.append((const uint8_t *)buf, len);
		str += (const uint8_t *)".";					  
		
		len = sprintf(buf,"%02u", d);
		str.append((const uint8_t *)buf, len);
		
		if(str == NULL_TYPE_DATE)
			isNull = true;
	}
	
	return isNull;
}

bool isRealArrayElementValueNull(PA_Variable variable, unsigned int i)
{
	bool isNull = false;
	
	if(_isNullMapRealUsed)
	{	
		char buf[20];
		size_t len;
		
		len = sprintf(buf,"%f", PA_GetRealInArray(variable, i));
		
		CUTF8String str((const uint8_t *)buf, len);
		
		if(str == NULL_TYPE_REAL)
			isNull = true;
	}
	
	return isNull;	
}

#pragma mark -

void setTextArrayValueNull(PA_Variable variable, unsigned int i)
{	
	C_TEXT temp;
	
	if(_isNullMapTextUsed)
	{
		temp.setUTF8String(&NULL_TYPE_TEXT);
	}
	
	PA_Unistring u = PA_CreateUnistring((PA_Unichar *)temp.getUTF16StringPtr());
	
	PA_SetStringInArray(variable, i, &u);
}

void setLongintArrayValueNull(PA_Variable variable, unsigned int i)
{
	if(_isNullMapLongintUsed)
	{
		
		PA_SetLongintInArray(variable, i, atoi((const char *)NULL_TYPE_LONGINT.c_str()));
		
	}else{
		
		PA_SetLongintInArray(variable, i, 0);
		
	}		
}

void setBooleanArrayValueNull(PA_Variable variable, unsigned int i)
{
	if(_isNullMapBooleanUsed)
	{
		
		PA_SetBooleanInArray(variable, i, atoi((const char *)NULL_TYPE_LONGINT.c_str()));
		
	}else{
		
		PA_SetBooleanInArray(variable, i, 0);
		
	}
}

void setRealArrayValueNull(PA_Variable variable, unsigned int i)
{
	if(_isNullMapRealUsed)
	{
		
		PA_SetRealInArray(variable, i, atof((const char *)NULL_TYPE_LONGINT.c_str()));
		
	}else{
		
		PA_SetRealInArray(variable, i, 0);
		
	}		
}

void setDateArrayValueNull(PA_Variable variable, unsigned int i)
{
	if(_isNullMapDateUsed)
	{
		
		unsigned int y = atoi((const char *)NULL_TYPE_DATE.substr(0,4).c_str());
		unsigned int m = atoi((const char *)NULL_TYPE_DATE.substr(5,2).c_str());		
		unsigned int d = atoi((const char *)NULL_TYPE_DATE.substr(8,2).c_str());	
		
		PA_SetDateInArray(variable, i, d, m, y);
		
	}else{
		
		PA_SetDateInArray(variable, i, 0, 0, 0);
		
	}		
}

#pragma mark -

bool isTextVariableValueNull(PA_Variable variable)
{
	bool isNull = false;
	
	if(_isNullMapTextUsed)
	{	
		PA_Unistring u = PA_GetStringVariable(variable);
		
		C_TEXT temp;
		temp.setUTF16String(&u);
		
		CUTF8String str;
		temp.copyUTF8String(&str);
		
		if(str == NULL_TYPE_TEXT)
			isNull = true;
	}
	
	return isNull;	
}

bool isLongintVariableValueNull(PA_Variable variable)
{
	bool isNull = false;
	
	if(_isNullMapLongintUsed)
	{	
		char buf[20];
		size_t len;
		
		len = sprintf(buf,"%d", (int)PA_GetLongintVariable(variable));
		
		CUTF8String str((const uint8_t *)buf, len);
		
		if(str == NULL_TYPE_LONGINT)
			isNull = true;
	}
	
	return isNull;	
}

bool isBooleanVariableValueNull(PA_Variable variable)
{
	bool isNull = false;
	
	if(_isNullMapBooleanUsed)
	{	
		char buf[20];
		size_t len;
		
		len = sprintf(buf,"%u", (int)PA_GetBooleanVariable(variable));
		
		CUTF8String str((const uint8_t *)buf, len);
		
		if(str == NULL_TYPE_BOOLEAN)
			isNull = true;
	}
	
	return isNull;		
}

bool isRealVariableValueNull(PA_Variable variable)
{
	bool isNull = false;
	
	if(_isNullMapRealUsed)
	{	
		
		char buf[20];
		size_t len;
		
		len = sprintf(buf,"%f", PA_GetRealVariable(variable));
		
		CUTF8String str((const uint8_t *)buf, len);
		
		if(str == NULL_TYPE_REAL)
			isNull = true;
		
	}
	
	return isNull;	
}

bool isDateVariableValueNull(PA_Variable variable)
{
	bool isNull = false;
	
	if(_isNullMapDateUsed)
	{	
		char buf[10];
		size_t len;
		
		short d; 
		short m; 
		short y;
		
		PA_GetDateVariable(variable, &d, &m, &y);
		
		len = sprintf(buf,"%04u", y);
		CUTF8String str((const uint8_t *)buf, len);
		str += (const uint8_t *)".";
		
		len = sprintf(buf,"%02u", m);
		str.append((const uint8_t *)buf, len);
		str += (const uint8_t *)".";					  
		
		len = sprintf(buf,"%02u", d);
		str.append((const uint8_t *)buf, len);
		
		if(str == NULL_TYPE_DATE)
			isNull = true;
	}
	
	return isNull;	
}

bool isTimeVariableValueNull(PA_Variable variable)
{
	bool isNull = false;
	
	if(_isNullMapTimeUsed)
	{	
		
		char buf[20];
		size_t len;
		
		unsigned int seconds = (unsigned int)PA_GetTimeVariable(variable);
		
		unsigned int h = seconds / 3600;
		unsigned int m = (seconds % 3600) / 60;	
		unsigned int s = (seconds % 3600) % 60;
		
		len = sprintf(buf,"%02u", h);
		CUTF8String str((const uint8_t *)buf, len);
		str += (const uint8_t *)":";
		
		len = sprintf(buf,"%02u", m);
		str.append((const uint8_t *)buf, len);
		str += (const uint8_t *)":";					  
		
		len = sprintf(buf,"%02u", s);
		str.append((const uint8_t *)buf, len);
		
		if(str == NULL_TYPE_TIME)
			isNull = true;
	}
	
	return isNull;	
}

#pragma mark -

void setTextVariableValueNull(PA_Variable variable)
{
	PA_Unistring u = PA_GetStringVariable(variable);
	
	C_TEXT temp;
	
	if(_isNullMapTextUsed)
	{
		temp.setUTF8String(&NULL_TYPE_TEXT);
	}
	
	PA_SetUnistring(&u, (PA_Unichar *)temp.getUTF16StringPtr());
	PA_SetStringVariable(&variable, &u);
}

void setLongintVariableValueNull(PA_Variable variable)
{
	if(_isNullMapLongintUsed)
	{
		
		PA_SetLongintVariable(&variable, atoi((const char *)NULL_TYPE_LONGINT.c_str()));
		
	}else{
		
		PA_SetLongintVariable(&variable, 0);
		
	}		
}

void setBooleanVariableValueNull(PA_Variable variable)
{
	if(_isNullMapBooleanUsed)
	{
		
		PA_SetBooleanVariable(&variable, atoi((const char *)NULL_TYPE_LONGINT.c_str()));
		
	}else{
		
		PA_SetBooleanVariable(&variable, 0);
		
	}		
}

void setRealVariableValueNull(PA_Variable variable)
{
	if(_isNullMapRealUsed)
	{
		
		PA_SetRealVariable(&variable, atof((const char *)NULL_TYPE_LONGINT.c_str()));
							  
	}else{
		
		PA_SetRealVariable(&variable, 0);
		
	}		
}

void setDateVariableValueNull(PA_Variable variable)
{
	if(_isNullMapDateUsed)
	{
		unsigned int y = atoi((const char *)NULL_TYPE_DATE.substr(0,4).c_str());
		unsigned int m = atoi((const char *)NULL_TYPE_DATE.substr(5,2).c_str());		
		unsigned int d = atoi((const char *)NULL_TYPE_DATE.substr(8,2).c_str());		
		
		PA_SetDateVariable(&variable, d, m, y);
		
	}else{
		
		PA_SetDateVariable(&variable, 0, 0, 0);
		
	}	
}

void setTimeVariableValueNull(PA_Variable variable)
{
	if(_isNullMapTimeUsed)
	{
		unsigned int h = atoi((const char *)NULL_TYPE_DATE.substr(0,2).c_str());
		unsigned int m = atoi((const char *)NULL_TYPE_DATE.substr(3,2).c_str());
		unsigned int s = atoi((const char *)NULL_TYPE_DATE.substr(6,2).c_str());
		
		PA_SetTimeVariable(&variable, s + (m * 60) + (h * 3600));

	}else{
		
		PA_SetTimeVariable(&variable, 0);
		
	}		
}

#pragma mark -

bool isAlphaFieldValueNull(PA_Variable variable)
{
	bool isNull = false;
	
	if(_isNullMapAlphaUsed)
	{
		PA_Unistring u = PA_GetStringField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber);
		
		C_TEXT temp;
		temp.setUTF16String(&u);
		
		CUTF8String str;
		temp.copyUTF8String(&str);
		
		if(str == NULL_TYPE_ALPHA)
			isNull = true;
		
	}else{
		
		isNull = PA_GetBooleanVariable(PA_ExecuteCommandByID(CMD_Is_field_value_Null, &variable, 1));
		
	}
	
	return isNull;
}

bool isTextFieldValueNull(PA_Variable variable)
{
	bool isNull = false;
	
	if(_isNullMapTextUsed)
	{
		PA_Unistring u = PA_GetStringField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber);
		
		C_TEXT temp;
		temp.setUTF16String(&u);
		
		CUTF8String str;
		temp.copyUTF8String(&str);
		
		if(str == NULL_TYPE_TEXT)
			isNull = true;
		
	}else{
		
		isNull = PA_GetBooleanVariable(PA_ExecuteCommandByID(CMD_Is_field_value_Null, &variable, 1));
		
	}
	
	return isNull;
}

bool isLongintFieldValueNull(PA_Variable variable)
{
	bool isNull = false;
	
	if(_isNullMapLongintUsed)
	{		
		char buf[20];
		size_t len;
		
		len = sprintf(buf,"%d", (int)PA_GetLongintField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber));
		
		CUTF8String str((const uint8_t *)buf, len);	
		
		if(str == NULL_TYPE_LONGINT)
			isNull = true;
		
	}else{
		
		isNull = PA_GetBooleanVariable(PA_ExecuteCommandByID(CMD_Is_field_value_Null, &variable, 1));
		
	}
	
	return isNull;
}

bool isIntegerFieldValueNull(PA_Variable variable)
{
	bool isNull = false;
	
	if(_isNullMapIntegerUsed)
	{		
		char buf[20];
		size_t len;
		
		len = sprintf(buf,"%d", PA_GetIntegerField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber));
		
		CUTF8String str((const uint8_t *)buf, len);	
		
		if(str == NULL_TYPE_INTEGER)
			isNull = true;
		
	}else{
		
		isNull = PA_GetBooleanVariable(PA_ExecuteCommandByID(CMD_Is_field_value_Null, &variable, 1));
		
	}
	
	return isNull;
}

bool isBooleanFieldValueNull(PA_Variable variable)
{
	bool isNull = false;
	
	if(_isNullMapBooleanUsed)
	{		
		char buf[20];
		size_t len;
		
		len = sprintf(buf,"%u", PA_GetBooleanField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber));
		
		CUTF8String str((const uint8_t *)buf, len);	
		
		if(str == NULL_TYPE_BOOLEAN)
			isNull = true;
		
	}else{
		
		isNull = PA_GetBooleanVariable(PA_ExecuteCommandByID(CMD_Is_field_value_Null, &variable, 1));
		
	}
	
	return isNull;
}

bool isRealFieldValueNull(PA_Variable variable)
{
	bool isNull = false;
	
	if(_isNullMapRealUsed)
	{		
		char buf[20];
		size_t len;
		
		len = sprintf(buf,"%f", PA_GetRealField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber));
		
		CUTF8String str((const uint8_t *)buf, len);	
		
		if(str == NULL_TYPE_REAL)
			isNull = true;
		
	}else{
		
		isNull = PA_GetBooleanVariable(PA_ExecuteCommandByID(CMD_Is_field_value_Null, &variable, 1));
		
	}
	
	return isNull;
}

bool isDateFieldValueNull(PA_Variable variable)
{
	bool isNull = false;
	
	if(_isNullMapDateUsed)
	{		
		char buf[20];
		size_t len;
		
		short d; 
		short m; 
		short y;
		
		PA_GetDateField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber, &d, &m, &y);
		
		len = sprintf(buf,"%04u", y);
		CUTF8String str((const uint8_t *)buf, len);
		str += (const uint8_t *)".";
		
		len = sprintf(buf,"%02u", m);
		str.append((const uint8_t *)buf, len);
		str += (const uint8_t *)".";					  
		
		len = sprintf(buf,"%02u", d);
		str.append((const uint8_t *)buf, len);
		
		if(str == NULL_TYPE_DATE)
			isNull = true;
		
	}else{
		
		isNull = PA_GetBooleanVariable(PA_ExecuteCommandByID(CMD_Is_field_value_Null, &variable, 1));
		
	}
	
	return isNull;
}

bool isTimeFieldValueNull(PA_Variable variable)
{
	bool isNull = false;
	
	if(_isNullMapTimeUsed)
	{		
		char buf[10];
		size_t len;
		
		unsigned int seconds = PA_GetTimeField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber);
		
		unsigned int h = seconds / 3600;
		unsigned int m = (seconds % 3600) / 60;	
		unsigned int s = (seconds % 3600) % 60;
		
		len = sprintf(buf,"%02u", h);
		CUTF8String str((const uint8_t *)buf, len);
		str += (const uint8_t *)":";
		
		len = sprintf(buf,"%02u", m);
		str.append((const uint8_t *)buf, len);
		str += (const uint8_t *)":";					  
		
		len = sprintf(buf,"%02u", s);
		str.append((const uint8_t *)buf, len);	
		
		if(str == NULL_TYPE_TIME)
			isNull = true;
		
	}else{
		
		isNull = PA_GetBooleanVariable(PA_ExecuteCommandByID(CMD_Is_field_value_Null, &variable, 1));
		
	}
	
	return isNull;
}

bool isBlobFieldValueNull(PA_Variable variable)
{		
	return PA_GetBooleanVariable(PA_ExecuteCommandByID(CMD_Is_field_value_Null, &variable, 1));
}

#pragma mark -

void setAlphaFieldValueNull(PA_Variable variable)
{
	C_TEXT temp;
	
	if(_isNullMapTextUsed)
	{
		temp.setUTF8String(&NULL_TYPE_TEXT);
	}else{
		PA_ExecuteCommandByID(CMD_SET_FIELD_VALUE_NULL, &variable, 1);
	}
	
}

void setTextFieldValueNull(PA_Variable variable)
{
	setAlphaFieldValueNull(variable);
}

void setLongintFieldValueNull(PA_Variable variable)
{
	if(_isNullMapLongintUsed)
	{
		PA_SetLongintField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber, atoi((const char *)NULL_TYPE_LONGINT.c_str()));
	}else{
		PA_ExecuteCommandByID(CMD_SET_FIELD_VALUE_NULL, &variable, 1);
	}		
}

void setIntegerFieldValueNull(PA_Variable variable)
{
	if(_isNullMapIntegerUsed)
	{
		PA_SetIntegerField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber, atoi((const char *)NULL_TYPE_INTEGER.c_str()));
	}else{
		PA_ExecuteCommandByID(CMD_SET_FIELD_VALUE_NULL, &variable, 1);
	}		
}

void setBooleanFieldValueNull(PA_Variable variable)
{
	if(_isNullMapBooleanUsed)
	{
		PA_SetBooleanField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber, atoi((const char *)NULL_TYPE_BOOLEAN.c_str()));
	}else{
		PA_ExecuteCommandByID(CMD_SET_FIELD_VALUE_NULL, &variable, 1);
	}		
}

void setRealFieldValueNull(PA_Variable variable)
{
	if(_isNullMapRealUsed)
	{
		PA_SetRealField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber, atof((const char *)NULL_TYPE_REAL.c_str()));
	}else{
		PA_ExecuteCommandByID(CMD_SET_FIELD_VALUE_NULL, &variable, 1);
	}		
}

void setDateFieldValueNull(PA_Variable variable)
{
	if(_isNullMapDateUsed)
	{
		unsigned int y = atoi((const char *)NULL_TYPE_DATE.substr(0,4).c_str());
		unsigned int m = atoi((const char *)NULL_TYPE_DATE.substr(5,2).c_str());		
		unsigned int d = atoi((const char *)NULL_TYPE_DATE.substr(8,2).c_str());		
		
		PA_SetDateField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber, d, m, y);
	}else{
		PA_ExecuteCommandByID(CMD_SET_FIELD_VALUE_NULL, &variable, 1);
	}		
}

void setTimeFieldValueNull(PA_Variable variable)
{
	if(_isNullMapTimeUsed)
	{
		unsigned int h = atoi((const char *)NULL_TYPE_DATE.substr(0,2).c_str());
		unsigned int m = atoi((const char *)NULL_TYPE_DATE.substr(3,2).c_str());
		unsigned int s = atoi((const char *)NULL_TYPE_DATE.substr(6,2).c_str());
		
		PA_SetTimeField(variable.uValue.fTableFieldDefinition.fTableNumber, variable.uValue.fTableFieldDefinition.fFieldNumber, s + (m * 60) + (h * 3600));
	}else{
		PA_ExecuteCommandByID(CMD_SET_FIELD_VALUE_NULL, &variable, 1);
	}			
}

void setBlobFieldValueNull(PA_Variable variable)
{
	PA_ExecuteCommandByID(CMD_SET_FIELD_VALUE_NULL, &variable, 1);
}

#pragma mark -
#pragma mark commands
#pragma mark -

void OD_SET_NULL_VALUE(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_TEXT Param2;
	C_LONGINT Param3;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
	
	bool unset = Param3.getIntValue();
	
	switch (Param1.getIntValue())
	{
		case OD_ALPHA:
			_isNullMapAlphaUsed = !(unset);
			Param2.copyUTF8String(&NULL_TYPE_ALPHA);
			break;
			
		case OD_REAL:
			_isNullMapRealUsed = !(unset);
			Param2.copyUTF8String(&NULL_TYPE_REAL);
			break;
			
		case OD_TEXT:
			_isNullMapTextUsed = !(unset);
			Param2.copyUTF8String(&NULL_TYPE_TEXT);
			break;		
			
		case OD_BOOLEAN:
			_isNullMapBooleanUsed = !(unset);
			Param2.copyUTF8String(&NULL_TYPE_BOOLEAN);
			break;	
			
		case OD_INTEGER:
			_isNullMapIntegerUsed = !(unset);
			Param2.copyUTF8String(&NULL_TYPE_INTEGER);
			break;			
			
		case OD_LONGINT:
			_isNullMapLongintUsed = !(unset);
			Param2.copyUTF8String(&NULL_TYPE_LONGINT);
			break;
			
		case OD_DATE:
			_isNullMapDateUsed = !(unset);
			if(_isNullMapDateUsed)
			{
				CUTF8String u;
				Param2.copyUTF8String(&u);
				if(u.length() == strlen("0000.00.00") && (u.at(4) == '.') && (u.at(7) == '.'))
					NULL_TYPE_DATE = u;	
			}
			break;				
		case OD_TIME:
			_isNullMapTimeUsed = !(unset);
			if(_isNullMapTimeUsed)
			{
				CUTF8String u;
				Param2.copyUTF8String(&u);
				if(u.length() == strlen("00:00:00") && (u.at(2) == ':') && (u.at(5) == ':'))
					NULL_TYPE_TIME = u;	
			}
			break;				
		default:
			break;
	}
	
}

#pragma mark -
#pragma mark deprecated commands
#pragma mark -

void OD_SET_OPTIONS(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT Param2;
	C_LONGINT Param3;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
	
	// --- write the code of OD_SET_OPTIONS here...
	
}

void OD_Get_Options(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	// --- write the code of OD_Get_Options here...
	
	returnValue.setReturn(pResult);
}

void OD_LOAD_STRUCTURE(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT Param2;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	
	// --- write the code of OD_LOAD_STRUCTURE here...
	
}

void OD_SET_CONFIGURATION_FILE(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	// --- write the code of OD_SET_CONFIGURATION_FILE here...
	
}

void OD_SET_NB_MODE(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT Param2;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	
	// --- write the code of OD_SET_NB_MODE here...
	
}

void OD_Get_NB_mode(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	// --- write the code of OD_Get_NB_mode here...
	
	returnValue.setReturn(pResult);
}

#pragma mark -
#pragma mark debug tool
#pragma mark -

void OD_INTROSPECT_NULL_VALUE_MAP(sLONG_PTR *pResult, PackagePtr pParams)
{
	ARRAY_TEXT Param1;
	ARRAY_TEXT Param2;
	
	Param1.setSize(1);
	Param2.setSize(1);
	
	if(_isNullMapAlphaUsed)
	{
		Param1.appendUTF8String(&DATA_TYPE_ALPHA);		
		Param2.appendUTF8String(&NULL_TYPE_ALPHA);
	}
	
	if(_isNullMapRealUsed)
	{
		Param1.appendUTF8String(&DATA_TYPE_REAL);		
		Param2.appendUTF8String(&NULL_TYPE_REAL);
	}	
	
	if(_isNullMapTextUsed)
	{
		Param1.appendUTF8String(&DATA_TYPE_TEXT);		
		Param2.appendUTF8String(&NULL_TYPE_TEXT);
	}		
	
	if(_isNullMapDateUsed)
	{
		Param1.appendUTF8String(&DATA_TYPE_DATE);		
		Param2.appendUTF8String(&NULL_TYPE_DATE);
	}	
	
	if(_isNullMapBooleanUsed)
	{
		Param1.appendUTF8String(&DATA_TYPE_BOOLEAN);		
		Param2.appendUTF8String(&NULL_TYPE_BOOLEAN);
	}		

	if(_isNullMapIntegerUsed)
	{
		Param1.appendUTF8String(&DATA_TYPE_INTEGER);		
		Param2.appendUTF8String(&NULL_TYPE_INTEGER);
	}	
	
	if(_isNullMapLongintUsed)
	{
		Param1.appendUTF8String(&DATA_TYPE_LONGINT);		
		Param2.appendUTF8String(&NULL_TYPE_LONGINT);
	}	

	if(_isNullMapTimeUsed)
	{
		Param1.appendUTF8String(&DATA_TYPE_TIME);		
		Param2.appendUTF8String(&NULL_TYPE_TIME);
	}	
	
	Param1.toParamAtIndex(pParams, 1);
	Param2.toParamAtIndex(pParams, 2);
}
