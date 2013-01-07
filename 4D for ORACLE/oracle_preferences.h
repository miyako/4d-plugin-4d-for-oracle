#ifndef __ORACLE_PREFERENCES_H__
#define __ORACLE_PREFERENCES_H__ 1

#include "4DPluginAPI.h"
#include "4DPlugin.h"

#define OD_ALPHA 0
#define OD_REAL 1
#define OD_TEXT 2
#define OD_DATE 4
#define OD_BOOLEAN 6
#define OD_INTEGER 8
#define OD_LONGINT 9
#define OD_TIME 11

#define CMD_Is_field_value_Null 964
#define CMD_SET_FIELD_VALUE_NULL 965

bool isTextArrayElementValueNull(PA_Variable variable, unsigned int i);
bool isLongintArrayElementValueNull(PA_Variable variable, unsigned int i);
bool isBooleanArrayElementValueNull(PA_Variable variable, unsigned int i);
bool isRealArrayElementValueNull(PA_Variable variable, unsigned int i);
bool isDateArrayElementValueNull(PA_Variable variable, unsigned int i);

void setTextArrayValueNull(PA_Variable variable, unsigned int i);
void setLongintArrayValueNull(PA_Variable variable, unsigned int i);
void setRealArrayValueNull(PA_Variable variable, unsigned int i);
void setBooleanArrayValueNull(PA_Variable variable, unsigned int i);
void setDateArrayValueNull(PA_Variable variable, unsigned int i);

bool isTextVariableValueNull(PA_Variable variable);
bool isLongintVariableValueNull(PA_Variable variable);
bool isBooleanVariableValueNull(PA_Variable variable);
bool isRealVariableValueNull(PA_Variable variable);
bool isDateVariableValueNull(PA_Variable variable);
bool isTimeVariableValueNull(PA_Variable variable);

void setTextVariableValueNull(PA_Variable variable);
void setLongintVariableValueNull(PA_Variable variable);
void setBooleanVariableValueNull(PA_Variable variable);
void setRealVariableValueNull(PA_Variable variable);
void setDateVariableValueNull(PA_Variable variable);
void setTimeVariableValueNull(PA_Variable variable);

bool isAlphaFieldValueNull(PA_Variable variable);
bool isTextFieldValueNull(PA_Variable variable);
bool isLongintFieldValueNull(PA_Variable variable);
bool isIntegerFieldValueNull(PA_Variable variable);
bool isBooleanFieldValueNull(PA_Variable variable);
bool isRealFieldValueNull(PA_Variable variable);
bool isDateFieldValueNull(PA_Variable variable);
bool isTimeFieldValueNull(PA_Variable variable);

void setAlphaFieldValueNull(PA_Variable variable);
void setTextFieldValueNull(PA_Variable variable);
void setLongintFieldValueNull(PA_Variable variable);
void setIntegerFieldValueNull(PA_Variable variable);
void setBooleanFieldValueNull(PA_Variable variable);
void setRealFieldValueNull(PA_Variable variable);
void setDateFieldValueNull(PA_Variable variable);
void setTimeFieldValueNull(PA_Variable variable);

// --- OD: Preferences
void OD_SET_OPTIONS(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Get_Options(sLONG_PTR *pResult, PackagePtr pParams);
void OD_SET_NULL_VALUE(sLONG_PTR *pResult, PackagePtr pParams);
void OD_LOAD_STRUCTURE(sLONG_PTR *pResult, PackagePtr pParams);
void OD_SET_CONFIGURATION_FILE(sLONG_PTR *pResult, PackagePtr pParams);
void OD_SET_NB_MODE(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Get_NB_mode(sLONG_PTR *pResult, PackagePtr pParams);

void OD_INTROSPECT_NULL_VALUE_MAP(sLONG_PTR *pResult, PackagePtr pParams);

#endif