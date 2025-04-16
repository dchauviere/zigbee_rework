#ifndef _ENDPOINT_CFG_H_
#define _ENDPOINT_CFG_H_

/**********************************************************************
 * CONSTANT
 */
#define ENDPOINT_1  0x01
#define ENDPOINT_2  0x02
#define ENDPOINT_3  0x03

#define ZCL_ATTRID_SWITCH_TYPE          0x0000
#define ZCL_ATTRID_SWITCH_ACTION		0x0010

#define ZCL_SWITCH_TYPE_TOGGLE                  0x00
#define ZCL_SWITCH_TYPE_MOMENTARY               0x01
#define ZCL_SWITCH_TYPE_MULTIFUNCTION           0x02

#define ZCL_SWITCH_ACTION_ON_OFF                0x00
#define ZCL_SWITCH_ACTION_OFF_ON                0x01
#define ZCL_SWITCH_ACTION_TOGGLE                0x02

/**
 *  @brief Defined for on/off cluster attributes
 */
typedef struct{
	u16	 onTime;
	u16	 offWaitTime;
	u8	 startUpOnOff;
	bool onOff;
	bool globalSceneControl;
}zcl_onOffAttr_t;

typedef struct{
	u8 switchType;
	u8 switchAction;
}zcl_onOffSwitchCfgAttr_t;

const af_simple_descriptor_t endpoint_simpleDesc;
zcl_onOffAttr_t g_zcl_onOffAttrs[BUTTON_NUM]; // for relays
zcl_onOffSwitchCfgAttr_t g_zcl_onOffSwitchCfgAttrs[BUTTON_NUM]; // for relays
#define zcl_onoffAttrGet(x)		&g_zcl_onOffAttrs[x]
#define zcl_onoffSwitchCfgAttrGet(x)		&g_zcl_onOffSwitchCfgAttrs[x]

void registerAllEndpoints(void);
void registerAllZCL(void);

#endif