/********************************************************************************************************
 * @file    sampleSwitchEp2Cfg.c
 *
 * @brief   This is the source file for sampleSwitchEpCfg
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
#include "tl_common.h"
#include "zcl_include.h"
#include "sampleSwitch.h"
#include "version_cfg.h"

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
const u16 sampleSwitch_inClusterList2[] =
{
#ifdef ZCL_ON_OFF
	ZCL_CLUSTER_GEN_ON_OFF,
#endif
};

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const u16 sampleSwitch_outClusterList2[] =
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
#define SAMPLESWITCH_IN_CLUSTER_NUM_2		(sizeof(sampleSwitch_inClusterList2)/sizeof(sampleSwitch_inClusterList2))
#define SAMPLESWITCH_OUT_CLUSTER_NUM_2	(sizeof(sampleSwitch_outClusterList2)/sizeof(sampleSwitch_outClusterList2))

/**
 *  @brief Definition for simple description for HA profile
 */
const af_simple_descriptor_t sampleSwitch_simpleDesc2 =
{
	HA_PROFILE_ID,                      	/* Application profile identifier */
	HA_DEV_ONOFF_SWITCH,                	/* Application device identifier */
	SAMPLE_SWITCH_ENDPOINT_2,               /* Endpoint */
	2,                                  	/* Application device version */
	0,										/* Reserved */
	SAMPLESWITCH_IN_CLUSTER_NUM_2,           	/* Application input cluster count */
	SAMPLESWITCH_OUT_CLUSTER_NUM_2,          	/* Application output cluster count */
	(u16 *)sampleSwitch_inClusterList2,    	/* Application input cluster list */
	(u16 *)sampleSwitch_outClusterList2,   	/* Application output cluster list */
};


#ifdef ZCL_ON_OFF
/* On/Off */
// zcl_onOffAttr_t g_zcl_onOffAttrs[1] = 
// {
// 	->onOff				= 0x00,
// 	->globalSceneControl	= 1,
// 	->onTime				= 0x0000,
// 	->offWaitTime		= 0x0000,
// 	->startUpOnOff 		= ZCL_START_UP_ONOFF_SET_ONOFF_TO_ON,
// };

const zclAttrInfo_t onOff_attrTbl2[6] =
{
	{ ZCL_ATTRID_ONOFF,  					ZCL_DATA_TYPE_BOOLEAN,  ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE,  (u8*)&g_zcl_onOffAttrs[1].onOff},
	{ ZCL_ATTRID_GLOBAL_SCENE_CONTROL, 		ZCL_DATA_TYPE_BOOLEAN, 	ACCESS_CONTROL_READ, 							  (u8*)&g_zcl_onOffAttrs[1].globalSceneControl},
	{ ZCL_ATTRID_ON_TIME, 					ZCL_DATA_TYPE_UINT16, 	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, 	  (u8*)&g_zcl_onOffAttrs[1].onTime},
	{ ZCL_ATTRID_OFF_WAIT_TIME, 			ZCL_DATA_TYPE_UINT16, 	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, 	  (u8*)&g_zcl_onOffAttrs[1].offWaitTime},
	{ ZCL_ATTRID_START_UP_ONOFF, 			ZCL_DATA_TYPE_ENUM8, 	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, 	  (u8*)&g_zcl_onOffAttrs[1].startUpOnOff},

	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 	ZCL_DATA_TYPE_UINT16,  	ACCESS_CONTROL_READ,  							  (u8*)&zcl_attr_global_clusterRevision},
};

#define ZCL_ONOFF_ATTR_NUM_2	 sizeof(onOff_attrTbl2) / sizeof(zclAttrInfo_t)
#endif

/**
 *  @brief Definition for simple switch ZCL specific cluster
 */
const zcl_specClusterInfo_t g_sampleSwitchClusterList2[] =
{
#ifdef ZCL_ON_OFF
	{ZCL_CLUSTER_GEN_ON_OFF,			 MANUFACTURER_CODE_NONE, 	ZCL_ONOFF_ATTR_NUM_2,		onOff_attrTbl2,			zcl_onOff_register,			 sampleSwitch_onOffCb},
#endif
};

u8 SAMPLE_SWITCH_CB_CLUSTER_NUM_2 = (sizeof(g_sampleSwitchClusterList2)/sizeof(g_sampleSwitchClusterList2[0]));


#endif	/* __PROJECT_TL_SWITCH__ */
