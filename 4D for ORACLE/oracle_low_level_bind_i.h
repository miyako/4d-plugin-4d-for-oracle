#ifndef __ORACLE_LOW_LEVEL_BIND_I_H__
#define __ORACLE_LOW_LEVEL_BIND_I_H__ 1

#include "4DPluginAPI.h"
#include "4DPlugin.h"

#include "oracle_low_level.h"

/*
 
 function to bind data sent to oracle

 for text we use OCIString object (OCI_UTF16|OCI_OBJECT must be specified at OCIEnvCreate)
 for date/time we use OCIDate structure
 for real/longint/boolean we use OCINumber structure
 
 */

sword _bindTextArrayTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos, sessionInfo *session);
sword _bindLongintArrayTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _bindBooleanArrayTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _bindRealArrayTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _bindDateArrayTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);

sword _bindAlphaFieldTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _bindTextFieldTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _bindLongintFieldTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _bindIntegerFieldTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _bindBooleanFieldTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _bindRealFieldTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _bindDateFieldTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _bindTimeFieldTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _bindBlobFieldTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);

sword _bindTextVariableTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _bindLongintVariableTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _bindBooleanVariableTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _bindRealVariableTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _bindDateVariableTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _bindTimeVariableTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _bindBlobVariableTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _bindLongRawBlobVariableTowardsSQL(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);

#endif