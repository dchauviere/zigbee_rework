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

#ifndef _APP_UI_H_
#define _APP_UI_H_

/**********************************************************************
 * CONSTANT
 */
#define LED_ON						1
#define LED_OFF						0

#define RELAY_ON						1
#define RELAY_OFF						0

#define APP_DEFAULT_ACTION_HOLD_THRESHOLD  2*1000*1000
#define APP_DEFAULT_ACTION_HOLD_TRANSITION 2*500*1000
#define APP_DEFAULT_ACTION_HOLD_STEP       50

/**********************************************************************
 * TYPEDEFS
 */
enum{
	APP_STATE_IDLE,
	APP_STATE_WAIT_KEY_MODE,
	APP_STATE_ACTION_CLICKS,
	APP_STATE_ACTION_HOLD,
	APP_STATE_WAIT_ACTION_END,
};


/**********************************************************************
 * FUNCTIONS
 */
void light_blink_start(u8 times, u16 ledOnTime, u16 ledOffTime);
void light_blink_stop(void);

void light_init(void);
void light_on(void);
void light_off(void);

void led_on(u32 pin);
void led_off(u32 pin);

void relay_on(u32 pin);
void relay_off(u32 pin);

void app_key_handler(void);

#endif	/* _APP_UI_H_ */
