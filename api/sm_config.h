#ifndef _SM_CONFIG_H_
#define _SM_CONFIG_H_

#define SM_DEBUG                            (0)

#define SM_ADV_AND_SCAN_COEXIST             (0)
#define SM_CONF_ADV_CHANNEL_MAP             (0x07)
#define SM_PB_GATT_SUPPORT                  (1)
#define SM_PB_GATT_ONLY                     (SM_PB_GATT_SUPPORT && 1)
#define SM_PROXY_GATT_SUPPORT               (1)
#define SM_PROXY_GATT_SERVER                (SM_PROXY_GATT_SUPPORT && 1)
#define SM_PROXY_GATT_CLIENT                (SM_PROXY_GATT_SUPPORT && 0)
#define SM_PROXY_GATT_ONLY                  (SM_PROXY_GATT_CLIENT && 0)
#define SM_RELAY_SUPPORT                    (1)
#define SM_DUPLICATED_CACHE_DEPTH           (8)
#define SM_TRANSMIC_64BIT                   (0)
#define SM_TRANS_REPLAY_PROTECT_LIST_SIZE   (16)
#define SM_PROVISIONER_SUPPORT              (1)
#define SM_LOW_POWER_NODE_SUPPORT           (0)
#define SM_FRIEND_NODE_SUPPORT              (0)
#define SM_IVI_INIT_VAL                     (0x00000000)
#define SM_TOPOLOGY_DETECT_SUPPORT          (0)

#if (NVDS_SUPPORT == 0) && !defined(CFG_TEST_API)
/**
 * ***************************************************************************
 *                                BEARER LAYER
 */

/**
 * -A-A-A-----A-A-A------A-A-A-----SSSSSSSS------------SSSSSSSS---------------->
 *  |<-intvl->|
 *  |<---pbadv send evt win------>|
 *                                |<-win->|
 *                                |<-pbadv scan intvl->|
 */
#define SM_CONF_PBADV_ADV_INTVL             (100)          // x1ms
#define SM_CONF_PBADV_SEND_EVT_WIN          (400)          // x1ms
#define SM_CONF_PBADV_SCAN_WIN              (100)          // x1ms
#define SM_CONF_PBADV_SCAN_INTVL            (100)          // x1ms
#define SM_CONF_DETECT_BEACON_WIN           (100)           // x1ms
#define SM_CONF_DETECT_BEACON_INTVL         (100)           // x1ms

#define SM_CONF_PBGATT_ADV_INTVL            (50)           // x1ms
#define SM_CONF_PBGATT_SCAN_INTVL           (100)          // x1ms
#define SM_CONF_PBGATT_SCAN_WIN             (100)           // x1ms
#define SM_CONF_PBGATT_SUPERVISION          (600)           // x10ms

#define SM_CONF_PROXY_ID_INTVL              (200)           // x10ms
#define SM_CONF_PROXY_ID_WIN                (5)            // x10ms
#define SM_CONF_PROXY_ID_ADV_INTVL          (20)             // x1ms
#define SM_CONF_PROXY_CON_INTVL_MIN         (60)           // x1.25ms
#define SM_CONF_PROXY_CON_INTVL_MAX         (60)          // x1.25ms
#define SM_CONF_PROXY_CON_LATENCY           (2)
#define SM_CONF_PROXY_CON_CE                (3)              // x0.625ms
#define SM_CONF_PROXY_SUPERVISION           (600)           // x10ms
#define SM_CONF_PROXY_CONN_UPDATE_TIMEOUT   (600)           // x10ms

/**
 * -A-A-A-------------A-A-A-------------A-A-A------------------------->
 *  |<---adv intvl--->|
 *  |<---------------send win------------------>|
 *
 *
 * -SSSSSSSSSSSSSS-----SSSSSSSSSSSSSS------------------------SSSSSSSSSSSSSS-----SSSSSSSSSSSSSS------->
 *  |<-scan win->|
 *  |<---scan intvl--->|
 *  |<-----------------------listen win--------------------->|
 *  |<---------------------------------------listen intvl-------------------------------------->|
 */
#define SM_CONF_ADV_INTVL                   (100)            // x1ms
#define SM_CONF_SEND_WIN                    (30)             // x10ms
#define SM_CONF_SCAN_WIN                    (100)            // x1ms
#define SM_CONF_SCAN_INTVL                  (100)            // x1ms
#define SM_CONF_LISTEN_WIN                  (30)             // x10ms
#define SM_CONF_LISTEN_INTVL                (30)            // x10ms

/**
 *****************************************************************************
 *                              MANAGEMENT LAYER
 */
#define SM_CONF_SECURE_BEACON_INTVL         (1000)          // x10ms
#define SM_CONF_SECURE_BEACON_EVT_NUM       (3)
#define SM_CONF_ATTENTION_DURATION          (5)             // x1s
#define SM_CONF_PERIOD_BEACON_INTVL         (10)            // x10ms
#define SM_CONF_PERIOD_BEACON_EVT_NUM       (1)
/**
 *****************************************************************************
 *                                NETWORK LAYER
 */
#define SM_CONF_RELAY_RANDOM_DELAY_MAX      (20)            // x10ms

/**
 *****************************************************************************
 *                                TRANSPORT LAYER
 */
#define SM_CONF_RETRANS_SEG_PKT_TIMES       (2)
#define SM_CONF_RETRANS_TX_TIMEOUT          (200)
#define SM_CONF_RETRANS_RX_TIMEOUT          (2000)

#define SM_CONF_FRND_POLL_TIMEOUT_CONDITION (3)

#define SM_CONF_TOPOLOGY_BCAST_HELLO_MIN    (1)             // x1s
#define SM_CONF_TOPOLOGY_BCAST_HELLO_MAX    (2)             // x1s
#define SM_CONF_TOPOLOGY_BCAST_NEIGHBOR_MIN (1)             // x1s
#define SM_CONF_TOPOLOGY_BCAST_NEIGHBOR_MAX (2)             // x1s
#define SM_CONF_TOPOLOGY_UCAST_REPORT_MIN   (2)
#define SM_CONF_TOPOLOGY_UCAST_REPORT_MAX   (4)
/**
 *****************************************************************************
 *                                APPLICATION LAYER
 */


/**
 *****************************************************************************
 *                                SMACC LAYER
 */
#define SM_PROV_CAP_PUBKEY_TYPE             (0)
#define SM_PROV_CAP_OUT_OOB_SIZE            (0)
#define SM_PROV_CAP_OUT_OOB_ACT             (0)
#define SM_PROV_CAP_IN_OOB_SIZE             (0)
#define SM_PROV_CAP_IN_OOB_ACT              (0)
#define SM_PROV_CAP_STATIC_OOB_TYPE         (0)

#define SM_PROV_OOB_TYPE_SUPPORTED          (0x01)          // for provisioner
#define SM_PROV_OOB_OUT_SIZE_SUPPORTED      (0)             // for provisioner
#define SM_PROV_OOB_OUT_ACT_SUPPORTED       (0)             // for provisioner
#define SM_PROV_OOB_IN_SIZE_SUPPORTED       (0)             // for provisioner
#define SM_PROV_OOB_IN_ACT_SUPPORTED        (0)             // for provisioner

#define SM_TTL_DEF_VAL                      (0x0C)


/**
 *****************************************************************************
 *                                Primary Element
 */
#define SM_SEC_BEACON_PERIOD                (1000)       // x10ms
/**
 *****************************************************************************
 *                             Configuration Model
 */

/**
 *****************************************************************************
 *                                  User Defined
 */
#define SM_COMPANY_ID                       (0x0000)    // 16-bit
#define SM_PRODUCT_ID                       (0x0001)    // 16-bit
#define SM_VERSION_ID                       (0x0000)    // 16-bit
#define SM_LOCAL_UUID                       {0xAA, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, \
                                             0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}

#define SM_ELT_LIGHT_CACHE_FLUASH_TIMEOUT   (0)//(360000)    // x10ms
#else
#include "sm_common.h"
extern uint16_t g_sm_conf_pbadv_adv_intvl;
extern uint16_t g_sm_conf_pbadv_send_evt_win;
extern uint16_t g_sm_conf_pbadv_scan_win;
extern uint16_t g_sm_conf_pbadv_scan_intvl;
extern uint16_t g_sm_conf_detect_beacon_win;
extern uint16_t g_sm_conf_detect_beacon_intvl;
extern uint16_t g_sm_conf_pbgatt_adv_intvl;
extern uint16_t g_sm_conf_pbgatt_scan_intvl;
extern uint16_t g_sm_conf_pbgatt_scan_win;
extern uint16_t g_sm_conf_pbgatt_supervision;
extern uint16_t g_sm_conf_proxy_id_intvl;
extern uint16_t g_sm_conf_proxy_id_win;
extern uint16_t g_sm_conf_proxy_id_adv_intvl;
extern uint16_t g_sm_conf_proxy_con_intvl_min;
extern uint16_t g_sm_conf_proxy_con_intvl_max;
extern uint16_t g_sm_conf_proxy_con_latency;
extern uint16_t g_sm_conf_proxy_con_ce;
extern uint16_t g_sm_conf_proxy_gatt_supervision;
extern uint16_t g_sm_conf_proxy_conn_update_timeout;
extern uint16_t g_sm_conf_adv_intvl;
extern uint16_t g_sm_conf_scan_win;
extern uint16_t g_sm_conf_scan_intvl;
extern uint16_t g_sm_conf_send_win;
extern uint16_t g_sm_conf_listen_win;
extern uint16_t g_sm_conf_listen_intvl;
extern uint16_t g_sm_conf_secure_beacon_intvl;
extern uint16_t g_sm_conf_secure_beacon_evt_num;
extern uint16_t g_sm_conf_secure_beacon_period;
extern uint16_t g_sm_conf_period_beacon_intvl;
extern uint16_t g_sm_conf_period_beacon_evt_num;
extern uint16_t g_sm_conf_relay_random_delay_max;
extern uint8_t g_sm_conf_retrans_seg_pkt_times;
extern uint16_t g_sm_conf_retrans_tx_timeout;
extern uint16_t g_sm_conf_retrans_rx_timeout;
extern uint8_t g_sm_conf_topology_hello_bcast_min;
extern uint8_t g_sm_conf_topology_hello_bcast_max;
extern uint8_t g_sm_conf_topology_neighborhoodbcast_min;
extern uint8_t g_sm_conf_topology_neighborhoodbcast_max;
extern uint16_t g_sm_conf_attention_duration;
extern uint8_t g_sm_conf_prov_cap_pubkey_type;
extern uint8_t g_sm_conf_prov_cap_out_oob_size;
extern uint16_t g_sm_conf_prov_cap_out_oob_act;
extern uint8_t g_sm_conf_prov_cap_in_oob_size;
extern uint16_t g_sm_conf_prov_cap_in_oob_act;
extern uint8_t g_sm_conf_prov_cap_static_oob_type;
extern uint8_t g_sm_conf_oob_cap_supported;
extern uint8_t g_sm_conf_oob_out_size_supported;
extern uint8_t g_sm_conf_oob_out_act_supported;
extern uint8_t g_sm_conf_oob_in_size_supported;
extern uint8_t g_sm_conf_oob_in_act_supported;
extern uint16_t g_sm_conf_ttl_def_val;
extern uint16_t g_sm_conf_company_id;
extern uint16_t g_sm_conf_product_id;
extern uint16_t g_sm_conf_version_id;
extern sm_uuid_t g_sm_conf_local_uuid;
extern uint32_t g_sm_conf_elt_light_cache_flush_timeout;
/**
 * ***************************************************************************
 *                                BEARER LAYER
 */


/**
 * -A-A-A-----A-A-A------A-A-A-----SSSSSSSS------------SSSSSSSS---------------->
 *  |<-intvl->|
 *  |<---pbadv send evt win------>|
 *                                |<-win->|
 *                                |<-pbadv scan intvl->|
 */
#define SM_CONF_PBADV_ADV_INTVL             (g_sm_conf_pbadv_adv_intvl)          // x1ms
#define SM_CONF_PBADV_SEND_EVT_WIN          (g_sm_conf_pbadv_send_evt_win)          // x1ms
#define SM_CONF_PBADV_SCAN_WIN              (g_sm_conf_pbadv_scan_win)          // x1ms
#define SM_CONF_PBADV_SCAN_INTVL            (g_sm_conf_pbadv_scan_intvl)          // x1ms
#define SM_CONF_DETECT_BEACON_WIN           (g_sm_conf_detect_beacon_win)           // x1ms
#define SM_CONF_DETECT_BEACON_INTVL         (g_sm_conf_detect_beacon_intvl)           // x1ms

#define SM_CONF_PBGATT_ADV_INTVL            (g_sm_conf_pbgatt_adv_intvl)           // x1ms
#define SM_CONF_PBGATT_SCAN_INTVL           (g_sm_conf_pbgatt_scan_intvl)          // x1ms
#define SM_CONF_PBGATT_SCAN_WIN             (g_sm_conf_pbgatt_scan_win)           // x1ms
#define SM_CONF_PBGATT_SUPERVISION          (g_sm_conf_pbgatt_supervision)           // x10ms

#define SM_CONF_PROXY_ID_INTVL              (g_sm_conf_proxy_id_intvl)           // x10ms
#define SM_CONF_PROXY_ID_WIN                (g_sm_conf_proxy_id_win)            // x10ms
#define SM_CONF_PROXY_ID_ADV_INTVL          (g_sm_conf_proxy_id_adv_intvl)             // x1ms
#define SM_CONF_PROXY_CON_INTVL_MIN         (g_sm_conf_proxy_con_intvl_min)           // x1.25ms
#define SM_CONF_PROXY_CON_INTVL_MAX         (g_sm_conf_proxy_con_intvl_max)          // x1.25ms
#define SM_CONF_PROXY_CON_LATENCY           (g_sm_conf_proxy_con_latency)
#define SM_CONF_PROXY_CON_CE                (g_sm_conf_proxy_con_ce)              // x0.625ms
#define SM_CONF_PROXY_SUPERVISION           (g_sm_conf_proxy_gatt_supervision)           // x10ms
#define SM_CONF_PROXY_CONN_UPDATE_TIMEOUT   (g_sm_conf_proxy_conn_update_timeout)           // x10ms

/**
 * -A-A-A-------------A-A-A-------------A-A-A------------------------->
 *  |<---adv intvl--->|
 *  |<---------------send win------------------>|
 *
 *
 * -SSSSSSSSSSSSSS-----SSSSSSSSSSSSSS------------------------SSSSSSSSSSSSSS-----SSSSSSSSSSSSSS------->
 *  |<-scan win->|
 *  |<---scan intvl--->|
 *  |<-----------------------listen win--------------------->|
 *  |<---------------------------------------listen intvl-------------------------------------->|
 */
#define SM_CONF_ADV_INTVL                   (g_sm_conf_adv_intvl)            // x1ms
#define SM_CONF_SEND_WIN                    (g_sm_conf_send_win)             // x10ms
#define SM_CONF_SCAN_WIN                    (g_sm_conf_scan_win)            // x1ms
#define SM_CONF_SCAN_INTVL                  (g_sm_conf_scan_intvl)            // x1ms
#define SM_CONF_LISTEN_WIN                  (g_sm_conf_listen_win)             // x10ms
#define SM_CONF_LISTEN_INTVL                (g_sm_conf_listen_intvl)            // x10ms

/**
 *****************************************************************************
 *                              MANAGEMENT LAYER
 */
#define SM_CONF_SECURE_BEACON_INTVL         (g_sm_conf_secure_beacon_intvl)          // x10ms
#define SM_CONF_SECURE_BEACON_EVT_NUM       (g_sm_conf_secure_beacon_evt_num)
#define SM_CONF_PERIOD_BEACON_INTVL         (g_sm_conf_period_beacon_intvl)            // x10ms
#define SM_CONF_PERIOD_BEACON_EVT_NUM       (g_sm_conf_period_beacon_evt_num)
#define SM_CONF_ATTENTION_DURATION          (g_sm_conf_attention_duration)             // x1s
/**
 *****************************************************************************
 *                                NETWORK LAYER
 */
#define SM_CONF_RELAY_RANDOM_DELAY_MAX      (g_sm_conf_relay_random_delay_max)

/**
 *****************************************************************************
 *                                TRANSPORT LAYER
 */
#define SM_CONF_RETRANS_SEG_PKT_TIMES       (g_sm_conf_retrans_seg_pkt_times)
#define SM_CONF_RETRANS_TX_TIMEOUT          (g_sm_conf_retrans_tx_timeout)
#define SM_CONF_RETRANS_RX_TIMEOUT          (g_sm_conf_retrans_rx_timeout)

#define SM_CONF_FRND_POLL_TIMEOUT_CONDITION (3)

#define SM_CONF_TOPOLOGY_BCAST_HELLO_MIN    (1)             // x1s
#define SM_CONF_TOPOLOGY_BCAST_HELLO_MAX    (2)             // x1s
#define SM_CONF_TOPOLOGY_BCAST_NEIGHBOR_MIN (1)             // x1s
#define SM_CONF_TOPOLOGY_BCAST_NEIGHBOR_MAX (2)             // x1s
#define SM_CONF_TOPOLOGY_UCAST_REPORT_MIN   (2)
#define SM_CONF_TOPOLOGY_UCAST_REPORT_MAX   (4)
/**
 *****************************************************************************
 *                                APPLICATION LAYER
 */


/**
 *****************************************************************************
 *                                SMACC LAYER
 */
#define SM_PROV_CAP_PUBKEY_TYPE             (g_sm_conf_prov_cap_pubkey_type)
#define SM_PROV_CAP_OUT_OOB_SIZE            (g_sm_conf_prov_cap_out_oob_size)
#define SM_PROV_CAP_OUT_OOB_ACT             (g_sm_conf_prov_cap_out_oob_act)
#define SM_PROV_CAP_IN_OOB_SIZE             (g_sm_conf_prov_cap_in_oob_size)
#define SM_PROV_CAP_IN_OOB_ACT              (g_sm_conf_prov_cap_in_oob_act)
#define SM_PROV_CAP_STATIC_OOB_TYPE         (g_sm_conf_prov_cap_static_oob_type)

#define SM_PROV_OOB_TYPE_SUPPORTED          (g_sm_conf_oob_cap_supported)       // for provisioner
#define SM_PROV_OOB_OUT_SIZE_SUPPORTED      (g_sm_conf_oob_out_size_supported)  // for provisioner
#define SM_PROV_OOB_OUT_ACT_SUPPORTED       (g_sm_conf_oob_out_act_supported)   // for provisioner
#define SM_PROV_OOB_IN_SIZE_SUPPORTED       (g_sm_conf_oob_in_size_supported)   // for provisioner
#define SM_PROV_OOB_IN_ACT_SUPPORTED        (g_sm_conf_oob_in_act_supported)    // for provisioner

#define SM_TTL_DEF_VAL                      (g_sm_conf_ttl_def_val)

/**
 *****************************************************************************
 *                                Primary Element
 */
#define SM_SEC_BEACON_PERIOD                (g_sm_conf_secure_beacon_period)       // x10ms
/**
 *****************************************************************************
 *                             Configuration Model
 */

/**
 *****************************************************************************
 *                                  User Defined
 */
#define SM_COMPANY_ID                       (g_sm_conf_company_id)    // 16-bit
#define SM_PRODUCT_ID                       (g_sm_conf_product_id)    // 16-bit
#define SM_VERSION_ID                       (g_sm_conf_version_id)    // 16-bit
#define SM_LOCAL_UUID                       (g_sm_conf_local_uuid)

#define SM_ELT_LIGHT_CACHE_FLUASH_TIMEOUT    (g_sm_conf_elt_light_cache_flush_timeout)
#endif

void sm_config_init(void);
#endif

