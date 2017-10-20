/** 
 * @file             sm_port_evt.h 
 * @brief            Event callback by porting platform
 * @author           chenjian
 * @date             2017.09.01
 * @version          1.0.0.0 
 * @par              Copyright (c): BLX corp. 2017-2018
 */

#ifndef _SM_PORT_EVT_H_
#define _SM_PORT_EVT_H_

/**
 * Indciate advertising report.
 * @param[in]  bd        bluetooth address that adv report comes from
 * @param[in]  adv_type  advertising type
 * @param[in]  data      advertising data
 * @param[in]  data_len  advertising data length
 * @param[in]  rssi      RSSI of the advertising packet
 */
void smport_evt_adv_report(sm_bdaddr_t* bd, uint8_t adv_type, uint8_t* data, uint8_t data_len, int8_t rssi);

/**
 * Indicate encryption result
 * @param[in]  cipher_text cipher text
 */
void smport_evt_encrypt_result(uint8_t* cipher_text);

/**
 * Indicate connection established.
 * @param[in]  conn_hdl   connection handle
 * @param[in]  bd         peer bluetooth address
 */
void smport_evt_connected(uint16_t conn_hdl, sm_bdaddr_t* bd);

/**
 * Indicate connection terminated
 * @param[in]  conn_hdl   connection handle
 * @param[in]  reason     disconnection reason
 */
void smport_evt_disconnected(uint16_t conn_hdl, uint8_t reason);

/**
 * Indicate received data on PB-GATT server side
 * @param[in]  data       received data
 * @param[in]  data_len   data length
 */
void smport_evt_prov_server_data_in(uint8_t* data, uint16_t data_len);

/**
 * Indicate PB-GATT server sending PDU completed
 * @param[in]  status     sucess or fail
 */
void smport_evt_prov_server_sent_complete(bool status);

/**
 * Indicate received data on PB-GATT client side
 * @param[in]  data       received data
 * @param[in]  data_len   data length
 */
void smport_evt_prov_client_data_in(uint8_t* data, uint16_t data_len);

/**
 * Indicate PB-GATT client sending PDU completed
 * @param[in]  status     sucess or fail
 */
void smport_evt_prov_client_sent_complete(bool status);

/**
 * Indicate received data on proxy server side
 * @param[in]  data       received data
 * @param[in]  data_len   data length
 */
void smport_evt_proxy_server_data_in(uint8_t* data, uint16_t data_len);

/**
 * Indicate proxy server sending PDU completed
 * @param[in]  status     sucess or fail
 */
void smport_evt_proxy_server_sent_complete(bool status);

/**
 * Indicate received data on proxy client side
 * @param[in]  data       received data
 * @param[in]  data_len   data length
 */
void smport_evt_proxy_client_data_in(uint8_t* data, uint16_t data_len);

/**
 * Indicate proxy client sending PDU completed
 * @param[in]  status     sucess or fail
 */
void smport_evt_proxy_client_sent_complete(bool status);
#endif
