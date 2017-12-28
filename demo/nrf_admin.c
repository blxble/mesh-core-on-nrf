#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "sm_config.h"
#include "sm_common.h"
#include "sm_port.h"
#include "sm_acc_msg.h"
#include "sm_acc.h"
#include "sm_mod_msg.h"
#include "sm_ui.h"
#include "sm_db.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_admin.h"
#include "nrf_switch.h"
#include "nrf_light.h"

#define NRF_ADMIN_SET_PROXY_TRY             (5)

typedef struct
{
    bool                    idle;
    sm_addr_t               dev_addr;
    sm_bdaddr_t             dev_bd;
    uint16_t                elt_idx;
    smdb_element_info_t*    elt_info;
    uint8_t                 proxy_try;
    nrf_admin_stage_t       stage;
} nrf_admin_env_t;

nrf_admin_env_t g_nrf_admin_env;

void nrf_admin_init(void)
{
    sm_memset(&g_nrf_admin_env, 0x00, sizeof(g_nrf_admin_env));
    smui_create_net();
}

void nrf_admin_new_device_detected(smui_new_device_detected_param_t* dev_det)
{
    if (g_nrf_admin_env.idle == true && dev_det->uuid.uuid[0] == 0x78)
    {
        NRF_LOG_INFO("nrf_admin_new_device_detected\n\r");
        g_nrf_admin_env.idle = false;
        g_nrf_admin_env.stage = NRF_ADMIN_STAGE_PROVISION;
        memcpy((uint8_t*)&g_nrf_admin_env.dev_bd, (uint8_t*)&dev_det->bd, sizeof(sm_bdaddr_t));
        smui_grant_new_proposer(&dev_det->uuid, &dev_det->bd, dev_det->pb_flag);
    }
}

void nrf_admin_device_added(smui_new_device_added_param_t* dev_added)
{
    if (dev_added->success == true)
    {
        NRF_LOG_INFO("nrf_admin_device_added\n\r");
        g_nrf_admin_env.dev_addr = dev_added->dev_addr;
        g_nrf_admin_env.stage = NRF_ADMIN_STAGE_CONFIG;

        smui_get_device_composition_data(dev_added->dev_addr);
        if (dev_added->dev_addr > 1)
        {
            smui_set_proxy_client(true, 0, SMUI_PROXY_CLI_POLICY_DEDICATED, 200, &g_nrf_admin_env.dev_bd);
            g_nrf_admin_env.proxy_try = NRF_ADMIN_SET_PROXY_TRY;
        }
    }
    else
    {
        g_nrf_admin_env.idle = true;
        g_nrf_admin_env.stage = NRF_ADMIN_STAGE_NONE;
        NRF_LOG_INFO("nrf_admin_device_added failed\n\r");
    }
}

void nrf_admin_proxy_status(smui_proxy_status_param_t* proxy_status)
{
    g_nrf_admin_env.proxy_try--;
    
    if (proxy_status->status == SMUI_PROXY_STATUS_TIMEOUT)
    {
        if (g_nrf_admin_env.proxy_try > 0)
        {
            smui_set_proxy_client(true, 0, SMUI_PROXY_CLI_POLICY_DEDICATED, 200, &g_nrf_admin_env.dev_bd);
        }
    }
}

void nrf_admin_config_done(smui_config_done_param_t* cfg_done)
{
    sm_addr_t pub_addr, subs_addr;

    NRF_LOG_INFO("config_done, success = %d, conf_op=%d\n\r", cfg_done->success, cfg_done->conf_op);

    if (cfg_done->success == false)
    {
        if (g_nrf_admin_env.elt_info != NULL)
        {
            if (g_nrf_admin_env.elt_info->mid_arr != NULL)
            {
                smport_free(g_nrf_admin_env.elt_info->mid_arr);
            }
            if (g_nrf_admin_env.elt_info->vmid_arr != NULL)
            {
                smport_free(g_nrf_admin_env.elt_info->vmid_arr);
            }
            smport_free(g_nrf_admin_env.elt_info);
            g_nrf_admin_env.elt_info = NULL;
        }
        
        g_nrf_admin_env.idle = true;
        g_nrf_admin_env.stage = NRF_ADMIN_STAGE_NONE;
        return;
    }
    
    switch (cfg_done->conf_op)
    {
        case SMUI_CONF_OP_GET_COMPOSITION_DATA:
            g_nrf_admin_env.elt_info = smdb_load_element_info_by_idx(cfg_done->addr, 1);
            g_nrf_admin_env.elt_idx = 1;
            
            smui_add_device_appkey(cfg_done->addr, 0, 0);
            break;
        case SMUI_CONF_OP_ADD_APPKEY:
            if (g_nrf_admin_env.elt_info->mid_arr[0] == NRF_SWITCH_MID)
            {
                pub_addr = 0x8000;
            }
            else if (g_nrf_admin_env.elt_info->mid_arr[0] == NRF_LIGHT_MID)
            {
                pub_addr = 0x8001;
            }
                
            smui_set_device_mod_publication(cfg_done->addr, g_nrf_admin_env.elt_info->elt_addr, 
                                            SMACC_MSG_MID2VMID(g_nrf_admin_env.elt_info->mid_arr[0]), 
                                            pub_addr, 0, 5);
            break;
        case SMUI_CONF_OP_SET_MOD_PUBLICATION:
            if (g_nrf_admin_env.elt_info->mid_arr[0] == NRF_SWITCH_MID)
            {
                subs_addr = 0x8001;
            }
            else if (g_nrf_admin_env.elt_info->mid_arr[0] == NRF_LIGHT_MID)
            {
                subs_addr = 0x8000;
            }
            smui_add_device_mod_subscription(cfg_done->addr, g_nrf_admin_env.elt_info->elt_addr, 
                                             SMACC_MSG_MID2VMID(g_nrf_admin_env.elt_info->mid_arr[0]), 
                                             subs_addr);
            break;
        case SMUI_CONF_OP_ADD_MOD_SUBSCRIPTION:
            smui_bind_device_mod_appkey(cfg_done->addr, g_nrf_admin_env.elt_info->elt_addr, 
                                        SMACC_MSG_MID2VMID(g_nrf_admin_env.elt_info->mid_arr[0]), 0);
            break;
        case SMUI_CONF_OP_BIND_MOD_APPKEY:
            if (smdb_get_element_info_num(cfg_done->addr) > (++g_nrf_admin_env.elt_idx))
            {
                if (g_nrf_admin_env.elt_info->mid_arr != NULL)
                {
                    smport_free(g_nrf_admin_env.elt_info->mid_arr);
                }
                if (g_nrf_admin_env.elt_info->vmid_arr != NULL)
                {
                    smport_free(g_nrf_admin_env.elt_info->vmid_arr);
                }
                smport_free(g_nrf_admin_env.elt_info);
                g_nrf_admin_env.elt_info = smdb_load_element_info_by_idx(cfg_done->addr, g_nrf_admin_env.elt_idx);

                if (g_nrf_admin_env.elt_info->mid_arr[0] == NRF_SWITCH_MID)
                {
                    pub_addr = 0x8000;
                }
                else if (g_nrf_admin_env.elt_info->mid_arr[0] == NRF_LIGHT_MID)
                {
                    pub_addr = 0x8001;
                }
                smui_set_device_mod_publication(cfg_done->addr, g_nrf_admin_env.elt_info->elt_addr, 
                                                SMACC_MSG_MID2VMID(g_nrf_admin_env.elt_info->mid_arr[0]), 
                                                pub_addr, 0, 5);
            }
            else
            {
                if (g_nrf_admin_env.elt_info->mid_arr != NULL)
                {
                    smport_free(g_nrf_admin_env.elt_info->mid_arr);
                }
                if (g_nrf_admin_env.elt_info->vmid_arr != NULL)
                {
                    smport_free(g_nrf_admin_env.elt_info->vmid_arr);
                }
                smport_free(g_nrf_admin_env.elt_info);
                g_nrf_admin_env.elt_info = NULL;

                smui_set_device_proxy_state(cfg_done->addr, SMUI_PROXY_STATE_DISABLE);
            }
            break;
        case SMUI_CONF_OP_SET_PROXY:
            g_nrf_admin_env.idle = true;
            g_nrf_admin_env.stage = NRF_ADMIN_STAGE_NONE;

            smui_set_proxy_client(false, 0, SMUI_PROXY_CLI_POLICY_DEDICATED, 200, &g_nrf_admin_env.dev_bd);
            break;
    }
}

nrf_admin_stage_t nrf_admin_get_stage(void)
{
    return g_nrf_admin_env.stage;
}

