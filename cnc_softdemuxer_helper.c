/*
 * cnc_softdemuxer_helper.c
 *
 *  Created on: 
 *      Author: Ganesh Kumar R.
 */

/* Define TRACE level override */
#ifdef CNC_TRACE_LEVEL_MEDIA_DEMUXER
#if (CNC_TRACE_LEVEL_MEDIA_DEMUXER >= 0)
#define TRACE_MODULE_LEVEL   CNC_TRACE_LEVEL_MEDIA_DEMUXER
#endif
#endif

#define TRACE_MODULE_ID      CNC_TRACE_MODULE_MEDIA_DEMUXER

#include <stdio.h>
#include <sys/stat.h>

#include "libavformat/avformat.h"
#include "libavutil/avutil.h"            //For AVMediaType
#include "cnc_softdemuxer_helper.h"
//#include "ffmpeg_test.h"
//#include "cnc_trace.h"



/**
*Define the supported stream types.
* Encoder only supports G729/726/723/711 and MP2ENC.Decoder can support all stream type
* @see CSAUD_SetCodecType function
*
*/
typedef enum
{
    AUD_STREAM_TYPE_MPA = 0,///<Stream type for MPEG1.2,2.5 audio.
    AUD_STREAM_TYPE_AC3 = 1,///<Stream type for Dolby AC-3 audio.
    AUD_STREAM_TYPE_DTS = 2,///<Stream type for DTS.
    AUD_STREAM_TYPE_LPCM = 3,///<Stream type for LPCM.
    AUD_STREAM_TYPE_AAC = 4,///<Stream type for MPEG2, 4 AAC audio.
    AUD_STREAM_TYPE_AAC_LATM = 5,///<Stream type for HEAAC
    AUD_STREAM_TYPE_AIB = 6,///<Stream type for Audio input block.
    AUD_STREAM_TYPE_G729 = 7,///<Stream type for G729 audio.
    AUD_STREAM_TYPE_G723 = 8,///<Stream type for G723 audio.
    AUD_STREAM_TYPE_G726 = 9,///<Stream type for G726
    AUD_STREAM_TYPE_G711 = 10,///<Stream type for G711
    AUD_STREAM_TYPE_MP2ENC = 11,///<Stream type for MP2ENC
    AUD_STREAM_TYPE_FLAC = 12,///<Stream type for FLAC
    AUD_STREAM_TYPE_DTSWAV = 13,///<Stream type for DTSWAV
    AUD_STREAM_TYPE_REAL_COOK = 14,///<Stream type for real cook
    AUD_STREAM_TYPE_WMA = 15,///<Stream type for WMA
    AUD_STREAM_TYPE_OGG_VORBIS = 16, ///<Stream type for Ogg Vorbis
    AUD_STREAM_TYPE_APE = 17,///<Stream type for APE
    AUD_STREAM_TYPE_REAL_ATRC = 18,///<Stream type for real atrc
    AUD_STREAM_TYPE_AV3_AASF = 19,///<Stream type for AV3 AASF
    AUD_STREAM_TYPE_AV3_AATF = 20,///<Stream type for AV3 AATF
    AUD_STREAM_TYPE_MS_WMAPRO = 21,///<Stream type for MS WMAPRO
    AUD_STREAM_TYPE_PCMENC = 22,///<Stream type for PCMENC
    AUD_STREAM_TYPE_REAL_SIPR = 23,///<Stream type for MS WMAPRO
    AUD_STREAM_TYPE_AMR=24,///<Stream type for AMR
    AUD_STREAM_TYPE_DOLBY = 25,
    AUD_STREAM_TYPE_LPCM_DVD=26,///<Stream type for LPCM_DVD
    AUD_STREAM_TYPE_PCM_MULAW = 27,///<Stream type for PCM_MULAW
    AUD_STREAM_TYPE_PCM_ALAW = 28,///<Stream type for PCM_ALAW
    AUD_STREAM_TYPE_ADPCM_G726 = 29,///<Stream type for ADPCM_G726
    AUD_STREAM_TYPE_TRUEHD = 30,///<Stream type for Dolby TrueHD
    AUD_STREAM_TYPE_DTS_HD = 31,///<Stream type for DTS HD MA
    AUD_STREAM_TYPE_UNKNOWN = 32  // 5 bit
} CSAUD_STREAM_TYPE;
static CNC_SINT32 convertAudioTypeToCNCMediaFormat(CNC_SINT32 codecType)
{
    CNC_SINT32       mediaFormatType = 0;

    switch(codecType)
    {
        case AUD_STREAM_TYPE_MPA:
            mediaFormatType = CNC_MEDIA_FORMAT_AUDIO_MP3;
            break;

        case AUD_STREAM_TYPE_DOLBY:
            mediaFormatType = CNC_MEDIA_FORMAT_AUDIO_DOLBY_TRUE_HD;
            break;

        case AUD_STREAM_TYPE_AC3:
            mediaFormatType = CNC_MEDIA_FORMAT_AUDIO_AC3;
            break;

        case AUD_STREAM_TYPE_AAC:
            mediaFormatType = CNC_MEDIA_FORMAT_AUDIO_MPEG2_AAC;
            break;

        case AUD_STREAM_TYPE_AAC_LATM:
            mediaFormatType = CNC_MEDIA_FORMAT_AUDIO_MPEG2_AAC;
            break;

        case AUD_STREAM_TYPE_LPCM:
            mediaFormatType = CNC_MEDIA_FORMAT_AUDIO_LPCM;
            break;

        case AUD_STREAM_TYPE_PCM_ALAW:
            mediaFormatType = CNC_MEDIA_FORMAT_AUDIO_ALAW;
            break;

        case AUD_STREAM_TYPE_PCM_MULAW:
            mediaFormatType = CNC_MEDIA_FORMAT_AUDIO_MULAW;
            break;
        case AUD_STREAM_TYPE_DTS_HD:
            mediaFormatType = CNC_MEDIA_FORMAT_AUDIO_DTS_HD;
            break;
        case AUD_STREAM_TYPE_DTS:
            mediaFormatType = CNC_MEDIA_FORMAT_AUDIO_DTS;
            break;

        case AUD_STREAM_TYPE_G729:
            mediaFormatType = CNC_MEDIA_FORMAT_AUDIO_G729;
            break;

        case AUD_STREAM_TYPE_G723:
            mediaFormatType = CNC_MEDIA_FORMAT_AUDIO_G723;
            break;

        case AUD_STREAM_TYPE_G726:
            mediaFormatType = CNC_MEDIA_FORMAT_AUDIO_G726;
            break;

        case AUD_STREAM_TYPE_G711:
            mediaFormatType = CNC_MEDIA_FORMAT_AUDIO_G711;
            break;

        case AUD_STREAM_TYPE_FLAC:
            mediaFormatType = CNC_MEDIA_FORMAT_AUDIO_FLAC;
            break;

        case AUD_STREAM_TYPE_WMA:
            mediaFormatType = CNC_MEDIA_FORMAT_AUDIO_WMA;
            break;

        case AUD_STREAM_TYPE_MS_WMAPRO:
            mediaFormatType = CNC_MEDIA_FORMAT_AUDIO_WMAPRO;
            break;

        case AUD_STREAM_TYPE_REAL_COOK:
            mediaFormatType = CNC_MEDIA_FORMAT_AUDIO_REAL_COOK;
            break;

        case AUD_STREAM_TYPE_DTSWAV:
            mediaFormatType = CNC_MEDIA_FORMAT_AUDIO_DTS;
            break;

        case AUD_STREAM_TYPE_OGG_VORBIS:
            mediaFormatType = CNC_MEDIA_FORMAT_AUDIO_OGG_VORBIS;
            break;

        case AUD_STREAM_TYPE_APE:
            mediaFormatType = CNC_MEDIA_FORMAT_AUDIO_APE;
            break;

        case AUD_STREAM_TYPE_AMR:
            mediaFormatType = CNC_MEDIA_FORMAT_AUDIO_AMR;
            break;
        case AUD_STREAM_TYPE_TRUEHD:
            mediaFormatType = CNC_MEDIA_FORMAT_AUDIO_TRUEHD;
            break;
        case AUD_STREAM_TYPE_AIB:
        case AUD_STREAM_TYPE_MP2ENC:
        case AUD_STREAM_TYPE_REAL_ATRC:
        case AUD_STREAM_TYPE_REAL_SIPR:
        case AUD_STREAM_TYPE_UNKNOWN:
        default:
            mediaFormatType = CNC_MEDIA_FORMAT_NONE;
            break;
    }

    //CNC_TRACE_INFO(0, "Converted mediaFormatType: [%d]\n", mediaFormatType);
    return mediaFormatType;
}


/*
 * Converts Codec_ID to CNCCodecType
 * Taken from cnc_softdemuxer_pub.c
 */
static CNC_SINT32 mapCodecTypeToCNCCodecType(CNC_SINT32 classType, CNC_SINT32 codec, CNC_SINT32 *p_cncCodecType)
{
    CNC_SINT32     retValue = 0;

    if(p_cncCodecType == NULL)
    {
        printf("Invalid Param\n");
        retValue = -1;
        goto END_MAP_CODEC_TYPE;
    }

#ifdef ARCH_CSM1900
    switch (codec)
    {
        case CODEC_ID_MPEG1VIDEO:
        case CODEC_ID_MPEG2VIDEO:
            *p_cncCodecType = VID_STREAM_TYPE_MPEG2_TS;
            break;

        case CODEC_ID_H263:
        case CODEC_ID_FLV1:
            *p_cncCodecType = VID_STREAM_TYPE_H263;
            break;

        case CODEC_ID_H264:
            *p_cncCodecType = VID_STREAM_TYPE_H264_TS;
            break;

        case CODEC_ID_MPEG4:
            *p_cncCodecType = VID_STREAM_TYPE_MPEG4;
            break;

        case CODEC_ID_MJPEG:
            *p_cncCodecType = VID_STREAM_TYPE_MJPG;
            break;

        case CODEC_ID_VC1:
        case CODEC_ID_WMV3:
            *p_cncCodecType = VID_STREAM_TYPE_VC1;
            break;

        case CODEC_ID_MSMPEG4V3:
            *p_cncCodecType = VID_STREAM_TYPE_DIVX;
            break;

        case CODEC_ID_CAVS:
            *p_cncCodecType = VID_STREAM_TYPE_AVS;
            break;
        case CODEC_ID_VP8:
            *p_cncCodecType = VID_STREAM_TYPE_VP8;
            break;
#ifdef SUPPORT_SOFT_DECODE
        case CODEC_ID_VP9:
            *p_cncCodecType = VID_STREAM_TYPE_VP9;
            break;
#endif
        case CODEC_ID_RV30:
        case CODEC_ID_RV40:
            *p_cncCodecType = VID_STREAM_TYPE_RV;
            break;
        case CODEC_ID_SIPR:
            *p_cncCodecType = AUD_STREAM_TYPE_REAL_SIPR;
            break;
        case CODEC_ID_ATRAC3:
            *p_cncCodecType = AUD_STREAM_TYPE_REAL_ATRC;
            break;
        case CODEC_ID_MP1:
        case CODEC_ID_MP2:
        case CODEC_ID_MP3:
            *p_cncCodecType = AUD_STREAM_TYPE_MPA;
            break;

#ifdef THIRDPARTY_DOLBY
        case CODEC_ID_AC3:
        case CODEC_ID_EAC3:
            *p_cncCodecType = AUD_STREAM_TYPE_DOLBY;
            break;
#else
        case CODEC_ID_AC3:
        case CODEC_ID_EAC3:
            *p_cncCodecType = AUD_STREAM_TYPE_AC3;
            break;
#endif
        case CODEC_ID_TRUEHD:
            *p_cncCodecType = AUD_STREAM_TYPE_TRUEHD;
            break;
        case CODEC_ID_AAC:
            *p_cncCodecType = AUD_STREAM_TYPE_AAC;
            break;

        case CODEC_ID_AAC_LATM:
            *p_cncCodecType = AUD_STREAM_TYPE_AAC_LATM;
            break;

        case CODEC_ID_PCM_MULAW:
            *p_cncCodecType = AUD_STREAM_TYPE_PCM_MULAW;
            break;

        case CODEC_ID_PCM_ALAW:
            *p_cncCodecType = AUD_STREAM_TYPE_PCM_ALAW;
            break;

        case CODEC_ID_WMAV1:
        case CODEC_ID_WMAV2:
            *p_cncCodecType = AUD_STREAM_TYPE_WMA;
            break;

        case CODEC_ID_WMAPRO:
            *p_cncCodecType = AUD_STREAM_TYPE_MS_WMAPRO;
            break;

        case CODEC_ID_DTS:
            *p_cncCodecType = AUD_STREAM_TYPE_DTS;
            break;

        case CODEC_ID_FLAC:
            *p_cncCodecType = AUD_STREAM_TYPE_FLAC;
            break;
        case CODEC_ID_VORBIS:
            *p_cncCodecType = AUD_STREAM_TYPE_OGG_VORBIS;
            break;

        case CODEC_ID_APE:
            *p_cncCodecType = AUD_STREAM_TYPE_APE;
            break;
        case CODEC_ID_COOK:
            *p_cncCodecType = AUD_STREAM_TYPE_REAL_COOK;
            break;
        case CODEC_ID_DTS14LE:
            *p_cncCodecType = AUD_STREAM_TYPE_DTSWAV;
            break;

        case CODEC_ID_AMR_NB:
            *p_cncCodecType = AUD_STREAM_TYPE_AMR;
            break;

        case  CODEC_ID_PCM_S16LE:
        case  CODEC_ID_PCM_U16LE:
        case  CODEC_ID_PCM_S8:
        case  CODEC_ID_PCM_U8:
        case  CODEC_ID_PCM_S32LE:
        case  CODEC_ID_PCM_U32LE:
        case  CODEC_ID_PCM_S24LE:
        case  CODEC_ID_PCM_U24LE:
        case  CODEC_ID_PCM_S24DAUD:
        case  CODEC_ID_PCM_ZORK:
        case  CODEC_ID_PCM_S16LE_PLANAR:
        case  CODEC_ID_PCM_DVD:
        case  CODEC_ID_PCM_F32LE:
        case  CODEC_ID_PCM_F64LE:
        case  CODEC_ID_PCM_BLURAY:
        case  CODEC_ID_PCM_LXF:
        case  CODEC_ID_S302M:
        case  CODEC_ID_ADPCM_IMA_QT:
        case  CODEC_ID_ADPCM_IMA_WAV:
        case  CODEC_ID_ADPCM_IMA_DK3:
        case  CODEC_ID_ADPCM_IMA_DK4:
        case  CODEC_ID_ADPCM_IMA_WS:
        case  CODEC_ID_ADPCM_IMA_SMJPEG:
        case  CODEC_ID_ADPCM_MS:
        case  CODEC_ID_ADPCM_4XM:
        case  CODEC_ID_ADPCM_XA:
        case  CODEC_ID_ADPCM_ADX:
        case  CODEC_ID_ADPCM_EA:
        case  CODEC_ID_ADPCM_CT:
        case  CODEC_ID_ADPCM_SWF:
        case  CODEC_ID_ADPCM_YAMAHA:
        case  CODEC_ID_ADPCM_SBPRO_4:
        case  CODEC_ID_ADPCM_SBPRO_3:
        case  CODEC_ID_ADPCM_SBPRO_2:
        case  CODEC_ID_ADPCM_THP:
        case  CODEC_ID_ADPCM_IMA_AMV:
        case  CODEC_ID_ADPCM_EA_R1:
        case  CODEC_ID_ADPCM_EA_R3:
        case  CODEC_ID_ADPCM_EA_R2:
        case  CODEC_ID_ADPCM_IMA_EA_SEAD:
        case  CODEC_ID_ADPCM_IMA_EA_EACS:
        case  CODEC_ID_ADPCM_EA_XAS:
        case  CODEC_ID_ADPCM_EA_MAXIS_XA:
        case  CODEC_ID_ADPCM_IMA_ISS:
        case  CODEC_ID_ADPCM_G722:
            pcm_bit_reverse_flag = 0;
            *p_cncCodecType = AUD_STREAM_TYPE_LPCM;
            break;
        case  CODEC_ID_PCM_S16BE:
        case  CODEC_ID_PCM_S32BE:
        case  CODEC_ID_PCM_U32BE:
        case  CODEC_ID_PCM_U16BE:
        case  CODEC_ID_PCM_F64BE:
        case  CODEC_ID_PCM_F32BE:
        case  CODEC_ID_PCM_U24BE:
        case  CODEC_ID_PCM_S24BE:
            pcm_bit_reverse_flag = 1;
            *p_cncCodecType = AUD_STREAM_TYPE_LPCM;
            break;
        case CODEC_ID_ADPCM_G726:
            *p_cncCodecType = AUD_STREAM_TYPE_G726;
            break;
        default:
            if (classType == AVMEDIA_TYPE_VIDEO)
            {
                *p_cncCodecType = VID_STREAM_TYPE_UNKNOWN;
                printf( "Unkown Video Codec ID: [0x%x]\n", codec);
            }
            else if (classType == AVMEDIA_TYPE_AUDIO)
            {
                *p_cncCodecType = AUD_STREAM_TYPE_UNKNOWN;

                printf("Unkown Audio Codec ID: [0x%x]\n", codec);
            }
            else
            {
                printf("Unkown classType [0x%x]\n", classType);
                *p_cncCodecType = -1;
            }
            break;
    }
#else
    switch (codec)
    {
        case CODEC_ID_MPEG1VIDEO:
        case CODEC_ID_MPEG2VIDEO:
            *p_cncCodecType = VID_STREAM_TYPE_MPEG2_TS;
            break;

        case CODEC_ID_H263:
        case CODEC_ID_FLV1:
            *p_cncCodecType = VID_STREAM_TYPE_H263;
            break;

        case CODEC_ID_H264:
            *p_cncCodecType = VID_STREAM_TYPE_H264_TS;
            break;

        case CODEC_ID_MPEG4:
            *p_cncCodecType = VID_STREAM_TYPE_MPEG4;
            break;

        case CODEC_ID_MJPEG:
            *p_cncCodecType = VID_STREAM_TYPE_MJPG;
            break;

        case CODEC_ID_VC1:
        case CODEC_ID_WMV3:
            *p_cncCodecType = VID_STREAM_TYPE_VC1;
            break;

        case CODEC_ID_MSMPEG4V3:
            *p_cncCodecType = VID_STREAM_TYPE_DIVX;
            break;

        case CODEC_ID_CAVS:
            *p_cncCodecType = VID_STREAM_TYPE_AVS;
            break;
        case CODEC_ID_RV30:
        case CODEC_ID_RV40:
            *p_cncCodecType = VID_STREAM_TYPE_RV;
            break;
        case CODEC_ID_SIPR:
            *p_cncCodecType = AUD_STREAM_TYPE_REAL_SIPR;
            break;
        case CODEC_ID_ATRAC3:
            *p_cncCodecType = AUD_STREAM_TYPE_REAL_ATRC;
            break;
        case CODEC_ID_MP1:
        case CODEC_ID_MP2:
        case CODEC_ID_MP3:
            *p_cncCodecType = AUD_STREAM_TYPE_MPA;
            break;

#ifdef THIRDPARTY_DOLBY
        case CODEC_ID_AC3:
        case CODEC_ID_EAC3:
            *p_cncCodecType = AUD_STREAM_TYPE_DOLBY;
            break;
#else
        case CODEC_ID_AC3:
        case CODEC_ID_EAC3:
            *p_cncCodecType = AUD_STREAM_TYPE_AC3;
            break;
#endif
        case CODEC_ID_TRUEHD:
            *p_cncCodecType = AUD_STREAM_TYPE_TRUEHD;
            break;

        case CODEC_ID_AAC:
            *p_cncCodecType = AUD_STREAM_TYPE_AAC;
            break;

        case CODEC_ID_AAC_LATM:
            *p_cncCodecType = AUD_STREAM_TYPE_AAC_LATM;
            break;

        case CODEC_ID_PCM_MULAW:
            *p_cncCodecType = AUD_STREAM_TYPE_PCM_MULAW;
            break;

        case CODEC_ID_PCM_ALAW:
            *p_cncCodecType = AUD_STREAM_TYPE_PCM_ALAW;
            break;

        case CODEC_ID_WMAV1:
        case CODEC_ID_WMAV2:
            *p_cncCodecType = AUD_STREAM_TYPE_WMA;
            break;

        case CODEC_ID_WMAPRO:
            *p_cncCodecType = AUD_STREAM_TYPE_MS_WMAPRO;
            break;

        case CODEC_ID_DTS:
            *p_cncCodecType = AUD_STREAM_TYPE_DTS;
            break;

        case CODEC_ID_FLAC:
            *p_cncCodecType = AUD_STREAM_TYPE_FLAC;
            break;
        case CODEC_ID_VORBIS:
            *p_cncCodecType = AUD_STREAM_TYPE_OGG_VORBIS;
            break;

        case CODEC_ID_APE:
            *p_cncCodecType = AUD_STREAM_TYPE_APE;
            break;
        case CODEC_ID_COOK:
            *p_cncCodecType = AUD_STREAM_TYPE_REAL_COOK;
            break;
#if(LIBAVFORMAT_VERSION_MAJOR == 53)  // NNNNN
        case CODEC_ID_DTS14LE:
            *p_cncCodecType = AUD_STREAM_TYPE_DTSWAV;
            break;
#endif
        case CODEC_ID_AMR_NB:
            *p_cncCodecType = AUD_STREAM_TYPE_AMR;
            break;

        case  CODEC_ID_PCM_S16LE:
        case  CODEC_ID_PCM_U16LE:
        case  CODEC_ID_PCM_S8:
        case  CODEC_ID_PCM_U8:
        case  CODEC_ID_PCM_S32LE:
        case  CODEC_ID_PCM_U32LE:
        case  CODEC_ID_PCM_S24LE:
        case  CODEC_ID_PCM_U24LE:
        case  CODEC_ID_PCM_S24DAUD:
        case  CODEC_ID_PCM_ZORK:
        case  CODEC_ID_PCM_S16LE_PLANAR:
        case  CODEC_ID_PCM_DVD:
        case  CODEC_ID_PCM_F32LE:
        case  CODEC_ID_PCM_F64LE:
        case  CODEC_ID_PCM_BLURAY:
        case  CODEC_ID_PCM_LXF:
        case  CODEC_ID_S302M:
        case  CODEC_ID_ADPCM_IMA_QT:
        case  CODEC_ID_ADPCM_IMA_WAV:
        case  CODEC_ID_ADPCM_IMA_DK3:
        case  CODEC_ID_ADPCM_IMA_DK4:
        case  CODEC_ID_ADPCM_IMA_WS:
        case  CODEC_ID_ADPCM_IMA_SMJPEG:
        case  CODEC_ID_ADPCM_MS:
        case  CODEC_ID_ADPCM_4XM:
        case  CODEC_ID_ADPCM_XA:
        case  CODEC_ID_ADPCM_ADX:
        case  CODEC_ID_ADPCM_EA:
        case  CODEC_ID_ADPCM_CT:
        case  CODEC_ID_ADPCM_SWF:
        case  CODEC_ID_ADPCM_YAMAHA:
        case  CODEC_ID_ADPCM_SBPRO_4:
        case  CODEC_ID_ADPCM_SBPRO_3:
        case  CODEC_ID_ADPCM_SBPRO_2:
        case  CODEC_ID_ADPCM_THP:
        case  CODEC_ID_ADPCM_IMA_AMV:
        case  CODEC_ID_ADPCM_EA_R1:
        case  CODEC_ID_ADPCM_EA_R3:
        case  CODEC_ID_ADPCM_EA_R2:
        case  CODEC_ID_ADPCM_IMA_EA_SEAD:
        case  CODEC_ID_ADPCM_IMA_EA_EACS:
        case  CODEC_ID_ADPCM_EA_XAS:
        case  CODEC_ID_ADPCM_EA_MAXIS_XA:
        case  CODEC_ID_ADPCM_IMA_ISS:
        case  CODEC_ID_ADPCM_G722:
            //pcm_bit_reverse_flag = 0;
            *p_cncCodecType = AUD_STREAM_TYPE_LPCM;
            break;
        case  CODEC_ID_PCM_S16BE:
        case  CODEC_ID_PCM_S32BE:
        case  CODEC_ID_PCM_U32BE:
        case  CODEC_ID_PCM_U16BE:
        case  CODEC_ID_PCM_F64BE:
        case  CODEC_ID_PCM_F32BE:
        case  CODEC_ID_PCM_U24BE:
        case  CODEC_ID_PCM_S24BE:
            //pcm_bit_reverse_flag = 1;
            *p_cncCodecType = AUD_STREAM_TYPE_LPCM;
            break;
        case CODEC_ID_ADPCM_G726:
            *p_cncCodecType = AUD_STREAM_TYPE_G726;
            break;
        default:
            if (classType == AVMEDIA_TYPE_VIDEO)
            {
                *p_cncCodecType = VID_STREAM_TYPE_UNKNOWN;
            }
            else if (classType == AVMEDIA_TYPE_AUDIO)
            {
                *p_cncCodecType = AUD_STREAM_TYPE_UNKNOWN;
            }
            else
            {
                *p_cncCodecType = -1;
            }
            printf("Unkown Codec ID: [0x%x]\n", codec);
            break;
    }
#endif
    printf("Codec: [0x%x] - Converted CNC Codec: [%d]\n", codec, *p_cncCodecType);

END_MAP_CODEC_TYPE:
    return retValue;
}


static CNC_SINT32 getContentMetadata(AVDictionary *p_avDictMetadata, cncMediaMetaData_t  *p_cncMediaMetaData, enum AVMediaType mediaType)
{
    AVDictionaryEntry   *p_dictEntry = NULL;
    CNC_SINT32          numOfEntries = 0;
    CNC_SINT32          retVal = 0;

    if (p_avDictMetadata == NULL || p_cncMediaMetaData == NULL)
    {
        //CNC_TRACE_ERROR(0, "Invalid Parameters \n");
        retVal = -1;
        goto _GET_CONTENT_METADATA_END;
    }

    /**
     * Get number of entries in p_avDictMetadata dictionary.
     */
    numOfEntries = av_dict_count(p_avDictMetadata);
    //CNC_TRACE_DEBUG(0, "numOf Meta data Entries :: [%d] \n", numOfEntries);

    while((p_dictEntry = av_dict_get(p_avDictMetadata, "", p_dictEntry, AV_DICT_IGNORE_SUFFIX)))
    {
        /* populate the global audio or video metadata strcuture */
        if(strcmp(p_dictEntry->key, "title") == 0)
        {
            av_strlcpy(p_cncMediaMetaData->title, p_dictEntry->value, sizeof(p_cncMediaMetaData->title));
            //CNC_TRACE_INFO(0, "title       :: %s \n", p_cncMediaMetaData->title);
        }
        else if(strcmp(p_dictEntry->key, "author") == 0)
        {
            av_strlcpy(p_cncMediaMetaData->author, p_dictEntry->value, sizeof(p_cncMediaMetaData->author));
            //CNC_TRACE_INFO(0, "author      :: %s \n", p_cncMediaMetaData->author);
        }
        else if(strcmp(p_dictEntry->key, "artist") == 0)
        {
            av_strlcpy(p_cncMediaMetaData->artist, p_dictEntry->value, sizeof(p_cncMediaMetaData->artist));
            //CNC_TRACE_INFO(0, "Artist      :: %s \n", p_cncMediaMetaData->artist);
        }
        else if(strcmp(p_dictEntry->key, "album") == 0)
        {
            av_strlcpy(p_cncMediaMetaData->album, p_dictEntry->value, sizeof(p_cncMediaMetaData->album));
            //CNC_TRACE_INFO(0, "Album       :: %s \n", p_cncMediaMetaData->album);
        }
        else if(strcmp(p_dictEntry->key, "genre") == 0)
        {
            av_strlcpy(p_cncMediaMetaData->genre, p_dictEntry->value, sizeof(p_cncMediaMetaData->genre));
            //CNC_TRACE_INFO(0, "Genre       :: %s \n", p_cncMediaMetaData->genre);
        }
        else if(strcmp(p_dictEntry->key, "date") == 0 || strcmp(p_dictEntry->key, "WM/Year") == 0 )
        {
            p_cncMediaMetaData->year = atoi(p_dictEntry->value);
            //CNC_TRACE_INFO(0, "Year        :: %d \n", p_cncMediaMetaData->year);
        }
        else if(strcmp(p_dictEntry->key, "track") == 0)
        {
            p_cncMediaMetaData->track = atoi(p_dictEntry->value);
            //CNC_TRACE_INFO(0, "track       :: %d \n", p_cncMediaMetaData->track);
        }
        else if(strcmp(p_dictEntry->key, "copyright") == 0)
        {
            av_strlcpy(p_cncMediaMetaData->copyright, p_dictEntry->value, sizeof(p_cncMediaMetaData->copyright));
            //CNC_TRACE_INFO(0, "Copy Right  :: %s \n", p_cncMediaMetaData->copyright);
        }
        else if(strcmp(p_dictEntry->key, "composer") == 0)
        {
            //av_strlcpy(p_cncMediaMetaData->artist, p_dictEntry->value, sizeof(p_cncMediaMetaData->artist));
            //TODO Need to add in cncMediaMetaData_t
            //CNC_TRACE_INFO(0, "composer    :: %s \n", p_dictEntry->value);
        }
    }

_GET_CONTENT_METADATA_END:

    return retVal;
}


CNC_SINT32 CNC_SoftDemuxer_ProbeInit(void)
{
    CNC_SINT32 retVal = 1;

    av_register_all();
    avcodec_register_all();

_CNC_SOFT_DEMUXER_PROBE_INIT_END:

    return retVal;
}


CNC_SINT32 CNC_SoftDemuxer_Probe(CNC_CHAR *p_fileName, cncMediaFormatInfo_t   *p_mediaFormatInfo)
{
    AVFormatContext         *p_avFormatContext = NULL;
    AVOutputFormat          *p_avOutputFormat = NULL;
    AVCodecContext          *p_codec = NULL;
    AVStream                **pp_streams = NULL;
    CNC_SINT32              cncCodecType = 0;
    CNC_SINT32              isFirstStream = 0;
    CNC_SINT32              retVal = 0;
    CNC_SINT32              i = 0;
    struct                  stat st;

    if (p_fileName == NULL || p_mediaFormatInfo == NULL)
    {
        //CNC_TRACE_ERROR(0, "NULL Argument\n");
        retVal = -1;
        goto _CNC_SOFT_DEMUXER_PROBE_MEDIA_FORMAT_DETAILS_END;
    }

    retVal = avformat_open_input(&p_avFormatContext, (const CNC_CHAR *)p_fileName, NULL, NULL);
    if (retVal != 0)
    {
        if (retVal == -2)
        {
            //CNC_TRACE_ERROR(0, "[%s] :: No such file or directory \n", p_fileName);
        }

        //CNC_TRACE_ERROR(0, "avformat_open_input() errCode :: [%d] \n", retVal);
        goto _CNC_SOFT_DEMUXER_PROBE_MEDIA_FORMAT_DETAILS_END;
    }

    retVal = av_find_stream_info(p_avFormatContext);
    if (retVal < 0 )
    {
        //CNC_TRACE_ERROR(0, "av_find_stream_info() errCode :: [%d]\n", retVal);
        retVal = -1;
        goto _CNC_SOFT_DEMUXER_PROBE_MEDIA_FORMAT_DETAILS_END;
    }
    else
    {
        ////CNC_TRACE_INFO(0, "File Info is \n");
        //av_dump_format(p_avFormatContext, 0, "", 0);
    }

    //Clear media format Structure.
    memset(p_mediaFormatInfo, 0, sizeof(cncMediaFormatInfo_t));

    /** This API used to guess the output file format based on the filename or mime_type.
     *  used to filter .so, .out files which are detecting as audio files [mp1 format]
     */
    p_avOutputFormat = av_guess_format(NULL, p_fileName, NULL);
    if (p_avOutputFormat == NULL)
    {
        //CNC_TRACE_WARNING(0, "Not a valid format Content\n");
        retVal = -1;
        goto _CNC_SOFT_DEMUXER_PROBE_MEDIA_FORMAT_DETAILS_END;
    }

    //Update BitRate.
    p_mediaFormatInfo->bitRate = p_avFormatContext->bit_rate;

    //Update Duration.
    p_mediaFormatInfo->durationInMsec = (CNC_SINT32)(((p_avFormatContext->duration) * 1000)/ AV_TIME_BASE);

    //Update Content Size.
    if (stat(p_fileName, &st) == 0)
    {
        p_mediaFormatInfo->size = (CNC_SINT64)st.st_size;
    }
    else
    {
        fprintf(stderr, "Cannot determine size of %s: %s\n",
                p_fileName, strerror(errno));
    }

    //CNC_TRACE_INFO(0, "Content Size :: [%lld] \n", p_mediaFormatInfo->size);

    //CNC_TRACE_DEBUG(0, "Number Of streams :: [%d] \n", p_avFormatContext->nb_streams);

    pp_streams = p_avFormatContext->streams;

    for (i = 0; i < p_avFormatContext->nb_streams; i++)
    {
        //p_codec = p_avFormatContext->streams[i]->codec;
        p_codec = pp_streams[i]->codec;

        if (p_codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            /*Both Video/Image Contents having AVMEDIA_TYPE_VIDEO type */

            //Check if file is Image type
            switch(p_codec->codec_id)
            {
                case CODEC_ID_MJPEG:
                    if(p_codec->has_b_frames == 0)
                    {
                        //Content is Image.
                        if (p_mediaFormatInfo->mediaClass == CNC_MEDIA_CLASS_NONE)
                        {
                            //Update Mediaclass with Image
                            p_mediaFormatInfo->mediaClass = CNC_MEDIA_CLASS_IMAGE;
                        }
                    }
                    else
                    {
                        //Video Content, Update Media Class with Video
                        p_mediaFormatInfo->mediaClass = CNC_MEDIA_CLASS_VIDEO;
                    }
                    break;

                case CODEC_ID_PNG:
                case CODEC_ID_GIF:
                    if (p_mediaFormatInfo->mediaClass == CNC_MEDIA_CLASS_NONE)
                    {
                        p_mediaFormatInfo->mediaClass = CNC_MEDIA_CLASS_IMAGE;
                    }
                    break;
                case CODEC_ID_ANSI:

                    /*This case will exicute if image file given with different extention.
                     * ffmpeg can not probe valid image details for different file extentions like img.txt.
                     */
                    //CNC_TRACE_WARNING(0, "Not Valid Extention for Image file\n");
                    continue;

                default:
                    p_mediaFormatInfo->mediaClass = CNC_MEDIA_CLASS_VIDEO;
                    break;
            }
#if 0
            if(p_avFormatContext->streams[i]->codec->codec_id == CODEC_ID_MJPEG)
            {
                if(p_avFormatContext->streams[i]->codec->has_b_frames == 0)
                {
                    //Content is Image.
                    if (isFirstStream == 0)
                    {
                        p_mediaFormatInfo->mediaClass = CNC_MEDIA_CLASS_IMAGE;
                        //Set Flag.
                        isFirstStream = 1;
                    }

                }
            }
            else if((p_avFormatContext->streams[i]->codec->codec_id == CODEC_ID_PNG) ||
                   (p_avFormatContext->streams[i]->codec->codec_id == CODEC_ID_GIF))
            {
                //CNC_TRACE_INFO(0, "Found image stream in media content\n");
               //Content is Image.
               if (isFirstStream == 0)
               {
                   p_mediaFormatInfo.mediaClass = CNC_MEDIA_CLASS_IMAGE;
                   //Set Flag.
                   isFirstStream = 1;
               }

               //Upate Image width, height.
               p_mediaFormatInfo->formatDetail.imageInfo.width = p_codec->width;
               p_mediaFormatInfo->formatDetail.imageInfo.height = p_codec->height;

               //CNC_TRACE_INFO(0, "Width  :: %d \n", p_mediaFormatInfo->formatDetail.imageInfo.width);
               //CNC_TRACE_INFO(0, "Height :: %d \n", p_mediaFormatInfo->formatDetail.imageInfo.height);
               continue;
            }
#endif

            if (p_mediaFormatInfo->mediaClass == CNC_MEDIA_CLASS_IMAGE || p_mediaFormatInfo->mediaClass == CNC_MEDIA_CLASS_AUDIO)
            {
                //CNC_TRACE_INFO(0, "Found image stream in media content\n");
                //Upate Image width, height.
                p_mediaFormatInfo->formatDetail.imageInfo.width = p_codec->width;
                p_mediaFormatInfo->formatDetail.imageInfo.height = p_codec->height;

                //CNC_TRACE_INFO(0, "Width  :: %d \n", p_mediaFormatInfo->formatDetail.imageInfo.width);
                //CNC_TRACE_INFO(0, "Height :: %d \n", p_mediaFormatInfo->formatDetail.imageInfo.height);
                continue;
            }

            //Content is Video.
            p_mediaFormatInfo->mediaClass = CNC_MEDIA_CLASS_VIDEO;

            //Update Metadata.
            getContentMetadata(p_avFormatContext->metadata, &(p_mediaFormatInfo->metaData), p_codec->codec_type);

            //Extract Media format.
            mapCodecTypeToCNCCodecType(AVMEDIA_TYPE_VIDEO, p_codec->codec_id, &(cncCodecType));

            //CNC_TRACE_DEBUG(0, "cncCodecType :: [%d] \n", cncCodecType);
            p_mediaFormatInfo->format = convertAudioTypeToCNCMediaFormat(cncCodecType);
            p_mediaFormatInfo->formatDetail.videoInfo.format = p_mediaFormatInfo->format;

            //Upate Video width, height.
            p_mediaFormatInfo->formatDetail.videoInfo.width = p_codec->width;
            p_mediaFormatInfo->formatDetail.videoInfo.height = p_codec->height;

            //Update BitRate.
            p_mediaFormatInfo->formatDetail.videoInfo.bitRate = p_mediaFormatInfo->bitRate;

            //Update Frame Rate.
            CNC_FLOAT32 avg_fps = pp_streams[i]->avg_frame_rate.den > 0 ? pp_streams[i]->avg_frame_rate.num / (CNC_FLOAT32)(pp_streams[i]->avg_frame_rate.den) : -1.00;
            CNC_FLOAT32 r_fps = pp_streams[i]->r_frame_rate.den > 0 ? pp_streams[i]->r_frame_rate.num / (CNC_FLOAT32)(pp_streams[i]->r_frame_rate.den) : -1.00;

            if ( avg_fps > 0 && r_fps > 0)
            {
                if(fabs(avg_fps - r_fps ) < 0.02)
                {
                    p_mediaFormatInfo->formatDetail.videoInfo.frameRate = r_fps;
                }
                else
                {
                    p_mediaFormatInfo->formatDetail.videoInfo.frameRate = avg_fps;
                }
            }
            else if (avg_fps > 0 && r_fps <= 0)
            {
                p_mediaFormatInfo->formatDetail.videoInfo.frameRate = avg_fps;
            }
            else if (r_fps > 0 && avg_fps <= 0)
            {
                p_mediaFormatInfo->formatDetail.videoInfo.frameRate = r_fps;
            }
            else
            {
                p_mediaFormatInfo->formatDetail.videoInfo.frameRate = 25.00;
            }

            //CNC_TRACE_INFO(0, "Frame rate :: [%f] \n", p_mediaFormatInfo->formatDetail.videoInfo.frameRate);

        }
        else if (p_codec->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            //Stream is Audio.
            if (p_mediaFormatInfo->mediaClass != CNC_MEDIA_CLASS_VIDEO)
            {
                //Update media class as Audio.
                p_mediaFormatInfo->mediaClass = CNC_MEDIA_CLASS_AUDIO;

                //Update Metadata.
                getContentMetadata(p_avFormatContext->metadata, &(p_mediaFormatInfo->metaData), p_codec->codec_type);

                //Extract audio stream Media format.
                mapCodecTypeToCNCCodecType(AVMEDIA_TYPE_AUDIO, p_codec->codec_id, &(cncCodecType));
                p_mediaFormatInfo->format = convertAudioTypeToCNCMediaFormat(cncCodecType);
                p_mediaFormatInfo->formatDetail.audioInfo.format = p_mediaFormatInfo->format;

            }
            else
            {
                //Extract audio stream media format of a video content.
                mapCodecTypeToCNCCodecType(AVMEDIA_TYPE_AUDIO, p_codec->codec_id, &(cncCodecType));
                p_mediaFormatInfo->formatDetail.audioInfo.format = convertAudioTypeToCNCMediaFormat(cncCodecType);
            }

            //Update Audio sample rate.
            p_mediaFormatInfo->formatDetail.audioInfo.sampleRate = p_codec->sample_rate;

            //Update NumOfChannels.
            p_mediaFormatInfo->formatDetail.audioInfo.numOfChannels = p_codec->channels;

            //Sample format. TODO Map ffmpeg sample_fmt to cncAudioSampleFormat_t.

        }
        else
        {
            //CNC_TRACE_INFO(0, "[%s] is Not an AVI Content \n", p_fileName);
        }
    }

//    printf("Content Size             :: [%lld] \n", p_mediaFormatInfo->size);
//    if (p_mediaFormatInfo->mediaClass == CNC_MEDIA_CLASS_VIDEO || p_mediaFormatInfo->mediaClass == CNC_MEDIA_CLASS_AUDIO)
//    {
//        printf("Bitrate                  :: [%d]\n", p_mediaFormatInfo->bitRate);
//        printf("DurationInMs             :: [%d]\n", p_mediaFormatInfo->durationInMsec);
//        printf("Sample rate              :: [%d]\n", p_mediaFormatInfo->formatDetail.audioInfo.sampleRate);
//        printf("NumOfChannels            :: [%d] \n", p_mediaFormatInfo->formatDetail.audioInfo.numOfChannels);
//        printf("Media format type        :: [%d]\n", p_mediaFormatInfo->format);
//    }


_CNC_SOFT_DEMUXER_PROBE_MEDIA_FORMAT_DETAILS_END:

    if (p_avFormatContext != NULL)
    {
        /**
        * Close an opened input AVFormatContext. Free it and all its contents
        * and set *avFormatContext to NULL.
        */
        avformat_close_input(&p_avFormatContext);
    }

    return retVal;
}


//For testing added main same file.
#if 0
int main (int argc, char **argv)
{
    int                     retVal = 0;
    cncMediaFormatInfo_t    mediaFormatInfo;

    if(argc < 2)
    {
        printf ("Invalid Arguments\nprovide file name\n");
        retVal = -1;
        goto _APP_MAIN_END;
    }

    CNC_SoftDemuxer_ProbeInit();

    retVal = CNC_SoftDemuxer_Probe(argv[1], &(mediaFormatInfo));
    if (retVal != 0)
    {
        //CNC_TRACE_ERROR(0, "CNC_SoftDemuxer_Probe Error Value :: %d\n", retVal);
        goto _APP_MAIN_END;
    }


    printf("\n\n********************************\n");
   printf("Extracted Meta data \n");
   if (mediaFormatInfo.mediaClass == CNC_MEDIA_CLASS_VIDEO )
   {
       printf("[%s] is Video file \n", argv[1]);
   }
   else if (mediaFormatInfo.mediaClass == CNC_MEDIA_CLASS_AUDIO)
   {
       printf("[%s] is Audio file \n", argv[1]);
   }
   else if (mediaFormatInfo.mediaClass == CNC_MEDIA_CLASS_IMAGE)
   {
       printf("[%s] is Image file \n", argv[1]);
   }

   printf("Content Size             :: [%lld] \n", mediaFormatInfo.size);
   if (mediaFormatInfo.mediaClass == CNC_MEDIA_CLASS_VIDEO || mediaFormatInfo.mediaClass == CNC_MEDIA_CLASS_AUDIO)
   {
       printf("Bitrate                  :: [%d]\n", mediaFormatInfo.bitRate);
       printf("DurationInMs             :: [%d]\n", mediaFormatInfo.durationInMsec);
       printf("Sample rate              :: [%d]\n", mediaFormatInfo.formatDetail.audioInfo.sampleRate);
       printf("NumOfChannels            :: [%d] \n", mediaFormatInfo.formatDetail.audioInfo.numOfChannels);
       printf("Media format type        :: [%d]\n", mediaFormatInfo.format);
   }

   if (mediaFormatInfo.mediaClass == CNC_MEDIA_CLASS_IMAGE)
   {
       //CNC_TRACE_INFO(0, "Width     :: %d \n", mediaFormatInfo.formatDetail.imageInfo.width);
       //CNC_TRACE_INFO(0, "Height    :: %d \n", mediaFormatInfo.formatDetail.imageInfo.height);
   }
_APP_MAIN_END:

    return retVal;
}
#endif
