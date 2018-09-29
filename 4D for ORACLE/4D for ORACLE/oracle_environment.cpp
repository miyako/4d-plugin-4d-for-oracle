#include "oracle_environment.h"


// -------------------------------- OD: Environment -------------------------------


#pragma mark -
#pragma mark commands
#pragma mark -

void OD_SET_ENVIRONMENT_VARIABLE(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	C_TEXT Param2;
	C_LONGINT Param3;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
	
#if VERSIONMAC
	if(Param3.getIntValue())
	{
		NSString *path = Param2.copyPath();
		Param2.setUTF16String(path);
		[path release];
	}	
	
	CUTF8String name, value;
	Param1.copyUTF8String(&name);
	Param2.copyUTF8String(&value);
	
	setenv((const char *)name.c_str(), (const char *)value.c_str(), 1);
#else

	std::wstring name = std::wstring((const wchar_t *)Param1.getUTF16StringPtr(), Param1.getUTF16Length());
	std::wstring value = std::wstring((const wchar_t *)Param2.getUTF16StringPtr(), Param2.getUTF16Length());

	_wputenv_s(name.c_str(), value.c_str());
#endif	
	
}

void OD_Get_environment_variable(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	C_LONGINT Param2;
	C_TEXT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	
#if VERSIONMAC
	CUTF8String u;
	Param1.copyUTF8String(&u);
	char *value = getenv((const char *)u.c_str());

	returnValue.setUTF8String((const uint8_t *)value, strlen(value));
	
	if(Param2.getIntValue())
	{
		NSString *path = returnValue.copyUTF16String();
		returnValue.setPath(path);
		[path release];
	}
#else
	
	std::wstring value = std::wstring(_wgetenv((const wchar_t *)Param1.getUTF16StringPtr()));
	returnValue.setUTF16String((const PA_Unichar *)value.c_str(), value.length());
	
#endif	
	
	returnValue.setReturn(pResult);
}