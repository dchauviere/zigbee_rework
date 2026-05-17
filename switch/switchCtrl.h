/********************************************************************************************************
 * @file    app_ui.h
 *
 * @brief   This is the header file for app_ui
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

#ifndef __SWITCH_CTRL_H__
#define __SWITCH_CTRL_H__

/**********************************************************************
 * CONSTANT
 */

#define NV_ITEM_APP_SWITCH_BASE 			0x50

#define ZCL_SWITCH_TYPE_TOGGLE                  0x00
#define ZCL_SWITCH_TYPE_MOMENTARY               0x01
#define ZCL_SWITCH_TYPE_MULTIFUNCTION           0x02

#define ZCL_SWITCH_ACTION_ON_OFF                0x00
#define ZCL_SWITCH_ACTION_OFF_ON                0x01
#define ZCL_SWITCH_ACTION_TOGGLE                0x02

#define ZCL_RELAY_CONTROL_MODE_OFF              0x00
#define ZCL_RELAY_CONTROL_MODE_TOGGLE           0x01

/**********************************************************************
 * TYPEDEFS
 */

typedef struct{
	u16 transitionTime;
	u8 switchMode;
	u8 switchAction;
	u8 relayControlMode;
	u8 relayAttached;
	u8 triggerMode;
}switchAttr_t;

extern switchAttr_t g_switchAttr[BUTTON_NUM];

/**********************************************************************
 * FUNCTIONS
 */

void initSwitches(void);
void switchesHandler(void);
nv_sts_t saveSwitchConfig(u8 sw);
nv_sts_t restoreSwitchConfig(u8 sw);

#endif	/* __BUTTON_CTRL_H__ */
