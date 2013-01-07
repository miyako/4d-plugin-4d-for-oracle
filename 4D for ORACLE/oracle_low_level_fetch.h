#ifndef __ORACLE_LOW_LEVEL_FETCH_H__
#define __ORACLE_LOW_LEVEL_FETCH_H__ 1

#include "4DPluginAPI.h"
#include "4DPlugin.h"

#include "oracle_low_level.h"

sword _checkNumberOfRecordsToFetch(ORACLE_SQL_CURSOR *cursor);

sword _fetchDataIntoTextArray(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos, sessionInfo *session);
sword _fetchDataIntoLongintArray(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _fetchDataIntoBooleanArray(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _fetchDataIntoRealArray(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _fetchDataIntoDateArray(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
 
sword _fetchDataIntoTextVariable(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos, sessionInfo *session);
sword _fetchDataIntoLongintVariable(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _fetchDataIntoBooleanVariable(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _fetchDataIntoRealVariable(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _fetchDataIntoDateVariable(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _fetchDataIntoTimeVariable(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);

sword _fetchDataIntoAlphaField(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos, sessionInfo *session);
sword _fetchDataIntoTextField(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos, sessionInfo *session);
sword _fetchDataIntoLongintField(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _fetchDataIntoIntegerField(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _fetchDataIntoBooleanField(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _fetchDataIntoRealField(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _fetchDataIntoDateField(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);
sword _fetchDataIntoTimeField(ORACLE_SQL_CURSOR *cursor, PA_Variable variable, unsigned int pos);

#endif