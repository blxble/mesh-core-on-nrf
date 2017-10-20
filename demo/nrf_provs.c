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
#include "sdk_common.h"
#include "nrf_error.h"
#include "ble_err.h"
#include "nrf_log.h"
#include "nrf.h"
#include "nordic_common.h"
#include "ble_advdata.h"
#include "ble_srv_common.h"

extern uint16_t g_nrf_conn_hdl;
static ble_gatts_char_handles_t g_nrf_provs_char_in_hdl;
static ble_gatts_char_handles_t g_nrf_provs_char_out_hdl;

static void _nrf_provs_on_ble_gap_evt_connected(ble_gap_evt_t const * gap_evt)
{
    ble_gap_evt_connected_t* conn_evt = (ble_gap_evt_connected_t*)&gap_evt->params.connected;
    sm_bdaddr_t bd;
        
    g_nrf_conn_hdl = gap_evt->conn_handle;

    //sd_ble_gap_scan_stop();
    //sd_ble_gap_adv_stop();

    bd.addr_type = conn_evt->peer_addr.addr_type;
    memcpy(bd.addr, conn_evt->peer_addr.addr, BLE_GAP_ADDR_LEN);
    smport_evt_connected(g_nrf_conn_hdl, &bd);
}

static void _nrf_provs_on_ble_gap_evt_disconnected(ble_gap_evt_t const * gap_evt)
{
    smport_evt_disconnected(g_nrf_conn_hdl, gap_evt->params.disconnected.reason);
    
    g_nrf_conn_hdl = BLE_CONN_HANDLE_INVALID;
}

static void _nrf_provs_on_ble_gatt_evt_mtu_request(ble_gatts_evt_t const * gatts_evt)
{
    uint16_t mtu = MESH_PROV_SVC_MTU;

    sd_ble_gatts_exchange_mtu_reply(gatts_evt->conn_handle, mtu);
}


void nrf_provs_on_ble_evt(ble_evt_t* ble_evt)
{
    ble_gap_evt_t* gap_evt;
    ble_gatts_evt_t* gatts_evt;
    ble_gatts_evt_write_t* write_evt;

    switch (ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            gap_evt = &ble_evt->evt.gap_evt;
            _nrf_provs_on_ble_gap_evt_connected(gap_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            gap_evt = &ble_evt->evt.gap_evt;
            _nrf_provs_on_ble_gap_evt_disconnected(gap_evt);
            break;
        case BLE_GATTS_EVT_WRITE:
            write_evt = &ble_evt->evt.gatts_evt.params.write;
            if (write_evt->handle == g_nrf_provs_char_in_hdl.value_handle)
            {
                smport_evt_prov_server_data_in(write_evt->data, write_evt->len);
            }
            break;
        case BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST:
            NRF_LOG_INFO("BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST");
            gatts_evt = &ble_evt->evt.gatts_evt;
            _nrf_provs_on_ble_gatt_evt_mtu_request(gatts_evt);
            break;

        case BLE_EVT_TX_COMPLETE:
            smport_evt_prov_server_sent_complete(true);
            break;
        default:
            break;
    }
}

void smport_pbgatt_add_server(void)
{
    ret_code_t    err_code;
    uint16_t      service_handle;
    ble_uuid_t    ble_uuid;
    ble_add_char_params_t char_params;

    ble_uuid.type = BLE_UUID_TYPE_BLE;
    ble_uuid.uuid = MESH_PROV_SVC_UUID;

    // Add service.
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &service_handle);

    
    memset(&char_params, 0, sizeof(char_params));

    char_params.uuid              = MESH_PROV_CHAR_DATA_IN_VAL_UUID;
    char_params.uuid_type         = 0;
    char_params.max_len           = MESH_PROV_SVC_MTU;
    char_params.char_props.write_wo_resp = 1;
    char_params.write_access      = SEC_OPEN;
    char_params.is_var_len        = 1;

    err_code = characteristic_add(service_handle, &char_params, &g_nrf_provs_char_in_hdl);

    memset(&char_params, 0, sizeof(char_params));
    char_params.uuid              = MESH_PROV_CHAR_DATA_OUT_VAL_UUID;
    char_params.uuid_type         = 0;
    char_params.max_len           = MESH_PROV_SVC_MTU;
    char_params.char_props.notify = 1;
    char_params.cccd_write_access = SEC_OPEN;
    char_params.is_var_len        = 1;

    err_code = characteristic_add(service_handle, &char_params, &g_nrf_provs_char_out_hdl);
}

void smport_pbgatt_server_send_pdu(uint8_t* data, uint16_t len)
{
    uint32_t err_code = NRF_SUCCESS;
    ble_gatts_hvx_params_t hvx_param =
    {
        .type   = BLE_GATT_HVX_NOTIFICATION,
        .handle = g_nrf_provs_char_out_hdl.value_handle,
        .p_len  = &len,
    };

    hvx_param.p_data = smport_malloc(len, SM_MEM_NON_RETENTION);
    memcpy(hvx_param.p_data, data, len);

    do
    {
        err_code = sd_ble_gatts_hvx(g_nrf_conn_hdl, &hvx_param);
    } while (err_code != NRF_SUCCESS);
}

uint16_t smport_pbgatt_server_get_mtu(uint16_t conn_hdl)
{
    return MESH_PROV_SVC_MTU;
}

#endif

