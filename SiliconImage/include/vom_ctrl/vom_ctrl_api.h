/******************************************************************************
 *
 * The copyright in this software is owned by Rockchip and/or its licensors.
 * This software is made available subject to the conditions of the license 
 * terms to be determined and negotiated by Rockchip and you.
 * THIS SOFTWARE IS PROVIDED TO YOU ON AN "AS IS" BASIS and ROCKCHP AND/OR 
 * ITS LICENSORS DISCLAIMS ANY AND ALL WARRANTIES AND REPRESENTATIONS WITH 
 * RESPECT TO SUCH SOFTWARE, WHETHER EXPRESS,IMPLIED, STATUTORY OR OTHERWISE, 
 * INCLUDING WITHOUT LIMITATION, ANY IMPLIED WARRANTIES OF TITLE, NON-INFRINGEMENT, 
 * MERCHANTABILITY, SATISFACTROY QUALITY, ACCURACY OR FITNESS FOR A PARTICULAR PURPOSE. 
 * Except as expressively authorized by Rockchip and/or its licensors, you may not 
 * (a) disclose, distribute, sell, sub-license, or transfer this software to any third party, 
 * in whole or part; (b) modify this software, in whole or part; (c) decompile, reverse-engineer, 
 * dissemble, or attempt to derive any source code from the software.
 *
 *****************************************************************************/
/**
 * @file vom_ctrl_api.h
 *
 * @brief
 *   Definition of vom ctrl API.
 *
 *****************************************************************************/
/**
 * @page vom_ctrl_page VOM Ctrl
 * The Video Output Module displays image buffers handed in via QuadMVDU_FX on
 * a connected HDMI display device.
 *
 * For a detailed list of functions and implementation detail refer to:
 * - @ref vom_ctrl_api
 * - @ref vom_ctrl_common
 * - @ref vom_ctrl
 * - @ref vom_ctrl_mvdu
 *
 * @defgroup vom_ctrl_api VOM Ctrl API
 * @{
 *
 */

#ifndef __VOM_CTRL_API_H__
#define __VOM_CTRL_API_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <ebase/types.h>

#include <common/return_codes.h>
#include <common/cea_861.h>
#include <common/hdmi_3d.h>

#include <hal/hal_api.h>

#include "vom_ctrl_common.h"

typedef struct vomCtrlConfig_s
{
    Cea861VideoFormat_t     VideoFormat;        //!< Reference to CEA style video format to be used for display
    bool_t                  Enable3D;           //!< Enable 3D display mode.
    Hdmi3DVideoFormat_t     VideoFormat3D;      //!< The HDMI 3D display mode to use; undefined if 3D not enabled.
    uint32_t                MaxPendingCommands; //!< Number of commands that can be queued and thus be pending at a time.
    uint32_t                MaxBuffers;
    vomCtrlCompletionCb_t   vomCbCompletion;    //!< Callback function for command completion.
    void                    *pUserContext;      //!< User context passed on to completion callback.
    HalHandle_t             HalHandle;          //!< HAL session to use for HW access

    vomCtrlHandle_t         VomCtrlHandle;      //!< Handle to created vom control context, set by @ref vomCtrlInit if successfull, undefined otherwise.
} vomCtrlConfig_t;

extern RESULT vomCtrlInit
(
    vomCtrlConfig_t  *pConfig           //!< Reference to configuration structure.
);

extern RESULT vomCtrlShutDown
(
    vomCtrlHandle_t  VomCtrlHandle      //!< Handle to vom control context as returned by @ref vomCtrlInit.
);

extern RESULT vomCtrlStart
(
    vomCtrlHandle_t  VomCtrlHandle      //!< Handle to vom control context as returned by @ref vomCtrlInit.
);

extern RESULT vomCtrlStop
(
    vomCtrlHandle_t  VomCtrlHandle      //!< Handle to vom control context as returned by @ref vomCtrlInit.
);

extern RESULT  vomCtrlShowBuffer
(
	vomCtrlHandle_t  VomCtrlHandle,
    MediaBuffer_t    *pBuffer
);

/* @} vom_ctrl_api */

#ifdef __cplusplus
}
#endif

#endif /* __VOM_CTRL_API_H__ */
