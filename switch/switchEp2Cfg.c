/********************************************************************************************************
 * @file    switchEp2Cfg.c
 *
 * @brief   This is the source file for switchEp2Cfg
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/

#if (__PROJECT_TL_SWITCH__)

/**********************************************************************
 * INCLUDES
 */
#include "../common/comm_cfg.h"

#include "tl_common.h"
#include "zcl_include.h"
#include "tuyaSwitch.h"

#include "zcl_sensorControl.h"
#include "timestamp.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */

/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * GLOBAL VARIABLES
 */

/**
 *  @brief Definition for Incoming cluster / Sever Cluster
 */
// const u16 switch_inClusterList[] =
// {
// };

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const u16 switch_outClusterList[] =
{
#ifdef ZCL_ON_OFF
	ZCL_CLUSTER_GEN_ON_OFF,
#endif
#ifdef ZCL_LEVEL_CTRL
	ZCL_CLUSTER_GEN_LEVEL_CONTROL,
#endif
};

/**
 *  @brief Definition for Server cluster number and Client cluster number
 */
//#define SWITCH_IN_CLUSTER_NUM		(sizeof(switch_inClusterList)/sizeof(switch_inClusterList[0]))
#define SWITCH_OUT_CLUSTER_NUM	(sizeof(switch_outClusterList)/sizeof(switch_outClusterList[0]))

/**
 *  @brief Definition for simple description for HA profile
 */
const af_simple_descriptor_t switch_simpleDesc =
{
	HA_PROFILE_ID,                      	/* Application profile identifier */
	HA_DEV_ONOFF_SWITCH,                	/* Application device identifier */
	SWITCH_ENDPOINT_2,                      /* Endpoint */
	2,                                  	/* Application device version */
	0,										/* Reserved */
	0,           	                        /* Application input cluster count */
	SWITCH_OUT_CLUSTER_NUM,          	    /* Application output cluster count */
	NULL,    	                            /* Application input cluster list */
	(u16 *)switch_outClusterList,   	    /* Application output cluster list */
};


/**********************************************************************
 * FUNCTIONS
 */

#endif	/* __PROJECT_TL_SWITCH__ */
