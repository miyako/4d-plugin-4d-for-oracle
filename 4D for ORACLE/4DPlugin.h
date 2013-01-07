#include "oci.h"

#ifndef __4D_PLUGIN_H__
#define __4D_PLUGIN_H__ 1

typedef struct
{
	
	OCIEnv		*envhp;
	OCIServer	*srvhp;
	OCISvcCtx	*svchp;
	OCISession	*authp;
	OCIError	*errhp;
	CUTF16String	user;
	CUTF16String	server;	
}sessionInfo;

sessionInfo *_sessionCreate(unsigned int *index, 
							OCIEnv	*envhp, 
							OCIServer *srvhp, 
							OCISvcCtx *svchp, 
							OCISession *authp,
							OCIError	*errhp,
							CUTF16String		&user,
							CUTF16String		&server);

sessionInfo *_sessionGet(unsigned int i);

void _sessionDelete(unsigned int i);

void OD_SetOracleLibraryDirectory();

#endif