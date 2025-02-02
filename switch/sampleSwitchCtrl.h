#ifndef _SAMPLE_SWITCH_CTRL_H_
#define _SAMPLE_SWITCH_CTRL_H_


/**********************************************************************
 * TYPEDEFS
 */
typedef enum
{
	SWITCH_STA_ON_OFF,
}switchSta_e;

/**********************************************************************
 * FUNCTIONS
 */
void switch_init(void);
void switch_adjust(void);
void switch_refresh(u8 btn, switchSta_e);

/*
void light_blink_start(u8 times, u16 ledOnTime, u16 ledOffTime);
void light_blink_stop(void);
*/

#endif	/* _SAMPLE_SWITCH_CTRL_H_ */
