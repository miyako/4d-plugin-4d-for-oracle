#ifndef __ORACLE_ENVIRONMENT_H__
#define __ORACLE_ENVIRONMENT_H__ 1

#include "4DPluginAPI.h"
#include "4DPlugin.h"

// --- OD: Environment
void OD_SET_ENVIRONMENT_VARIABLE(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Get_environment_variable(sLONG_PTR *pResult, PackagePtr pParams);

#endif
