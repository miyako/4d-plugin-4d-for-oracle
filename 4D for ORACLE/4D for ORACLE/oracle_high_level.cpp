#include "oracle_high_level.h"


// -------------------------------- OD: High Level --------------------------------


#pragma mark commands

void OD_COMMIT(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	
	Param1.fromParamAtIndex(pParams, 1);

	uint32_t sessionId = Param1.getIntValue();
	sessionInfo *session = _sessionGet(sessionId);
		
	if(session)
	{
		if(OCITransCommit(session->svchp, session->errhp, OCI_DEFAULT))
		{
			//http://docs.oracle.com/cd/B10501_01/appdev.920/a96584/oci16m89.htm
			_errorInfoSet(0, session->errhp, sessionId, 0, false, PA_GetCurrentProcessNumber(), 0);		
		}
		
	}
	
}

void OD_ROLLBACK(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	uint32_t sessionId = Param1.getIntValue();
	sessionInfo *session = _sessionGet(sessionId);
	
	if(session)
	{
		if(OCITransRollback(session->svchp, session->errhp, OCI_DEFAULT))
		{
			//http://docs.oracle.com/cd/B10501_01/appdev.920/a96584/oci16m94.htm#423234
			_errorInfoSet(0, session->errhp, sessionId, 0, false, PA_GetCurrentProcessNumber(), 0);
		}
		
	}
	
}

#pragma mark -
#pragma mark deprecated commands
#pragma mark -

void OD_Find_in_context(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	// --- write the code of OD_Find_in_context here...
	
	returnValue.setReturn(pResult);
}

void OD_Describe_object(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_TEXT Param2;
	C_POINTER Param3;
	C_POINTER Param4;
	C_POINTER Param5;
	C_POINTER Param6;
	C_POINTER Param7;
	C_POINTER Param8;
	C_POINTER Param9;
	C_POINTER Param10;
	C_INTEGER returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
	Param4.fromParamAtIndex(pParams, 4);
	Param5.fromParamAtIndex(pParams, 5);
	Param6.fromParamAtIndex(pParams, 6);
	Param7.fromParamAtIndex(pParams, 7);
	Param8.fromParamAtIndex(pParams, 8);
	Param9.fromParamAtIndex(pParams, 9);
	Param10.fromParamAtIndex(pParams, 10);
	
	// --- write the code of OD_Describe_object here...
	
	returnValue.setReturn(pResult);
}

void OD_Execute_object(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_TEXT Param2;
	C_POINTER Param3;
	C_INTEGER returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
	
	// --- write the code of OD_Execute_object here...
	
	returnValue.setReturn(pResult);
}

void OD_Execute_SQL(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_TEXT Param2;
	C_LONGINT Param3;
	C_POINTER Param4;
	C_POINTER Param5;
	C_POINTER Param6;
	C_POINTER Param7;
	C_POINTER Param8;
	C_POINTER Param9;
	C_POINTER Param10;
	C_POINTER Param11;
	C_POINTER Param12;
	C_POINTER Param13;
	C_POINTER Param14;
	C_POINTER Param15;
	C_POINTER Param16;
	C_POINTER Param17;
	C_POINTER Param18;
	C_POINTER Param19;
	C_POINTER Param20;
	C_POINTER Param21;
	C_POINTER Param22;
	C_POINTER Param23;
	C_POINTER Param24;
	C_POINTER Param25;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
	Param4.fromParamAtIndex(pParams, 4);
	Param5.fromParamAtIndex(pParams, 5);
	Param6.fromParamAtIndex(pParams, 6);
	Param7.fromParamAtIndex(pParams, 7);
	Param8.fromParamAtIndex(pParams, 8);
	Param9.fromParamAtIndex(pParams, 9);
	Param10.fromParamAtIndex(pParams, 10);
	Param11.fromParamAtIndex(pParams, 11);
	Param12.fromParamAtIndex(pParams, 12);
	Param13.fromParamAtIndex(pParams, 13);
	Param14.fromParamAtIndex(pParams, 14);
	Param15.fromParamAtIndex(pParams, 15);
	Param16.fromParamAtIndex(pParams, 16);
	Param17.fromParamAtIndex(pParams, 17);
	Param18.fromParamAtIndex(pParams, 18);
	Param19.fromParamAtIndex(pParams, 19);
	Param20.fromParamAtIndex(pParams, 20);
	Param21.fromParamAtIndex(pParams, 21);
	Param22.fromParamAtIndex(pParams, 22);
	Param23.fromParamAtIndex(pParams, 23);
	Param24.fromParamAtIndex(pParams, 24);
	Param25.fromParamAtIndex(pParams, 25);
	
	// --- write the code of OD_Execute_SQL here...
	
	returnValue.setReturn(pResult);
}

void OD_Clone_4D_Table(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_INTEGER Param2;
	C_LONGINT Param3;
	C_INTEGER returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
	
	// --- write the code of OD_Clone_4D_Table here...
	
	returnValue.setReturn(pResult);
}