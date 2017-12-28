#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "sm_config.h"
#include "sm_common.h"
#include "sm_port.h"
#include "sm_port_evt.h"
#include "sm_ui.h"
#if (SM_PORT_PLATFORM == SM_PORT_NORDIC)
#include "sdk_config.h"
#include "pca10040.h"
#include "nrf.h"
#include "ble.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "softdevice_handler.h"
#include "nrf_demo_config.h"
#include "nrf_ipc.h"
#include "nrf_admin.h"
#include "nrf_light.h"
#include "nrf_switch.h"

#define NRF_INDICATOR_PIN       (31)

extern void nrf_on_ble_gap_evt(ble_evt_t * ble_evt);
extern void nrf_provc_on_ble_evt(ble_evt_t* ble_evt);
extern void nrf_provs_on_ble_evt(ble_evt_t* ble_evt);
extern void nrf_proxyc_on_ble_evt(ble_evt_t* ble_evt);
extern void nrf_proxys_on_ble_evt(ble_evt_t* ble_evt);

bool g_nrf_main_joint_net = false;

static void _nrf_ble_evt_dispatch(ble_evt_t * ble_evt)
{
    nrf_on_ble_gap_evt(ble_evt);
#if (SM_PB_GATT_SUPPORT)
#if (NRF_DEMO_ADMIN_SUPPORT)
    if (nrf_admin_get_stage() == NRF_ADMIN_STAGE_PROVISION)
    {
        nrf_provc_on_ble_evt(ble_evt);
    }
#else
    if (g_nrf_main_joint_net == false)
    {
        nrf_provs_on_ble_evt(ble_evt);
    }
#endif
#endif

#if (NRF_DEMO_ADMIN_SUPPORT)
#if (SM_PROXY_GATT_CLIENT)
    if (nrf_admin_get_stage() == NRF_ADMIN_STAGE_CONFIG)
    {
        nrf_proxyc_on_ble_evt(ble_evt);
    }
#endif
#else
#if (SM_PROXY_GATT_SERVER)
    if (g_nrf_main_joint_net == true)
    {
        nrf_proxys_on_ble_evt(ble_evt);
    }
#endif
#endif
}

static void _nrf_mesh_attention(smui_attention_param_t* param)
{
    if (param->duration > 0)
    {
        nrf_gpio_pin_write(NRF_INDICATOR_PIN, 1);
    }
}

static void _nrf_mesh_ui_cbk(smui_notice_t notice, void* param)
{
    switch (notice)
    {
#if (NRF_DEMO_ADMIN_SUPPORT)
    case SMUI_NOTICE_NET_CREATED:
        break;
    case SMUI_NOTICE_NEW_DEVICE_DETECTED:
        nrf_admin_new_device_detected((smui_new_device_detected_param_t*)param);
        break;
    case SMUI_NOTICE_NEW_DEVICE_ADDED:
        nrf_admin_device_added((smui_new_device_added_param_t*)param);
        break;
    case SMUI_NOTICE_CONFIG_DONE:
        nrf_admin_config_done((smui_config_done_param_t*)param);
        break;
#endif
    case SMUI_NOTICE_ATTENTION:
        _nrf_mesh_attention((smui_attention_param_t*)param);
        break;
    case SMUI_NOTICE_NET_JOINT:
        if (((smui_net_joint_param_t*)param)->success)
        {
            g_nrf_main_joint_net = true;
            nrf_gpio_pin_write(NRF_INDICATOR_PIN, 0);
            nrf_delay_ms(60);
            nrf_gpio_pin_write(NRF_INDICATOR_PIN, 1);
            nrf_delay_ms(60);
            nrf_gpio_pin_write(NRF_INDICATOR_PIN, 0);
        }
        else
        {
            smui_join_net(0, 0);
        }
        break;
    case SMUI_NOTICE_SEC_KEY_UPDATED:
        if (((smui_key_updated_param_t*)param)->key_type == SMUI_KEYTYPE_NETKEY)
        {
#if (NRF_DEMO_ADMIN_SUPPORT == 0)
            NRF_LOG_INFO("set proxy server\n\r");
            smui_set_proxy_server(true, ((smui_key_updated_param_t*)param)->idx);
#endif
        }
        break;
    case SMUI_NOTICE_PROXY_STATUS:
        NRF_LOG_INFO("Proxy status = %d\n\r", ((smui_proxy_status_param_t*)param)->status);
#if (NRF_DEMO_ADMIN_SUPPORT)
        nrf_admin_proxy_status((smui_proxy_status_param_t*)param);
#endif
        break;
    }
}

static void _nrf_ble_stack_init(void)
{
    uint32_t err_code;

    // Initialize the SoftDevice handler library.
    nrf_clock_lf_cfg_t clock_lf_cfg = NRF_CLOCK_LFCLKSRC;
    
    SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, NULL);
    
    // Retrieve default BLE stack configuration parameters.
    ble_enable_params_t ble_enable_params;

    ble_enable_params.common_enable_params.vs_uuid_count = 0;
    ble_enable_params.common_enable_params.p_conn_bw_counts = NULL;
    ble_enable_params.gap_enable_params.periph_conn_count = 1;
    ble_enable_params.gap_enable_params.central_conn_count = 2;
    ble_enable_params.gap_enable_params.central_sec_count = 0;
    ble_enable_params.gap_enable_params.p_device_name = NULL;
    ble_enable_params.gatt_enable_params.att_mtu = 69;
    ble_enable_params.gatts_enable_params.service_changed = 0;
    ble_enable_params.gatts_enable_params.attr_tab_size = 1024;
    // Enable BLE stack.
    err_code = softdevice_enable(&ble_enable_params);
    NRF_LOG_INFO("stack init, err=0x%X\n\r",err_code);

    // Register a BLE event handler with the SoftDevice handler library.
    err_code = softdevice_ble_evt_handler_set(_nrf_ble_evt_dispatch);
}

void main(void)
{
    NRF_LOG_INIT(NULL);
    
    nrf_mem_init();

    nrf_gpio_cfg_output(NRF_INDICATOR_PIN);
    nrf_gpio_pin_write(NRF_INDICATOR_PIN, 1);

    _nrf_ble_stack_init();

    nrf_ipc_init();
    
    smport_platform_init();
    smui_reg_callback(_nrf_mesh_ui_cbk);
    smui_init();
#if (NRF_DEMO_SWITCH_SUPPORT)
    nrf_switch_init();
#endif
#if (NRF_DEMO_LIGHT_SUPPORT)
    nrf_light_init();
#endif
#if (NRF_DEMO_ADMIN_SUPPORT)
    nrf_admin_init();
#else
    smui_join_net(0, 0);
#endif
    
    for (;;)
    {
        nrf_ipc_schedule();
        
        smport_platform_schedule();

        NRF_LOG_PROCESS();
        
        //sd_app_evt_wait();
    }
}

#endif

