#ifndef _ENDPOINT_CFG_H_
#define _ENDPOINT_CFG_H_

/**********************************************************************
 * CONSTANT
 */
#define CONFIG_ENDPOINT                         0x01
#define SWITCH_ENDPOINT_1                       0x02
#define SWITCH_ENDPOINT_2                       0x03
#define SWITCH_ENDPOINT_3                       0x04
#define RELAY_ENDPOINT_1                        0x05
#define RELAY_ENDPOINT_2                        0x06
#define RELAY_ENDPOINT_3                        0x07

#define getRelayFromEndpoint(x)                 (x - RELAY_ENDPOINT_1)
#define getSwitchFromEndpoint(x)                (x - SWITCH_ENDPOINT_1)

#define getEndpointFromRelay(x)                 (x + RELAY_ENDPOINT_1)
#define getEndpointFromSwitch(x)                (x + SWITCH_ENDPOINT_1)

#define ZCL_ATTRID_SWITCH_TYPE                  0x0000
#define ZCL_ATTRID_SWITCH_ACTION		        0x0010

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

const af_simple_descriptor_t configEndpointSimpleDesc;

void registerConfigEndpoint(void);
void registerConfigZCL(void);

void registerSwitchEndpoints(void);
void registerSwitchZCL(void);

void registerRelayEndpoints(void);
void registerRelayZCL(void);

#endif