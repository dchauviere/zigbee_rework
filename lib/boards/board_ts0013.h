/********************************************************************************************************
 * @file    board_8258_evk.h
 *
 * @brief   This is the header file for board_8258_evk
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

#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#ifndef ZCL_BASIC_MFG_NAME
#define ZCL_BASIC_MFG_NAME     {4,'M','o','e','s'}
#endif
#ifndef ZCL_BASIC_MODEL_ID
#define ZCL_BASIC_MODEL_ID	   {7,'Z','S','-','E','U','B', '3'}
#endif

#define HARDWARE_REV                0x01
#define CHIP_TYPE					TLSR_8258_1M
#define DEVICE_TYPE					IMAGE_TYPE_SWITCH_TS0013
// BUTTON

#define BUTTON_NUM                  3

// GPIO_PC4
#define BUTTON1               		GPIO_PC4
#define PC4_FUNC			  		AS_GPIO
#define PC4_OUTPUT_ENABLE	  		0
#define PC4_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PC4	  		PM_PIN_PULLUP_10K

// GPIO_PB6
#define BUTTON2             		GPIO_PB6
#define PB6_FUNC			  		AS_GPIO
#define PB6_OUTPUT_ENABLE	  		0
#define PB6_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PB6	  		PM_PIN_PULLUP_10K

// GPIO_PD7 ?
#define BUTTON3             		GPIO_PC1
#define PC1_FUNC			  		AS_GPIO
#define PC1_OUTPUT_ENABLE	  		0
#define PC1_INPUT_ENABLE	  		1
#define	PULL_WAKEUP_SRC_PC1	  		PM_PIN_PULLUP_10K

// RELAY
// GPIO_PB4
#define RELAY1     					GPIO_PB4
#define PB4_FUNC					AS_GPIO
#define PB4_OUTPUT_ENABLE			1
#define PB4_INPUT_ENABLE			0

// GPIO_PB5
#define RELAY2     					GPIO_PB5
#define PB5_FUNC					AS_GPIO
#define PB5_OUTPUT_ENABLE			1
#define PB5_INPUT_ENABLE			0

// GPIO_PD2
#define RELAY3     					GPIO_PD2
#define PD2_FUNC					AS_GPIO
#define PD2_OUTPUT_ENABLE			1
#define PD2_INPUT_ENABLE			0

#define RELAYS_NUM 3

// LED
// GPIO_C0
#define LED1     					GPIO_PC0
#define PC0_FUNC					AS_GPIO
#define PC0_OUTPUT_ENABLE			1
#define PC0_INPUT_ENABLE			0

// GPIO_D3
#define LED2     					GPIO_PD3
#define PD3_FUNC					AS_GPIO
#define PD3_OUTPUT_ENABLE			1
#define PD3_INPUT_ENABLE			0

// GPIO_?
#define LED3     					GPIO_PD7
#define PD7_FUNC					AS_GPIO
#define PD7_OUTPUT_ENABLE			1
#define PD7_INPUT_ENABLE			0

#define LEDS_NUM 3

#define LED_POWER					LED1

#define	PM_WAKEUP_LEVEL		  		PM_WAKEUP_LEVEL_LOW

#define VOLTAGE_DETECT_PIN			GPIO_PC5

// UART
#define UART_TX_PIN         	UART_TX_PB1
#define UART_RX_PIN         	UART_RX_PB7

#define UART_PIN_CFG()			uart_gpio_set(UART_TX_PIN, UART_RX_PIN);// uart tx/rx pin set

// UART
#if ZBHCI_UART
	#error please configurate uart PIN!!!!!!
#endif

// DEBUG
#define	DEBUG_INFO_TX_PIN	    GPIO_PB1//print

#if !defined(__ASSEMBLER__)
enum{
	VK_SW1 = 0x01,
	VK_SW2 = 0x02,
	VK_SW3 = 0x03
};
#endif

#define	KB_MAP_NORMAL	{{VK_SW1,},{VK_SW2,},{VK_SW3,},}

#define	KB_MAP_NUM		KB_MAP_NORMAL
#define	KB_MAP_FN		KB_MAP_NORMAL

#define KB_DRIVE_PINS  {0}
#define KB_SCAN_PINS   {BUTTON1, BUTTON2, BUTTON3}

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
