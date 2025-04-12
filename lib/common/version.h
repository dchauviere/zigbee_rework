#pragma once

#include "comm_cfg.h"

#define APP_RELEASE							0x10//app release 1.0
#define APP_BUILD							0x8//app build 01
#define STACK_RELEASE						0x30//stack release 3.0
#define STACK_BUILD							0x01//stack build 01

#ifndef ZCL_BASIC_MFG_NAME
#define ZCL_BASIC_MFG_NAME     {6,'D','C','E','D','e','v'}
#endif
#ifndef ZCL_BASIC_MODEL_ID
#define ZCL_BASIC_MODEL_ID	   {14,'Z','S','-','E','U','B','2','_','T','S','0','0','1','2'}
#endif

#ifndef ZCL_BASIC_BUILD_ID
	#define ZCL_BASIC_BUILD_ID     {10,'0','1','0','5','0','2','2','0','2','5'}
#endif

#ifndef ZCL_BASIC_SW_BUILD_ID //max 16 chars
	#define ZCL_BASIC_SW_BUILD_ID       {7,(APP_RELEASE >> 4) + 0x30,'.',APP_BUILD + 0x30,'.',(STACK_RELEASE >> 4) + 0x30,'.',STACK_BUILD + 0x30}
#endif

#define MANUFACTURER_CODE_TELINK           	0x1141//Telink ID

#define	FILE_VERSION					  	((APP_RELEASE << 24) | (APP_BUILD << 16) | (STACK_RELEASE << 8) | STACK_BUILD)
