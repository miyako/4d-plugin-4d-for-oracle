#include "oracle_using_context.h"


// ------------------------------- OD: Using Context ------------------------------


#pragma mark -
#pragma mark deprecated commands
#pragma mark -

void OD_Activate_context(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT Param2;
	C_LONGINT Param3;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
	
	// --- write the code of OD_Activate_context here...
	
	returnValue.setReturn(pResult);
}

void OD_DEACTIVATE_CONTEXT(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	// --- write the code of OD_DEACTIVATE_CONTEXT here...
	
}

void OD_Context_state(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	// --- write the code of OD_Context_state here...
	
	returnValue.setReturn(pResult);
}

void OD_Next_in_context(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT Param2;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	
	// --- write the code of OD_Next_in_context here...
	
	returnValue.setReturn(pResult);
}

void OD_Previous_in_context(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT Param2;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	
	// --- write the code of OD_Previous_in_context here...
	
	returnValue.setReturn(pResult);
}

void OD_Number_in_context(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	// --- write the code of OD_Number_in_context here...
	
	returnValue.setReturn(pResult);
}

void OD_Goto_in_context(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT Param2;
	C_LONGINT Param3;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
	
	// --- write the code of OD_Goto_in_context here...
	
	returnValue.setReturn(pResult);
}

void OD_Records_in_context(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	// --- write the code of OD_Records_in_context here...
	
	returnValue.setReturn(pResult);
}

void OD_Update_in_context(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT Param2;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	
	// --- write the code of OD_Update_in_context here...
	
	returnValue.setReturn(pResult);
}

void OD_Insert_in_context(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT Param2;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	
	// --- write the code of OD_Insert_in_context here...
	
	returnValue.setReturn(pResult);
}

void OD_Delete_in_context(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	// --- write the code of OD_Delete_in_context here...
	
	returnValue.setReturn(pResult);
}

void OD_Load_rows_context(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT Param2;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	
	// --- write the code of OD_Load_rows_context here...
	
	returnValue.setReturn(pResult);
}

void OD_Reset_context(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	// --- write the code of OD_Reset_context here...
	
	returnValue.setReturn(pResult);
}
