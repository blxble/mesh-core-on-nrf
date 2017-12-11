#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "sm_config.h"
#include "sm_common.h"
#include "sm_port.h"
#include "sm_acc_msg.h"
#include "sm_acc.h"
#include "sm_mod_msg.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_light.h"
#include "nrf_gpio.h"

#define NRF_LED_PIN         (30)

typedef struct
{
    uint8_t         elt_idx;
    uint8_t         onoff;
} nrf_light_elt_t;

static nrf_light_elt_t g_nrf_light_elt;

static sm_mid_t g_nrf_light_mid[] = 
{
    NRF_LIGHT_MID,
};

static void _nrf_light_on_acc_msg(uint8_t elt_idx, smacc_msg_t* msg, sm_addr_t src_addr, int32_t appkey_idx, int8_t rssi)
{
    const sm_msg_opcode_t onoff_set_op = SMMOD_MSG_OPCODE_GENERIC_ONOFF_SET;
    const sm_msg_opcode_t onoff_status_op = SMMOD_MSG_OPCODE_GENERIC_ONOFF_STATUS;
    uint8_t onoff, pre_onoff;
    uint8_t tid;
    uint8_t trans_time;
    uint8_t delay;
    smacc_msg_t status_msg;
    uint8_t status_msg_param[3];
    
    if (elt_idx == g_nrf_light_elt.elt_idx)
    {
        if (memcmp(&msg->opcode, &onoff_set_op, sizeof(sm_msg_opcode_t)) == 0)
        {
            onoff = *(msg->param + 0);
            tid = *(msg->param + 1);
            trans_time = *(msg->param + 2);
            delay = *(msg->param + 3);

            // TODO: set device onoff 
            if (onoff > 0)
            {
                NRF_LOG_INFO("Light ON!");
                nrf_gpio_pin_write(NRF_LED_PIN, 1);
            }
            else
            {
                NRF_LOG_INFO("Light OFF!");
                nrf_gpio_pin_write(NRF_LED_PIN, 0);
            }

            pre_onoff = g_nrf_light_elt.onoff;
            g_nrf_light_elt.onoff = onoff;

            memcpy(&status_msg.opcode, &onoff_status_op, sizeof(sm_msg_opcode_t));
            status_msg.param_len = 3;
            status_msg_param[0] = pre_onoff;
            status_msg_param[1] = onoff;
            status_msg_param[2] = 0;
            status_msg.param = status_msg_param;

            smacc_respond_msg(g_nrf_light_elt.elt_idx, src_addr, appkey_idx, &status_msg);
        }
        else
        {
        }
    }

    if (msg->param != NULL)
    {
        SM_MEM_FREE(msg->param);
    }
}

void nrf_light_init(void)
{
    nrf_gpio_cfg_output(NRF_LED_PIN);
    
    memset(&g_nrf_light_elt, 0x00, sizeof(g_nrf_light_elt));
    
    g_nrf_light_elt.elt_idx = smacc_reg_element(sizeof(g_nrf_light_mid)/sizeof(sm_mid_t), g_nrf_light_mid, 0, NULL, 
                                                    _nrf_light_on_acc_msg);
}
