/********************************************************************************************************
 * @file    sampleSwitchEpCfg.c
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

#define ZCL_ATTRID_BASIC_GENERIC_DEVICE_CLASS  0x0008
#define ZCL_ATTRID_BASIC_GENERIC_DEVICE_TYPE   0x0009

/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * GLOBAL VARIABLES
 */
/**
 *  @brief Definition for Incoming cluster / Sever Cluster
 */
const u16 sampleSwitch_inClusterList[] =
{
	ZCL_CLUSTER_GEN_BASIC,
	ZCL_CLUSTER_GEN_IDENTIFY,
#ifdef ZCL_GROUP
	ZCL_CLUSTER_GEN_GROUPS,
#endif
#ifdef ZCL_SCENE
	ZCL_CLUSTER_GEN_SCENES,
#endif
#ifdef ZCL_ON_OFF
	ZCL_CLUSTER_GEN_ON_OFF,
#endif
#ifdef ZCL_POLL_CTRL
	ZCL_CLUSTER_GEN_POLL_CONTROL,
#endif
};

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const u16 sampleSwitch_outClusterList[] =
{
#ifdef ZCL_GROUP
	ZCL_CLUSTER_GEN_GROUPS,
#endif
#ifdef ZCL_SCENE
	ZCL_CLUSTER_GEN_SCENES,
#endif
#ifdef ZCL_ON_OFF
	ZCL_CLUSTER_GEN_ON_OFF,
#endif
#ifdef ZCL_LEVEL_CTRL
	ZCL_CLUSTER_GEN_LEVEL_CONTROL,
#endif
#ifdef ZCL_OTA
    ZCL_CLUSTER_OTA,
#endif
#ifdef ZCL_ZLL_COMMISSIONING
	ZCL_CLUSTER_TOUCHLINK_COMMISSIONING,
#endif
};

/**
 *  @brief Definition for Server cluster number and Client cluster number
 */
#define SAMPLESWITCH_IN_CLUSTER_NUM		(sizeof(sampleSwitch_inClusterList)/sizeof(sampleSwitch_inClusterList[0]))
#define SAMPLESWITCH_OUT_CLUSTER_NUM	(sizeof(sampleSwitch_outClusterList)/sizeof(sampleSwitch_outClusterList[0]))

/**
 *  @brief Definition for simple description for HA profile
 */
const af_simple_descriptor_t sampleSwitch_simpleDesc =
{
	HA_PROFILE_ID,                      	/* Application profile identifier */
	HA_DEV_ONOFF_SWITCH,                	/* Application device identifier */
	SAMPLE_SWITCH_ENDPOINT,                 /* Endpoint */
	2,                                  	/* Application device version */
	0,										/* Reserved */
	SAMPLESWITCH_IN_CLUSTER_NUM,           	/* Application input cluster count */
	SAMPLESWITCH_OUT_CLUSTER_NUM,          	/* Application output cluster count */
	(u16 *)sampleSwitch_inClusterList,    	/* Application input cluster list */
	(u16 *)sampleSwitch_outClusterList,   	/* Application output cluster list */
};


/* Basic */
zcl_basicAttr_t g_zcl_basicAttrs =
{
	.zclVersion 	= 0x03,
	.appVersion 	= (APP_RELEASE|APP_BUILD), //0x00,
	.stackVersion 	= (STACK_RELEASE|STACK_BUILD), //0x02,
	.hwVersion		= HARDWARE_REV, //0x00,
	.manuName		= ZCL_BASIC_MFG_NAME,
	.modelId		= ZCL_BASIC_MODEL_ID,
	.dateCode       = ZCL_BASIC_DATE_CODE,
	.powerSource	= POWER_SOURCE_MAINS_1_PHASE,
	.genDevClass    = 0x00,  //Lightning
	.genDevType     = 0xe1,  //Wall Switch
	.swBuildId		= ZCL_BASIC_SW_BUILD_ID,
	.deviceEnable	= TRUE,
};

const zclAttrInfo_t basic_attrTbl[] =
{
	{ ZCL_ATTRID_BASIC_ZCL_VER,      		ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,  						(u8*)&g_zcl_basicAttrs.zclVersion},
	{ ZCL_ATTRID_BASIC_APP_VER,      		ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,  						(u8*)&g_zcl_basicAttrs.appVersion},
	{ ZCL_ATTRID_BASIC_STACK_VER,    		ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,  						(u8*)&g_zcl_basicAttrs.stackVersion},
	{ ZCL_ATTRID_BASIC_HW_VER,       		ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,  						(u8*)&g_zcl_basicAttrs.hwVersion},
	{ ZCL_ATTRID_BASIC_MFR_NAME,     		ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,  						(u8*)g_zcl_basicAttrs.manuName},
	{ ZCL_ATTRID_BASIC_MODEL_ID,     		ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,  						(u8*)g_zcl_basicAttrs.modelId},
	{ ZCL_ATTRID_BASIC_DATE_CODE,     		ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,  						(u8*)g_zcl_basicAttrs.dateCode},
	{ ZCL_ATTRID_BASIC_POWER_SOURCE, 		ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ,  						(u8*)&g_zcl_basicAttrs.powerSource},
	{ ZCL_ATTRID_BASIC_GENERIC_DEVICE_CLASS,ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ,                        (u8*)&g_zcl_basicAttrs.genDevClass},
	{ ZCL_ATTRID_BASIC_GENERIC_DEVICE_TYPE, ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ,                        (u8*)&g_zcl_basicAttrs.genDevType},
	{ ZCL_ATTRID_BASIC_DEV_ENABLED,  		ZCL_DATA_TYPE_BOOLEAN,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_zcl_basicAttrs.deviceEnable},
	{ ZCL_ATTRID_BASIC_SW_BUILD_ID,  		ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,  						(u8*)&g_zcl_basicAttrs.swBuildId},

	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 	ZCL_DATA_TYPE_UINT16,  	ACCESS_CONTROL_READ,  						(u8*)&zcl_attr_global_clusterRevision},
};

#define	ZCL_BASIC_ATTR_NUM		 sizeof(basic_attrTbl) / sizeof(zclAttrInfo_t)


/* Identify */
zcl_identifyAttr_t g_zcl_identifyAttrs =
{
	.identifyTime	= 0x0000,
};

const zclAttrInfo_t identify_attrTbl[] =
{
	{ ZCL_ATTRID_IDENTIFY_TIME,  			ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_zcl_identifyAttrs.identifyTime },

	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 	ZCL_DATA_TYPE_UINT16,  	ACCESS_CONTROL_READ,  						(u8*)&zcl_attr_global_clusterRevision},
};

#define	ZCL_IDENTIFY_ATTR_NUM			sizeof(identify_attrTbl) / sizeof(zclAttrInfo_t)

#ifdef ZCL_GROUP
/* Group */
zcl_groupAttr_t g_zcl_groupAttrs =
{
	.nameSupport	= 0,
};

const zclAttrInfo_t group_attrTbl[] =
{
	{ ZCL_ATTRID_GROUP_NAME_SUPPORT,  		ZCL_DATA_TYPE_BITMAP8,  ACCESS_CONTROL_READ,  (u8*)&g_zcl_groupAttrs.nameSupport },

	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 	ZCL_DATA_TYPE_UINT16,  	ACCESS_CONTROL_READ,  (u8*)&zcl_attr_global_clusterRevision},
};

#define ZCL_GROUP_ATTR_NUM	  sizeof(group_attrTbl) / sizeof(zclAttrInfo_t)
#endif

#ifdef ZCL_SCENE
/* Scene */
zcl_sceneAttr_t g_zcl_sceneAttrs =
{
	.sceneCount		= 0,
	.currentScene	= 0,
	.currentGroup	= 0x0000,
	.sceneValid		= FALSE,
	.nameSupport	= 0,
};

const zclAttrInfo_t scene_attrTbl[] =
{
    { ZCL_ATTRID_SCENE_SCENE_COUNT,       	ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,  (u8*)&g_zcl_sceneAttrs.sceneCount		},
    { ZCL_ATTRID_SCENE_CURRENT_SCENE,     	ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,  (u8*)&g_zcl_sceneAttrs.currentScene   },
    { ZCL_ATTRID_SCENE_CURRENT_GROUP,     	ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,  (u8*)&g_zcl_sceneAttrs.currentGroup   },
    { ZCL_ATTRID_SCENE_SCENE_VALID,       	ZCL_DATA_TYPE_BOOLEAN,  ACCESS_CONTROL_READ,  (u8*)&g_zcl_sceneAttrs.sceneValid     },
    { ZCL_ATTRID_SCENE_NAME_SUPPORT,      	ZCL_DATA_TYPE_BITMAP8,  ACCESS_CONTROL_READ,  (u8*)&g_zcl_sceneAttrs.nameSupport    },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 	ZCL_DATA_TYPE_UINT16,  	ACCESS_CONTROL_READ,  (u8*)&zcl_attr_global_clusterRevision},
};

#define ZCL_SCENE_ATTR_NUM	 sizeof(scene_attrTbl) / sizeof(zclAttrInfo_t)
#endif

#ifdef ZCL_ON_OFF
/* On/Off */
zcl_onOffAttr_t g_zcl_onOffAttrs[2] =
{
	[0].onOff				= 0x00,
	[0].globalSceneControl	= 1,
	[0].onTime				= 0x0000,
	[0].offWaitTime		= 0x0000,
	[0].startUpOnOff 		= ZCL_START_UP_ONOFF_SET_ONOFF_TO_ON,
	[1].onOff				= 0x00,
	[1].globalSceneControl	= 1,
	[1].onTime				= 0x0000,
	[1].offWaitTime		= 0x0000,
	[1].startUpOnOff 		= ZCL_START_UP_ONOFF_SET_ONOFF_TO_ON,
};

const zclAttrInfo_t onOff_attrTbl[] =
{
	{ ZCL_ATTRID_ONOFF,  					ZCL_DATA_TYPE_BOOLEAN,  ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE,  (u8*)&g_zcl_onOffAttrs[0].onOff},
	{ ZCL_ATTRID_GLOBAL_SCENE_CONTROL, 		ZCL_DATA_TYPE_BOOLEAN, 	ACCESS_CONTROL_READ, 							  (u8*)&g_zcl_onOffAttrs[0].globalSceneControl},
	{ ZCL_ATTRID_ON_TIME, 					ZCL_DATA_TYPE_UINT16, 	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, 	  (u8*)&g_zcl_onOffAttrs[0].onTime},
	{ ZCL_ATTRID_OFF_WAIT_TIME, 			ZCL_DATA_TYPE_UINT16, 	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, 	  (u8*)&g_zcl_onOffAttrs[0].offWaitTime},
	{ ZCL_ATTRID_START_UP_ONOFF, 			ZCL_DATA_TYPE_ENUM8, 	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, 	  (u8*)&g_zcl_onOffAttrs[0].startUpOnOff},

	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 	ZCL_DATA_TYPE_UINT16,  	ACCESS_CONTROL_READ,  							  (u8*)&zcl_attr_global_clusterRevision},
};

#define ZCL_ONOFF_ATTR_NUM	 sizeof(onOff_attrTbl) / sizeof(zclAttrInfo_t)
#endif

#ifdef ZCL_POLL_CTRL
/* Poll Control */
zcl_pollCtrlAttr_t g_zcl_pollCtrlAttrs =
{
	.chkInInterval			= 0x3840,
	.longPollInterval		= 0x14,
	.shortPollInterval		= 0x02,
	.fastPollTimeout		= 0x28,
	.chkInIntervalMin		= 0x00,
	.longPollIntervalMin	= 0x00,
	.fastPollTimeoutMax		= 0x00,
};

const zclAttrInfo_t pollCtrl_attrTbl[] =
{
	{ ZCL_ATTRID_CHK_IN_INTERVAL,  		ZCL_DATA_TYPE_UINT32, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_zcl_pollCtrlAttrs.chkInInterval },
	{ ZCL_ATTRID_LONG_POLL_INTERVAL, 	ZCL_DATA_TYPE_UINT32, ACCESS_CONTROL_READ, 						  (u8*)&g_zcl_pollCtrlAttrs.longPollInterval },
	{ ZCL_ATTRID_SHORT_POLL_INTERVAL, 	ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, 						  (u8*)&g_zcl_pollCtrlAttrs.shortPollInterval },
	{ ZCL_ATTRID_FAST_POLL_TIMEOUT, 	ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_zcl_pollCtrlAttrs.fastPollTimeout },
	{ ZCL_ATTRID_CHK_IN_INTERVAL_MIN, 	ZCL_DATA_TYPE_UINT32, ACCESS_CONTROL_READ, 						  (u8*)&g_zcl_pollCtrlAttrs.chkInIntervalMin},
	{ ZCL_ATTRID_LONG_POLL_INTERVAL_MIN,ZCL_DATA_TYPE_UINT32, ACCESS_CONTROL_READ, 						  (u8*)&g_zcl_pollCtrlAttrs.longPollIntervalMin },
	{ ZCL_ATTRID_FAST_POLL_TIMEOUT_MAX, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, 						  (u8*)&g_zcl_pollCtrlAttrs.fastPollTimeoutMax},

	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,  					  (u8*)&zcl_attr_global_clusterRevision},
};

#define	ZCL_POLLCTRL_ATTR_NUM			sizeof(pollCtrl_attrTbl) / sizeof(zclAttrInfo_t)
#endif

/**
 *  @brief Definition for simple switch ZCL specific cluster
 */
const zcl_specClusterInfo_t g_sampleSwitchClusterList[] =
{
	{ZCL_CLUSTER_GEN_BASIC,			MANUFACTURER_CODE_NONE,	ZCL_BASIC_ATTR_NUM, 	basic_attrTbl,  	zcl_basic_register,		sampleSwitch_basicCb},
	{ZCL_CLUSTER_GEN_IDENTIFY,		MANUFACTURER_CODE_NONE,	ZCL_IDENTIFY_ATTR_NUM,	identify_attrTbl,	zcl_identify_register,	sampleSwitch_identifyCb},
#ifdef ZCL_GROUP
	{ZCL_CLUSTER_GEN_GROUPS,		MANUFACTURER_CODE_NONE,	0, 						NULL,  				zcl_group_register,		sampleSwitch_groupCb},
#endif
#ifdef ZCL_SCENE
	{ZCL_CLUSTER_GEN_SCENES,		MANUFACTURER_CODE_NONE,	0,						NULL,				zcl_scene_register,		sampleSwitch_sceneCb},
#endif
#ifdef ZCL_ON_OFF
	{ZCL_CLUSTER_GEN_ON_OFF,			 MANUFACTURER_CODE_NONE, 	ZCL_ONOFF_ATTR_NUM,		onOff_attrTbl,			zcl_onOff_register,			 sampleSwitch_onOffCb},
#endif
#ifdef ZCL_POLL_CTRL
	{ZCL_CLUSTER_GEN_POLL_CONTROL,	MANUFACTURER_CODE_NONE,	ZCL_POLLCTRL_ATTR_NUM,	pollCtrl_attrTbl, 	zcl_pollCtrl_register, 	sampleSwitch_pollCtrlCb},
#endif
};

u8 SAMPLE_SWITCH_CB_CLUSTER_NUM = (sizeof(g_sampleSwitchClusterList)/sizeof(g_sampleSwitchClusterList[0]));

/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      zcl_onOffAttr_save
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_onOffAttr_save(void)
{
	nv_sts_t st = NV_SUCC;

#ifdef ZCL_ON_OFF
#if NV_ENABLE
	zcl_nv_onOff_t zcl_nv_onOff[2];

	st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_ZCL_ON_OFF, sizeof(zcl_nv_onOff_t)*2, (u8*)&zcl_nv_onOff);

	if(st == NV_SUCC){
		if((zcl_nv_onOff[0].onOff != g_zcl_onOffAttrs[0].onOff) || (zcl_nv_onOff[0].startUpOnOff != g_zcl_onOffAttrs[0].startUpOnOff) ||
		   (zcl_nv_onOff[1].onOff != g_zcl_onOffAttrs[1].onOff) || (zcl_nv_onOff[1].startUpOnOff != g_zcl_onOffAttrs[1].startUpOnOff)){
			zcl_nv_onOff[0].onOff = g_zcl_onOffAttrs[0].onOff;
			zcl_nv_onOff[0].startUpOnOff = g_zcl_onOffAttrs[0].startUpOnOff;
			zcl_nv_onOff[1].onOff = g_zcl_onOffAttrs[1].onOff;
			zcl_nv_onOff[1].startUpOnOff = g_zcl_onOffAttrs[1].startUpOnOff;

			st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_ON_OFF, sizeof(zcl_nv_onOff_t)*2, (u8*)&zcl_nv_onOff);
		}
	}else if(st == NV_ITEM_NOT_FOUND){
		zcl_nv_onOff[0].onOff = g_zcl_onOffAttrs[0].onOff;
		zcl_nv_onOff[0].startUpOnOff = g_zcl_onOffAttrs[0].startUpOnOff;
		zcl_nv_onOff[1].onOff = g_zcl_onOffAttrs[1].onOff;
		zcl_nv_onOff[1].startUpOnOff = g_zcl_onOffAttrs[1].startUpOnOff;

		st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_ON_OFF, sizeof(zcl_nv_onOff_t)*2, (u8*)&zcl_nv_onOff);
	}
#else
	st = NV_ENABLE_PROTECT_ERROR;
#endif
#endif

	return st;
}

/*********************************************************************
 * @fn      zcl_onOffAttr_restore
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_onOffAttr_restore(void)
{
	nv_sts_t st = NV_SUCC;

#ifdef ZCL_ON_OFF
#if NV_ENABLE
	zcl_nv_onOff_t zcl_nv_onOff[2];

	st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_ZCL_ON_OFF, sizeof(zcl_nv_onOff_t)*2, (u8*)&zcl_nv_onOff);

	if(st == NV_SUCC){
		g_zcl_onOffAttrs[0].onOff = zcl_nv_onOff[0].onOff;
		g_zcl_onOffAttrs[0].startUpOnOff = zcl_nv_onOff[0].startUpOnOff;
		g_zcl_onOffAttrs[1].onOff = zcl_nv_onOff[1].onOff;
		g_zcl_onOffAttrs[1].startUpOnOff = zcl_nv_onOff[1].startUpOnOff;
	}
#else
	st = NV_ENABLE_PROTECT_ERROR;
#endif
#endif

	return st;
}

/*********************************************************************
 * @fn      zcl_sampleLightAttrsInit
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
void zcl_sampleSwitchAttrsInit(void)
{
	zcl_onOffAttr_restore();
}



#endif	/* __PROJECT_TL_SWITCH__ */
