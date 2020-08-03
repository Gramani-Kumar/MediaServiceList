
#include<cnc_dmsClm.h>


typedef struct _clmDbTag
{
   char *p_url;
   char *p_dbName;
   char **pp_tableNames;
   sqlite3 *p_dbHandle;

} clmDb_t;



int dmsClmDbInit(void);

int dmsClmDbCreate(char *p_url, clmDb_t **pp_dbHandle);

int dmsClmDbDestroy(clmDb_t *p_dbHandle);

int dmsClmDbUpdateChildCount(clmDb_t *p_dbHandle, int folderId, int childCount);

int dmsClmDbAddContent(clmDb_t *p_dbHandle, cncDmsContent_t *p_content);

int dmsClmDbRemoveContent(clmDb_t *p_dbHandle, cncDmsContent_t *p_content);

int dmsClmDbGetContentListByID(clmDb_t *p_dbHandle, int id, cncDmsContentList **pp_contentList);

//int dmsClmDbGetContetnListByValue();

/* TBA : To retrieve specific content */

/* TBA :: retrieving any specific values */

int dmsClmDbDeInit(void);

