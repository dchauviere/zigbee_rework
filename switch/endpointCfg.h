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

#define ZCL_ONOFF_BACKLIGHT_MODE_ON             0x00
#define ZCL_ONOFF_BACKLIGHT_MODE_OFF            0x01
#define ZCL_ONOFF_BACKLIGHT_MODE_ONOFF          0x02
#define ZCL_ONOFF_BACKLIGHT_MODE_OFFON          0x03

#define ZCL_ONOFF_ON_SWITCH_TRIGGER_UNLINKED    0x00
#define ZCL_ONOFF_ON_SWITCH_TRIGGER_FRONT       0x01
#define ZCL_ONOFF_ON_SWITCH_TRIGGER_SINGLE      0x02

/**
 *  @brief Defined for on/off cluster attributes
 */

const af_simple_descriptor_t endpoint_simpleDesc;

void registerAllEndpoints(void);
void registerAllZCL(void);

#endif