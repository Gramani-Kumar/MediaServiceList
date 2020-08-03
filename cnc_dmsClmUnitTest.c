/*
 * cnc_dmsClmUnitTest.c
 *
 *      Author: Ganesh Kumar R
 */

/* Define TRACE level override */
#ifdef CNC_TRACE_LEVEL_DLNA_DMS
#if (CNC_TRACE_LEVEL_DLNA_DMS >= 0)
#define TRACE_MODULE_LEVEL   CNC_TRACE_LEVEL_DLNA_DMS
#endif
#endif

#include <stdio.h>
#include <string.h>

#include "cnc_trace.h"
#include "cnc_dmsClm.h"


/* Define TRACE module ID */
#define TRACE_MODULE_ID               //CNC_TRACE_MODULE_DLNA_DMS



/* Notification Callback */
void appDmsFsmNotification(cncDmsFsmNotification_t *p_notification)
{
    //CNC_TRACE_INFO(0, "cncDmsFsmNotifier \n");
}




CNC_SINT32 main(int argc, char **argv)
{
    char                    a_path[512] = {0};
    CNC_SINT32              errCode = 0;
    cncDmsContentListMgt_t  *p_contentListMgt = NULL;
    CNC_SINT32              maxEntry = 500;

    if(argc < 2)
    {
    	printf("Error in Usage \n ./app [Shared Path]\n");
    	return -1;
    }

    printf("\n************************************************************\n");
    //CNC_TRACE_INFO(0, "Enter Shared path \n");
    printf("Enter Shared path :: %s\n", argv[1]);
    //scanf("%s", a_path);
    strcpy(a_path, argv[1]);

    printf("\n************************************************************\n");

    errCode = cnc_dmsClmInit();
    if (errCode < 0)
    {
        //CNC_TRACE_ERROR(0, "cnc_dmsClmInit errCode :: [%d]\n", errCode);
        goto _MAIN_END;
    }

    errCode = cnc_dmsClmCreate(&p_contentListMgt,
                               a_path,
                               appDmsFsmNotification,
                               maxEntry);
    if (errCode < 0)
    {
        //CNC_TRACE_ERROR(0, "cnc_dmsClmCreate errCode :: [%d]\n", errCode);
        goto _MAIN_END;
    }
    printf("retVal :: %d \n",errCode);

_MAIN_END:

    return errCode;
}






