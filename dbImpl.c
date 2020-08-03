/*
 * dbImpl.c
 *
 *  Created on: 11-Mar-2014
 *      Author: ganesh
 */



#include<cnc_dmsClm.h>
#include<sqlite3.h>
#include<stdio.h>

typedef struct _clmDbTag
{
   char     *p_url;
   char     *p_dbName;
   char     **pp_tableNames;
   sqlite3  *p_db;

} clmDb_t;

#define DB_NAME         "dmsClmDB.db"
#define URL_LENGTH      (512)
#define CONTENT_TABLE   "CONTENT"

char   *p_contentTable = "CREATE TABLE CONTENT("\
                         "ID                INT UNIQUE,"\
                         "PARENT_ID         INT,"\
                         "FILE_PATH         CHAR(256) NOT NULL,"\
                         "TITLE             CHAR(32)  NOT NULL,"\
                         "CONTENT_CLASS     INT,"\                      //DIR, IMAGE, AUDIO, VIDEO
                         "CONTENT_URL       CHAR(128),"\
                         "WD                INT,"\                      //Folder Specific data,
                         "CHILD_COUNT       INT,"\                      //Folder Specific data,
                         "MEDIA_FORMAT      INT,"\
                         "DURATION          INT,"\
                         "BIT_RATE          INT,"\
                         "SIZE              INT,"\
                         "ALBUM_ART_URL     CHAR(512),"\                //Media Meta Data
                         "AUTHOR            CHAR(128),"\                //Media Meta Data
                         "ALBUM             CHAR(128),"\                //Media Meta Data
                         "GENRE             CHAR(32),"\                 //Media Meta Data
                         "ARTIST            CHAR(128),"\                //Media Meta Data
                         "YEAR              INT,"\                      //Media Meta Data
                         "TRACK             INT,"\                      //Media Meta Data
                         "A_FORMAT          INT,"\
                         "A_BITRATE         INT,"\
                         "A_SAMPLE_RATE     INT,"\
                         "A_NUM_OF_CHANNELS INT,"\
                         "A_BIG_ENDIAN      INT,"\
                         "A_SAMPLE_FORMAT   INT,"\
                         "A_ALBUM_ART_TYPE  INT,"\
                         "V_FORMAT          INT,"\
                         "V_BIT_RATE        INT,"\
                         "V_FRAME_RATE      INT,"\
                         "V_WIDTH           INT,"\
                         "V_HEIGHT          INT,"\
                         "I_FORMAT          INT,"\
                         "I_WIDHT           INT,"\
                         "I_HEIGHT          INT"\
                         ");";



/**************************************************** Function *************************************/

int dmsClmDbInit(void)
{
    return 0;
}


int dmsClmDbCreate(char *p_url, clmDb_t **pp_dbHandle)
{
    int         retValue = 0;
    int         sqReturn = 0;
    char        a_url[URL_LENGTH] = {0};
    clmDb_t     *p_dbHandle = NULL;
    sqlite3     *p_sqlite   = NULL;
    char        *p_errMsg   = NULL;

    if(p_url == NULL || pp_dbHandle == NULL)
    {
        retValue = -1;
        printf("INVALID Arguments \n");
        goto _dmsClmDbCreate_END;
    }

    //Allocate Handle
    p_dbHandle = (clmDb_t*) malloc (sizeof(clmDb_t));
    if(p_dbHandle == NULL)
    {
        retValue = -2;
        printf("INVALID Arguments \n");
        goto _dmsClmDbCreate_END;
    }

    //Clear
    memset(p_dbHandle, 0, sizeof(clmDb_t));

    //Verify PATH and append db file path.
    sprintf(a_url, "%s/%s", p_url, DB_NAME);

    sqReturn = sqlite3_open(a_url, &p_sqlite);
    if(sqReturn != SQLITE_OK)
    {
        printf("Not able to create DB for given URL %s :: Error Code %d \n", a_url, sqReturn);
        retValue = sqReturn;
        goto _dmsClmDbCreate_END;
    }

    //Update
    p_dbHandle->p_db        = p_sqlite;
    p_dbHandle->p_url       = strdup(a_url);
    p_dbHandle->p_dbName    = NULL;
    p_dbHandle->pp_tableNames = NULL;

    //Create Table.
    sqReturn = sqlite3_exec(p_dbHandle, p_contentTable, NULL, NULL, &p_errMsg);
    if(sqReturn != SQLITE_OK)
    {
        printf("The Query %s \n", p_contentTable);
        printf("The Error Code %d  : Err_Msg %s\n", sqReturn, p_errMsg);
        retValue = sqReturn;
        goto _dmsClmDbCreate_END;
    }

_dmsClmDbCreate_END:
    return retValue;
}


/**
 *
 * **/
int dmsClmDbDestroy(clmDb_t *p_dbHandle)
{
    if(p_dbHandle == NULL)
    {
        goto _dmsClmDbDestroy_END;
    }

    p_dbHandle->p_dbName = NULL;

    free(p_dbHandle->p_url);

    sqlite3_close(p_dbHandle->p_db);

    free(p_dbHandle);

_dmsClmDbDestroy_END:

    return 0;
}


/*
 *
 */
int dmsClmDbAddContent(clmDb_t *p_dbHandle, cncDmsContent_t *p_content)
{

    int retValue = 0;
    char *query = NULL;

    if(p_content == NULL || p_dbHandle == NULL)
    {
        printf("Invalid Argument \n");
        goto _dmsClmDbAddContent_END;
    }

    "ID                INT UNIQUE,"\
    "PARENT_ID         INT,"\
    "FILE_PATH         CHAR(256) NOT NULL,"\
    "TITLE             CHAR(32)  NOT NULL,"\
    "CONTENT_CLASS     INT,"\                      //DIR, IMAGE, AUDIO, VIDEO
    "CONTENT_URL       CHAR(128),"\
    "WD                INT,"\                      //Folder Specific data,
    "CHILD_COUNT       INT,"\                      //Folder Specific data,
    "MEDIA_FORMAT      INT,"\
    "DURATION          INT,"\
    "BIT_RATE          INT,"\
    "SIZE              INT,"\
    "ALBUM_ART_URL     CHAR(512),"\                //Media Meta Data
    "AUTHOR            CHAR(128),"\                //Media Meta Data
    "ALBUM             CHAR(128),"\                //Media Meta Data
    "GENRE             CHAR(32),"\                 //Media Meta Data
    "ARTIST            CHAR(128),"\                //Media Meta Data
    "YEAR              INT,"\                      //Media Meta Data
    "TRACK             INT,"\                      //Media Meta Data
    "A_FORMAT          INT,"\
    "A_BITRATE         INT,"\
    "A_SAMPLE_RATE     INT,"\
    "A_NUM_OF_CHANNELS INT,"\
    "A_BIG_ENDIAN      INT,"\
    "A_SAMPLE_FORMAT   INT,"\
    "A_ALBUM_ART_TYPE  INT,"\
    "V_FORMAT          INT,"\
    "V_BIT_RATE        INT,"\
    "V_FRAME_RATE      INT,"\
    "V_WIDTH           INT,"\
    "V_HEIGHT          INT,"\
    "I_FORMAT          INT,"\
    "I_WIDHT           INT,"\
    "I_HEIGHT          INT"\;


    p_content->id,
    p_content->parentId,
    p_content->type,
    p_content->path,
    p_content->wd,
    p_content->p_url,


    query = sqlite3_mprintf("INSERT INTO CONTENT VALUES ("\
                            "%d,%d,'%q','%q',%d,'%q',%d,%d,%d,%d,%d,%d,'%q','%q','%q','%q','%q','%q',%d,%d,"\
                            "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d);",
                            p_content->id,
                            p_content->parentId,
                            p_content->path,
                            p_content->);

    //sprintf(sql, "INSERT INTO CONTENT VALUES ( %d,%d,'%s',%d,%d,%d);", , pid, wd, name, c_count, size, format);
    sqReturn = sqlite3_exec(p_dbHandle, sql, NULL, NULL, &p_errMsg);
    if(sqReturn != SQLITE_OK)
    {
        printf("The Query %s \n", p_cTable);
        printf("The Error Code %d  : Err_Msg %s\n", sqReturn, p_errMsg);
        return -3;
    }



_dmsClmDbAddContent_END:

    return retValue;
}


int dmsClmDbRemoveContent(clmDb_t *p_dbHandle, int ID)
{

    return 0;
}



/*
 *
 */
int dmsClmDbGetContentListByID(clmDb_t *p_dbHandle, int id, cncDmsContentList **pp_contentList)
{
    return 0;
}

//int dmsClmDbGetContetnListByValue();

/* TBA : To retrieve specific content */

/* TBA :: retrieving any specific values */


/*
 *
 */

int dmsClmDbDeInit(void)
{
    return 0;
}
