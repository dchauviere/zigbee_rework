#ifndef _ENDPOINT_CFG_H_
#define _ENDPOINT_CFG_H_

/**********************************************************************
 * CONSTANT
 */
#define ENDPOINT_1  0x01
#define ENDPOINT_2  0x02

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

const af_simple_descriptor_t endpoint_simpleDesc;
zcl_onOffAttr_t g_zcl_onOffAttrs[BUTTON_NUM]; // for relays
#define zcl_onoffAttrGet(x)		&g_zcl_onOffAttrs[x]

void registerAllEndpoints(void);
void registerAllZCL(void);

#endif