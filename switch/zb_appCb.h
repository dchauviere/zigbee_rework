#ifndef _ZB_APP_CB_H_
#define _ZB_APP_CB_H_

ota_callBack_t switch_otaCb;

void switch_otaProcessMsgHandler(u8 evt, u8 status);
void switch_leaveCnfHandler(nlme_leave_cnf_t *pLeaveCnf);
void switch_leaveIndHandler(nlme_leave_ind_t *pLeaveInd);

#endif