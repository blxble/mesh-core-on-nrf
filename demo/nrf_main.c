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
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "softdevice_handler.h"
#include "nrf_light.h"

extern void nrf_on_ble_gap_evt(ble_evt_t * ble_evt);
extern void nrf_provc_on_ble_evt(ble_evt_t* ble_evt);
extern void nrf_provs_on_ble_evt(ble_evt_t* ble_evt);
extern void nrf_proxyc_on_ble_evt(ble_evt_t* ble_evt);
extern void nrf_proxys_on_ble_evt(ble_evt_t* ble_evt);

static void nrf_ble_evt_dispatch(ble_evt_t * ble_evt)
{
    nrf_on_ble_gap_evt(ble_evt);
    //nrf_provc_on_ble_evt(ble_evt);
    nrf_provs_on_ble_evt(ble_evt);
    //nrf_proxyc_on_ble_evt(ble_evt);
    nrf_proxys_on_ble_evt(ble_evt);
}

static void nrf_mesh_ui_cbk(smui_notice_t notice, void* param)
{
    switch (notice)
    {
    case SMUI_NOTICE_NET_JOINT:
        if (((smui_net_joint_param_t*)param)->success)
        {
            smui_enable_proxy();
        }
        break;
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
    nrf_light_init();
    smui_join_net(0, 0);
    
    for (;;)
    {
        smport_platform_schedule();

        NRF_LOG_PROCESS();
        
        //sd_app_evt_wait();
    }
}

#endif

