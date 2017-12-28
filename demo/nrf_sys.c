#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "sm_config.h"
#include "sm_common.h"
#include "sm_port.h"

#if (SM_PORT_PLATFORM == SM_PORT_NORDIC)
#include "mem_manager.h"
#include "app_util_platform.h"
#include "app_timer.h"

#include "nrf_ipc.h"

typedef struct
{
    app_timer_t             app_timer;
    smport_timer_handler    handler;
    void*                   context;
} nrf_sys_timer_t;

static void _nrf_sys_timer_handler(void* arg)
{
    nrf_sys_timer_t* sys_timer = (nrf_sys_timer_t*)arg;
    nrf_ipc_timer_expired_param_t timer_exp;

    timer_exp.handler = sys_timer->handler;
    timer_exp.param = sys_timer->context;

    nrf_ipc_write(NRF_IPC_TAG_TIMER_EXPIRED, (nrf_ipc_write_param_t*)&timer_exp);
}

void nrf_sys_timer_expired(smport_timer_handler handler, void* param)
{
    if (handler != NULL)
    {
        handler(param);
    }
}

void* smport_malloc(uint16_t size, uint8_t type)
{
    return nrf_malloc(size);
}

void smport_free(void* mem)
{
    nrf_free(mem);
}

void smport_init_timer(void)
{
    APP_TIMER_INIT(0, 32, NULL); // 10ms, 64 queue size
}

void* smport_create_timer()
{
    app_timer_id_t timer_id;
    nrf_sys_timer_t* sys_timer;


    sys_timer = smport_malloc(sizeof(nrf_sys_timer_t), SM_MEM_RETENTION);

    if (sys_timer == NULL)
        return NULL;

    timer_id = (app_timer_id_t)&sys_timer->app_timer;

    sm_memset(timer_id, 0x00, sizeof(app_timer_t));
    
    app_timer_create(&timer_id, APP_TIMER_MODE_SINGLE_SHOT, (app_timer_timeout_handler_t)_nrf_sys_timer_handler);

    return (void*)timer_id;
}

void smport_start_timer(void* timer, uint32_t timeout, smport_timer_handler handler, void* context)
{
    nrf_sys_timer_t* sys_timer = (nrf_sys_timer_t*)timer;
    uint32_t ticks = APP_TIMER_TICKS((timeout * 10), 0);

    sys_timer->handler = (void*)handler;
    sys_timer->context = context;

    if (ticks < APP_TIMER_MIN_TIMEOUT_TICKS)
        ticks = APP_TIMER_MIN_TIMEOUT_TICKS;
    
    app_timer_start((app_timer_id_t)timer, ticks, (void*)sys_timer);
}

void smport_stop_timer(void* timer)
{
    app_timer_stop((app_timer_id_t)timer);
}

void smport_clear_timer(void* timer)
{
    smport_free(timer);
}

void smport_enter_critical(uint32_t* cr)
{
    CRITICAL_REGION_ENTER() 
    *cr = __CR_NESTED;}
}

void smport_exit_critical(uint32_t cr)
{
    { uint8_t __CR_NESTED;
    __CR_NESTED = (uint8_t)cr;
    CRITICAL_REGION_EXIT();
}

#endif
