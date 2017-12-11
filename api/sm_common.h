#ifndef _SM_COMMON_H_
#define _SM_COMMON_H_
#include <stdlib.h>
#include <stdio.h>
#include "string.h"

#define SM_UUID_SIZE                (16)
#define SM_KEY_SIZE                 (16)
#define SM_NETWORK_ID_SIZE          (8)
#define SM_NODE_ID_HASH_SIZE        (8)
#define SM_NODE_ID_RAND_SIZE        (8)

#define SM_ADV_TYPE_MESH_BEACON     (0x2B)
#define SM_ADV_TYPE_PB_ADV          (0x29)
#define SM_ADV_TYPE_CORE_MSG        (0x2A)
#define SM_PROVISION_SERVICE_UUID   (ATT_SVC_PROVS)
#define SM_PROXY_SERVICE_UUID       (ATT_SVC_PROXYS)

#define SM_ADDR_UNASSIGNED          (0x0000)
#define SM_ADDR_ALL_PROXIES         (0xFFFC)
#define SM_ADDR_ALL_FRIENDS         (0xFFFD)
#define SM_ADDR_ALL_RELAYS          (0xFFFE)
#define SM_ADDR_BROADCAST           (0xFFFF)
#define SM_ADDR_IS_UNASSIGNED(addr) (addr == SM_ADDR_UNASSIGNED)
#define SM_ADDR_IS_BROADCAST(addr)  (addr == SM_ADDR_BROADCAST)
#define SM_ADDR_IS_ALL_FRIENDS(addr) (addr == SM_ADDR_ALL_FRIENDS)
#define SM_ADDR_IS_UNICAST(addr)    ((addr & 0x8000) == 0)
#define SM_ADDR_IS_GROUP(addr)      (((addr & 0xC000) == 0xC000) && (addr != SM_ADDR_BROADCAST))
#define SM_ADDR_IS_VIRTUAL(addr)    ((addr & 0xC000) == 0x8000)

#if (SM_MESH_DEBUG)
#include "log.h"
#define SM_ASSERT_ERR               ASSERT_ERR
#define SM_LOG_INFO                 LOG_IPRINT
#define SM_DUMP_DATA(data, len)                         \
                                    do {                \
                                        uint32_t i = 0;                         \
                                        LOG_PRINT("\t dump data:");             \
                                        for (i = 0;i < len;i++)                 \
                                        {                                       \
                                            LOG_PRINT("0x%02X ", data[i]);      \
                                        }                                       \
                                        LOG_PRINT("\n\r");                      \
                                    } while (0)
#else
#define SM_ASSERT_ERR(conf)
#define SM_LOG_INFO(format,...)
#define SM_DUMP_DATA(data, len)
#endif

#define SM_MEM_NON_RETENTION                    (0)
#define SM_MEM_RETENTION                        (1)
#define SM_MEM_ALLOC(size, type)                ke_malloc(size, type)
#define SM_MEM_FREE(p)                          ke_free(p)

typedef uint32_t    sm_link_id_t;
typedef uint8_t     sm_net_id_t;
typedef uint16_t    sm_addr_t;
typedef uint8_t     sm_virt_addr_lab_t[16];
typedef uint32_t    sm_ivi_t;

typedef uint16_t    sm_mid_t;       // model id
typedef uint32_t    sm_vmid_t;      // vendor model id

typedef struct
{
    uint8_t         addr[6];
    uint8_t         addr_type;
} sm_bdaddr_t;

typedef struct
{
    uint8_t         uuid[SM_UUID_SIZE];
} sm_uuid_t;

typedef struct
{
    uint16_t                cid;
    uint16_t                pid;
    uint16_t                vid;
} sm_device_info_t;

typedef struct
{
    uint8_t         op0;
    uint8_t         op1;
    uint8_t         op2;
} sm_msg_opcode_t;

#endif
