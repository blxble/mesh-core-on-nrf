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
#include "nrf_switch.h"
#include "app_timer.h"
#include "app_button.h"

#define NRF_SWITCH_PIN         13//(4)

typedef struct
{
    uint8_t         elt_idx;
    uint8_t         onoff;
} nrf_switch_elt_t;

static nrf_switch_elt_t g_nrf_switch_elt;

static sm_mid_t g_nrf_switch_mid[] = 
{
    NRF_SWITCH_MID,
};

static void _nrf_switch_on_acc_msg(uint8_t elt_idx, smacc_msg_t* msg, sm_addr_t src_addr, int32_t appkey_idx, int8_t rssi)
{
    const sm_msg_opcode_t onoff_status_op = SMMOD_MSG_OPCODE_GENERIC_ONOFF_STATUS;
    
    if (elt_idx == g_nrf_switch_elt.elt_idx)
    {
        if (memcmp(&msg->opcode, &onoff_status_op, sizeof(sm_msg_opcode_t)) == 0)
        {
            
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

static void _nrf_switch_on_off(void)
{
    const sm_msg_opcode_t onoff_set_op = SMMOD_MSG_OPCODE_GENERIC_ONOFF_SET;
    uint8_t onoff;
    smacc_msg_t onoff_msg;
    uint8_t onoff_msg_param[4];

    sm_memcpy((void*)&onoff_msg.opcode, (void*)&onoff_set_op, sizeof(sm_msg_opcode_t));
    onoff = g_nrf_switch_elt.onoff ? 0 : 1;
    onoff_msg_param[0] = onoff;
    onoff_msg_param[1] = 0;     // tid
    onoff_msg_param[2] = 0;     // transition time
    onoff_msg_param[3] = 0;     // delay
    onoff_msg.param = onoff_msg_param;
    onoff_msg.param_len = 4;

    smacc_publish_msg(g_nrf_switch_elt.elt_idx, 0, &onoff_msg);

    g_nrf_switch_elt.onoff = onoff;
}

bool timer_start = false;
static void _nrf_switch_button_event_handler(uint8_t pin_no, uint8_t button_action)
{
#if 1
    static app_timer_id_t timer_id;

    if (button_action == APP_BUTTON_PUSH)
    {
        if (timer_start == false)
        {
            timer_id = (app_timer_id_t)smport_malloc(sizeof(app_timer_t), SM_MEM_RETENTION);
            sm_memset(timer_id, 0x00, sizeof(app_timer_t));
            app_timer_create(&timer_id, APP_TIMER_MODE_REPEATED, (app_timer_timeout_handler_t)_nrf_switch_on_off);
            app_timer_start(timer_id, APP_TIMER_TICKS((200 * 10), 0), NULL);
        }
        else
        {
            app_timer_stop(timer_id);
            smport_free(timer_id);
        }
    }
#else
    
    if (button_action == APP_BUTTON_PUSH)
    {
        _nrf_switch_on_off();
    }
#endif
}

void nrf_switch_init(void)
{
    static app_button_cfg_t buttons[] =
    {
        {NRF_SWITCH_PIN, false, NRF_GPIO_PIN_PULLUP, _nrf_switch_button_event_handler},
    };
    
    app_button_init(buttons, 1, APP_TIMER_TICKS(50, 0));
    app_button_enable();
    
    sm_memset(&g_nrf_switch_elt, 0x00, sizeof(g_nrf_switch_elt));
    
    g_nrf_switch_elt.elt_idx = smacc_reg_element(sizeof(g_nrf_switch_mid)/sizeof(sm_mid_t), g_nrf_switch_mid, 0, NULL, 
                                                    _nrf_switch_on_acc_msg);
}


