#ifndef _ZCL_APP_H_
#define _ZCL_APP_H_

void switch_zclProcessIncomingMsg(zclIncoming_t *pInHdlrMsg);

// Basic
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
extern zcl_identifyAttr_t g_zcl_identifyAttrs;
status_t switch_identifyCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);

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
#define zcl_sceneAttrGet()		&g_zcl_sceneAttrs
status_t switch_sceneCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);

// OnOff
typedef struct{
	u16	 onTime;
	u16	 offWaitTime;
	u8	 startUpOnOff;
	bool onOff;
	bool globalSceneControl;
	u8 	 onSwitchTrigger;
	u8   backlightMode;
}zcl_onOffAttr_t;

status_t switch_onOffCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);

// OnOffSwitchConfig
typedef struct{
	u8 switchMode;
	u8 switchAction;
	u8 longAction;
}zcl_onOffSwitchCfgAttr_t;

status_t zcl_onOffSwitchCfg_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);
status_t switch_onOffSwitchCfgCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);

#endif