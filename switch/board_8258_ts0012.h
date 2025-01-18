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

// BUTTON 
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

// LED
// GPIO_D3
#define LED1     					GPIO_PD3
#define PD3_FUNC					AS_GPIO
#define PD3_OUTPUT_ENABLE			1
#define PD3_INPUT_ENABLE			0

// GPIO_C0
#define LED2     					GPIO_PC0
#define PC0_FUNC					AS_GPIO
#define PC0_OUTPUT_ENABLE			1
#define PC0_INPUT_ENABLE			0

#define LED_POWER					LED1
#define LED_PERMIT					LED2

#define LED_ON						0//1
#define LED_OFF						1//0
#define HAVE_1_LED                  1

// #define UART_ENABLE 1

// UART
#if UART_ENABLE
	#define UART_TX_PIN         	UART_TX_PB1
	#define UART_RX_PIN         	UART_RX_PB7

	#define UART_PIN_CFG()			uart_gpio_set(UART_TX_PIN, UART_RX_PIN);// uart tx/rx pin set
#endif

#define	UART_PRINTF_MODE				1

// DEBUG
#if UART_PRINTF_MODE
	#define	DEBUG_INFO_TX_PIN	    GPIO_PC1//print
#endif

// USB
#if ZBHCI_USB_PRINT || ZBHCI_USB_CDC || ZBHCI_USB_HID
	#define HW_USB_CFG()			do{ \
										usb_set_pin_en();	\
									}while(0)
#endif

#if !defined(__ASSEMBLER__)
enum{
	VK_SW1 = 0x01,
	VK_SW2 = 0x02
};
#endif

#define	KB_MAP_NORMAL	{{VK_SW1,},{VK_SW2,},}

#define	KB_MAP_NUM		KB_MAP_NORMAL
#define	KB_MAP_FN		KB_MAP_NORMAL

#define KB_DRIVE_PINS  {NULL }
#define KB_SCAN_PINS   {BUTTON1,  BUTTON2}

#define IMAGE_TYPE                          ((CHIP_TYPE << 8) | IMAGE_TYPE_BOOTLOADER) //0x03FF
//zigbee model id
#define ZCL_BASIC_MODEL_ID          {20,'F','i','x','e','d','_','Z','S','-','E','U','B','2','_','T','S','0','0','1','2'}

//
#define HARDWARE_REV                0x01

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
