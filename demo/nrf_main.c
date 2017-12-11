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
#include "nrf_light.h"
#include "nrf_switch.h"

#define NRF_INDICATOR_PIN       (31)

extern void nrf_on_ble_gap_evt(ble_evt_t * ble_evt);
extern void nrf_provc_on_ble_evt(ble_evt_t* ble_evt);
extern void nrf_provs_on_ble_evt(ble_evt_t* ble_evt);
extern void nrf_proxyc_on_ble_evt(ble_evt_t* ble_evt);
extern void nrf_proxys_on_ble_evt(ble_evt_t* ble_evt);

static void nrf_ble_evt_dispatch(ble_evt_t * ble_evt)
{
    nrf_on_ble_gap_evt(ble_evt);
#if (SM_PB_GATT_SUPPORT)
#if (NRF_DEMO_ADMIN_SUPPORT)
    nrf_provc_on_ble_evt(ble_evt);
#else
    nrf_provs_on_ble_evt(ble_evt);
#endif
#endif
#if (SM_PROXY_GATT_CLIENT)
    nrf_proxyc_on_ble_evt(ble_evt);
#endif
#if (SM_PROXY_GATT_SERVER)
    nrf_proxys_on_ble_evt(ble_evt);
#endif
}

static void nrf_mesh_ui_cbk(smui_notice_t notice, void* param)
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
#else
    case SMUI_NOTICE_NET_JOINT:
        if (((smui_net_joint_param_t*)param)->success)
        {
            smui_enable_proxy();
        }

        // twinkle
        nrf_gpio_cfg_output(NRF_INDICATOR_PIN);
        nrf_gpio_pin_write(NRF_INDICATOR_PIN, 1);
        nrf_delay_ms(100);
        nrf_gpio_pin_write(NRF_INDICATOR_PIN, 0);
        nrf_delay_ms(100);
        nrf_gpio_pin_write(NRF_INDICATOR_PIN, 1);
        nrf_delay_ms(100);
        nrf_gpio_pin_write(NRF_INDICATOR_PIN, 0);
        break;
#endif
    }
}

static void nrf_ble_stack_init(void)
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
    ble_enable_params.gap_enable_params.central_conn_count = 1;
    ble_enable_params.gap_enable_params.central_sec_count = 0;
    ble_enable_params.gap_enable_params.p_device_name = NULL;
    ble_enable_params.gatt_enable_params.att_mtu = 69;
    ble_enable_params.gatts_enable_params.service_changed = 0;
    ble_enable_params.gatts_enable_params.attr_tab_size = 1024;
    // Enable BLE stack.
    err_code = softdevice_enable(&ble_enable_params);

    // Register a BLE event handler with the SoftDevice handler library.
    err_code = softdevice_ble_evt_handler_set(nrf_ble_evt_dispatch);
}

void main(void)
{
    NRF_LOG_INIT(NULL);
    
    nrf_mem_init();
    nrf_ble_stack_init();

    smport_platform_init();
    smui_reg_callback(nrf_mesh_ui_cbk);
    smui_init();
#if (NRF_DEMO_ADMIN_SUPPORT)
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
        smport_platform_schedule();

        NRF_LOG_PROCESS();
        
        //sd_app_evt_wait();
    }
}

#endif

