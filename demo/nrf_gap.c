#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "sm_config.h"
#include "sm_common.h"
#include "sm_port_evt.h"
#include "sm_port.h"

#if (SM_PORT_PLATFORM == SM_PORT_NORDIC)

#include "sdk_config.h"
#include "nrf.h"
#include "ble.h"
#include "nordic_common.h"
#include "nrf_soc.h"
#include "ble_advdata.h"

uint16_t g_nrf_conn_hdl;
sm_bdaddr_t g_nrf_peer_bd;

static void _nrf_on_ble_gap_evt_adv_report(ble_gap_evt_t const * gap_evt)
{
    ble_gap_evt_adv_report_t* rpt = (ble_gap_evt_adv_report_t*)&gap_evt->params.adv_report;
    sm_bdaddr_t bd;

    bd.addr_type = rpt->peer_addr.addr_type;
    memcpy(bd.addr, rpt->peer_addr.addr, BLE_GAP_ADDR_LEN);
    
    smport_evt_adv_report(&bd, rpt->type, rpt->data, rpt->dlen, rpt->rssi);
}


void nrf_on_ble_gap_evt(ble_evt_t * ble_evt)
{
    uint32_t              err_code;
    ble_gap_evt_t* gap_evt = &ble_evt->evt.gap_evt;
    const ble_gap_conn_params_t conn_param =
    {
        .min_conn_interval = (uint16_t)60,   // Minimum connection interval.
        .max_conn_interval = (uint16_t)60,   // Maximum connection interval.
        .slave_latency     = (uint16_t)0,    // Slave latency.
        .conn_sup_timeout  = (uint16_t)100   // Supervisory timeout.
    };

    switch (ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_ADV_REPORT:
            _nrf_on_ble_gap_evt_adv_report(gap_evt);
            break;
        case BLE_GAP_EVT_CONNECTED:
            g_nrf_peer_bd.addr_type = gap_evt->params.connected.peer_addr.addr_type;
            memcpy(g_nrf_peer_bd.addr, gap_evt->params.connected.peer_addr.addr, BLE_GAP_ADDR_LEN);

            sd_ble_gap_conn_param_update(g_nrf_conn_hdl, &conn_param);
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            break;
        case BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST:
            break;
        default:
            break;
    }
}

void smport_start_advertise(uint8_t adv_type, uint16_t intvl_min, uint16_t intval_max, uint8_t chan_map,
                                    uint8_t* data, uint8_t data_len)
{
    uint32_t err;
    
    ble_gap_adv_params_t adv_params =
    {
        .type         = adv_type,
        .p_peer_addr  = NULL,
        .fp           = BLE_GAP_ADV_FP_ANY,
        .interval     = intvl_min,
        .timeout      = 0,
        .channel_mask = chan_map,
    };
    
    err = sd_ble_gap_adv_data_set(data, data_len, NULL, 0);
    err = sd_ble_gap_adv_start(&adv_params);
}

void smport_stop_advertise(void)
{
    sd_ble_gap_adv_stop();
}

void smport_start_scan(uint16_t intvl, uint16_t win)
{
    ble_gap_scan_params_t scan_params;
    scan_params.active = 0;
    scan_params.use_whitelist = 0;
    scan_params.adv_dir_report = 0;
    scan_params.interval = intvl;
    scan_params.window = win;
    scan_params.timeout = 0;
    
    sd_ble_gap_scan_start(&scan_params);
}

void smport_stop_scan(void)
{
    sd_ble_gap_scan_stop();
}

void smport_aes_encrypt(uint8_t* key, uint8_t* clear_text)
{
    nrf_ecb_hal_data_t* ecb_data;

    ecb_data = smport_malloc(sizeof(nrf_ecb_hal_data_t), SM_MEM_NON_RETENTION);
    memcpy(&ecb_data->key, key, SOC_ECB_KEY_LENGTH);
    memcpy(&ecb_data->cleartext, clear_text, SOC_ECB_CLEARTEXT_LENGTH);

    sd_ecb_block_encrypt(ecb_data);

    smport_evt_encrypt_result((uint8_t*)&ecb_data->ciphertext);

    smport_free(ecb_data);
}

void smport_gatt_advertise(uint16_t svc_uuid, uint8_t* svc_data, uint8_t data_len)
{
    uint8_t* adv_data;
    uint8_t len = sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint16_t) + 
                            sizeof(uint8_t) + sizeof(uint8_t) + + sizeof(uint16_t) + data_len;
    adv_data = smport_malloc(data_len, SM_MEM_NON_RETENTION);

    adv_data[0] = 3;
    adv_data[1] = 0x03;     // comlete 16bit-uuid
    adv_data[2] = (uint8_t)(svc_uuid & 0x00FF);
    adv_data[3] = (uint8_t)(svc_uuid >> 8);
    adv_data[4] = 3 + data_len;
    adv_data[5] = 0x16;
    adv_data[6] = (uint8_t)(svc_uuid & 0x00FF);
    adv_data[7] = (uint8_t)(svc_uuid >> 8);
    memcpy(&adv_data[8], svc_data, data_len);

    smport_start_advertise(BLE_GAP_ADV_TYPE_ADV_IND, 0xA0, 0xA0, 0x07, adv_data, len);

    smport_free(adv_data);
}

void smport_gatt_create_connect(sm_bdaddr_t* bd, uint16_t gatt_svc_uuid)
{
    ble_gap_addr_t addr;
    ble_gap_scan_params_t const scan_param =
    {
        .active         = 0x00,
        .use_whitelist  = 0x00,
        .adv_dir_report = 0,
        .interval       = SM_CONF_PBGATT_SCAN_INTVL,
        .window         = SM_CONF_PBGATT_SCAN_WIN,
        .timeout        = 0x0000,
    };

    ble_gap_conn_params_t const conn_param =
    {
        .min_conn_interval = 0x6,
        .max_conn_interval = 0x6,
        .slave_latency     = 0,
        .conn_sup_timeout  = SM_CONF_PBGATT_SUPERVISION,
    };

    addr.addr_type = bd->addr_type;
    memcpy(addr.addr, bd->addr, BLE_GAP_ADDR_LEN);

    sd_ble_gap_connect(&addr, &scan_param, &conn_param);
}

void smport_gatt_disconnect(uint16_t conn_hdl, uint8_t reason, uint16_t gatt_svc_uuid)
{
    sd_ble_gap_disconnect(conn_hdl, reason);
}

#endif

