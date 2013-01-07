#include "4DPluginAPI.h"
#include "4DPlugin.h"

// --- OD: Using Context
void OD_Activate_context(sLONG_PTR *pResult, PackagePtr pParams);
void OD_DEACTIVATE_CONTEXT(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Context_state(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Next_in_context(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Previous_in_context(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Number_in_context(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Goto_in_context(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Records_in_context(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Update_in_context(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Insert_in_context(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Delete_in_context(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Load_rows_context(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Reset_context(sLONG_PTR *pResult, PackagePtr pParams);