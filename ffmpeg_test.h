/*
 * ffmpeg_test.h
 *
 *  Created on: 
 *      Author: Ganesh Kumar R
 */

#ifndef FFMPEG_TEST_H_
#define FFMPEG_TEST_H_

#include "cnc_common_typedef.h"

#if 1
/**
*Define the stream type.
*
*@see CSVID_SetStreamType().
*/
typedef enum {
    VID_STREAM_TYPE_UNKNOWN = 0,///
    VID_STREAM_TYPE_MPEG2_TS = 1,///<<Stream type is MPEG2.
    VID_STREAM_TYPE_H264_TS = 2,///<Stream type is H264.
    VID_STREAM_TYPE_MPEG4 = 3,///<Stream type is MPEG4.
    VID_STREAM_TYPE_H263 = 4,///<Stream type is H263
    VID_STREAM_TYPE_VC1 = 5,///<Stream type is VC1.
    VID_STREAM_TYPE_DIVX = 6,///<Stream type is DIVX.
    VID_STREAM_TYPE_RV = 7,///<Stream type is RV.
    VID_STREAM_TYPE_AVS = 8,///<Stream type is AVS
    VID_STREAM_TYPE_MJPG = 9,///<Stream type is MJPG
    VID_STREAM_TYPE_VIB = 10,///<Stream type is VIB
    VID_STREAM_TYPE_YUV = 11,///<Stream type is YUV
    VID_STREAM_TYPE_MAX_NUM,///<MAX number
} CSVID_STREAM_TYPE;


/**
 * \enum    cncMediaClass_t
 * \brief   Definition of different Media Class
 */
typedef enum _cncMediaClass_t
{
    CNC_MEDIA_CLASS_NONE = 0,    /** None */
    CNC_MEDIA_CLASS_IMAGE,       /** Image */
    CNC_MEDIA_CLASS_AUDIO,       /** Audio (only) */
    CNC_MEDIA_CLASS_VIDEO,       /** Video only or both Audio/Video */
} cncMediaClass_t;

/**
 * \enum    cncMediaFormat_t
 * \brief   Definition of various media formats
 *          This will be used to identify the format of stream (say file) in general and also the individual elementary streams
 */
typedef enum _cncMediaFormat_t
{
    CNC_MEDIA_FORMAT_NONE = 0,

    /** Audio */
    CNC_MEDIA_FORMAT_AUDIO_ATRAC3,                 /*1*/
    CNC_MEDIA_FORMAT_AUDIO_ATRAC3_PLUS,            /*2*/
    CNC_MEDIA_FORMAT_AUDIO_G711,                   /*3*/
    CNC_MEDIA_FORMAT_AUDIO_G722,                   /*4*/
    CNC_MEDIA_FORMAT_AUDIO_G723,                   /*5*/
    CNC_MEDIA_FORMAT_AUDIO_G726,
    CNC_MEDIA_FORMAT_AUDIO_G729,
    CNC_MEDIA_FORMAT_AUDIO_AMR,
    CNC_MEDIA_FORMAT_AUDIO_AC3,
    CNC_MEDIA_FORMAT_AUDIO_FLAC,                  /*10*/
    CNC_MEDIA_FORMAT_AUDIO_DD_PLUS,
    CNC_MEDIA_FORMAT_AUDIO_MLP,
    CNC_MEDIA_FORMAT_AUDIO_DOLBY_TRUE_HD,
    CNC_MEDIA_FORMAT_AUDIO_DTS,
    CNC_MEDIA_FORMAT_AUDIO_DTS_HD,
    CNC_MEDIA_FORMAT_AUDIO_MP1,                  /*16*/
    CNC_MEDIA_FORMAT_AUDIO_MP2,                  /*17*/
    CNC_MEDIA_FORMAT_AUDIO_MP3,                  /*18*/
    CNC_MEDIA_FORMAT_AUDIO_MPEG2_AAC,            /*19*/
    CNC_MEDIA_FORMAT_AUDIO_MPEG4_AAC,            /*20*/
    CNC_MEDIA_FORMAT_AUDIO_MPEG4_HE_AAC,
    CNC_MEDIA_FORMAT_AUDIO_WMA,/*22*/
    CNC_MEDIA_FORMAT_AUDIO_WMAPRO,
    CNC_MEDIA_FORMAT_AUDIO_WMA_LOSSLESS,
    CNC_MEDIA_FORMAT_AUDIO_OGG_VORBIS,
    CNC_MEDIA_FORMAT_AUDIO_RA,
    CNC_MEDIA_FORMAT_AUDIO_REAL_COOK,
    CNC_MEDIA_FORMAT_AUDIO_APE,
    CNC_MEDIA_FORMAT_AUDIO_LPCM,/*29*/
    CNC_MEDIA_FORMAT_AUDIO_AIFF,
    CNC_MEDIA_FORMAT_AUDIO_AU,
    CNC_MEDIA_FORMAT_AUDIO_WAV,
    CNC_MEDIA_FORMAT_AUDIO_ALAW,
    CNC_MEDIA_FORMAT_AUDIO_MULAW,
    CNC_MEDIA_FORMAT_AUDIO_TRUEHD,

    /** Video */
    CNC_MEDIA_FORMAT_VIDEO_MPEG1_PART2,/*36*/
    CNC_MEDIA_FORMAT_VIDEO_MPEG2_PART2,
    CNC_MEDIA_FORMAT_VIDEO_MPEG4_PART2,/*38*/
    CNC_MEDIA_FORMAT_VIDEO_MPEG4_PART10,/*39*/
    CNC_MEDIA_FORMAT_VIDEO_H261,
    CNC_MEDIA_FORMAT_VIDEO_H263,
    CNC_MEDIA_FORMAT_VIDEO_H264,/*42*/
    CNC_MEDIA_FORMAT_VIDEO_WMV,
    CNC_MEDIA_FORMAT_VIDEO_VC1,
    CNC_MEDIA_FORMAT_VIDEO_AVS,
    CNC_MEDIA_FORMAT_VIDEO_RV,
    CNC_MEDIA_FORMAT_VIDEO_MJPEG,/*47*/
#ifdef ARCH_CSM1900
    CNC_MEDIA_FORMAT_VIDEO_VP8,
#ifdef SUPPORT_SOFT_DECODE
    CNC_MEDIA_FORMAT_VIDEO_VP9,
#endif
#endif

    /** Container */
    CNC_MEDIA_FORMAT_CONTAINER_3GP,
    CNC_MEDIA_FORMAT_CONTAINER_3G2,
    CNC_MEDIA_FORMAT_CONTAINER_ASF,
    CNC_MEDIA_FORMAT_CONTAINER_AVI,
    CNC_MEDIA_FORMAT_CONTAINER_FLV,
    CNC_MEDIA_FORMAT_CONTAINER_M2TS,
    CNC_MEDIA_FORMAT_CONTAINER_MPEG_PS,
    CNC_MEDIA_FORMAT_CONTAINER_MPEG_TS,
    CNC_MEDIA_FORMAT_CONTAINER_MP4,
    CNC_MEDIA_FORMAT_CONTAINER_DMF,
    CNC_MEDIA_FORMAT_CONTAINER_MKV,
    CNC_MEDIA_FORMAT_CONTAINER_MXF,
    CNC_MEDIA_FORMAT_CONTAINER_MOV,
    CNC_MEDIA_FORMAT_CONTAINER_OGG,
    CNC_MEDIA_FORMAT_CONTAINER_RM,
    CNC_MEDIA_FORMAT_CONTAINER_VOB,
#ifdef ARCH_CSM1900
    CNC_MEDIA_FORMAT_CONTAINER_WEBM,
#endif

    /** Image */
    CNC_MEDIA_FORMAT_IMAGE_JPG,
    CNC_MEDIA_FORMAT_IMAGE_PNG,
    CNC_MEDIA_FORMAT_IMAGE_BMP,
    CNC_MEDIA_FORMAT_IMAGE_GIF
} cncMediaFormat_t;

/* TODO: Move Audio MetaData structure to proper place */
/**
 * Media Meta Data Info
 */
typedef struct _cncMediaMetaData_t
{
    CNC_CHAR                        albumArtUrl[512];
    CNC_CHAR                        title[128];
    CNC_CHAR                        author[128];
    CNC_CHAR                        copyright[128];
    CNC_CHAR                        album[128];
    CNC_CHAR                        genre[32];        /**< ID3 genre */
    CNC_CHAR                        artist[128];
    CNC_SINT32                      year;              /**< ID3 year, 0 if none */
    CNC_SINT32                      track;             /**< track number, 0 if none */
} cncMediaMetaData_t;

/**
 * \enum    cncAudioSampleFormat_t
 * \brief   Definition of different Audio PCM Sample Formats
 */
typedef enum _cncAudioSampleFormat_t
{
    CNC_AUDIO_SAMPLE_FMT_NONE = -1,
    CNC_AUDIO_SAMPLE_FMT_U8,
    CNC_AUDIO_SAMPLE_FMT_S8,
    CNC_AUDIO_SAMPLE_FMT_U16,
    CNC_AUDIO_SAMPLE_FMT_S16,
    CNC_AUDIO_SAMPLE_FMT_U24,
    CNC_AUDIO_SAMPLE_FMT_S24,
    CNC_AUDIO_SAMPLE_FMT_U32,
    CNC_AUDIO_SAMPLE_FMT_S32,
    CNC_AUDIO_SAMPLE_FMT_FLT,
    CNC_AUDIO_SAMPLE_FMT_DBL,
    CNC_AUDIO_SAMPLE_FMT_NB           ///< Number of sample formats. DO NOT USE if linking dynamically
} cncAudioSampleFormat_t;


typedef struct _cncAudioFormatInfo_t
{
    cncMediaFormat_t                format;
    CNC_SINT32                      bitRate;
    CNC_SINT32                      sampleRate;
    CNC_SINT32                      numOfChannels;
    CNC_SINT32                      bigEndian;
    cncAudioSampleFormat_t          audSampleFormat;
    struct
    {
        cncMediaFormat_t            albumArtImageType;
        CNC_CHAR                   *albumArtBuf;
        CNC_SINT32                  albumArtSize;
    } albumArtInfo;
} cncAudioFormatInfo_t;

typedef struct _cncVideoFormatInfo_t
{
    cncMediaFormat_t                format;
    CNC_SINT32                      bitRate;
    CNC_FLOAT32                     frameRate;
    CNC_SINT32                      width;
    CNC_SINT32                      height;
} cncVideoFormatInfo_t;

typedef struct _cncImageFormatInfo_t
{
    cncMediaFormat_t                format;
    CNC_SINT32                      width;
    CNC_SINT32                      height;
} cncImageFormatInfo_t;


typedef struct _cncMediaFormatDetail_t
{
    cncAudioFormatInfo_t            audioInfo;
    cncVideoFormatInfo_t            videoInfo;
    cncImageFormatInfo_t            imageInfo;
} cncMediaFormatDetail_t;


typedef struct _cncMediaFormatInfo_t
{
    cncMediaClass_t                 mediaClass;
    cncMediaFormat_t                format;
    cncMediaFormatDetail_t          formatDetail;
    cncMediaMetaData_t              metaData;
    CNC_SINT32                      durationInMsec;
    CNC_SINT32                      bitRate;
    CNC_SINT64                      size;
} cncMediaFormatInfo_t;

#endif



#endif /* FFMPEG_TEST_H_ */
