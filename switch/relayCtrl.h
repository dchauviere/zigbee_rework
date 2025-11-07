#ifndef _RELAY_CTRL_H_
#define _RELAY_CTRL_H_

#define NV_ITEM_APP_ON_OFF_SWITCH_CFG_BASE 	0x40
#define NV_ITEM_APP_ON_OFF_BASE 			0x45


// OnOff
typedef struct{
	u16	 onTime;
	u16	 offWaitTime;
	u8	 startUpOnOff;
	bool onOff;
	bool globalSceneControl;
	u8   backlightMode;
} relayAttr_t;

extern relayAttr_t g_relayAttr[RELAYS_NUM];


/**********************************************************************
 * FUNCTIONS
 */
void initRelays(void);
void setHardwareRelay(u8 relay, bool state);
void refreshRelay(u8 relay);
void setRelay(u8 relay, u8 cmd);
void setRelayOffWithEffectProcess(u8 relay, zcl_onoff_offWithEffectCmd_t *cmd);
void setRelayOnWithRecallGlobalSceneProcess(u8 relay);
void setRelayOnWithTimedOffProcess(u8 relay, zcl_onoff_onWithTimeOffCmd_t *cmd);
nv_sts_t saveRelayConfig(u8 relay);
nv_sts_t restoreRelayConfig(u8 relay);

#endif	/* _RELAY_CTRL_H_ */
