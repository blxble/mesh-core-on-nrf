/** 
 * @file             sm_acc_msg.h 
 * @brief            Access message interfaces
 * @author           chenjian
 * @date             2017.09.01
 * @version          1.0.0.0 
 * @par              Copyright (c): BLX corp. 2017-2018
 */
#ifndef _SM_ACC_MSG_H_
#define _SM_ACC_MSG_H_


#define SMACC_MSG_VMID_SIG                                      (0xFFFF)   ///< not official definition
#define SMACC_MSG_MID2VMID(mid)                                 (((sm_vmid_t)(mid)) | ((sm_vmid_t)SMACC_MSG_VMID_SIG << 16))
#define SMACC_MSG_IS_VMID(mid)                                  ((((mid) >> 16) & SMACC_MSG_VMID_SIG) == SMACC_MSG_VMID_SIG)

/**
 * access message definition
 */
typedef struct smacc_msg_tag
{
    sm_msg_opcode_t                         opcode;             ///< message opcode
    uint8_t*                                param;              ///< message payload
    uint16_t                                param_len;          ///< payload length
} smacc_msg_t;

/**
 * access message period resolution
 */
typedef enum smacc_msg_period_resolution_tag
{
    SMACC_MSG_PERIOD_RES_100MS,
    SMACC_MSG_PERIOD_RES_1S,
    SMACC_MSG_PERIOD_RES_10S,
    SMACC_MSG_PERIOD_RES_10MIN
} smacc_msg_period_resolution_t;

typedef struct
{
    smacc_msg_period_resolution_t           resolution;
    uint8_t                                 steps;          // 0 ~ 0x3F, 0 means disable
} smacc_msg_period_t;

/**
 * Package an access message
 * @param[in]  msg      message to be packaged
 * @param[in]  msg_buf  returned buffer, this buffer shall be freed when useless
 * @retval     buffer length
 */
uint16_t smacc_pack_msg(smacc_msg_t* msg, uint8_t** msg_buf);

/**
 * Unpackage an access message
 * @param[in]  msg_buf   message buffer
 * @param[in]  msg_len   message buffer length
 * @param[out] msg       returned message, the message param shall be freed when useless
 */
void smacc_unpack_msg(uint8_t* msg_buf, uint8_t msg_len, smacc_msg_t* msg);

#endif

