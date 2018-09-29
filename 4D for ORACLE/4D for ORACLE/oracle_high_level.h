#include "4DPluginAPI.h"
#include "4DPlugin.h"

#include "oracle_control.h"

// --- OD: High Level
void OD_Execute_SQL(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Clone_4D_Table(sLONG_PTR *pResult, PackagePtr pParams);
void OD_COMMIT(sLONG_PTR *pResult, PackagePtr pParams);
void OD_ROLLBACK(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Find_in_context(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Describe_object(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Execute_object(sLONG_PTR *pResult, PackagePtr pParams);