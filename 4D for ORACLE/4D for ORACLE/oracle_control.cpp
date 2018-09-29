#include "oracle_control.h"
#include "oracle_low_level.h"


// -------------------------------- OD: Preferences -------------------------------


#pragma mark -
#pragma mark internal
#pragma mark -

OCIEnv		*lastErrorEnvhp		= 0;
OCIError	*lastErrorErrhp		= 0;
uint32_t	lastErrorSession	= 0;
uint32_t	lastErrorCursor		= 0;
bool		lastErrorIsInternal	= false;
uint32_t	lastErrorProcess	= 0;
uint32_t	lastErrorCode		= 0;

void _errorInfoSet(OCIEnv *envhp, OCIError *errhp, uint32_t session, uint32_t cursor, bool isInternal, uint32_t process, uint32_t errorCode)
{
	lastErrorEnvhp = envhp;
	lastErrorErrhp = errhp;
	lastErrorSession = session;
	lastErrorCursor = cursor;
	lastErrorIsInternal = isInternal;
	lastErrorProcess = process;
	lastErrorCode = errorCode;
}


// ---------------------------------- OD: Control ---------------------------------


#pragma mark -
#pragma mark commands
#pragma mark -

void OD_CANCEL_LOADING(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	uint32_t cursorId = Param1.getIntValue();
	
	ORACLE_SQL_CURSOR *cursor = _cursorGetAndCheckActive(cursorId);
	
	if(cursor)
	{
		sessionInfo *session = _sessionGet(cursor->sessionId);
		
		if(session)
		{
			_cursorClearBind(session, cursor);
			
		}
		
	}
	
}

void OD_Last_error(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	C_LONGINT Param2;
	C_LONGINT Param3;
	C_TEXT Param4;
	C_LONGINT Param5;
	C_LONGINT Param6;
	C_LONGINT returnValue;
	
	sb4 errcode = 0;
	text errbuf[512];
	sword err = 0;
	
	if(lastErrorIsInternal)
	{			
		Param2.setIntValue(200);
		Param6.setIntValue(lastErrorProcess);
		returnValue.setIntValue(lastErrorCode);
		
		_errorInfoSet(0, 0, 0, 0, false, 0, 0);
		
	}else{
		
	/*
	 removed for simplicity
	 
		if(lastErrorEnvhp)
		{
			err =OCIErrorGet(lastErrorEnvhp, 1, NULL, &errcode,
							 errbuf, sizeof(errbuf), OCI_HTYPE_ENV);
			//http://docs.oracle.com/cd/B10501_01/appdev.920/a96584/oci16m97.htm
			
		}
	*/	

		if(lastErrorErrhp)
		{
			err =OCIErrorGet(lastErrorErrhp, 1, NULL, &errcode,
							 errbuf, sizeof(errbuf), OCI_HTYPE_ERROR);
			//http://docs.oracle.com/cd/B10501_01/appdev.920/a96584/oci16m97.htm
			
			
			CUTF16String u((const PA_Unichar *)errbuf);
			
			Param1.setUTF16String(&u);			
			Param2.setIntValue(300);
			Param5.setIntValue(lastErrorSession);
			Param6.setIntValue(lastErrorProcess);
			returnValue.setIntValue(errcode);
			
			if(err == OCI_SUCCESS)
				_errorInfoSet(0, 0, 0, 0, false, 0, 0);
		}
		
	}	
	
	Param1.toParamAtIndex(pParams, 1);	
	Param2.toParamAtIndex(pParams, 2);	
	Param3.toParamAtIndex(pParams, 3);	
	Param4.toParamAtIndex(pParams, 4);
	Param5.toParamAtIndex(pParams, 5);	
	Param6.toParamAtIndex(pParams, 6);
	
	returnValue.setReturn(pResult);
}


#pragma mark -
#pragma mark deprecated commands
#pragma mark -


void OD_ON_ERROR_CALL(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	// --- write the code of OD_ON_ERROR_CALL here...
	
}

void OD_OPEN_DEBUG_WINDOW(sLONG_PTR *pResult, PackagePtr pParams)
{
	// --- write the code of OD_OPEN_DEBUG_WINDOW here...
	
}

void OD_CLOSE_DEBUG_WINDOW(sLONG_PTR *pResult, PackagePtr pParams)
{
	// --- write the code of OD_CLOSE_DEBUG_WINDOW here...
	
}

void OD_LOGIN_INFORMATIONS(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	// --- write the code of OD_LOGIN_INFORMATIONS here...
	
}

void OD_MESSAGE_DEBUG(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	// --- write the code of OD_MESSAGE_DEBUG here...
	
}

void OD_Check_configuration(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT returnValue;
	
	// --- write the code of OD_Check_configuration here...
	
	returnValue.setReturn(pResult);
}

void OD_GET_SERVER_LIST(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	// --- write the code of OD_GET_SERVER_LIST here...
	
}

