#ifndef _NRF_IPC_H_
#define _NRF_IPC_H_

typedef enum
{
    NRF_IPC_TAG_ADV_REPORT              = 0,
    NRF_IPC_TAG_CONNECTED,
    NRF_IPC_TAG_DISCONNECTED,
    NRF_IPC_TAG_PROVS_DATA_IN,
    NRF_IPC_TAG_PROVS_SENT,
    NRF_IPC_TAG_PROVC_DATA_IN,
    NRF_IPC_TAG_PROVC_SENT,
    NRF_IPC_TAG_PROXYS_DATA_IN,
    NRF_IPC_TAG_PROXYS_SENT,
    NRF_IPC_TAG_PROXYC_DATA_IN,
    NRF_IPC_TAG_PROXYC_SENT,

    NRF_IPC_TAG_TIMER_EXPIRED,

    NRF_IPC_TAG_NONE                    = 0xFF
} nrf_ipc_tag_t;


typedef struct
{
    sm_bdaddr_t*                bd;
    uint8_t                     adv_type;
    uint8_t*                    data;
    uint8_t                     data_len;
    int8_t                      rssi;
} nrf_ipc_adv_report_param_t;

typedef struct
{
    uint16_t                    conn_hdl;
    sm_bdaddr_t*                bd;
} nrf_ipc_connected_param_t;

typedef struct
{
    uint16_t                    conn_hdl;
    uint8_t                     reason;
} nrf_ipc_disconnected_param_t;

typedef struct
{
    uint8_t*                    data;
    uint16_t                    data_len;
    uint16_t                    conn_handle;
} nrf_ipc_prov_proxy_data_t;

typedef struct
{
    bool                        status;
    uint16_t                    conn_handle;
} nrf_ipc_prov_proxy_sent_param_t;

typedef struct
{
    smport_timer_handler        handler;
    void*                       param;
} nrf_ipc_timer_expired_param_t;

typedef union
{
    nrf_ipc_adv_report_param_t          adv_rpt;
    nrf_ipc_connected_param_t           connected;
    nrf_ipc_disconnected_param_t        disconnected;
    nrf_ipc_prov_proxy_data_t           data;
    nrf_ipc_prov_proxy_sent_param_t     sent;
    nrf_ipc_timer_expired_param_t       timer;
} nrf_ipc_write_param_t;

void nrf_ipc_init(void);
bool nrf_ipc_write(nrf_ipc_tag_t tag, nrf_ipc_write_param_t* param);
void nrf_ipc_schedule(void);
#endif
