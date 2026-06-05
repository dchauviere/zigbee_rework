#include "tl_common.h"
#include "zclApp.h"

epconfig_attr_t g_epConfigAttrs[BUTTON_NUM];


/*********************************************************************
 * @fn      saveEPConfig
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t saveEPConfig(u8 endpoint)
{
	nv_sts_t st = NV_SUCC;
	bool changed = false;

  epconfig_attr_t l_epConfigAttr;  

	st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_APP_EPCONFIG_BASE + endpoint, sizeof(epconfig_attr_t), (u8*)&l_epConfigAttr);

	if(st == NV_SUCC){
		if((l_epConfigAttr.simpleClickDevice != g_epConfigAttrs[endpoint].simpleClickDevice) || (l_epConfigAttr.doubleClickDevice != g_epConfigAttrs[endpoint].doubleClickDevice) ||
		   	(l_epConfigAttr.longPressDevice != g_epConfigAttrs[endpoint].longPressDevice) || (l_epConfigAttr.relayMode != g_epConfigAttrs[endpoint].relayMode) ||
			(l_epConfigAttr.backlightMode != g_epConfigAttrs[endpoint].backlightMode) || (l_epConfigAttr.simpleClickDeviceEp != g_epConfigAttrs[endpoint].simpleClickDeviceEp) ||
			(l_epConfigAttr.doubleClickDeviceEp != g_epConfigAttrs[endpoint].doubleClickDeviceEp) || (l_epConfigAttr.longPressDeviceEp != g_epConfigAttrs[endpoint].longPressDeviceEp)) {
			changed = true;
		}
	}

	if (changed == true || st == NV_ITEM_NOT_FOUND) {
		st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_APP_EPCONFIG_BASE + endpoint, sizeof(epconfig_attr_t), (u8*)&g_epConfigAttrs[endpoint]);
	}

	return st;
}

/*********************************************************************
 * @fn      restoreEPConfig
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t restoreEPConfig(u8 endpoint)
{
	nv_sts_t st = NV_SUCC;

  epconfig_attr_t l_epConfigAttr;

	st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_APP_EPCONFIG_BASE + endpoint, sizeof(epconfig_attr_t), (u8*)&l_epConfigAttr);

	if(st == NV_SUCC){
		g_epConfigAttrs[endpoint].simpleClickDevice		= l_epConfigAttr.simpleClickDevice;
		g_epConfigAttrs[endpoint].simpleClickDeviceEp	= l_epConfigAttr.simpleClickDeviceEp;
		g_epConfigAttrs[endpoint].doubleClickDevice		= l_epConfigAttr.doubleClickDevice;
		g_epConfigAttrs[endpoint].doubleClickDeviceEp	= l_epConfigAttr.doubleClickDeviceEp;
		g_epConfigAttrs[endpoint].longPressDevice			= l_epConfigAttr.longPressDevice;
		g_epConfigAttrs[endpoint].longPressDeviceEp		= l_epConfigAttr.longPressDeviceEp;
		g_epConfigAttrs[endpoint].relayMode				    = l_epConfigAttr.relayMode;
		g_epConfigAttrs[endpoint].backlightMode			  = l_epConfigAttr.backlightMode;
	}else{
		g_epConfigAttrs[endpoint].simpleClickDevice		= 0xFFFE;
		g_epConfigAttrs[endpoint].simpleClickDeviceEp	= 0x01;
		g_epConfigAttrs[endpoint].doubleClickDevice		= 0xFFFE;
		g_epConfigAttrs[endpoint].doubleClickDeviceEp	= 0x01;
		g_epConfigAttrs[endpoint].longPressDevice			= 0xFFFE;
		g_epConfigAttrs[endpoint].longPressDeviceEp		= 0x01;
		g_epConfigAttrs[endpoint].relayMode				    = ZCL_EPCONFIG_RELAY_MODE_ATTACHED;
		g_epConfigAttrs[endpoint].backlightMode			  = ZCL_EPCONFIG_BACKLIGHT_MODE_ONOFF;
  	st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_APP_EPCONFIG_BASE + endpoint, sizeof(epconfig_attr_t), (u8*)&g_epConfigAttrs[endpoint]);
	}

	return st;
}
