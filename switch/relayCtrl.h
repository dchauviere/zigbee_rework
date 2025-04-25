#ifndef _RELAY_CTRL_H_
#define _RELAY_CTRL_H_

#define NV_ITEM_APP_ON_OFF_SWITCH_CFG_BASE 	0x40
#define NV_ITEM_APP_ON_OFF_BASE 			0x45

/**********************************************************************
 * FUNCTIONS
 */
void refreshRelay(u8 relay);
void setRelay(u8 relay, u8 cmd);
void setRelayOffWithEffectProcess(u8 relay, zcl_onoff_offWithEffectCmd_t *cmd);
void setRelayOnWithRecallGlobalSceneProcess(u8 relay);
void setRelayOnWithTimedOffProcess(u8 relay, zcl_onoff_onWithTimeOffCmd_t *cmd);
void saveRelayAll(void);
void restoreRelayAll(void);
nv_sts_t saveRelayState(u8 relay);
nv_sts_t restoreRelayState(u8 relay);
nv_sts_t saveRelayConfig(u8 relay);
nv_sts_t restoreRelayConfig(u8 relay);

#endif	/* _RELAY_CTRL_H_ */
