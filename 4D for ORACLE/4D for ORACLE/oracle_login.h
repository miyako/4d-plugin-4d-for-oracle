#ifndef __ORACLE_LOGIN_H__
#define __ORACLE_LOGIN_H__ 1

#include "4DPluginAPI.h"
#include "4DPlugin.h"

#include "oracle_control.h"

// --- OD: Login
void OD_Login(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Login_dialog(sLONG_PTR *pResult, PackagePtr pParams);
void OD_LOGOUT(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Login_state(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Clone_table(sLONG_PTR *pResult, PackagePtr pParams);

#endif