#ifndef _ENDPOINT_CFG_H_
#define _ENDPOINT_CFG_H_

/**********************************************************************
 * CONSTANT
 */
#define ENDPOINT_1  0x01
#define ENDPOINT_2  0x02
#define ENDPOINT_3  0x03

#define getRelayFromEndpoint(x)                 (x - 1)
#define getSwitchFromEndpoint(x)                (x - 1)

#define getEndpointFromRelay(x)                 (x + 1)
#define getEndpointFromSwitch(x)                (x + 1)

/**
 *  @brief Defined for on/off cluster attributes
 */

af_simple_descriptor_t g_simpleDescs[BUTTON_NUM];

void registerEndpoint(u8 endpoint);
void registerZCL(u8 endpoint);

#endif