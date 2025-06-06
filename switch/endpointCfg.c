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
#include "version_cfg.h"
#include "endpointCfg.h"
#include "zclApp.h"
#include "switchApp.h"

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
const u16 endpoint_inClusterList[] =
{
	ZCL_CLUSTER_GEN_BASIC,
	ZCL_CLUSTER_GEN_IDENTIFY,
	ZCL_CLUSTER_GEN_GROUPS,
	ZCL_CLUSTER_GEN_SCENES,
	ZCL_CLUSTER_GEN_ON_OFF,
	ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,
};

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const u16 endpoint_outClusterList[] =
{
	ZCL_CLUSTER_GEN_GROUPS,
	ZCL_CLUSTER_GEN_SCENES,
	ZCL_CLUSTER_GEN_ON_OFF,
	ZCL_CLUSTER_GEN_LEVEL_CONTROL,
    ZCL_CLUSTER_OTA,
};

const u16 extra_Endpoint_inClusterList[] =
{
	ZCL_CLUSTER_GEN_ON_OFF,
	ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,
};

const u16 extra_Endpoint_outClusterList[] =
{	ZCL_CLUSTER_GEN_ON_OFF,
	ZCL_CLUSTER_GEN_LEVEL_CONTROL,
};

/**
 *  @brief Definition for Server cluster number and Client cluster number
 */
#define ENDPOINT_IN_CLUSTER_NUM		(sizeof(endpoint_inClusterList)/sizeof(endpoint_inClusterList[0]))
#define ENDPOINT_OUT_CLUSTER_NUM	(sizeof(endpoint_outClusterList)/sizeof(endpoint_outClusterList[0]))
#define EXTRA_ENDPOINT_IN_CLUSTER_NUM	(sizeof(extra_Endpoint_inClusterList)/sizeof(extra_Endpoint_inClusterList[0]))
#define EXTRA_ENDPOINT_OUT_CLUSTER_NUM	(sizeof(extra_Endpoint_outClusterList)/sizeof(extra_Endpoint_outClusterList[0]))

/**
 *  @brief Definition for simple description for HA profile
 */
const af_simple_descriptor_t endpoint_simpleDesc =
{
	HA_PROFILE_ID,                      	/* Application profile identifier */
	HA_DEV_ONOFF_SWITCH,                	/* Application device identifier */
	ENDPOINT_1,                 /* Endpoint */
	2,                                  	/* Application device version */
	0,										/* Reserved */
	ENDPOINT_IN_CLUSTER_NUM,           	/* Application input cluster count */
	ENDPOINT_OUT_CLUSTER_NUM,          	/* Application output cluster count */
	(u16 *)endpoint_inClusterList,    	/* Application input cluster list */
	(u16 *)endpoint_outClusterList,   	/* Application output cluster list */
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
    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 	ZCL_DATA_TYPE_UINT16,  	ACCESS_CONTROL_READ,  (u8*)&zcl_attr_global_clusterRevision },
};

#define ZCL_SCENE_ATTR_NUM	 sizeof(scene_attrTbl) / sizeof(zclAttrInfo_t)

/* On/Off */
#define ZCL_ATTRID_ON_SWITCH_TRIGGER						  0x4100
#define ZCL_ATTRID_BACKLIGHT_MODE   						  0x4101

#define get_onOff_attrTbl(x) { \
	{ ZCL_ATTRID_ONOFF,  					ZCL_DATA_TYPE_BOOLEAN,  ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE,  (u8*)&g_switchAppCtx.relayAttrs[x].onOff}, \
	{ ZCL_ATTRID_GLOBAL_SCENE_CONTROL, 		ZCL_DATA_TYPE_BOOLEAN, 	ACCESS_CONTROL_READ, 							  (u8*)&g_switchAppCtx.relayAttrs[x].globalSceneControl}, \
	{ ZCL_ATTRID_ON_TIME, 					ZCL_DATA_TYPE_UINT16, 	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, 	  (u8*)&g_switchAppCtx.relayAttrs[x].onTime}, \
	{ ZCL_ATTRID_OFF_WAIT_TIME, 			ZCL_DATA_TYPE_UINT16, 	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, 	  (u8*)&g_switchAppCtx.relayAttrs[x].offWaitTime}, \
	{ ZCL_ATTRID_START_UP_ONOFF, 			ZCL_DATA_TYPE_ENUM8, 	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, 	  (u8*)&g_switchAppCtx.relayAttrs[x].startUpOnOff}, \
	{ ZCL_ATTRID_ON_SWITCH_TRIGGER, 		ZCL_DATA_TYPE_ENUM8, 	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, 	  (u8*)&g_switchAppCtx.relayAttrs[x].onSwitchTrigger}, \
	{ ZCL_ATTRID_BACKLIGHT_MODE, 		    ZCL_DATA_TYPE_ENUM8, 	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, 	  (u8*)&g_switchAppCtx.relayAttrs[x].backlightMode}, \
	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 	ZCL_DATA_TYPE_UINT16,  	ACCESS_CONTROL_READ,  							  (u8*)&zcl_attr_global_clusterRevision}, \
}

const zclAttrInfo_t onOff_attrTbl[] = get_onOff_attrTbl(0);

#define ZCL_ONOFF_ATTR_NUM	 sizeof(onOff_attrTbl) / sizeof(zclAttrInfo_t)


/* On/Off Configuration */
#define ZCL_SWITCH_MODE_TOGGLE                  0x00
#define ZCL_SWITCH_MODE_MOMENTARY               0x01
#define ZCL_SWITCH_MODE_MULTIFUNCTION           0x02

#define ZCL_ATTRID_SWITCH_MODE					0x4000
#define ZCL_ATTRID_SWITCH_MODE					0x4000

#define get_onOffSwitchCfg_attrTbl(x) { \
	{ ZCL_ATTRID_SWITCH_MODE, ZCL_DATA_TYPE_ENUM8, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_switchAppCtx.relayCfgAttrs[x].switchMode}, \
	{ ZCL_ATTRID_SWITCH_ACTION, ZCL_DATA_TYPE_ENUM8, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_switchAppCtx.relayCfgAttrs[x].switchAction}, \
	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 	ZCL_DATA_TYPE_UINT16,  	ACCESS_CONTROL_READ,  							  (u8*)&zcl_attr_global_clusterRevision}, \
}

const zclAttrInfo_t onOffSwitchCfg_attrTbl[] = get_onOffSwitchCfg_attrTbl(0);

#define ZCL_ONOFFSWITCHCFG_ATTR_NUM	 sizeof(onOffSwitchCfg_attrTbl) / sizeof(zclAttrInfo_t)

/**
 *  @brief Definition for simple switch ZCL specific cluster
 */
const zcl_specClusterInfo_t endpointSpecClusterInfo[] =
{
	{ZCL_CLUSTER_GEN_BASIC,			MANUFACTURER_CODE_NONE,	ZCL_BASIC_ATTR_NUM, 	basic_attrTbl,  	zcl_basic_register,		switch_basicCb},
	{ZCL_CLUSTER_GEN_IDENTIFY,		MANUFACTURER_CODE_NONE,	ZCL_IDENTIFY_ATTR_NUM,	identify_attrTbl,	zcl_identify_register,	switch_identifyCb},
	{ZCL_CLUSTER_GEN_GROUPS,		MANUFACTURER_CODE_NONE,	0, 						NULL,  				zcl_group_register,		switch_groupCb},
	{ZCL_CLUSTER_GEN_SCENES,		MANUFACTURER_CODE_NONE,	0,						NULL,				zcl_scene_register,		switch_sceneCb},
	{ZCL_CLUSTER_GEN_ON_OFF,		MANUFACTURER_CODE_TELINK, ZCL_ONOFF_ATTR_NUM,		onOff_attrTbl,		zcl_onOff_register,		switch_onOffCb},
	{ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,		MANUFACTURER_CODE_TELINK, ZCL_ONOFFSWITCHCFG_ATTR_NUM,		onOffSwitchCfg_attrTbl,		zcl_onOffSwitchCfg_register,		switch_onOffSwitchCfgCb},
};

u8 ENDPOINT_CB_CLUSTER_NUM = (sizeof(endpointSpecClusterInfo)/sizeof(endpointSpecClusterInfo[0]));

#if BUTTON_NUM > 1
const af_simple_descriptor_t endpoint2_simpleDesc =
{
    HA_PROFILE_ID,
    HA_DEV_ONOFF_SWITCH,
    ENDPOINT_2,
	2,
    0,
    EXTRA_ENDPOINT_IN_CLUSTER_NUM,
    EXTRA_ENDPOINT_OUT_CLUSTER_NUM,
    (u16 *)extra_Endpoint_inClusterList,
    (u16 *)extra_Endpoint_outClusterList,
};
const zclAttrInfo_t onOff_2_attrTbl[] = get_onOff_attrTbl(1);
const zclAttrInfo_t onOffSwitchCfg_2_attrTbl[] = get_onOffSwitchCfg_attrTbl(1);
#define ZCL_ONOFF_2_ATTR_NUM	 sizeof(onOff_2_attrTbl) / sizeof(zclAttrInfo_t)
const zcl_specClusterInfo_t endpoint2SpecClusterInfo[] =
{
	{ZCL_CLUSTER_GEN_ON_OFF,		MANUFACTURER_CODE_NONE, ZCL_ONOFF_2_ATTR_NUM,		onOff_2_attrTbl,		zcl_onOff_register,		switch_onOffCb},
	{ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,		MANUFACTURER_CODE_NONE, ZCL_ONOFFSWITCHCFG_ATTR_NUM,		onOffSwitchCfg_2_attrTbl,		zcl_onOffSwitchCfg_register,		switch_onOffSwitchCfgCb},
};
u8 ENDPOINT_2_CB_CLUSTER_NUM = (sizeof(endpoint2SpecClusterInfo)/sizeof(endpoint2SpecClusterInfo[0]));
#endif

#if BUTTON_NUM > 2
const af_simple_descriptor_t endpoint3_simpleDesc =
{
    HA_PROFILE_ID,
    HA_DEV_ONOFF_SWITCH,
    ENDPOINT_3,
	2,
    0,
    EXTRA_ENDPOINT_IN_CLUSTER_NUM,
    EXTRA_ENDPOINT_OUT_CLUSTER_NUM,
    (u16 *)extra_Endpoint_inClusterList,
    (u16 *)extra_Endpoint_outClusterList,
};
const zclAttrInfo_t onOff_3_attrTbl[] = get_onOff_attrTbl(2);
#define ZCL_ONOFF_3_ATTR_NUM	 sizeof(onOff_3_attrTbl) / sizeof(zclAttrInfo_t)
const zclAttrInfo_t onOffSwitchCfg_3_attrTbl[] = get_onOffSwitchCfg_attrTbl(2);
const zcl_specClusterInfo_t endpoint3SpecClusterInfo[] =
{
	{ZCL_CLUSTER_GEN_ON_OFF,		MANUFACTURER_CODE_NONE, ZCL_ONOFF_3_ATTR_NUM,		onOff_3_attrTbl,		zcl_onOff_register,		switch_onOffCb},
	{ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,		MANUFACTURER_CODE_NONE, ZCL_ONOFFSWITCHCFG_ATTR_NUM,		onOffSwitchCfg_3_attrTbl,		zcl_onOffSwitchCfg_register,		switch_onOffSwitchCfgCb},
};
u8 ENDPOINT_3_CB_CLUSTER_NUM = (sizeof(endpoint3SpecClusterInfo)/sizeof(endpoint3SpecClusterInfo[0]));
#endif

void registerAllEndpoints(void) {
	af_endpointRegister(ENDPOINT_1, (af_simple_descriptor_t *)&endpoint_simpleDesc, zcl_rx_handler, NULL);
#if BUTTON_NUM > 1
	af_endpointRegister(ENDPOINT_2, (af_simple_descriptor_t *)&endpoint2_simpleDesc, zcl_rx_handler, NULL);
#endif
#if BUTTON_NUM > 2
	af_endpointRegister(ENDPOINT_3, (af_simple_descriptor_t *)&endpoint3_simpleDesc, zcl_rx_handler, NULL);
#endif
}

void registerAllZCL(void) {
	zcl_register(ENDPOINT_1, ENDPOINT_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)endpointSpecClusterInfo);
#if BUTTON_NUM > 1
	zcl_register(ENDPOINT_2, ENDPOINT_2_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)endpoint2SpecClusterInfo);
#endif
#if BUTTON_NUM > 2
		zcl_register(ENDPOINT_3, ENDPOINT_3_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)endpoint3SpecClusterInfo);
#endif
	}

#endif	/* __PROJECT_TL_SWITCH__ */
