#ifndef __ORACLE_LOW_LEVEL_BIND_O_H__
#define __ORACLE_LOW_LEVEL_BIND_O_H__ 1

#include "4DPluginAPI.h"
#include "4DPlugin.h"

#include "oracle_low_level.h"

/*
 
 function to bind data received from oracle

 for text we use OCIString object (OCI_UTF16|OCI_OBJECT must be specified at OCIEnvCreate)
 for date/time we use OCIDate structure
 for real/longint/boolean we use OCINumber structure
 
 */

sword _bindTextArrayTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos);
sword _bindLongintArrayTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos);
sword _bindBooleanArrayTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos);
sword _bindRealArrayTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos);
sword _bindDateArrayTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos);

sword _bindTextVariableTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos);
sword _bindLongintVariableTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos);
sword _bindBooleanVariableTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos);
sword _bindRealVariableTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos);
sword _bindDateVariableTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos);
sword _bindTimeVariableTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos);
sword _bindBlobVariableTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos);

sword _bindAlphaFieldTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos);
sword _bindTextFieldTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos);
sword _bindLongintFieldTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos);
sword _bindIntegerFieldTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos);
sword _bindBooleanFieldTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos);
sword _bindRealFieldTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos);
sword _bindDateFieldTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos);
sword _bindTimeFieldTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos);
sword _bindBlobFieldTowards4D(ORACLE_SQL_CURSOR *cursor, unsigned int pos);

#endif