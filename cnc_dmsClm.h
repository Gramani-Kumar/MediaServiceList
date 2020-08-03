/*
 # COPYRIGHT NOTICE
 #
 #***************************************************************************

 # END OF COPYRIGHT NOTICE
 #
 */

/**
 * @file cnc_dmsClm.h
 * @brief All cnc_dms Content List Manager Module api's declarations will be present.
 * @ingroup dlna
 */
#ifndef _CNC_DMS_CLM_H__
#define _CNC_DMS_CLM_H__

/* ============================ System Includes =========================== */
// NONE

/* ============================= User Includes ============================ */
#include "cnc_common_typedef.h"
//#include "cnc_mediainfo.h"
#include "ffmpeg_test.h" //TODO Remove later.
/* =========================== Macro definitions ========================== */
// NONE

/* =========================== Type definitions =========================== */
  /*Content List Management*/

/* content Type */
/**
 * \ enum  _cncDmsContentType_t
 * \brief  Used to select content type.
 */
typedef enum _cncDmsContentType_t
{
   CNC_DMS_CONTENT_TYPE_DIRECTORY,
   CNC_DMS_CONTENT_TYPE_REGU_FILE,
   CNC_DMS_CONTNET_TYPE_ROOT
}cncDmsContentType_t;


/**
 * \ struct  _cncDmsContent_t
 * \brief    Used to store Content information
 */
typedef struct _cncDmsContent_t
{
    CNC_SINT32   	  id;                            /* Content Id*/
    CNC_SINT32   	  parentId;                      /* Content's parent Directory Id*/
    CNC_SINT32            childCount;                    /* Child count if content is Directory type*/
    CNC_SINT32            wd;                            /* Watch discriptor given by FSM*/
    CNC_CHAR              *p_path;                       /* Contains file full path*/
    cncDmsContentType_t   type;                          /* Content type*/
    cncMediaFormatInfo_t  *p_mediaFormatInfo;            /* Contains Media format Information*/
    CNC_CHAR              *p_url;                        /* Content URL */
    CNC_CHAR              *p_name;                       /* Content Name */

}cncDmsContent_t;


/**
 * \ struct  _cncDmsContentList_t
 * \brief    Used to store Content List
 */
typedef struct _cncDmsContentList_t
{
    cncDmsContent_t	        *p_content;              /*Content Inforamtion*/
    struct _cncDmsContentList_t *p_next;                 /*Points Next Content List Node*/

}cncDmsContentList_t;


/**
 * \ struct  _cncDmsFsmNotification_t
 * \brief     Contains FS change Notifications Information
 */
typedef struct _cncDmsFsmNotification_t
{
  CNC_VOID      *p_cookie;         //Event Mgr / CDS Object
  CNC_SINT32    numOfEvent;
  CNC_SINT32    **pp_contentUpdateID;
  CNC_SINT32    **pp_containerID;

}cncDmsFsmNotification_t;

/* Notification Callback */
typedef void (cncDmsFsmNotifier)(cncDmsFsmNotification_t *p_notification);

typedef struct _clmContentIds_t
{
    int     contentId;
    int     parentId;
    int     maxContents;
    int     totalContents;

} clmContentIds_t;


/**
 * \ struct  _cncDmsContentListMgt_t
 * \brief    Content List Management
 */
typedef struct _cncDmsContentListMgt_t
{
 CNC_SINT32	    totalEntry;                          /* Total numOfContents entried in to Database*/
 CNC_SINT32	    maxEntry;                            /* Maximum limit for content entries*/
 CNC_SINT32	    upnpHandle;                          /* UPnP Handle*/
 CNC_VOID 	    *p_fsm;                              /* FSM Object*/
 CNC_VOID 	    *p_mfd;                              /* MFD Object*/
 CNC_VOID 	    *p_db;                               /* Data base object*/
 CNC_VOID 	    *p_notify;                           /* Notify Object*/
 cncDmsFsmNotifier  *p_fsmNotifier;                      /* FS Notifier*/

 //tmp.
 clmContentIds_t    *p_contentId;                        /* Private Object*/

}cncDmsContentListMgt_t;

/* ======================== Function declarations ========================= */

CNC_SINT32   cnc_dmsClmInit(CNC_VOID);

CNC_SINT32   cnc_dmsClmCreate(cncDmsContentListMgt_t    **pp_contentListMgt,
                              CNC_CHAR                  *p_shareDirPath,
                              cncDmsFsmNotifier         *p_fsmNotifier,
                              CNC_SINT32                maxEntry);

CNC_SINT32   cnc_dmsClmGetContentListFromId(cncDmsContentListMgt_t *p_listMgt, cncDmsContentList_t  **pp_contentList, CNC_SINT32 *p_numOfContent);

CNC_SINT32   cnc_dmsClmGetContentListFromWd(cncDmsContentListMgt_t *p_listMgt, cncDmsContentList_t  **pp_contentList);

/* Ext, Search / Browse actions. */

CNC_SINT32   cnc_dmsClmFreeContentList(cncDmsContentList_t  *p_contentList);

CNC_SINT32   cnc_dmsClmDestroy(cncDmsContentListMgt_t *p_listMgt);

CNC_SINT32   cnc_dmsClmDeInit(CNC_VOID);

/****************************************************/

#endif  //_CNC_DMS_CLM_H__

