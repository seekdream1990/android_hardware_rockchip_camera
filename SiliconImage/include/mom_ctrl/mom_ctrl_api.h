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
 * @file mom_ctrl_api.h
 *
 * @brief
 *   ADD_DESCRIPTION_HERE
 *
 *****************************************************************************/
/**
 * @page module_name_page Module Name
 * Describe here what this module does.
 *
 * For a detailed list of functions and implementation detail refer to:
 * - @ref module_name
 *
 * @defgroup module_name Module Name
 * @{
 *
 */
#ifndef __MOM_CTRL_API_H__
#define __MOM_CTRL_API_H__

#include <ebase/types.h>
#include <common/return_codes.h>

#include <hal/hal_api.h>

#include <bufferpool/media_buffer.h>

#include <cameric_drv/cameric_drv_api.h>

#include "mom_ctrl_common.h"



/**
 * @brief   Configuration structure of the mom-ctrl
 *
 * @note
 *
 */
typedef struct MomCtrlConfig_s
{
    uint32_t                MaxPendingCommands;     /**< Number of commands that can be queued and thus be pending at a time. */

    uint32_t                NumBuffersMainPath;     /**< Number of buffers in main path bufferpool */
    uint32_t                NumBuffersSelfPath;     /**< Number of buffers in self path bufferpool */

    MediaBufPool_t          *pPicBufPoolMainPath;   /**< Reference to output picture buffer pool */
    MediaBufPool_t          *pPicBufPoolSelfPath;   /**< Reference to output picture buffer pool */

    MomCtrlCompletionCb_t   momCbCompletion;        /**< Callback function for command completion. */
    void                    *pUserContext;          /**< User context passed on to completion callback. */

    CamerIcDrvHandle_t      hCamerIc;               /**< CamerIc Driver handle */
    HalHandle_t             HalHandle;

    MomCtrlContextHandle_t  hMomContext;            /**< Handle to mom control context, set by @ref MomCtrlInit if successfull, undefined otherwise. */
} MomCtrlConfig_t;



/*****************************************************************************/
/**
 * @brief   Initialize the MOM-Control
 *
 * This functions initializes the Memory-Output-Modul .
 *
 * Some detailed description goes here ...
 *
 * @param   param1      Describe the parameter 1.
 * @param   param2      Describe the parameter 2
 *
 * @return              Return the result of the function call.
 * @retval              RET_VAL1
 * @retval              RET_VAL2
 *
 *****************************************************************************/
RESULT MomCtrlInit
(
    MomCtrlConfig_t         *pConfig
);



/*****************************************************************************/
/**
 * @brief   Shutdown the MOM-Control
 *
 * This functions releases and shutdowns the Memory-Output-Modul .
 *
 * Some detailed description goes here ...
 *
 * @param   param1      Describe the parameter 1.
 * @param   param2      Describe the parameter 2
 *
 * @return              Return the result of the function call.
 * @retval              RET_VAL1
 * @retval              RET_VAL2
 *
 *****************************************************************************/
RESULT MomCtrlShutDown
(
    MomCtrlContextHandle_t hMomContext
);



/*****************************************************************************/
/**
 * @brief   Start the MOM-Control
 *
 * This functions starts the Memory-Output-Modul .
 *
 * Some detailed description goes here ...
 *
 * @param   param1      Describe the parameter 1.
 * @param   param2      Describe the parameter 2
 *
 * @return              Return the result of the function call.
 * @retval              RET_VAL1
 * @retval              RET_VAL2
 *
 *****************************************************************************/
RESULT MomCtrlStart
(
    MomCtrlContextHandle_t hMomContext
);



/*****************************************************************************/
/**
 * @brief   Initialize the MOM-Control
 *
 * This functions initializes the Memory-Output-Modul .
 *
 * Some detailed description goes here ...
 *
 * @param   param1      Describe the parameter 1.
 * @param   param2      Describe the parameter 2
 *
 * @return              Return the result of the function call.
 * @retval              RET_VAL1
 * @retval              RET_VAL2
 *
 *****************************************************************************/
RESULT MomCtrlStop
(
    MomCtrlContextHandle_t hMomContext
);



/*****************************************************************************/
/**
 * @brief   attach a media buffer queue to an output path
 *
 * This functions initializes the Memory-Output-Modul .
 *
 * Some detailed description goes here ...
 *
 * @param   param1      Describe the parameter 1.
 * @param   param2      Describe the parameter 2
 *
 * @return              Return the result of the function call.
 * @retval              RET_VAL1
 * @retval              RET_VAL2
 *
 *****************************************************************************/
RESULT MomCtrlAttachQueueToPath
(
    MomCtrlContextHandle_t  hMomContext,
    const uint32_t          path,
    osQueue                 *pQueue
);



/*****************************************************************************/
/**
 * @brief   detach a media buffer queue from an output path
 *
 * This functions initializes the Memory-Output-Modul .
 *
 * Some detailed description goes here ...
 *
 * @param   param1      Describe the parameter 1.
 * @param   param2      Describe the parameter 2
 *
 * @return              Return the result of the function call.
 * @retval              RET_VAL1
 * @retval              RET_VAL2
 *
 *****************************************************************************/
RESULT MomCtrlDetachQueueToPath
(
    MomCtrlContextHandle_t  hMomContext,
    const uint32_t          path,
    osQueue                 *pQueue
);



/*****************************************************************************/
/**
 * @brief   TODO
 *
 *****************************************************************************/
RESULT  MomCtrlStartDraw
(
    MomCtrlContextHandle_t  hMomContext,
    MomCtrlDrawConfig_t     *pDrawConfig
);



/*****************************************************************************/
/**
 * @brief   TODO
 *
 *****************************************************************************/
RESULT  MomCtrlStopDraw
(
    MomCtrlContextHandle_t  hMomContext,
    MomCtrlDrawHandle_t     hDrawContext
);



/*****************************************************************************/
/**
 * @brief   TODO
 *
 *****************************************************************************/
RESULT  MomCtrlDraw
(
    MomCtrlContextHandle_t  hMomContext,
    MomCtrlDrawHandle_t     hDrawContext,
    uint32_t                numCmds,
    MomCtrlDrawCmd_t        *pDrawCmds
);



/*****************************************************************************/
/**
 * @brief   TODO
 *
 *****************************************************************************/
RESULT  MomCtrlRegisterBufferCb
(
    MomCtrlContextHandle_t  hMomContext,
    const uint32_t          path,
    MomCtrlBufferCb_t       fpCallback,
    void                    *pBufferCbCtx
);



/*****************************************************************************/
/**
 * @brief   TODO
 *
 *****************************************************************************/
RESULT  MomCtrlDeRegisterBufferCb
(
    MomCtrlContextHandle_t  hMomContext,
    const uint32_t          path
);

/* @} module_name*/

#endif /* __MOM_CTRL_API_H__ */

