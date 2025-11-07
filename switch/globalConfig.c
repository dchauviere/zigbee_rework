#include "tl_common.h"
#include "globalConfig.h"

global_config_t g_globalConfig;

/*********************************************************************
 * @fn      saveGlobalConfig
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t saveGlobalConfig(void)
{
	nv_sts_t st = NV_SUCC;
	bool changed = false;

	global_config_t app_globalConfig;

	st = nv_flashReadNew(1, NV_MODULE_ZCL, NV_ITEM_APP_GLOBAL_CONFIG, sizeof(app_globalConfig), (u8*)&app_globalConfig);

	if(st == NV_SUCC){
		if((app_globalConfig.actionHoldStep != g_globalConfig.actionHoldStep) 
            || (app_globalConfig.actionHoldThreshold != g_globalConfig.actionHoldThreshold)
            || (app_globalConfig.actionHoldTransition != g_globalConfig.actionHoldTransition)
            || (app_globalConfig.identifyDuration != g_globalConfig.identifyDuration)
            || (app_globalConfig.resetDuration != g_globalConfig.resetDuration)
			|| (app_globalConfig.actionclickTransition != g_globalConfig.actionclickTransition)
            ){
			changed = true;
		}
	}

	if (changed == true || st == NV_ITEM_NOT_FOUND) {
		st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_APP_GLOBAL_CONFIG, sizeof(app_globalConfig), (u8*)&g_globalConfig);
	}

	return st;
}

/*********************************************************************
 * @fn      restoreGlobalConfig
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t restoreGlobalConfig(void)
{
	nv_sts_t st = NV_SUCC;

	global_config_t app_globalConfig;

	st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_APP_GLOBAL_CONFIG, sizeof(app_globalConfig), (u8*)&app_globalConfig);

	if(st == NV_SUCC){
        g_globalConfig.actionclickTransition = app_globalConfig.actionclickTransition;
        g_globalConfig.actionHoldStep = app_globalConfig.actionHoldStep;
        g_globalConfig.actionHoldThreshold = app_globalConfig.actionHoldThreshold;
        g_globalConfig.actionHoldTransition = app_globalConfig.actionHoldTransition;
        g_globalConfig.identifyDuration = app_globalConfig.identifyDuration;
        g_globalConfig.resetDuration = app_globalConfig.resetDuration;
	}else{
		g_globalConfig.actionclickTransition = GLOBAL_DEFAULT_ACTION_CLICK_TRANSITION;
        g_globalConfig.actionHoldStep = GLOBAL_DEFAULT_ACTION_HOLD_STEP;
        g_globalConfig.actionHoldThreshold = GLOBAL_DEFAULT_ACTION_HOLD_THRESHOLD;
        g_globalConfig.actionHoldTransition = GLOBAL_DEFAULT_ACTION_HOLD_TRANSITION;
        g_globalConfig.identifyDuration = GLOBAL_DEFAULT_IDENTIFY_DURATION;
        g_globalConfig.resetDuration = GLOBAL_DEFAULT_RESET_DURATION;
	}

	return st;
}
