#ifndef _SM_UI_CBK_H_
#define _SM_UI_CBK_H_


void smui_cbk_net_created(bool success);
void smui_cbk_net_joint(bool success);
void smui_cbk_back_to_net(bool success);
void smui_cbk_attention(uint8_t duration);
void smui_cbk_key_updated(uint8_t key_type, uint16_t idx);
void smui_cbk_indcate_auth_value(uint8_t auth_type, uint8_t auth_act, uint8_t size, uint8_t* data);
#if (SM_PROVISIONER_SUPPORT)
void smui_cbk_new_device_detected(sm_uuid_t* uuid, sm_bdaddr_t* bd, bool pb_flag,
                                                    uint8_t* ext_data, uint8_t ext_data_len, uint8_t rssi);
void smui_cbk_new_device(bool success, sm_addr_t dev_addr, sm_uuid_t* uuid);
void smui_cbk_read_pub_key_oob(void);
#endif

#endif
