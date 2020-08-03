/*
 * cncDmsClm.c
 *
 *  Created on: 
 *      Author: Ganesh Kumar R.
 */

/*
 # COPYRIGHT NOTICE
 #
 #***************************************************************************



 ALL RIGHTS RESERVED.

 #***************************************************************************
 #
 # END OF COPYRIGHT NOTICE
 #
 */

/* ============================= File Header ================================
 \brief     :All cnc_dms Content List Manager Core api's definition will be present.
 \file      :cnc_dmsClm.c
 \author    :
 \date      :
 \version   :
 \attention :
 \see       :
 * ======================================================================== */

/* Define TRACE level override */
#ifdef CNC_TRACE_LEVEL_DLNA_DMS
#if (CNC_TRACE_LEVEL_DLNA_DMS >= 0)
#define TRACE_MODULE_LEVEL   CNC_TRACE_LEVEL_DLNA_DMS
#endif
#endif

/* ======================== System Includes =============================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
/* ======================== User Includes ==================================*/
#include "cnc_dmsClm.h"
#include "cnc_softdemuxer_helper.h"
#include "cnc_trace.h"
/* ================= (Internal) Macro definitions ========================= */
/* Define TRACE module ID */
#define TRACE_MODULE_ID     //CNC_TRACE_MODULE_DLNA_DMS

/* ================= (Internal) Type definitions ========================== */
/* None */

/* ============================ Global variables ========================== */
//None
/* ===================== Helper Function Declarations ===================== */
/* None */



static int clmScanDir(char *p_shareFolder, cncDmsContentListMgt_t *p_clmObj)
{
    struct dirent   **namelist = NULL;
    int             retValue = 0;
    int             numOfEntries = 0;
    int             index = 0;
    struct stat     fileStat;
    char            filePath[1024] = {0};
    int             folderId = 0;
    int             childCount = 0;

    clmContentIds_t         *p_content;
    cncDmsContent_t         s_content;
    cncMediaFormatInfo_t    s_mediaFormatInfo;

    if(p_shareFolder == NULL || p_clmObj == NULL)
    {
        return -1;
    }

    p_content = p_clmObj->p_contentId;
    folderId  = p_content->contentId;

    numOfEntries = scandir(p_shareFolder, &namelist, NULL, NULL);
    if(numOfEntries < 0)
    {
        printf("Error occurred for scanning directory %s\n", p_shareFolder);
        perror("");
        return -2;
    }

    index = -1;
    while(index < numOfEntries -1)
    {
       index ++;

       if(strcmp( namelist[index]->d_name, ".") == 0 || strcmp( namelist[index]->d_name, "..") == 0)
       {
           free(namelist[index]);
           continue;
       }

       //File Path
       sprintf(filePath,"%s/%s",p_shareFolder, namelist[index]->d_name);

       retValue = lstat(filePath, &fileStat);
       if(retValue < 0)
       {
           free(namelist[index]);
           printf("Not Sure why it is not able to access \n");
           perror("");
           continue;
       }

       //Verify If max Content reaches
       if(p_content->totalContents == p_content->maxContents)
       {
           //Free NameList;
           while(index < numOfEntries)
           {
               free(namelist[index]);
               index ++;
           }
           break;
       }

       if(S_ISREG(fileStat.st_mode) )
       {
           memset(&s_content, 0, sizeof(s_content));

           //DO MFD
           retValue = CNC_SoftDemuxer_Probe(filePath, &s_mediaFormatInfo);
           if(retValue < 0)
           {
               printf("Error :: Not able to file format %d \n", retValue);
           }
           else
           {
               //Update ID
               p_content->contentId ++;
               p_content->totalContents ++;
               childCount ++;

               //Prepare
               s_content.id                = p_content->contentId;
               s_content.p_path            = filePath;
               s_content.p_mediaFormatInfo = &s_mediaFormatInfo;
               s_content.type              = CNC_DMS_CONTENT_TYPE_REGU_FILE;
               s_content.wd                = -1;
               s_content.parentId          = folderId;
               s_content.p_name            = namelist[index]->d_name;

               //printf("[%3d] The File Name :: %s \n", p_content->contentId, namelist[index]->d_name);

               //Insert Content into DB

               retValue = dmsClmDbAddContent(p_clmObj->p_db,  &s_content);
               if(retValue < 0)
               {
                   printf("Error :: on DB insert \n");

               }
           }

           free(namelist[index]);
       }
       else if(S_ISDIR(fileStat.st_mode))
       {
           p_content->contentId ++;
           p_content->totalContents ++;
           childCount ++;

           //printf("[%3d] :: [%2d] The Dir  Name :: %s \n", p_content->contentId, folderId, namelist[index]->d_name);

           //DO FSM

           //Prepare
           s_content.id                = p_content->contentId;
           s_content.p_path            = filePath;
           s_content.p_mediaFormatInfo = NULL;
           s_content.type              = CNC_DMS_CONTENT_TYPE_DIRECTORY;
           s_content.wd                = -1;                                   //TODO update
           s_content.parentId          = folderId;
           s_content.p_name            = namelist[index]->d_name;
           s_content.p_url             = NULL;

           //Insert Content into DB
           retValue = dmsClmDbAddContent(p_clmObj->p_db,  &s_content);
           if(retValue < 0)
           {
               printf("Error :: on DB insert \n");
           }

           clmScanDir(filePath, p_clmObj);
           free(namelist[index]);
       }
       else
       {
           //Neither Directory nor Regular File
           free(namelist[index]);
           //printf("Content Neither Directory nor Regular File [%s]\n", filePath);
           continue;
       }
    }

    free(namelist);

    //Update Child Count into DB
    dmsClmDbUpdateChildCount(p_clmObj->p_db, folderId, childCount);


    printf("The Directory [%d] contains [%d] Childs\n", folderId, childCount);

    return 0;
}




CNC_SINT32   cnc_dmsClmInit(CNC_VOID)
{
    CNC_SINT32 errCode = 0;

    ////CNC_TRACE_FUNCTION_ENTRY();

    //DB
    dmsClmDbInit();

    //FSM

    //Initialization
    CNC_SoftDemuxer_ProbeInit();

    ////CNC_TRACE_FUNCTION_EXIT();
    return errCode;
}


CNC_SINT32   cnc_dmsClmCreate(cncDmsContentListMgt_t    **pp_contentListMgt,
                              CNC_CHAR                  *p_shareDirPath,
                              cncDmsFsmNotifier         *p_fsmNotifier,
                              CNC_SINT32                maxEntry)
{
    CNC_SINT32              errCode = 0;
    CNC_SINT32              numOfContents = 0;
    cncDmsContentListMgt_t  *p_contentListMgt = NULL;
    clmContentIds_t          s_contentId;

    ////CNC_TRACE_FUNCTION_ENTRY();
    if (pp_contentListMgt == NULL || p_shareDirPath == NULL || p_fsmNotifier == NULL || maxEntry < 0)
    {
        ////CNC_TRACE_ERROR(0, "NULL Argument \n");
        errCode = -1;
        goto _CNC_DMS_CLM_CREATE_END;
    }

    //Clear output pointer.
    *pp_contentListMgt = NULL;

    //Allocate Memory
    p_contentListMgt = (cncDmsContentListMgt_t *)malloc (sizeof(cncDmsContentListMgt_t));
    if (p_contentListMgt == NULL)
    {
        ////CNC_TRACE_ERROR(0, "Memory Allocation\n");
        errCode = -1;
        goto _CNC_DMS_CLM_CREATE_END;
    }

    memset(p_contentListMgt, 0, sizeof(cncDmsContentListMgt_t));

    //Create DB Instance
    errCode = dmsClmDbCreate(".", &(p_contentListMgt->p_db));
    if(errCode < 0)
    {
        ////CNC_TRACE_ERROR(0, "dmsClmDbCreate [%d]\n", errCode);
        goto _CNC_DMS_CLM_CREATE_END;
    }

    //No MFD instance.
    p_contentListMgt->p_mfd = NULL;

    //TODO
    p_contentListMgt->p_fsm = NULL;

    p_contentListMgt->p_contentId = malloc(sizeof(clmContentIds_t));
    if(p_contentListMgt->p_contentId  == NULL)
    {
        ////CNC_TRACE_ERROR(0, "Memory Allocation\n");
        errCode = -1;
        goto _CNC_DMS_CLM_CREATE_END;
    }


    p_contentListMgt->p_fsmNotifier = p_fsmNotifier;
    p_contentListMgt->maxEntry = maxEntry;
    p_contentListMgt->upnpHandle = -1;

    //First Entry is the Root Entry;
    s_contentId.parentId      = -1;
    s_contentId.maxContents   =  maxEntry;
    s_contentId.totalContents =  0;

    //First Entry insert into DB.
    //  ID = 0
    //  Child Count = 1 (only FS view)
    //  ParentId    = -1
    //  name        =  'root'

    // ParentID = 0;
    // ID       = Start From 1001

    p_contentListMgt->p_contentId->parentId      = 0;
    p_contentListMgt->p_contentId->totalContents = 0;
    p_contentListMgt->p_contentId->maxContents   = maxEntry;
    p_contentListMgt->p_contentId->contentId     = 1000;

    errCode = clmScanDir(p_shareDirPath, p_contentListMgt);
    if (errCode < 0)
    {
        ////CNC_TRACE_ERROR(0, "cncDmsScanDirectory errCode :: [%d] \n", errCode);
        goto _CNC_DMS_CLM_CREATE_END;
    }

    ////CNC_TRACE_INFO(0, "Total Num of Contents in [%s] Total num of entries [%d]\n", p_shareDirPath, p_contentListMgt->p_contentId->totalContents );
    printf("Total Num of Contents in [%s] Total num of entries [%d]\n", p_shareDirPath, p_contentListMgt->p_contentId->totalContents );

_CNC_DMS_CLM_CREATE_END:

    //Update Output pointer
    *pp_contentListMgt = p_contentListMgt;

    ////CNC_TRACE_FUNCTION_EXIT();
    return errCode;
}


CNC_SINT32   cnc_dmsClmGetContentListFromId(cncDmsContentListMgt_t *p_listMgt, cncDmsContentList_t  **pp_contentList, CNC_SINT32 *p_numOfContent)
{

    CNC_SINT32 errCode = 0;

    ////CNC_TRACE_FUNCTION_ENTRY();

#if 0
    //Update Content List.
           if (*p_contentList == NULL)
           {
               p_contentList->p_content = p_content;
               gp_endContent = p_content;
           }
           else
           {
               //Adding Content Node at the end of ContentList.
               gp_endContent->p_next = p_content;
               gp_endContent = p_content;
           }
#endif

    ////CNC_TRACE_FUNCTION_EXIT();
    return errCode;
}


CNC_SINT32   cnc_dmsClmGetContentListFromWd(cncDmsContentListMgt_t *p_listMgt, cncDmsContentList_t  **pp_contentList)
{

    CNC_SINT32 errCode = 0;

    ////CNC_TRACE_FUNCTION_ENTRY();

    ////CNC_TRACE_FUNCTION_EXIT();
    return errCode;
}


CNC_SINT32   cnc_dmsClmFreeContentList(cncDmsContentList_t  *p_contentList)
{
    CNC_SINT32          errCode = 0;
    cncDmsContentList_t *p_tmpContentList = NULL;
    cncDmsContent_t     *p_content = NULL;

    //CNC_TRACE_FUNCTION_ENTRY();
    if (p_contentList == NULL)
    {
        //CNC_TRACE_ERROR(0, "NULL argument \n");
        errCode = -1;
        goto _CNC_DMS_CLM_FREE_CONTENT_LIST_END;
    }

    while (p_contentList != NULL)
    {
        p_content = p_contentList->p_content;

        if (p_content != NULL)
        {
            if (p_content->p_mediaFormatInfo != NULL)
            {
                free(p_content->p_mediaFormatInfo);
            }

            free(p_content);
        }

        p_contentList = p_contentList->p_next;
        p_tmpContentList = p_contentList;

        free(p_tmpContentList);
    }



_CNC_DMS_CLM_FREE_CONTENT_LIST_END:

    //CNC_TRACE_FUNCTION_EXIT();
    return errCode;
}


CNC_SINT32   cnc_dmsClmDestroy(cncDmsContentListMgt_t *p_contentListMgt)
{

    CNC_SINT32      errCode = 0;

    //CNC_TRACE_FUNCTION_ENTRY();
    if (p_contentListMgt == NULL)
    {
        //CNC_TRACE_ERROR(0, "NULL argument \n");
        errCode = -1;
        goto _CNC_DMS_CLM_DESTROY_END;
    }

    //TODO Call fsm, mfd, db, notify destory functions.

    p_contentListMgt->p_fsmNotifier = NULL;

    //free content list mgt
    free(p_contentListMgt);

_CNC_DMS_CLM_DESTROY_END:

    //CNC_TRACE_FUNCTION_EXIT();
    return errCode;
}


CNC_SINT32   cnc_dmsClmDeInit(CNC_VOID)
{
    CNC_SINT32 errCode = 0;

    //CNC_TRACE_FUNCTION_ENTRY();

    //CNC_TRACE_FUNCTION_EXIT();
    return errCode;
}

