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
 * @file linux_compat.h
 *
 * @brief
 *   Linux compatibility layer.
 *
 *****************************************************************************/
#ifndef LINUX_COMPAT_H_
#define LINUX_COMPAT_H_

#ifdef __cplusplus
	extern "C"
    {
#endif

/***** macro definitions *****************************************************/

#   ifdef __cplusplus
#       include <cstdio>
//#       include <cstdlib>  //conflict with stlport,zyc
#   else
#       include <stdio.h>
#       include <stdlib.h>
#   endif

/***** public type definitions ***********************************************/

/***** public function prototypes ********************************************/

#ifdef __cplusplus
	}
#endif
#endif /* LINUX_COMPAT_H_ */

