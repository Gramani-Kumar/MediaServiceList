static CNC_SINT32 cncDmsScanDirectory(CNC_CHAR *p_filePath, clmContentIds_t *p_contentId)
{
    CNC_CHAR                filePath[1024] = {0};
    CNC_SINT32              retVal = 0;
    CNC_SINT32              dirCount = 0;
    CNC_SINT32              isDirectory = 0;
    CNC_SINT32              i = 0;
    struct dirent           **pp_nameList = NULL;
    struct stat             st;
    cncMediaFormatInfo_t    *p_mediaFormatInfo = NULL;
    cncDmsContent_t         s_content;
    clmContentIds_t         s_contentId;

    if (p_filePath == NULL || p_contentId == NULL)
    {
        CNC_TRACE_ERROR(0, "NULL parameter \n");
        retVal = -1;
        goto _CNC_DMS_SCAN_DIRECTORY_END;
    }

    /*
     * Scan directory and find the files and sub-directories non-recursively
     */
    dirCount = scandir(p_filePath, &pp_nameList, NULL, NULL); //,alphasort);
    if (dirCount < 0)
    {
        CNC_TRACE_ERROR(0, "scandir() errorVal :: [%d] \n", dirCount);
        retVal = dirCount;
        goto _CNC_DMS_SCAN_DIRECTORY_END;
    }

    //Update numOfContents present in Parent Directory. Exclude . , .. directories

    CNC_TRACE_INFO(0, "HS_ Dir count :: [%d] \n", dirCount);

    for (i = 0; i < dirCount; i++)
    {
        if (pp_nameList[i] == NULL)
        {
            continue;
        }

        CNC_TRACE_INFO(0, "Name of File [%s] \n", pp_nameList[i]->d_name);

        /* Ignore "." , ".." Directories.*/
        if (strncmp(pp_nameList[i]->d_name, ".", 1) == 0 || strncmp(pp_nameList[i]->d_name, "..", 2) == 0)
        {
            //Free name list of ith instance.
            free(pp_nameList[i]);
            pp_nameList[i] = NULL;
            continue;
        }

        //Clear Content.
        memset(&s_content, 0, sizeof(cncDmsContent_t));

        snprintf(filePath, sizeof(filePath), "%s%s%s", p_filePath, "/", pp_nameList[i]->d_name);

        CNC_TRACE_ERROR(0, " filePath [%s], p_path[%s] \n", filePath);

        retVal = stat(filePath, &st);
        if (retVal < 0)
        {
            CNC_TRACE_ERROR(0, "Error in stat() for [%s]\n", pp_nameList[i]->d_name);
            perror("Error :");
            //Free name list of ith instance.
            free(pp_nameList[i]);
            pp_nameList[i] = NULL;
            continue;
        }

        //Update Content Path.
        s_content.p_path = strdup(filePath);
        if (s_content.p_path == NULL)
        {
            CNC_TRACE_ERROR(0, "Memory Allocation using strdup() \n");
            retVal = -1;
            goto _CNC_DMS_SCAN_DIRECTORY_END;
        }

        //Check if Item is directory type.
        s_content.type = S_ISDIR(st.st_mode)?CNC_DMS_CONTENT_TYPE_DIRECTORY:CNC_DMS_CONTENT_TYPE_REGU_FILE;

        if (s_content.type == CNC_DMS_CONTENT_TYPE_DIRECTORY)
        {

            p_contentId->lastId ++;
            p_contentId->parentId =

            //Call recursively.
            retVal = cncDmsScanDirectory(s_content.p_path, p_contentId);
            if (retVal < 0)
            {
                CNC_TRACE_ERROR(0, "cncDmsScanDirectory for [%s] errCode [%d]\n", filePath, retVal);
            }

            //Update Entry
            p_contentId->totalContents ++;

            CNC_TRACE_INFO(0, "*****************************************************************\n");
            CNC_TRACE_INFO(0, "Child Count of Directory [%s] is [%d] \n", s_content.p_path, s_content.childCount);
            CNC_TRACE_INFO(0, "*****************************************************************\n");

            //TODO Insert info FSM.
        }
        else
        {
            s_content.p_mediaFormatInfo = (cncMediaFormatInfo_t *)malloc(sizeof(cncMediaFormatInfo_t));
            if(s_content.p_mediaFormatInfo == NULL)
            {
                CNC_TRACE_ERROR(0, "Memory Allocation for media format structure\n");
                retVal = -1;
                goto _CNC_DMS_SCAN_DIRECTORY_END;
            }

            p_mediaFormatInfo = s_content.p_mediaFormatInfo;
            p_contentId->totalContents ++;

#if 0
            retVal = CNC_SoftDemuxer_Probe(s_content.p_path, p_mediaFormatInfo);
            if(retVal < 0)
            {
                CNC_TRACE_ERROR(0, "CNC_SoftDemuxer_Probe errorCode :: [%d] \n", retVal);
                //Free name list of ith instance.
                free(pp_nameList[i]);
                pp_nameList[i] = NULL;
                //Override errCode.
                retVal = 0;
                continue;
            }

            //Update Entry
            p_contentId->totalContents ++;

            printf("***************************************************\n");
            printf("Extracted Meta data \n");
            if (p_mediaFormatInfo->mediaClass == CNC_MEDIA_CLASS_VIDEO )
            {
              printf("[%s] is Video file \n", pp_nameList[i]->d_name);
            }
            else if (p_mediaFormatInfo->mediaClass == CNC_MEDIA_CLASS_AUDIO)
            {
              printf("[%s] is Audio file \n", pp_nameList[i]->d_name);
            }
            else if (p_mediaFormatInfo->mediaClass == CNC_MEDIA_CLASS_IMAGE)
            {
              printf("[%s] is Image file \n", pp_nameList[i]->d_name);
            }

            printf("Content Size             :: [%lld] \n", p_mediaFormatInfo->size);
            if (p_mediaFormatInfo->mediaClass == CNC_MEDIA_CLASS_VIDEO || p_mediaFormatInfo->mediaClass == CNC_MEDIA_CLASS_AUDIO)
            {
              printf("Bitrate                  :: [%d]\n", p_mediaFormatInfo->bitRate);
              printf("DurationInMs             :: [%d]\n", p_mediaFormatInfo->durationInMsec);
              printf("Sample rate              :: [%d]\n", p_mediaFormatInfo->formatDetail.audioInfo.sampleRate);
              printf("NumOfChannels            :: [%d] \n",p_mediaFormatInfo->formatDetail.audioInfo.numOfChannels);
              printf("Media format type        :: [%d]\n", p_mediaFormatInfo->format);
            }

            if (p_mediaFormatInfo->mediaClass == CNC_MEDIA_CLASS_IMAGE)
            {
              CNC_TRACE_INFO(0, "Width     :: %d \n", p_mediaFormatInfo->formatDetail.imageInfo.width);
              CNC_TRACE_INFO(0, "Height    :: %d \n", p_mediaFormatInfo->formatDetail.imageInfo.height);
            }
#endif
        }

        printf("***************************************************\n");

        //Free name list of ith instance.
        free(pp_nameList[i]);
        pp_nameList[i] = NULL;
    }

    free(pp_nameList);
    pp_nameList = NULL;

_CNC_DMS_SCAN_DIRECTORY_END:
    return retVal;
}

