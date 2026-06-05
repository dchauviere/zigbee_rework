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

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zcl_include.h"
#include "version_cfg.h"
#include "endpointCfg.h"
#include "zclApp.h"
#include "switchApp.h"
#include "switchCtrl.h"
#include "relayCtrl.h"


/**********************************************************************
 * GLOBAL VARIABLES
 */

af_simple_descriptor_t g_simpleDescs[BUTTON_NUM];
zclAttrInfo_t g_identify_tbls[BUTTON_NUM][2];
zclAttrInfo_t g_onOff_tbls[BUTTON_NUM][6];
zclAttrInfo_t g_onOffSwitchCfg_tbls[BUTTON_NUM][3];
zclAttrInfo_t g_epConfig_tbls[BUTTON_NUM][9];

/**
 *  @brief Definition for Incoming cluster / Sever Cluster
 */
const u16 endpoint_inClusterList[] =
{
	ZCL_CLUSTER_GEN_BASIC,
	ZCL_CLUSTER_GEN_GROUPS,
	ZCL_CLUSTER_GEN_SCENES,
	ZCL_CLUSTER_GEN_IDENTIFY,
	ZCL_CLUSTER_GEN_ON_OFF,
	ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,
	ZCL_CLUSTER_EPCONFIG,
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
	ZCL_CLUSTER_GEN_IDENTIFY,
	ZCL_CLUSTER_GEN_ON_OFF,
	ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,
	ZCL_CLUSTER_EPCONFIG,
};

const u16 extra_Endpoint_outClusterList[] =
{
	ZCL_CLUSTER_GEN_ON_OFF,
	ZCL_CLUSTER_GEN_LEVEL_CONTROL,
};


/**
 *  @brief Definition for Server cluster number and Client cluster number
 */
#define ENDPOINT_IN_CLUSTER_NUM		(sizeof(endpoint_inClusterList)/sizeof(endpoint_inClusterList[0]))
#define ENDPOINT_OUT_CLUSTER_NUM	(sizeof(endpoint_outClusterList)/sizeof(endpoint_outClusterList[0]))
#define EXTRA_ENDPOINT_IN_CLUSTER_NUM	(sizeof(extra_Endpoint_inClusterList)/sizeof(extra_Endpoint_inClusterList[0]))
#define EXTRA_ENDPOINT_OUT_CLUSTER_NUM	(sizeof(extra_Endpoint_outClusterList)/sizeof(extra_Endpoint_outClusterList[0]))



/* On/Off Configuration */
#define ZCL_SWITCH_MODE_TOGGLE                  0x00
#define ZCL_SWITCH_MODE_MOMENTARY               0x01
#define ZCL_SWITCH_MODE_MULTIFUNCTION           0x02

#define ZCL_ATTRID_SWITCH_MODE					0x4000


void registerCommonZCL(u8 endpoint) {
	static const zclAttrInfo_t basic_attrTbl[] =
	{
		{ ZCL_ATTRID_BASIC_ZCL_VER,      		     ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,  											 (u8*)&g_zcl_basicAttrs.zclVersion},
		{ ZCL_ATTRID_BASIC_APP_VER,      		     ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,  											 (u8*)&g_zcl_basicAttrs.appVersion},
		{ ZCL_ATTRID_BASIC_STACK_VER,    		     ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,  											 (u8*)&g_zcl_basicAttrs.stackVersion},
		{ ZCL_ATTRID_BASIC_HW_VER,       		     ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,  											 (u8*)&g_zcl_basicAttrs.hwVersion},
		{ ZCL_ATTRID_BASIC_MFR_NAME,     		     ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,  											 (u8*)g_zcl_basicAttrs.manuName},
		{ ZCL_ATTRID_BASIC_MODEL_ID,     		     ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,  											 (u8*)g_zcl_basicAttrs.modelId},
		{ ZCL_ATTRID_BASIC_DATE_CODE,     		   ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,  											 (u8*)g_zcl_basicAttrs.dateCode},
		{ ZCL_ATTRID_BASIC_POWER_SOURCE, 		     ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ,  											 (u8*)&g_zcl_basicAttrs.powerSource},
		{ ZCL_ATTRID_BASIC_GENERIC_DEVICE_CLASS, ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ,                        (u8*)&g_zcl_basicAttrs.genDevClass},
		{ ZCL_ATTRID_BASIC_GENERIC_DEVICE_TYPE,  ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ,                        (u8*)&g_zcl_basicAttrs.genDevType},
		{ ZCL_ATTRID_BASIC_DEV_ENABLED,  		     ZCL_DATA_TYPE_BOOLEAN,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_zcl_basicAttrs.deviceEnable},
		{ ZCL_ATTRID_BASIC_SW_BUILD_ID,  		     ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ,  											 (u8*)&g_zcl_basicAttrs.swBuildId},
		{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 	 ZCL_DATA_TYPE_UINT16,  	ACCESS_CONTROL_READ,  										 (u8*)&zcl_attr_global_clusterRevision},
	};
	zcl_basic_register(endpoint, MANUFACTURER_CODE_NONE, sizeof(basic_attrTbl) / sizeof(zclAttrInfo_t), (zclAttrInfo_t *)basic_attrTbl, switch_basicCb);

	static const zclAttrInfo_t group_attrTbl[] =
	{
		{ ZCL_ATTRID_GROUP_NAME_SUPPORT,  		ZCL_DATA_TYPE_BITMAP8,  ACCESS_CONTROL_READ,  (u8*)&g_zcl_groupAttrs.nameSupport },
		{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  	ACCESS_CONTROL_READ,  (u8*)&zcl_attr_global_clusterRevision},
	};
	zcl_group_register(endpoint, MANUFACTURER_CODE_NONE, sizeof(group_attrTbl) / sizeof(zclAttrInfo_t), (zclAttrInfo_t *)group_attrTbl, switch_groupCb);
	
	static const zclAttrInfo_t scene_attrTbl[] =
	{
    { ZCL_ATTRID_SCENE_SCENE_COUNT,       	ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,  (u8*)&g_zcl_sceneAttrs.sceneCount		  },
    { ZCL_ATTRID_SCENE_CURRENT_SCENE,     	ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ,  (u8*)&g_zcl_sceneAttrs.currentScene   },
    { ZCL_ATTRID_SCENE_CURRENT_GROUP,     	ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ,  (u8*)&g_zcl_sceneAttrs.currentGroup   },
    { ZCL_ATTRID_SCENE_SCENE_VALID,       	ZCL_DATA_TYPE_BOOLEAN,  ACCESS_CONTROL_READ,  (u8*)&g_zcl_sceneAttrs.sceneValid     },
    { ZCL_ATTRID_SCENE_NAME_SUPPORT,      	ZCL_DATA_TYPE_BITMAP8,  ACCESS_CONTROL_READ,  (u8*)&g_zcl_sceneAttrs.nameSupport    },
    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 	ZCL_DATA_TYPE_UINT16,  	ACCESS_CONTROL_READ,  (u8*)&zcl_attr_global_clusterRevision },
	};
	zcl_scene_register(endpoint, MANUFACTURER_CODE_NONE, sizeof(scene_attrTbl) / sizeof(zclAttrInfo_t), (zclAttrInfo_t *)scene_attrTbl, switch_sceneCb);
}

void registerZCL(u8 endpoint) {
	status_t st = ZCL_STA_SUCCESS;

	if (endpoint == ENDPOINT_1) {
		registerCommonZCL(endpoint);
	}

	zclAttrInfo_t *identify_attrTbl = g_identify_tbls[endpoint-1];
	identify_attrTbl[0] = (zclAttrInfo_t) { ZCL_ATTRID_IDENTIFY_TIME,  			    ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_zcl_identifyAttrs[endpoint-1].identifyTime };
	identify_attrTbl[1] = (zclAttrInfo_t)	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  	ACCESS_CONTROL_READ,  					            (u8*)&zcl_attr_global_clusterRevision};
	zcl_identify_register(endpoint, MANUFACTURER_CODE_NONE, sizeof(g_identify_tbls[endpoint-1]) / sizeof(zclAttrInfo_t), (zclAttrInfo_t *)g_identify_tbls[endpoint-1], switch_identifyCb);

	zclAttrInfo_t *onOff_attrTbl = g_onOff_tbls[endpoint-1];
	onOff_attrTbl[0] = (zclAttrInfo_t) { ZCL_ATTRID_ONOFF,  					        ZCL_DATA_TYPE_BOOLEAN, ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8*)&g_relayAttr[endpoint-1].onOff};
	onOff_attrTbl[1] = (zclAttrInfo_t) { ZCL_ATTRID_GLOBAL_SCENE_CONTROL, 		ZCL_DATA_TYPE_BOOLEAN, ACCESS_CONTROL_READ, 							              (u8*)&g_relayAttr[endpoint-1].globalSceneControl};
	onOff_attrTbl[2] = (zclAttrInfo_t) { ZCL_ATTRID_ON_TIME, 					      ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, 	    (u8*)&g_relayAttr[endpoint-1].onTime};
	onOff_attrTbl[3] = (zclAttrInfo_t) { ZCL_ATTRID_OFF_WAIT_TIME, 			    ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, 	    (u8*)&g_relayAttr[endpoint-1].offWaitTime};
	onOff_attrTbl[4] = (zclAttrInfo_t) { ZCL_ATTRID_START_UP_ONOFF, 			    ZCL_DATA_TYPE_ENUM8, 	 ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, 	    (u8*)&g_relayAttr[endpoint-1].startUpOnOff};
	onOff_attrTbl[5] = (zclAttrInfo_t) { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ,  							              (u8*)&zcl_attr_global_clusterRevision};
	st = zcl_onOff_register(endpoint, MANUFACTURER_CODE_NONE, sizeof(g_onOff_tbls[endpoint-1]) / sizeof(zclAttrInfo_t), (zclAttrInfo_t *)g_onOff_tbls[endpoint-1], switch_onOffCb);
	if(st != ZCL_STA_SUCCESS) {
    printf("ERR: cluster 0x%04X registration failed (0x%02X)\n", ZCL_CLUSTER_EPCONFIG, st);
	}

	zclAttrInfo_t *onOffSwitchCfg_attrTbl = g_onOffSwitchCfg_tbls[endpoint-1];
	onOffSwitchCfg_attrTbl[0] = (zclAttrInfo_t) { ZCL_ATTRID_SWITCH_MODE,             ZCL_DATA_TYPE_ENUM8,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_switchAttr[endpoint-1].switchMode};
	onOffSwitchCfg_attrTbl[1] = (zclAttrInfo_t) { ZCL_ATTRID_SWITCH_ACTION,           ZCL_DATA_TYPE_ENUM8,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_switchAttr[endpoint-1].switchAction};
	onOffSwitchCfg_attrTbl[2] = (zclAttrInfo_t) { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ,  							        (u8*)&zcl_attr_global_clusterRevision};
	st = zcl_onOffSwitchCfg_register(endpoint, MANUFACTURER_CODE_TELINK, sizeof(g_onOffSwitchCfg_tbls[endpoint-1]) / sizeof(zclAttrInfo_t), (zclAttrInfo_t *)g_onOffSwitchCfg_tbls[endpoint-1], switch_onOffSwitchCfgCb);
	if(st != ZCL_STA_SUCCESS) {
    printf("ERR: cluster 0x%04X registration failed (0x%02X)\n", ZCL_CLUSTER_EPCONFIG, st);
	}

	zclAttrInfo_t *epConfig_attrTbl = g_epConfig_tbls[endpoint-1];
	epConfig_attrTbl[0] = (zclAttrInfo_t) { ZCL_ATTRID_EPCONFIG_SIMPLE_CLICK_DEVICE, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_epConfigAttrs[endpoint-1].simpleClickDevice };
	epConfig_attrTbl[1] = (zclAttrInfo_t) { ZCL_ATTRID_EPCONFIG_SIMPLE_CLICK_DEVICE_EP, ZCL_DATA_TYPE_UINT8,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_epConfigAttrs[endpoint-1].simpleClickDeviceEp };
	epConfig_attrTbl[2] = (zclAttrInfo_t) { ZCL_ATTRID_EPCONFIG_DOUBLE_CLICK_DEVICE, ZCL_DATA_TYPE_UINT16,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_epConfigAttrs[endpoint-1].doubleClickDevice };
	epConfig_attrTbl[3] = (zclAttrInfo_t) { ZCL_ATTRID_EPCONFIG_DOUBLE_CLICK_DEVICE_EP, ZCL_DATA_TYPE_UINT8,  ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_epConfigAttrs[endpoint-1].doubleClickDeviceEp };
	epConfig_attrTbl[4] = (zclAttrInfo_t) { ZCL_ATTRID_EPCONFIG_LONG_PRESS_DEVICE, 	ZCL_DATA_TYPE_UINT16,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_epConfigAttrs[endpoint-1].longPressDevice };
	epConfig_attrTbl[5] = (zclAttrInfo_t) { ZCL_ATTRID_EPCONFIG_LONG_PRESS_DEVICE_EP, ZCL_DATA_TYPE_UINT8,	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_epConfigAttrs[endpoint-1].longPressDeviceEp };
	epConfig_attrTbl[6] = (zclAttrInfo_t) { ZCL_ATTRID_EPCONFIG_RELAY_MODE, 		    ZCL_DATA_TYPE_ENUM8,	 ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_epConfigAttrs[endpoint-1].relayMode };
	epConfig_attrTbl[7] = (zclAttrInfo_t) { ZCL_ATTRID_EPCONFIG_BACKLIGHT_MODE, 	    ZCL_DATA_TYPE_ENUM8,	 ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_epConfigAttrs[endpoint-1].backlightMode };
	epConfig_attrTbl[8] = (zclAttrInfo_t) { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 			    ZCL_DATA_TYPE_UINT16,	 ACCESS_CONTROL_READ,  							        (u8*)&zcl_attr_global_clusterRevision};
	st = zcl_registerCluster(endpoint, ZCL_CLUSTER_EPCONFIG, MANUFACTURER_CODE_TELINK, sizeof(g_epConfig_tbls[endpoint-1])/sizeof(zclAttrInfo_t), (zclAttrInfo_t *)g_epConfig_tbls[endpoint-1], NULL, NULL);
	
	if(st != ZCL_STA_SUCCESS) {
    printf("ERR: cluster 0x%04X registration failed (0x%02X)\n", ZCL_CLUSTER_EPCONFIG, st);
	}
}

void registerEndpoint(u8 endpoint) {
	af_simple_descriptor_t *simpleDesc = &g_simpleDescs[endpoint-1];
	simpleDesc->app_profile_id = HA_PROFILE_ID;
	simpleDesc->app_dev_id = HA_DEV_ONOFF_SWITCH;
	simpleDesc->endpoint = endpoint;
	simpleDesc->app_dev_ver = 2;
	simpleDesc->reserved = 0;

	if (endpoint == ENDPOINT_1) {
		simpleDesc->app_in_cluster_count = ENDPOINT_IN_CLUSTER_NUM;
		simpleDesc->app_out_cluster_count = ENDPOINT_OUT_CLUSTER_NUM;
		simpleDesc->app_in_cluster_lst = (u16 *)endpoint_inClusterList;
		simpleDesc->app_out_cluster_lst = (u16 *)endpoint_outClusterList;
		af_endpointRegister(endpoint, (af_simple_descriptor_t *)simpleDesc, zcl_rx_handler, NULL);
	} else {
		simpleDesc->app_in_cluster_count = EXTRA_ENDPOINT_IN_CLUSTER_NUM;
		simpleDesc->app_out_cluster_count = EXTRA_ENDPOINT_OUT_CLUSTER_NUM;
		simpleDesc->app_in_cluster_lst = (u16 *)extra_Endpoint_inClusterList;
		simpleDesc->app_out_cluster_lst = (u16 *)extra_Endpoint_outClusterList;
		af_endpointRegister(endpoint, (af_simple_descriptor_t *)simpleDesc, zcl_rx_handler, NULL);
	}
}