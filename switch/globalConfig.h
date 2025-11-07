#ifndef __GLOBAL_CONFIG_H__
#define __GLOBAL_CONFIG_H__

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
