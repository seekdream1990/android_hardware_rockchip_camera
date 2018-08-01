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
 * @file mipi_drv_common.h
 *
 * @brief   Definition of MIPI driver stuff common to API and internal implementation.
 *
 *****************************************************************************/

#ifndef __MIPI_DRV_COMMON_H__
#define __MIPI_DRV_COMMON_H__

#include <ebase/types.h>
#include <common/return_codes.h>
#include <common/mipi.h>

#include <hal/hal_api.h>

#ifdef MIPI_USE_CAMERIC
    #include <cameric_drv/cameric_drv_api.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************/
/**
 * @brief   Max number of MIPI lanes supported by driver.
 *
 *****************************************************************************/
#define MAX_NUMBER_OF_MIPI_LANES     4


/******************************************************************************/
/**
 * @brief Handle to MIPI driver context.
 *
 ******************************************************************************/
typedef struct MipiDrvContext_s *MipiDrvHandle_t;


/******************************************************************************/
/**
 * @brief MIPI config data structure.
 *
 ******************************************************************************/
typedef struct MipiConfig_s
{
    uint32_t                        NumLanes;       //!< Number of MIPI lanes to use.
    MipiVirtualChannel_t            VirtChannel;    //!< Virtual channel to use.
    MipiDataType_t                  DataType;       //!< Type of data to be exchanged.
    MipiDataCompressionScheme_t     CompScheme;     //!< Compression scheme to use.
    MipiPredictorBlock_t            PredBlock;      //!< Predictor block to use.
    uint32_t                        Freq;
    uint32_t                        DevId;          //attach to which sensor device
} MipiConfig_t;


/******************************************************************************/
/**
 * @brief   MIPI driver configuration structure
 *
 ******************************************************************************/
typedef struct MipiDrvConfig_s
{
    uint32_t            ItfNum;             //!< Zero based number of MIPI interface
    uint32_t            InstanceNum;        //!< Zero based number of instance of MIPI module to use.

    HalHandle_t         HalHandle;          //!< HAL session to use for HW access.

    MipiDrvHandle_t     MipiDrvHandle;      //!< Handle to created MIPI driver context, set by @ref MipiDrvInit if successfull, undefined otherwise.

#ifdef MIPI_USE_CAMERIC
    CamerIcDrvHandle_t  CamerIcDrvHandle;   //!< Handle to existing CamerIc driver instance, only required if CamerIc MIPI module is in use, set to NULL otherwise; @note Must be valid for the whole lifetime of this driver instance!
#endif
} MipiDrvConfig_t;


#ifdef __cplusplus
}
#endif

#endif /* __MIPI_DRV_COMMON_H__ */
