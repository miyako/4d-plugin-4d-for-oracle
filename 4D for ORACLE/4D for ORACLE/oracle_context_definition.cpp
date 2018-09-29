#include "oracle_context_definition.h"


// ---------------------------- OD: Context Definition ----------------------------


#pragma mark -
#pragma mark deprecated commands
#pragma mark -

void OD_Create_context(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	// --- write the code of OD_Create_context here...
	
	returnValue.setReturn(pResult);
}

void OD_Create_context_dialog(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_TEXT Param2;
	C_LONGINT Param3;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
	
	// --- write the code of OD_Create_context_dialog here...
	
	returnValue.setReturn(pResult);
}

void OD_DROP_CONTEXT(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	// --- write the code of OD_DROP_CONTEXT here...
	
}

void OD_ADD_TO_CONTEXT(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_TEXT Param2;
	C_POINTER Param3;
	C_LONGINT Param4;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
	Param4.fromParamAtIndex(pParams, 4);
	
	// --- write the code of OD_ADD_TO_CONTEXT here...
	
}

void OD_EDIT_CLAUSES_IN_CONTEXT(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT Param2;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	
	// --- write the code of OD_EDIT_CLAUSES_IN_CONTEXT here...
	
}

void OD_SET_CLAUSE_IN_CONTEXT(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT Param2;
	C_TEXT Param3;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
	
	// --- write the code of OD_SET_CLAUSE_IN_CONTEXT here...
	
}

void OD_Get_clause_in_context(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT Param2;
	C_TEXT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	
	// --- write the code of OD_Get_clause_in_context here...
	
	returnValue.setReturn(pResult);
}

void OD_Save_context_picture(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_PICTURE returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	// --- write the code of OD_Save_context_picture here...
	
	returnValue.setReturn(pResult);
}

void OD_Open_context_picture(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_PICTURE Param1;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	// --- write the code of OD_Open_context_picture here...
	
	returnValue.setReturn(pResult);
}

void OD_SAVE_CONTEXT_FILE(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_TEXT Param2;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	
	// --- write the code of OD_SAVE_CONTEXT_FILE here...
	
}

void OD_Open_context_file(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	// --- write the code of OD_Open_context_file here...
	
	returnValue.setReturn(pResult);
}
