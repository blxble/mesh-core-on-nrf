#ifndef _NRF_ADMIN_H_
#define _NRF_ADMIN_H_

void nrf_admin_init(void);
void nrf_admin_new_device_detected(smui_new_device_detected_param_t* dev_det);
void nrf_admin_device_added(smui_new_device_added_param_t* dev_added);
void nrf_admin_config_done(smui_config_done_param_t* cfg_done);

#endif

