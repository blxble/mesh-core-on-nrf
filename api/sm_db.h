#ifndef _SM_DB_H_
#define _SM_DB_H_

#define SM_DB_VIRTUAL                       (0)
#define SM_DB_FS                            (1)
#define SM_DB_NVDS                          (2)
#if defined (__ANDROID__)
#define SM_DB_STORAGE                       (SM_DB_VIRTUAL)
#elif (CFG_TEST_API)
#define SM_DB_STORAGE                       (SM_DB_VIRTUAL)
#else
#define SM_DB_STORAGE                       (SM_DB_VIRTUAL)
#endif
#define SMDB_MAX_APPKEY_NUM                 (64)

#define SMDB_KEY_REFRESH_FLAG               (0x80000000)

typedef struct
{
    sm_addr_t                   elt_addr;
    uint8_t                     mid_num;
    uint8_t                     vmid_num;
    sm_mid_t*                   mid_arr;
    sm_vmid_t*                  vmid_arr;
} smdb_element_info_t;

typedef enum smdb_err_tag
{
    SMDB_SUCCESS,
    SMDB_RESOURCE_NOT_FOUND,
    SMDB_FULL,
    SMDB_KEY_IDX_ALREADY_EXIST,
    SMDB_STORAGE_FAILURE,
    SMDB_UPDATE_FAILURE,
    SMDB_REMOVE_FAILURE,
} smdb_err_t;


void smdb_init(void);
void smdb_reset(void);
uint16_t smdb_store_element(uint16_t idx, sm_addr_t elt_addr);
uint16_t smdb_get_element_num(void);
void smdb_store_publication(sm_addr_t elt_addr, sm_vmid_t mid, sm_addr_t pub_addr, uint16_t appkey_idx, uint8_t ttl, uint8_t pub_period);
smdb_err_t smdb_load_publication(sm_addr_t elt_addr, sm_vmid_t mid, sm_addr_t* pub_addr, uint16_t* appkey_idx, uint8_t* ttl, uint8_t* pub_period);
void smdb_store_subscription(sm_addr_t elt_addr, sm_vmid_t mid, sm_addr_t subs_addr);
bool smdb_check_subscription(sm_addr_t elt_addr, sm_addr_t subs_addr);
smdb_err_t smdb_load_subscription(sm_addr_t elt_addr, sm_vmid_t mid, sm_addr_t** subs_list, uint16_t* subs_num);
smdb_err_t smdb_remove_subscription(sm_addr_t elt_addr, sm_vmid_t mid, sm_addr_t subs_addr);
smdb_err_t smdb_remove_subscription_all(sm_addr_t elt_addr, sm_vmid_t mid);
smdb_err_t smdb_store_model_boundkey(sm_addr_t elt_addr, sm_vmid_t mid, uint16_t appkey_idx);
smdb_err_t smdb_load_model_boundkey(sm_addr_t elt_addr, sm_vmid_t mid, uint16_t** appkey_idx_list, uint16_t* boundkey_idx_num);
smdb_err_t smdb_remove_model_boundkey(sm_addr_t elt_addr, sm_vmid_t mid, uint16_t appkey_idx);
bool smdb_check_model_boundkey(sm_addr_t elt_addr, sm_vmid_t mid, uint16_t appkey_idx);
sm_addr_t smdb_load_dev_address(void);
void smdb_store_devkey(uint8_t* dev_key);
uint8_t* smdb_load_devkey(void);
smdb_err_t smdb_store_appkey(uint16_t netkey_idx, uint16_t appkey_idx, uint8_t* app_key, uint8_t aid);
uint8_t* smdb_load_appkey(uint16_t appkey_idx, uint8_t* aid, uint16_t* netkey_idx);
uint8_t* smdb_load_appkey_at_pos(uint16_t pos, uint16_t* appkey_idx, uint8_t* aid, uint16_t* netkey_idx);
uint16_t smdb_get_appkey_num(void);
smdb_err_t smdb_remove_appkey(uint16_t netkey_idx, uint16_t appkey_idx);
bool smdb_update_appkey(uint16_t netkey_idx, uint16_t appkey_idx, uint8_t* app_key, uint8_t aid);
smdb_err_t smdb_get_appkey_idx(uint16_t netkey_idx, uint16_t** appkey_idx_list, uint16_t* idx_num);
smdb_err_t smdb_store_netkey(uint32_t netkey_idx, uint8_t* net_key, uint8_t nid, uint8_t* nwk_id, uint8_t* enc_key, uint8_t* pri_key, uint8_t* bcon_key);
uint8_t* smdb_load_netkey(uint32_t idx, uint8_t* nid, uint8_t** nwk_id, uint8_t** enc_key, uint8_t** pri_key, uint8_t** bcon_key);
uint8_t* smdb_load_netkey_at_pos(uint16_t pos, uint32_t* netkey_idx, uint8_t* nid, uint8_t** nwk_id, uint8_t** enc_key, uint8_t** pri_key, uint8_t** bcon_key);
uint16_t smdb_get_netkey_num(void);
bool smdb_remove_netkey(uint32_t idx);
bool smdb_update_netkey(uint32_t idx, uint8_t* net_key, uint8_t nid, uint8_t* nwk_id, uint8_t* enc_key, uint8_t* pri_key, uint8_t* bcon_key);
void smdb_revoke_netkey(uint32_t idx);
smdb_err_t smdb_store_kr_netkey(uint32_t netkey_idx, uint8_t* net_key, uint8_t nid, uint8_t* nwk_id, uint8_t* enc_key, uint8_t* pri_key, uint8_t* bcon_key);
bool smdb_update_kr_netkey(uint32_t idx, uint8_t* net_key, uint8_t nid, uint8_t* nwk_id, uint8_t* enc_key, uint8_t* pri_key, uint8_t* bcon_key);
uint8_t* smdb_load_kr_netkey(uint32_t idx, uint8_t* nid, uint8_t** nwk_id, uint8_t** enc_key, uint8_t** pri_key, uint8_t** bcon_key);
void smdb_set_primary_netkey_idx(uint16_t idx);
uint16_t smdb_get_primary_netkey_idx(void);
void smdb_store_seq(uint32_t seq);
uint32_t smdb_load_seq(void);
void smdb_update_ivi(sm_ivi_t new_ivi);
sm_ivi_t smdb_get_ivi(void);
uint8_t smdb_load_attention_timer(void);
void smdb_store_attention_timer(uint8_t att_timer);
uint8_t smdb_load_default_ttl(void);
void smdb_store_default_ttl(uint8_t ttl);
#if (SM_PROVISIONER_SUPPORT)
void smdb_store_allocated_address(sm_addr_t addr_alloc);
sm_addr_t smdb_load_allocated_address(void);
void smdb_store_provisioned_node(sm_uuid_t* uuid, sm_addr_t dev_addr, uint8_t* dev_key);
void smdb_remove_provisioned_node(sm_addr_t dev_addr);
sm_uuid_t* smdb_get_node_uuid_by_dev_addr(sm_addr_t dev_addr);
sm_addr_t smdb_get_node_dev_addr_by_uuid(sm_uuid_t* uuid);
sm_addr_t smdb_get_node_dev_addr_by_idx(uint16_t idx);
uint16_t smdb_get_provisioned_node_number(void);
void smdb_store_element_info(sm_addr_t dev_addr, sm_addr_t elt_addr, uint8_t mid_num, uint8_t vmid_num,
                                             sm_mid_t* mid_arr, sm_vmid_t* vmid_arr);
uint16_t smdb_get_element_info_num(sm_addr_t dev_addr);
smdb_element_info_t* smdb_load_element_info_by_idx(sm_addr_t dev_addr, uint8_t elt_idx);
uint8_t* smdb_load_node_devkey(sm_addr_t dev_addr);
#endif
#endif
