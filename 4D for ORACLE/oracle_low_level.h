#ifndef __ORACLE_LOW_LEVEL_H__
#define __ORACLE_LOW_LEVEL_H__ 1

#include "4DPluginAPI.h"
#include "4DPlugin.h"

#define eVK_PointerToField 3

#define BUFFER_SIZE_TEXT_VARIABLE 4000	//bytes
#define BUFFER_SIZE_TEXT_ARRAY_ELEMENT 4000	//bytes

//meta data
typedef std::vector<PA_Variable> ORACLE_SQL_SUBSTITUTION_LIST;
typedef std::vector<PointerBlock> ORACLE_SQL_SUBSTITUTION_POINTER_LIST;
typedef std::vector<bool> ORACLE_SQL_SUBSTITUTION_TYPE_LIST;
typedef std::vector<bool> ORACLE_SQL_BIND_TYPE_LIST;
typedef std::vector<CUTF16String> ORACLE_SQL_BIND_NAME_LIST;

//bindbypos
typedef std::vector<OCIBind*> ORACLE_BIND_LIST;
typedef std::vector<sb2> ORACLE_INDICATOR_LIST;

//definebypos
typedef std::vector<OCIDefine*> ORACLE_DEFINE_LIST;

//containers
typedef std::vector<OCIDate> ORACLE_SQL_BIND_DATE_LIST;
typedef std::vector<OCINumber> ORACLE_SQL_BIND_NUMBER_LIST;
typedef std::vector< std::vector<uint8_t> > ORACLE_SQL_BIND_STRING_LIST;

//array containers
typedef std::vector< std::vector<OCIDate> > ORACLE_SQL_BIND_DATE_LIST_LIST;
typedef std::vector< std::vector<OCINumber> > ORACLE_SQL_BIND_NUMBER_LIST_LIST;
typedef std::vector< std::vector<OCIString*> > ORACLE_SQL_BIND_TEXT_LIST_LIST;

//text arrays
typedef std::vector< std::vector<bool> > ORACLE_SQL_BIND_TEXT_STATUS_LIST_LIST;
typedef std::vector<bool> ORACLE_SQL_BIND_TEXT_STATUS_LIST;

typedef std::vector<ORACLE_INDICATOR_LIST> ORACLE_INDICATOR_LIST_LIST;
typedef std::vector< std::vector<ub2> > ORACLE_LENGTH_LIST_LIST;
//typedef std::vector<ub2> ORACLE_RETURN_CODE_LIST;

typedef struct
{
	bool			isActive;
	uint32_t		sessionId;
	OCIStmt			*stmtp;
	OCIBind			*bindp;
	OCIError		*errhp;	
	size_t			itemCount;	//this is the count of array elements, not the could of arrays (binds. params)
	CUTF16String	sql;
	unsigned int	sql_type;
	ub4				rowCount;
	ub4				rowsFetched;
	bool			isEndOfSelection;
	
	ORACLE_SQL_SUBSTITUTION_LIST			substitutions;
	ORACLE_SQL_SUBSTITUTION_POINTER_LIST	pointers;
	ORACLE_SQL_SUBSTITUTION_TYPE_LIST		isTowardsSQL;
	ORACLE_SQL_BIND_TYPE_LIST				isByName;//not used; internally we always bind by position
	ORACLE_INDICATOR_LIST					indicators;//for null	
	ORACLE_SQL_BIND_NAME_LIST				names;//for logging purposes
	ORACLE_BIND_LIST						binds;
	ORACLE_DEFINE_LIST						defines;
	ORACLE_SQL_BIND_DATE_LIST				dates;//for real, boolean, longint
	ORACLE_SQL_BIND_NUMBER_LIST				numbers;//for date, time
	ORACLE_SQL_BIND_STRING_LIST				texts;//for text, alpha
	ORACLE_SQL_BIND_TEXT_STATUS_LIST_LIST	isObjectElementValid;//for text array
	ORACLE_SQL_BIND_TEXT_STATUS_LIST		isObjectValid;//for text array	
	ORACLE_SQL_BIND_DATE_LIST_LIST			arrayOfDates;
	ORACLE_SQL_BIND_NUMBER_LIST_LIST		arrayOfNumbers;	
	ORACLE_SQL_BIND_TEXT_LIST_LIST			arrayOfTexts;
	ORACLE_INDICATOR_LIST_LIST				indicatorLists;
	ORACLE_LENGTH_LIST_LIST					lengths;
	
} ORACLE_SQL_CURSOR;

ORACLE_SQL_CURSOR *_cursorGet(unsigned int i);
ORACLE_SQL_CURSOR * _cursorGetAndCheckActive(uint32_t cursorId);
void _cursorClearBind(sessionInfo *session, ORACLE_SQL_CURSOR *cursor);

// --- OD: Low Level
void OD_Create_cursor(sLONG_PTR *pResult, PackagePtr pParams);
void OD_DROP_CURSOR(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Cursor_state(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Set_SQL_in_cursor(sLONG_PTR *pResult, PackagePtr pParams);
void OD_BIND_TOWARDS_SQL(sLONG_PTR *pResult, PackagePtr pParams);
void OD_BIND_TOWARDS_4D(sLONG_PTR *pResult, PackagePtr pParams);
void OD_EXECUTE_CURSOR(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Load_rows_cursor(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Number_rows_processed(sLONG_PTR *pResult, PackagePtr pParams);
void OD_GET_COLUMN_ATTRIBUTES(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Get_column_title(sLONG_PTR *pResult, PackagePtr pParams);
void OD_Number_of_columns(sLONG_PTR *pResult, PackagePtr pParams);

#endif