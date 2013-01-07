#ifndef __ORACLE_CONTROL_H__
#define __ORACLE_CONTROL_H__ 1

#include "4DPluginAPI.h"
#include "4DPlugin.h"

#define PLUGIN_ERROR_LOGIN_INVALID 1001
#define PLUGIN_ERROR_CURSOR_INVALID 1002
#define PLUGIN_ERROR_STATEMENT_INVALID 1084
#define PLUGIN_ERROR_NOT_ENOUGH_MEMORY 1018
#define PLUGIN_ERROR_COLUMN_INVALID 1023
#define PLUGIN_ERROR_CURSOR_NOT_ACTIVE 1005
#define PLUGIN_ERROR_CURSOR_ALREADY_ACTIVE 1006
#define PLUGIN_ERROR_CURSOR_NOT_INACTIVE 1007
#define PLUGIN_ERROR_CURSOR_ALREADY_INACTIVE 1008

void _errorInfoSet(OCIEnv *envhp, OCIError *errhp, uint32_t session, uint32_t cursor, bool isInternal, uint32_t process, uint32_t errorCode);
// --- OD: Control
void OD_CANCEL_LOADING(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Last_error(sLONG_PTR *pResult, PackagePtr pParams);
void OD_ON_ERROR_CALL(sLONG_PTR *pResult, PackagePtr pParams);
void OD_OPEN_DEBUG_WINDOW(sLONG_PTR *pResult, PackagePtr pParams);
void OD_CLOSE_DEBUG_WINDOW(sLONG_PTR *pResult, PackagePtr pParams);
void OD_LOGIN_INFORMATIONS(sLONG_PTR *pResult, PackagePtr pParams);
void OD_MESSAGE_DEBUG(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Check_configuration(sLONG_PTR *pResult, PackagePtr pParams);
void OD_GET_SERVER_LIST(sLONG_PTR *pResult, PackagePtr pParams);

#endif