#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "sm_config.h"
#include "sm_common.h"
#include "sm_port.h"

uint16_t g_sm_conf_pbadv_adv_intvl                     = (100);          // x1ms
uint16_t g_sm_conf_pbadv_send_evt_win                  = (400);          // x1ms
uint16_t g_sm_conf_pbadv_scan_win                      = (100);          // x1ms
uint16_t g_sm_conf_pbadv_scan_intvl                    = (100);          // x1ms
uint16_t g_sm_conf_detect_beacon_win                   = (100);          // x1ms
uint16_t g_sm_conf_detect_beacon_intvl                 = (100);          // x1ms
uint16_t g_sm_conf_pbgatt_adv_intvl                    = (50);           // x1ms
uint16_t g_sm_conf_pbgatt_scan_intvl                   = (100);          // x1ms
uint16_t g_sm_conf_pbgatt_scan_win                     = (100);          // x1ms
uint16_t g_sm_conf_pbgatt_supervision                  = (600);          // x10ms
uint16_t g_sm_conf_proxy_id_intvl                      = (200);          // x10ms
uint16_t g_sm_conf_proxy_id_win                        = (5);            // x10ms
uint16_t g_sm_conf_proxy_id_adv_intvl                  = (20);           // x1ms
uint16_t g_sm_conf_proxy_con_intvl_min                 = (200);          // x1.25ms
uint16_t g_sm_conf_proxy_con_intvl_max                 = (1000);         // x1.25ms
uint16_t g_sm_conf_proxy_con_latency                   = (6);
uint16_t g_sm_conf_proxy_con_ce                        = (3);            // x0.625ms
uint16_t g_sm_conf_proxy_gatt_supervision              = (600);          // x10ms
uint16_t g_sm_conf_proxy_conn_update_timeout           = (600);          // x10ms
uint16_t g_sm_conf_adv_intvl                           = (100);          // x1ms
uint16_t g_sm_conf_scan_win                            = (100);          // x1ms
uint16_t g_sm_conf_scan_intvl                          = (100);          // x1ms
uint16_t g_sm_conf_send_win                            = (30);           // x10ms
uint16_t g_sm_conf_listen_win                          = (300);          // x10ms
uint16_t g_sm_conf_listen_intvl                        = (300);          // x10ms
uint16_t g_sm_conf_secure_beacon_intvl                 = (1000);         // x10ms
uint16_t g_sm_conf_secure_beacon_evt_num               = (3);
uint16_t g_sm_conf_secure_beacon_period                = (1000);
uint16_t g_sm_conf_period_beacon_intvl                 = (10);           // x10ms
uint16_t g_sm_conf_period_beacon_evt_num               = (1);
uint16_t g_sm_conf_relay_random_delay_max              = (20);           // x10ms
uint8_t g_sm_conf_retrans_seg_pkt_times                = (2);
uint16_t g_sm_conf_retrans_tx_timeout                  = (200);          // x10ms
uint16_t g_sm_conf_retrans_rx_timeout                  = (2000);         // x10ms
uint8_t g_sm_conf_topology_hello_bcast_min             = (1);            // x1s
uint8_t g_sm_conf_topology_hello_bcast_max             = (5);            // x1s
uint8_t g_sm_conf_topology_neighborhoodbcast_min       = (1);            // x1s
uint8_t g_sm_conf_topology_neighborhoodbcast_max       = (5);            // x1s
uint16_t g_sm_conf_attention_duration                  = (5);             // x1s
uint8_t g_sm_conf_prov_cap_pubkey_type                 = (0);
uint8_t g_sm_conf_prov_cap_out_oob_size                = (0);
uint16_t g_sm_conf_prov_cap_out_oob_act                = (0);
uint8_t g_sm_conf_prov_cap_in_oob_size                 = (0);
uint16_t g_sm_conf_prov_cap_in_oob_act                 = (0);
uint8_t g_sm_conf_prov_cap_static_oob_type             = (0);
uint8_t g_sm_conf_oob_cap_supported                    = (0x01);
uint8_t g_sm_conf_oob_out_size_supported               = (0);
uint8_t g_sm_conf_oob_out_act_supported                = (0x00);
uint8_t g_sm_conf_oob_in_size_supported                = (0);
uint8_t g_sm_conf_oob_in_act_supported                 = (0x00);
uint16_t g_sm_conf_ttl_def_val                         = (0x0C);
uint16_t g_sm_conf_company_id                          = (0x0000);         // 16-bit
uint16_t g_sm_conf_product_id                          = (0x0001);         // 16-bit
uint16_t g_sm_conf_version_id                          = (0x0000);         // 16-bit
sm_uuid_t g_sm_conf_local_uuid                         = {0xAA, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, \
                                                          0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
uint32_t g_sm_conf_elt_light_cache_flush_timeout       = (360000);         // x10ms