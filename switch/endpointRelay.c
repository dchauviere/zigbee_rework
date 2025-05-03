#include "tl_common.h"
#include "zcl_include.h"
#include "version_cfg.h"
#include "endpoints.h"
#include "zclApp.h"
#include "switchApp.h"

const u16 relayEndpointInClusterList[] =
{
	ZCL_CLUSTER_GEN_ON_OFF,
};

const u16 relayEndpointOutClusterList[] = {};

#define RELAY_ENDPOINT_IN_CLUSTER_NUM       (sizeof(relayEndpointInClusterList)/sizeof(relayEndpointInClusterList[0]))
#define RELAY_ENDPOINT_OUT_CLUSTER_NUM      (sizeof(relayEndpointOutClusterList)/sizeof(relayEndpointOutClusterList[0]))

/* On/Off */
#define ZCL_ATTRID_ON_SWITCH_TRIGGER        0x4100
#define ZCL_ATTRID_BACKLIGHT_MODE           0x4101

#define get_onOff_attrTbl(x) { \
	{ ZCL_ATTRID_ONOFF,  					ZCL_DATA_TYPE_BOOLEAN,  ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE,  (u8*)&g_switchAppCtx.relayAttrs[x-1].onOff}, \
	{ ZCL_ATTRID_GLOBAL_SCENE_CONTROL, 		ZCL_DATA_TYPE_BOOLEAN, 	ACCESS_CONTROL_READ, 							  (u8*)&g_switchAppCtx.relayAttrs[x-1].globalSceneControl}, \
	{ ZCL_ATTRID_ON_TIME, 					ZCL_DATA_TYPE_UINT16, 	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, 	  (u8*)&g_switchAppCtx.relayAttrs[x-1].onTime}, \
	{ ZCL_ATTRID_OFF_WAIT_TIME, 			ZCL_DATA_TYPE_UINT16, 	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, 	  (u8*)&g_switchAppCtx.relayAttrs[x-1].offWaitTime}, \
	{ ZCL_ATTRID_START_UP_ONOFF, 			ZCL_DATA_TYPE_ENUM8, 	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, 	  (u8*)&g_switchAppCtx.relayAttrs[x-1].startUpOnOff}, \
	{ ZCL_ATTRID_ON_SWITCH_TRIGGER, 		ZCL_DATA_TYPE_ENUM8, 	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, 	  (u8*)&g_switchAppCtx.relayAttrs[x-1].onSwitchTrigger}, \
	{ ZCL_ATTRID_BACKLIGHT_MODE, 		    ZCL_DATA_TYPE_ENUM8, 	ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, 	  (u8*)&g_switchAppCtx.relayAttrs[x-1].backlightMode}, \
	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 	ZCL_DATA_TYPE_UINT16,  	ACCESS_CONTROL_READ,  							  (u8*)&zcl_attr_global_clusterRevision}, \
}

const af_simple_descriptor_t relayEndpointSimpleDesc_1 =
{
    HA_PROFILE_ID,
    HA_DEV_ONOFF_SWITCH,
    SWITCH_ENDPOINT_1,
	2,
    0,
    RELAY_ENDPOINT_IN_CLUSTER_NUM,
    RELAY_ENDPOINT_OUT_CLUSTER_NUM,
    (u16 *)relayEndpointInClusterList,
    (u16 *)relayEndpointOutClusterList,
};
const zclAttrInfo_t onOff_attrTbl_1[] = get_onOff_attrTbl(1);
#define ZCL_ONOFF_ATTR_NUM         sizeof(onOff_attrTbl_1) / sizeof(zclAttrInfo_t)
const zcl_specClusterInfo_t relayEndpointSpecClusterInfo_1[] =
{
	{ ZCL_CLUSTER_GEN_ON_OFF,   MANUFACTURER_CODE_TELINK,   ZCL_ONOFF_ATTR_NUM, onOff_attrTbl_1,  zcl_onOff_register, switch_onOffCb},
};
u8 RELAY_ENDPOINT_CB_CLUSTER_NUM = (sizeof(relayEndpointSpecClusterInfo_1)/sizeof(relayEndpointSpecClusterInfo_1[0]));

#if BUTTON_NUM > 1
const af_simple_descriptor_t relayEndpointSimpleDesc_2 =
{
    HA_PROFILE_ID,
    HA_DEV_ONOFF_SWITCH,
    SWITCH_ENDPOINT_2,
	2,
    0,
    RELAY_ENDPOINT_IN_CLUSTER_NUM,
    RELAY_ENDPOINT_OUT_CLUSTER_NUM,
    (u16 *)relayEndpointInClusterList,
    (u16 *)relayEndpointOutClusterList,
};
const zclAttrInfo_t onOff_attrTbl_2[] = get_onOff_attrTbl(2);
const zcl_specClusterInfo_t relayEndpointSpecClusterInfo_2[] =
{
	{ ZCL_CLUSTER_GEN_ON_OFF,   MANUFACTURER_CODE_TELINK,   ZCL_ONOFF_ATTR_NUM, onOff_attrTbl_2,  zcl_onOff_register, switch_onOffCb},
};
#endif

#if BUTTON_NUM > 2
const af_simple_descriptor_t relayEndpointSimpleDesc_3 =
{
    HA_PROFILE_ID,
    HA_DEV_ONOFF_SWITCH,
    SWITCH_ENDPOINT_3,
	2,
    0,
    RELAY_ENDPOINT_IN_CLUSTER_NUM,
    RELAY_ENDPOINT_OUT_CLUSTER_NUM,
    (u16 *)relayEndpointInClusterList,
    (u16 *)relayEndpointOutClusterList,
};
const zclAttrInfo_t onOff_attrTbl_3[] = get_onOff_attrTbl(3);
const zcl_specClusterInfo_t relayEndpointSpecClusterInfo_3[] =
{
	{ ZCL_CLUSTER_GEN_ON_OFF,   MANUFACTURER_CODE_TELINK,   ZCL_ONOFF_ATTR_NUM, onOff_attrTbl_3,  zcl_onOff_register, switch_onOffCb},
};
#endif

void registerRelayEndpoints(void) {
	af_endpointRegister(RELAY_ENDPOINT_1, (af_simple_descriptor_t *)&relayEndpointSimpleDesc_1, zcl_rx_handler, NULL);
#if BUTTON_NUM > 1
    af_endpointRegister(RELAY_ENDPOINT_2, (af_simple_descriptor_t *)&relayEndpointSimpleDesc_2, zcl_rx_handler, NULL);
#endif
#if BUTTON_NUM > 2
    af_endpointRegister(RELAY_ENDPOINT_3, (af_simple_descriptor_t *)&relayEndpointSimpleDesc_3, zcl_rx_handler, NULL);
#endif
}

void registerRelayZCL(void) {
	zcl_register(RELAY_ENDPOINT_1, RELAY_ENDPOINT_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)relayEndpointSpecClusterInfo_1);
#if BUTTON_NUM > 1
    zcl_register(RELAY_ENDPOINT_2, RELAY_ENDPOINT_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)relayEndpointSpecClusterInfo_2);
#endif
#if BUTTON_NUM > 2
    zcl_register(RELAY_ENDPOINT_3, RELAY_ENDPOINT_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)relayEndpointSpecClusterInfo_3);
#endif
}
