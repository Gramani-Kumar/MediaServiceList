/*
 * cnc_softdemuxer_helper.h
 *
 *  Created on: 
 *      Author:Ganesh Kumar R 
 */

#ifndef CNC_SOFTDEMUXER_HELPER_H_
#define CNC_SOFTDEMUXER_HELPER_H_

#include "cnc_common_typedef.h"
#include "ffmpeg_test.h"

CNC_SINT32 CNC_SoftDemuxer_ProbeInit(void);

CNC_SINT32 CNC_SoftDemuxer_Probe(CNC_CHAR *p_fileName, cncMediaFormatInfo_t   *p_mediaFormatInfo);

#endif /* CNC_SOFTDEMUXER_HELPER_H_ */
