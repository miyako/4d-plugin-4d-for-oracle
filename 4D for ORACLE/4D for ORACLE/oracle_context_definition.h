#include "4DPluginAPI.h"
#include "4DPlugin.h"

// --- OD: Context Definition
void OD_Create_context(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Create_context_dialog(sLONG_PTR *pResult, PackagePtr pParams);
void OD_DROP_CONTEXT(sLONG_PTR *pResult, PackagePtr pParams);
void OD_ADD_TO_CONTEXT(sLONG_PTR *pResult, PackagePtr pParams);
void OD_EDIT_CLAUSES_IN_CONTEXT(sLONG_PTR *pResult, PackagePtr pParams);
void OD_SET_CLAUSE_IN_CONTEXT(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Get_clause_in_context(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Save_context_picture(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Open_context_picture(sLONG_PTR *pResult, PackagePtr pParams);
void OD_SAVE_CONTEXT_FILE(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Open_context_file(sLONG_PTR *pResult, PackagePtr pParams);