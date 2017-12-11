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

void* smport_create_timer(smport_timer_handler handler)
{
    app_timer_id_t timer_id;

    timer_id = (app_timer_id_t)smport_malloc(sizeof(app_timer_t), SM_MEM_RETENTION);

    memset(timer_id, 0x00, sizeof(app_timer_t));
    
    app_timer_create(&timer_id, APP_TIMER_MODE_SINGLE_SHOT, (app_timer_timeout_handler_t)handler);

    return (void*)timer_id;
}

void smport_start_timer(void* timer, uint32_t timeout, void* context)
{
    uint32_t ticks = APP_TIMER_TICKS((timeout * 10), 0);

    if (ticks < APP_TIMER_MIN_TIMEOUT_TICKS)
        ticks = APP_TIMER_MIN_TIMEOUT_TICKS;
    
    app_timer_start((app_timer_id_t)timer, ticks, context);
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
