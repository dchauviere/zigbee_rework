#ifndef _ZCL_APP_H_
#define _ZCL_APP_H_

#include "tl_common.h"
#include "zcl_include.h"

void switch_zclProcessIncomingMsg(zclIncoming_t *pInHdlrMsg);

void saveAllAttrsToNVRAM(void);
void restoreAllAttrsFromNVRAM(void);


// Basic
#define ZCL_ATTRID_BASIC_GENERIC_DEVICE_CLASS  0x0008
#define ZCL_ATTRID_BASIC_GENERIC_DEVICE_TYPE   0x0009
typedef struct{
	u8 	zclVersion;
	u8	appVersion;
	u8	stackVersion;
	u8	hwVersion;
	u8	manuName[ZCL_BASIC_MAX_LENGTH];
	u8	modelId[ZCL_BASIC_MAX_LENGTH];
	u8  dateCode[ZCL_BASIC_MAX_LENGTH];
	u8	swBuildId[ZCL_BASIC_MAX_LENGTH];
	u8	powerSource;
	u8  genDevClass;                        //attr 8
	u8  genDevType;                         //attr 9
	u8	deviceEnable;
}zcl_basicAttr_t;
extern zcl_basicAttr_t g_zcl_basicAttrs;
status_t switch_basicCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);

// Identify
typedef struct{
	u16	identifyTime;
}zcl_identifyAttr_t;
extern zcl_identifyAttr_t g_zcl_identifyAttrs[BUTTON_NUM];
status_t switch_identifyCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
void initIdentify(void);

// Groups
typedef struct{
	u8	nameSupport;
}zcl_groupAttr_t;
extern zcl_groupAttr_t g_zcl_groupAttrs;
status_t switch_groupCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);

// Scenes
typedef struct{
	u8	 sceneCount;
	u8	 currentScene;
	u8	 nameSupport;
	bool sceneValid;
	u16	 currentGroup;
}zcl_sceneAttr_t;
extern zcl_sceneAttr_t g_zcl_sceneAttrs;

status_t switch_sceneCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);


status_t switch_onOffCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);

#define NV_ITEM_APP_SWITCH_BASE 			0x50

#define ZCL_SWITCH_TYPE_TOGGLE                  0x00
#define ZCL_SWITCH_TYPE_MOMENTARY               0x01
#define ZCL_SWITCH_TYPE_MULTIFUNCTION           0x02

#define ZCL_SWITCH_ACTION_ON_OFF                0x00
#define ZCL_SWITCH_ACTION_OFF_ON                0x01
#define ZCL_SWITCH_ACTION_TOGGLE                0x02

#define NV_ITEM_ZCL_ON_OFF_SWITCH_CFG 0x40

#define ZCL_ATTRID_SWITCH_TYPE                  0x0000
#define ZCL_ATTRID_SWITCH_ACTION		        0x0010

#define ZCL_ONOFF_ON_SWITCH_TRIGGER_UNLINKED    0x00
#define ZCL_ONOFF_ON_SWITCH_TRIGGER_FRONT       0x01
#define ZCL_ONOFF_ON_SWITCH_TRIGGER_SINGLE      0x02

typedef struct{
	u16 transitionTime;
	u8 switchMode;
	u8 switchAction;
	u8 triggerMode;
}switchAttr_t;

extern switchAttr_t g_switchAttr[BUTTON_NUM];

status_t zcl_onOffSwitchCfg_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);
status_t switch_onOffSwitchCfgCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
nv_sts_t saveSwitchConfig(u8 sw);
nv_sts_t restoreSwitchConfig(u8 sw);

/***********************************************
  EPConfig
 ***********************************************/
#define ZCL_CLUSTER_EPCONFIG                          0xFC10

#define ZCL_EPCONFIG_BACKLIGHT_MODE_ON             	 0x00
#define ZCL_EPCONFIG_BACKLIGHT_MODE_OFF            	 0x01
#define ZCL_EPCONFIG_BACKLIGHT_MODE_ONOFF             0x02
#define ZCL_EPCONFIG_BACKLIGHT_MODE_OFFON             0x03

// Attribute IDs
#define ZCL_ATTRID_EPCONFIG_SIMPLE_CLICK_DEVICE       0x0001
#define ZCL_ATTRID_EPCONFIG_DOUBLE_CLICK_DEVICE       0x0002
#define ZCL_ATTRID_EPCONFIG_LONG_PRESS_DEVICE         0x0003
#define ZCL_ATTRID_EPCONFIG_RELAY_MODE                0x0004
#define ZCL_ATTRID_EPCONFIG_BACKLIGHT_MODE            0x0005

#define ZCL_EPCONFIG_RELAY_MODE_DETACHED              0x00
#define ZCL_EPCONFIG_RELAY_MODE_ATTACHED              0x01

#define NV_ITEM_APP_EPCONFIG_BASE		0x70

// Structure en RAM
typedef struct {
    u16 simpleClickDevice;
    u16 doubleClickDevice;
    u16 longPressDevice;
    u8 relayMode;
		u8 backlightMode;
} epconfig_attr_t;

extern epconfig_attr_t g_epConfigAttrs[BUTTON_NUM];

nv_sts_t saveEPConfig(u8 endpoint);
nv_sts_t restoreEPConfig(u8 endpoint);

/***********************
 * Global Config
 ************************/
#define NV_ITEM_APP_GLOBAL_CONFIG               0x60

#define GLOBAL_DEFAULT_ACTION_HOLD_THRESHOLD    2*1000
#define GLOBAL_DEFAULT_ACTION_HOLD_TRANSITION   2*500
#define GLOBAL_DEFAULT_ACTION_HOLD_STEP         50
#define GLOBAL_DEFAULT_ACTION_CLICK_TRANSITION  500
#define GLOBAL_DEFAULT_IDENTIFY_DURATION        10
#define GLOBAL_DEFAULT_RESET_DURATION           30

typedef struct {
  u8 resetDuration;           // duration in seconds waiting for reset
  u8 identifyDuration;        // duration in seconds for identify
  u8 actionclickTransition;   // max transition time between 2 clicks
  u16 actionHoldThreshold;    // starting hold action at threshold in milliseconds
  u16 actionHoldTransition;   // transition time between 2 hold step
  u8 actionHoldStep;          // step size on hold action 
} global_config_t;

extern global_config_t g_globalConfig;

nv_sts_t saveGlobalConfig(void);
nv_sts_t restoreGlobalConfig(void);

#endif