#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "sm_config.h"
#include "sm_common.h"
#include "sm_port.h"
#include "sm_port_evt.h"
#include "nrf_sys.h"
#include "nrf_ipc.h"

#define NRF_IPC_BUFFER_SIZE         (80)
#define NRF_IPC_BUFFER_COUNT        (60)

 uint8_t g_nrf_ipc_buf[NRF_IPC_BUFFER_COUNT][NRF_IPC_BUFFER_SIZE];
 uint8_t g_nrf_ipc_read;
 uint8_t g_nrf_ipc_write;

void nrf_ipc_init(void)
{
    g_nrf_ipc_read = 0xFF;
    g_nrf_ipc_write = 0xFF;
}

bool nrf_ipc_write(nrf_ipc_tag_t tag, nrf_ipc_write_param_t* param)
{
    uint8_t* p;

    if (g_nrf_ipc_write == 0xFF)
    {
        g_nrf_ipc_write = 0;
        g_nrf_ipc_read = 0;
    }
    
    if (((g_nrf_ipc_read > g_nrf_ipc_write) && ((g_nrf_ipc_read - g_nrf_ipc_write) > 1)) ||
         ((g_nrf_ipc_read < g_nrf_ipc_write) && ((NRF_IPC_BUFFER_COUNT - g_nrf_ipc_write + g_nrf_ipc_read) > 1)) ||
         (g_nrf_ipc_write == g_nrf_ipc_read))
    {
        p = &g_nrf_ipc_buf[g_nrf_ipc_write][0];
        
        *p++ = (uint8_t)tag;
        switch (tag)
        {
        case NRF_IPC_TAG_ADV_REPORT:
            sm_memcpy(p, param->adv_rpt.bd->addr, 6);
            p += 6;
            *p++ = param->adv_rpt.bd->addr_type;
            *p++ = param->adv_rpt.adv_type;
            *p++ = param->adv_rpt.data_len;
            sm_memcpy(p, param->adv_rpt.data, param->adv_rpt.data_len);
            p += param->adv_rpt.data_len;
            *p++ = (uint8_t)param->adv_rpt.rssi;
            break;
        case NRF_IPC_TAG_CONNECTED:
            *p++ = (uint8_t)(param->connected.conn_hdl & 0xFF);
            *p++ = (uint8_t)(param->connected.conn_hdl >> 8);
            sm_memcpy(p, param->connected.bd->addr, 6);
            p += 6;
            *p++ = param->connected.bd->addr_type;
            break;
        case NRF_IPC_TAG_DISCONNECTED:
            *p++ = (uint8_t)(param->disconnected.conn_hdl & 0xFF);
            *p++ = (uint8_t)(param->disconnected.conn_hdl >> 8);
            *p++ = param->disconnected.reason;
            break;
        case NRF_IPC_TAG_PROVS_DATA_IN:
        case NRF_IPC_TAG_PROVC_DATA_IN:
        case NRF_IPC_TAG_PROXYS_DATA_IN:
        case NRF_IPC_TAG_PROXYC_DATA_IN:
            *p++ = (uint8_t)(param->data.conn_handle & 0xFF);
            *p++ = (uint8_t)(param->data.conn_handle >> 8);
            *p++ = (uint8_t)(param->data.data_len & 0xFF);
            *p++ = (uint8_t)(param->data.data_len >> 8);
            sm_memcpy(p, param->data.data, param->data.data_len);
            p += param->data.data_len;
            break;
        case NRF_IPC_TAG_PROVS_SENT:
        case NRF_IPC_TAG_PROVC_SENT:
        case NRF_IPC_TAG_PROXYS_SENT:
        case NRF_IPC_TAG_PROXYC_SENT:
            *p++ = (uint8_t)(param->data.conn_handle & 0xFF);
            *p++ = (uint8_t)(param->data.conn_handle >> 8);
            *p++ = (uint8_t)param->sent.status;
            break;
        case NRF_IPC_TAG_TIMER_EXPIRED:
            *p++ = (uint8_t)(((uint32_t)param->timer.handler) & 0xFF);
            *p++ = (uint8_t)((((uint32_t)param->timer.handler) >> 8) & 0xFF);
            *p++ = (uint8_t)((((uint32_t)param->timer.handler) >> 16) & 0xFF);
            *p++ = (uint8_t)((((uint32_t)param->timer.handler) >> 24) & 0xFF);
            *p++ = (uint8_t)(((uint32_t)param->timer.param) & 0xFF);
            *p++ = (uint8_t)((((uint32_t)param->timer.param) >> 8) & 0xFF);
            *p++ = (uint8_t)((((uint32_t)param->timer.param) >> 16) & 0xFF);
            *p++ = (uint8_t)((((uint32_t)param->timer.param) >> 24) & 0xFF);
            break;
        default:
            return false;
        }

        g_nrf_ipc_write = (g_nrf_ipc_write + 1) % NRF_IPC_BUFFER_COUNT;
        return true;
    }

    return false;
}

void nrf_ipc_schedule(void)
{
    uint8_t* p;
    nrf_ipc_tag_t tag;

    //do
    {
        if (g_nrf_ipc_read == g_nrf_ipc_write)
        {
            return;
        }

        p = &g_nrf_ipc_buf[g_nrf_ipc_read][0];
        
        tag = *p++;
        switch (tag)
        {
        case NRF_IPC_TAG_ADV_REPORT:
            {
                sm_bdaddr_t bd;
                uint8_t len, adv_type;
                uint8_t* data;
                int8_t rssi;
                
                memcpy(bd.addr, p, 6);
                p += 6;
                bd.addr_type = *p++;
                adv_type = *p++;
                len = *p++;
                data = p;
                p += len;
                rssi = (int8_t)(*p++);
                
                smport_evt_adv_report(&bd, adv_type, data, len, rssi);
            }
            break;
        case NRF_IPC_TAG_CONNECTED:
            {
                uint16_t conn_hdl;
                sm_bdaddr_t bd;

                conn_hdl = ((uint16_t)(*p)) | (((uint16_t)(*(p + 1))) << 8);
                p += 2;
                memcpy(bd.addr, p, 6);
                p += 6;
                bd.addr_type = *p++;

                smport_evt_connected(conn_hdl, &bd);
            }
            break;
        case NRF_IPC_TAG_DISCONNECTED:
            {
                uint16_t conn_hdl;
                uint8_t reason;

                conn_hdl = ((uint16_t)(*p)) | (((uint16_t)(*(p + 1))) << 8);
                p += 2;
                reason = *p++;
                
                smport_evt_disconnected(conn_hdl, reason);
            }
            break;
        case NRF_IPC_TAG_PROVS_DATA_IN:
            {
                uint16_t len;
                uint16_t conn_hdl;

                conn_hdl = ((uint16_t)(*p)) | (((uint16_t)(*(p + 1))) << 8);
                p += 2;
                len = ((uint16_t)(*p)) | (((uint16_t)(*(p + 1))) << 8);
                p += 2;

                smport_evt_prov_server_data_in(conn_hdl, p, len);
            }
            break;
        case NRF_IPC_TAG_PROVS_SENT:
            {
                bool status;
                uint16_t conn_hdl;

                conn_hdl = ((uint16_t)(*p)) | (((uint16_t)(*(p + 1))) << 8);
                p += 2;

                status = (bool)(*p++);

                smport_evt_prov_server_sent_complete(conn_hdl, status);
            }
            break;
        case NRF_IPC_TAG_PROVC_DATA_IN:
            {
                uint16_t len;
                uint16_t conn_hdl;

                conn_hdl = ((uint16_t)(*p)) | (((uint16_t)(*(p + 1))) << 8);
                p += 2;

                len = ((uint16_t)(*p)) | (((uint16_t)(*(p + 1))) << 8);
                p += 2;

                smport_evt_prov_client_data_in(conn_hdl, p, len);
            }
            break;
        case NRF_IPC_TAG_PROVC_SENT:
            {
                bool status;
                uint16_t conn_hdl;

                conn_hdl = ((uint16_t)(*p)) | (((uint16_t)(*(p + 1))) << 8);
                p += 2;

                status = (bool)(*p++);

                smport_evt_prov_client_sent_complete(conn_hdl, status);
            }
            break;
        case NRF_IPC_TAG_PROXYS_DATA_IN:
            {
                uint16_t len;
                uint16_t conn_hdl;

                conn_hdl = ((uint16_t)(*p)) | (((uint16_t)(*(p + 1))) << 8);
                p += 2;

                len = ((uint16_t)(*p)) | (((uint16_t)(*(p + 1))) << 8);
                p += 2;

                smport_evt_proxy_server_data_in(conn_hdl, p, len);
            }
            break;
        case NRF_IPC_TAG_PROXYS_SENT:
            {
                bool status;
                uint16_t conn_hdl;

                conn_hdl = ((uint16_t)(*p)) | (((uint16_t)(*(p + 1))) << 8);
                p += 2;

                status = (bool)(*p++);

                smport_evt_proxy_server_sent_complete(conn_hdl, status);
            }
            break;
        case NRF_IPC_TAG_PROXYC_DATA_IN:
            {
                uint16_t len;
                uint16_t conn_hdl;

                conn_hdl = ((uint16_t)(*p)) | (((uint16_t)(*(p + 1))) << 8);
                p += 2;

                len = ((uint16_t)(*p)) | (((uint16_t)(*(p + 1))) << 8);
                p += 2;

                smport_evt_proxy_client_data_in(conn_hdl, p, len);
            }
            break;
        case NRF_IPC_TAG_PROXYC_SENT:
            {
                bool status;
                uint16_t conn_hdl;

                conn_hdl = ((uint16_t)(*p)) | (((uint16_t)(*(p + 1))) << 8);
                p += 2;

                status = (bool)(*p++);

                smport_evt_proxy_client_sent_complete(conn_hdl, status);
            }
            break;
        case NRF_IPC_TAG_TIMER_EXPIRED:
            {
                smport_timer_handler handler;
                void* param;

                handler = (smport_timer_handler)(((uint32_t)(*p)) |
                                                  (((uint32_t)(*(p + 1))) << 8) |
                                                  (((uint32_t)(*(p + 2))) << 16) |
                                                  (((uint32_t)(*(p + 3))) << 24));
                p += 4;
                param = (void*)(((uint32_t)(*p)) |
                                (((uint32_t)(*(p + 1))) << 8) |
                                (((uint32_t)(*(p + 2))) << 16) |
                                (((uint32_t)(*(p + 3))) << 24));
                p += 4;

                nrf_sys_timer_expired(handler, param);
            }
            break;
        }

        g_nrf_ipc_read = (g_nrf_ipc_read + 1) % NRF_IPC_BUFFER_COUNT;
    } //while (1);
}

