/** 
 * @file             sm_port.h 
 * @brief            porting interfaces
 * @author           chenjian
 * @date             2017.09.01
 * @version          1.0.0.0 
 * @par              Copyright (c): BLX corp. 2017-2018
 */

#ifndef _SM_PORT_H_
#define _SM_PORT_H_

#define SM_PORT_LOCAL                           (0)
#define SM_PORT_ANDROID                         (1)
#define SM_PORT_NORDIC                          (2)
#define SM_PORT_DIALOG                          (3)


#define SM_GAP_ADV_TYPE_ADV_IND                 (0x00)   /**< Connectable undirected. */
#define SM_GAP_ADV_TYPE_ADV_DIRECT_IND          (0x01)   /**< Connectable directed. */
#define SM_GAP_ADV_TYPE_ADV_SCAN_IND            (0x02)   /**< Scannable undirected. */
#define SM_GAP_ADV_TYPE_ADV_NONCONN_IND         (0x03)   /**< Non connectable undirected. */

#define MESH_PROV_SVC_UUID                      (0x1827)
#define MESH_PROV_CHAR_DATA_IN_VAL_UUID         (0x2ADB)
#define MESH_PROV_CHAR_DATA_OUT_VAL_UUID        (0x2ADC)
#define MESH_PROXY_SVC_UUID                     (0x1828)
#define MESH_PROXY_CHAR_DATA_IN_VAL_UUID        (0x2ADD)
#define MESH_PROXY_CHAR_DATA_OUT_VAL_UUID       (0x2ADE)
#define MESH_PROV_SVC_MTU                       (69)
#define MESH_PROXY_SVC_MTU                      (33)


/**
 * allocate memory dynamically.
 * @param[in]  size      size to be allocated in bytes
 * @param[in]  type      memory type, @SM_MEM_NON_RETENTION / @SM_MEM_RETENTION
 * @retval     allcated memory pointer
 */
void* smport_malloc(uint16_t size, uint8_t type);

/**
 * free allcated memory
 * @param[in]  mem       memory to be freed
 */
void smport_free(void* mem);

/**
 * enter critical exectution protection
 * @param[out]  cr       critical nest value
 */
void smport_enter_critical(uint32_t* cr);

/**
 * exit from critical execution protection
 * @param[in]   cr       critical nest value
 */
void smport_exit_critical(uint32_t cr);

/**
 * timer callback definition
 * @param[in]  context   context pointer comes from timer starting
 */
typedef void (*smport_timer_handler)(void* context);

/**
 * Initialize timer module
 */
void smport_init_timer(void);

/**
 * Create a timer.
 * Timer should stand by after created.
 * @retval     created timer id
 */
void* smport_create_timer(void);

/**
 * Start a timer.
 * Timer starts to run.
 * @param[in]  timer   timer id
 * @param[in]  timeout timer period, in 10 millisecond
 * @param[in]  handler  timeout callback handler
 * @param[in]  context to be passed to timeout handler when timer expired
 */
void smport_start_timer(void* timer, uint32_t timeout, smport_timer_handler handler, void* context);

/**
 * Stop a timer.
 * Timer will be stopped, but not cleared.
 * @param[in]  timer   timer id
 */
void smport_stop_timer(void* timer);

/**
 * Clear a timer
 * Timer will be cleared. If it is running, it should be stopped firstly.
 * @param[in]  timer   timer id
 */
void smport_clear_timer(void* timer);

/**
 * Start BLE adevertise
 * @parma[in]  adv_type  advertising type, @SM_GAP_ADV_TYPE_xxx
 * @param[in]  intvl_min minimum advertising interval, 0x20 ~ 0x4000
 * @param[in]  intvl_max maximum advertising interval, 0x20 ~ 0x4000
 * @param[in]  chan_map  channel selection, lowest 3 bits present channel 37, 38 and 39 
 * @param[in]  data      advertising data
 * @param[in]  data_len  advertising data length
 */
void smport_start_advertise(uint8_t adv_type, uint16_t intvl_min, uint16_t intval_max, uint8_t chan_map,
                                    uint8_t* data, uint8_t data_len);

/**
 * Stop BLE advertise
 */
void smport_stop_advertise(void);

/**
 * Start passive scan
 * The scan filter policy shall be 'accept all advertising packets'. Use @smport_evt_adv_report()
 * to indicate advertising report.
 * @param[in]  intvl     scan interval, 0x04 ~ 0x4000
 * @param[in]  win       scan window, 0x04 ~ 0x4000
 */
void smport_start_scan(uint16_t intvl, uint16_t win);

/**
 * Stop scan
 */
void smport_stop_scan(void);

/**
 * Use AES-128 to encrypt
 * To return encryption result, use @smport_evt_encrypt_result()
 * @param[in]  key        secret key
 * @param[in]  claar_text plain text
 */
void smport_aes_encrypt(uint8_t* key, uint8_t* clear_text);

/**
 * Start advertise and wait for GATT connection.
 * The advertising type shall be ADV_IND.
 * @param[in]  svc_uui    advertise data field: service uuid
 * @param[in]  svc_data   advertise data field: service data
 * @param[in]  data_len   service data length
 */
void smport_gatt_advertise(uint16_t svc_uuid, uint8_t* svc_data, uint8_t data_len);

/**
 * Initial a GATT connection.
 * After connection established, use @smport_evt_connected() to indicate mesh core
 * @param[in]  bd            dedicated bluetooth address
 * @param[in]  gatt_svc_uuid service uuid
 */
void smport_gatt_create_connect(sm_bdaddr_t* bd, uint16_t gatt_svc_uuid);

/**
 * Disconnect a GATT connection
 * @param[in]  conn_hdl  connection handle
 * @param[in]  reason    disconnecting reason
 * @param[in]  gatt_svc_uuid service uuid
 */
void smport_gatt_disconnect(uint16_t conn_hdl, uint8_t reason, uint16_t gatt_svc_uuid);

/**
 * Add PB-GATT service server
 */
void smport_pbgatt_add_server(void);

/**
 * Get PB-GATT connection ATT-MTU
 * @retval      ATT-MTU
 */
uint16_t smport_pbgatt_server_get_mtu(uint16_t conn_hdl);

/**
 * Send PB-GATT PDU on server side
 * Use @smport_evt_prov_server_sent_complete() to indicate sending completed
 * @param[in]  conn_hdl   connection handle
 * @param[in]  data      PDU data
 * @param[in]  len       PDU data length
 */
void smport_pbgatt_server_send_pdu(uint16_t conn_hdl, uint8_t* data, uint16_t len);

/**
 * Add PB-GATT service client
 */
void smport_pbgatt_add_client(void);

/**
 * Get PB-GATT connection ATT-MTU
 * @retval      ATT-MTU
 */
uint16_t smport_pbgatt_client_get_mtu(uint16_t conn_hdl);

/**
 * Send PB-GATT PDU on client side
 * Use @smport_evt_prov_client_sent_complete() to indicate sending completed
 * @param[in]  conn_hdl  connection handle
 * @param[in]  data      PDU data
 * @param[in]  len       PDU data length
 */
void smport_pbgatt_client_send_pdu(uint16_t conn_hdl, uint8_t* data, uint16_t len);

/**
 * Add proxy service server
 */
void smport_proxy_add_server(void);

/**
 * Get proxy service connection ATT-MTU
 * @retval      ATT-MTU
 */
uint16_t smport_proxy_server_get_mtu(uint16_t conn_hdl);

/**
 * Send proxy PDU on server side
 * Use @smport_evt_proxy_server_sent_complete() to indicate sending completed
 * @param[in]  conn_hdl   connection handle
 * @param[in]  data      PDU data
 * @param[in]  len       PDU data length
 */
void smport_proxy_server_send_pdu(uint16_t conn_hdl, uint8_t* data, uint16_t len);


/**
 * Add proxy service client
 */
void smport_proxy_add_client(void);

/**
 * Add proxy service server
 */
uint16_t smport_proxy_client_get_mtu(uint16_t conn_hdl);

/**
 * Send proxy PDU on client side
 * Use @smport_evt_proxy_client_sent_complete() to indicate sending completed
 * @param[in]  conn_hdl   connection handle
 * @param[in]  data      PDU data
 * @param[in]  len       PDU data length
 */
void smport_proxy_client_send_pdu(uint16_t conn_hdl, uint8_t* data, uint16_t len);
#endif
