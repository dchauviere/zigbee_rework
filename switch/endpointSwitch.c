#include "tl_common.h"
#include "zcl_include.h"
#include "version_cfg.h"
#include "endpoints.h"
#include "zclApp.h"
#include "switchApp.h"

const u16 switch_Endpoint_inClusterList[] =
{
	ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,
};

const u16 switch_Endpoint_outClusterList[] =
{	
    ZCL_CLUSTER_GEN_ON_OFF,
	ZCL_CLUSTER_GEN_LEVEL_CONTROL,
};

#define SWITCH_ENDPOINT_IN_CLUSTER_NUM      (sizeof(switch_Endpoint_inClusterList)/sizeof(switch_Endpoint_inClusterList[0]))
#define SWITCH_ENDPOINT_OUT_CLUSTER_NUM     (sizeof(switch_Endpoint_outClusterList)/sizeof(switch_Endpoint_outClusterList[0]))

/* On/Off Configuration */
#define ZCL_SWITCH_MODE_TOGGLE              0x00
#define ZCL_SWITCH_MODE_MOMENTARY           0x01
#define ZCL_SWITCH_MODE_MULTIFUNCTION       0x02

#define ZCL_ATTRID_SWITCH_MODE				0x4000

#define get_onOffSwitchCfg_attrTbl(x) { \
	{ ZCL_ATTRID_SWITCH_MODE,               ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_switchAppCtx.switchCfgAttrs[x-1].switchMode     }, \
	{ ZCL_ATTRID_SWITCH_ACTION,             ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8*)&g_switchAppCtx.switchCfgAttrs[x-1].switchAction   }, \
	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 	ZCL_DATA_TYPE_UINT16,  	ACCESS_CONTROL_READ,                        (u8*)&zcl_attr_global_clusterRevision                   }, \
}

const af_simple_descriptor_t switchEndpointSimpleDesc_1 =
{
    HA_PROFILE_ID,
    HA_DEV_DIMMER_SWITCH,
    SWITCH_ENDPOINT_1,
	2,
    0,
    SWITCH_ENDPOINT_IN_CLUSTER_NUM,
    SWITCH_ENDPOINT_OUT_CLUSTER_NUM,
    (u16 *)switch_Endpoint_inClusterList,
    (u16 *)switch_Endpoint_outClusterList,
};
const zclAttrInfo_t onOffSwitchCfg_attrTbl_1[] = get_onOffSwitchCfg_attrTbl(1);
#define ZCL_ONOFFSWITCHCFG_ATTR_NUM         sizeof(onOffSwitchCfg_attrTbl_1) / sizeof(zclAttrInfo_t)
const zcl_specClusterInfo_t switchEndpointSpecClusterInfo_1[] =
{
	{ ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,  MANUFACTURER_CODE_NONE, ZCL_ONOFFSWITCHCFG_ATTR_NUM,    onOffSwitchCfg_attrTbl_1,   zcl_onOffSwitchCfg_register,    switch_onOffSwitchCfgCb },
};
u8 SWITCH_ENDPOINT_CB_CLUSTER_NUM = (sizeof(switchEndpointSpecClusterInfo_1)/sizeof(switchEndpointSpecClusterInfo_1[0]));

#if BUTTON_NUM > 1
const af_simple_descriptor_t switchEndpointSimpleDesc_2 =
{
    HA_PROFILE_ID,
    HA_DEV_DIMMER_SWITCH,
    SWITCH_ENDPOINT_2,
	2,
    0,
    SWITCH_ENDPOINT_IN_CLUSTER_NUM,
    SWITCH_ENDPOINT_OUT_CLUSTER_NUM,
    (u16 *)switch_Endpoint_inClusterList,
    (u16 *)switch_Endpoint_outClusterList,
};
const zclAttrInfo_t onOffSwitchCfg_attrTbl_2[] = get_onOffSwitchCfg_attrTbl(2);
const zcl_specClusterInfo_t switchEndpointSpecClusterInfo_2[] =
{
	{ ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,  MANUFACTURER_CODE_NONE, ZCL_ONOFFSWITCHCFG_ATTR_NUM,    onOffSwitchCfg_attrTbl_2,   zcl_onOffSwitchCfg_register,    switch_onOffSwitchCfgCb },
};
#endif

#if BUTTON_NUM > 2
const af_simple_descriptor_t switchEndpointSimpleDesc_3 =
{
    HA_PROFILE_ID,
    HA_DEV_DIMMER_SWITCH,
    SWITCH_ENDPOINT_3,
	2,
    0,
    SWITCH_ENDPOINT_IN_CLUSTER_NUM,
    SWITCH_ENDPOINT_OUT_CLUSTER_NUM,
    (u16 *)switch_Endpoint_inClusterList,
    (u16 *)switch_Endpoint_outClusterList,
};
const zclAttrInfo_t onOffSwitchCfg_attrTbl_3[] = get_onOffSwitchCfg_attrTbl(3);
const zcl_specClusterInfo_t switchEndpointSpecClusterInfo_3[] =
{
	{ ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,  MANUFACTURER_CODE_NONE, ZCL_ONOFFSWITCHCFG_ATTR_NUM,    onOffSwitchCfg_attrTbl_3,   zcl_onOffSwitchCfg_register,    switch_onOffSwitchCfgCb },
};
#endif

void registerSwitchEndpoints(void) {
	af_endpointRegister(SWITCH_ENDPOINT_1, (af_simple_descriptor_t *)&switchEndpointSimpleDesc_1, zcl_rx_handler, NULL);
#if BUTTON_NUM > 1
    af_endpointRegister(SWITCH_ENDPOINT_2, (af_simple_descriptor_t *)&switchEndpointSimpleDesc_2, zcl_rx_handler, NULL);
#endif
#if BUTTON_NUM > 2
    af_endpointRegister(SWITCH_ENDPOINT_3, (af_simple_descriptor_t *)&switchEndpointSimpleDesc_3, zcl_rx_handler, NULL);
#endif
}

void registerSwitchZCL(void) {
	zcl_register(SWITCH_ENDPOINT_1, SWITCH_ENDPOINT_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)switchEndpointSpecClusterInfo_1);
#if BUTTON_NUM > 1
    zcl_register(SWITCH_ENDPOINT_2, SWITCH_ENDPOINT_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)switchEndpointSpecClusterInfo_2);
#endif
#if BUTTON_NUM > 2
    zcl_register(SWITCH_ENDPOINT_3, SWITCH_ENDPOINT_CB_CLUSTER_NUM, (zcl_specClusterInfo_t *)switchEndpointSpecClusterInfo_3);
#endif
}
