#include "tl_common.h"
#include "zcl_include.h"
#include "version_cfg.h"
#include "endpoints.h"
#include "zclApp.h"
#include "switchApp.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */

 #define ZCL_ATTRID_BASIC_GENERIC_DEVICE_CLASS  0x0008
 #define ZCL_ATTRID_BASIC_GENERIC_DEVICE_TYPE   0x0009

/**
 *  @brief Definition for Incoming cluster / Sever Cluster
 */
const u16 configEndpointInClusterList[] =
{
	ZCL_CLUSTER_GEN_BASIC,
	ZCL_CLUSTER_GEN_IDENTIFY,
	ZCL_CLUSTER_GEN_GROUPS,
	ZCL_CLUSTER_GEN_SCENES,
};

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const u16 configEndpointOutClusterList[] =
{
	ZCL_CLUSTER_GEN_GROUPS,
	ZCL_CLUSTER_GEN_SCENES,
    ZCL_CLUSTER_OTA,
};

/**
 *  @brief Definition for Server cluster number and Client cluster number
 */
#define CONFIG_ENDPOINT_IN_CLUSTER_NUM		(sizeof(configEndpointInClusterList)/sizeof(configEndpointInClusterList[0]))
#define CONFIG_ENDPOINT_OUT_CLUSTER_NUM		(sizeof(configEndpointOutClusterList)/sizeof(configEndpointOutClusterList[0]))

/**
 *  @brief Definition for simple description for HA profile
 */
const af_simple_descriptor_t configEndpointSimpleDesc =
{
	HA_PROFILE_ID,                      /* Application profile identifier */
	HA_DEV_CONFIG_TOOL,                	/* Application device identifier */
	CONFIG_ENDPOINT,                         /* Endpoint */
	2,                                  /* Application device version */
	0,									/* Reserved */
	CONFIG_ENDPOINT_IN_CLUSTER_NUM,           	/* Application input cluster count */
	CONFIG_ENDPOINT_OUT_CLUSTER_NUM,          	/* Application output cluster count */
	(u16 *)configEndpointInClusterList,    	/* Application input cluster list */
	(u16 *)configEndpointOutClusterList,   	/* Application output cluster list */
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

/**
 *  @brief Definition for simple switch ZCL specific cluster
 */
const zcl_specClusterInfo_t configEndpointSpecClusterInfo[] =
{
	{ZCL_CLUSTER_GEN_BASIC,			MANUFACTURER_CODE_NONE,	ZCL_BASIC_ATTR_NUM, 	basic_attrTbl,  	zcl_basic_register,		switch_basicCb},
	{ZCL_CLUSTER_GEN_IDENTIFY,		MANUFACTURER_CODE_NONE,	ZCL_IDENTIFY_ATTR_NUM,	identify_attrTbl,	zcl_identify_register,	switch_identifyCb},
	{ZCL_CLUSTER_GEN_GROUPS,		MANUFACTURER_CODE_NONE,	0, 						NULL,  				zcl_group_register,		switch_groupCb},
	{ZCL_CLUSTER_GEN_SCENES,		MANUFACTURER_CODE_NONE,	0,						NULL,				zcl_scene_register,		switch_sceneCb},
};

u8 CONFIG_ENDPOINT_CB_CLUSTER_NUM = (sizeof(configEndpointSpecClusterInfo)/sizeof(configEndpointSpecClusterInfo[0]));

void registerConfigEndpoint(void) {
	af_endpointRegister(CONFIG_ENDPOINT, (af_simple_descriptor_t *)&configEndpointSimpleDesc, zcl_rx_handler, NULL);
}

void registerConfigZCL(void) {
	zcl_register(CONFIG_ENDPOINT, CONFIG_ENDPOINT_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)configEndpointSpecClusterInfo);
}
