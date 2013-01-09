#include "oracle_login.h"


// ----------------------------------- OD: Login ----------------------------------


#pragma mark -
#pragma mark commands
#pragma mark -

void OD_Login(sLONG_PTR *pResult, PackagePtr pParams)
{		
	OD_SetOracleLibraryDirectory();
	
	C_TEXT Param1;
	C_TEXT Param2;
	C_TEXT Param3;
	C_LONGINT Param4;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
	Param4.fromParamAtIndex(pParams, 4);
	
	sword err = 0;
	
	OCIEnv		*envhp = 0;
	OCIError	*errhp = 0;
	OCIServer	*srvhp = 0;
	OCISvcCtx	*svchp = 0;
	OCISession	*authp = 0;

	err = OCIEnvCreate(&envhp, OCI_UTF16|OCI_OBJECT, 0, 0, 0, 0, 0, 0);
	//http://docs.oracle.com/cd/A91202_01/901_doc/appdev.901/a89857/oci15re4.htm#556151
	
	if(!err)
	{
		OCIHandleAlloc((dvoid *)envhp, (dvoid **)&errhp, OCI_HTYPE_ERROR,	0, 0);
		OCIHandleAlloc((dvoid *)envhp, (dvoid **)&srvhp, OCI_HTYPE_SERVER,	0, 0);
		OCIHandleAlloc((dvoid *)envhp, (dvoid **)&svchp, OCI_HTYPE_SVCCTX,	0, 0);	
		OCIHandleAlloc((dvoid *)envhp, (dvoid **)&authp, OCI_HTYPE_SESSION,	0, 0);
		//http://docs.oracle.com/cd/A97630_01/appdev.920/a96584/oci15r24.htm
				
		err = OCIServerAttach(srvhp, errhp, (text *)Param3.getUTF16StringPtr(), (Param3.getUTF16Length() * sizeof(PA_Unichar)), OCI_DEFAULT);
		//Its value is in number of bytes.
		//http://docs.oracle.com/cd/B10501_01/appdev.920/a96584/oci15r11.htm
		
		if(!err)
		{
			err = OCIAttrSet(svchp, OCI_HTYPE_SVCCTX, srvhp, 0,	OCI_ATTR_SERVER, errhp);
			err = OCIAttrSet(authp, OCI_HTYPE_SESSION,	(dvoid *)Param1.getUTF16StringPtr(), (Param1.getUTF16Length() * sizeof(PA_Unichar)), OCI_ATTR_USERNAME,	errhp);
			err = OCIAttrSet(authp, OCI_HTYPE_SESSION,	(dvoid *)Param2.getUTF16StringPtr(), (Param2.getUTF16Length() * sizeof(PA_Unichar)), OCI_ATTR_PASSWORD,	errhp);
			//must be passed in set to the length of the string in bytes, regardless of encoding. (dvoid*)
			//http://docs.oracle.com/cd/A97630_01/appdev.920/a96584/oci15r21.htm#443806
			
			err = OCISessionBegin(svchp, errhp, authp, OCI_CRED_RDBMS, OCI_DEFAULT);
			//http://docs.oracle.com/cd/A97630_01/appdev.920/a96584/oci15r13.htm
			
			if(!err)
			{
				err = OCIAttrSet(svchp, OCI_HTYPE_SVCCTX, authp, 0, OCI_ATTR_SESSION, errhp);
			}else{OCIServerDetach(srvhp, errhp, OCI_DEFAULT);
				//http://docs.oracle.com/cd/B10500_01/appdev.920/a96584/oci15r12.htm
			}
		}
	}

	if(!err)
	{
		unsigned int i = 0;
		
		CUTF16String user;
		CUTF16String server;
		
		Param1.copyUTF16String(&user);
		Param3.copyUTF16String(&server);
		
		_sessionCreate(&i, envhp, srvhp, svchp, authp, errhp, user, server);
		returnValue.setIntValue(i);

		if(!i)
		{
			OCIHandleFree(errhp, OCI_HTYPE_ERROR);
			OCIHandleFree(srvhp, OCI_HTYPE_SERVER);		
			OCIHandleFree(svchp, OCI_HTYPE_SVCCTX);
			OCIHandleFree(authp, OCI_HTYPE_SESSION);		
			OCIHandleFree(envhp, OCI_HTYPE_ENV);
			
			returnValue.setIntValue(-1);//plugin internal error
		}
		
	}else{

		_errorInfoSet(0, errhp, 0, 0, false, PA_GetCurrentProcessNumber(), 0);
		returnValue.setIntValue(err);

	}
	
	returnValue.setReturn(pResult);
}

void OD_LOGOUT(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	
	Param1.fromParamAtIndex(pParams, 1);
	uint32_t sessionId = Param1.getIntValue();
	
	sessionInfo *session = _sessionGet(sessionId);
	
	if(session)
	{
		OCISessionEnd(session->svchp, session->errhp, session->authp, OCI_DEFAULT);
		//http://docs.oracle.com/cd/B10500_01/appdev.920/a96584/oci15r14.htm
		
		OCIServerDetach(session->srvhp, session->errhp, OCI_DEFAULT);
		//http://docs.oracle.com/cd/B10500_01/appdev.920/a96584/oci15r12.htm
		
		_sessionDelete(Param1.getIntValue());
	}

}

void OD_Login_state(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_TEXT Param2;
	C_TEXT Param3;
	C_LONGINT returnValue;

	returnValue.setIntValue(0);
	
	Param1.fromParamAtIndex(pParams, 1);
	
	uint32_t sessionId = Param1.getIntValue();
	
	sessionInfo *session = _sessionGet(sessionId);
	
	if(session)
	{
		returnValue.setIntValue(1);
		Param2.setUTF16String(&session->user);
		Param3.setUTF16String(&session->server);
	}
	
	Param2.toParamAtIndex(pParams, 2);
	Param3.toParamAtIndex(pParams, 3);
	
	returnValue.setReturn(pResult);
}

#pragma mark -
#pragma mark deprecated commands
#pragma mark -

void OD_Clone_table(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_TEXT Param2;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	
	returnValue.setReturn(pResult);
}

void OD_Login_dialog(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	returnValue.setReturn(pResult);
}
