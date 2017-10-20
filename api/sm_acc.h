/** 
 * @file             sm_acc.h 
 * @brief            Access layer interfaces
 * @author           chenjian
 * @date             2017.09.01
 * @version          1.0.0.0 
 * @par              Copyright (c): BLX corp. 2017-2018
 */

#ifndef _SM_ACC_H_
#define _SM_ACC_H_

/**
 * Access layer status
 */
typedef enum smacc_status_tag
{
    SMACC_STATUS_SUCCESS                       = 0x00,
    SMACC_STATUS_INVALID_ADDRESS,
    SMACC_STATUS_INVALID_MODEL,
    SMACC_STATUS_INVALID_APPKEY,
    SMACC_STATUS_INVALID_NETKEY,
    SMACC_STATUS_INSUFFICIENT_RESOURCES,
    SMACC_STATUS_KEY_IDX_ALREADY_STORED,
    SMACC_STATUS_NOT_PUBLISH_MODEL,
    SMACC_STATUS_NOT_SUBSCRIBE_MODEL,
    SMACC_STATUS_STORAGE_FAILURE,
    SMACC_STATUS_FEATURE_NOT_FOUND,
    SMACC_STATUS_CANNOT_UPDATE,
    SMACC_STATUS_CANNOT_REMOVE,
    SMACC_STATUS_PERIODIC_NOT_SUPPORTED,
    SMACC_STATUS_CANNOT_BIND,
    SMACC_STATUS_TEMPORARILY_UNABLE_TO_CHANGE_STATE,
    SMACC_STATUS_CANNOT_SET,
    SMACC_STATUS_UNSPECIFIED_ERROR,

    SMACC_STATUS_RELAY_STATE_CHANGED           = 0xF0,
    SMACC_STATUS_BUSY,
    SMACC_STATUS_MSG_DROPED,
    SMACC_STATUS_NO_PROCESS,
    SMACC_STATUS_FAIL,
} smacc_status_t;


/**
 * access message indication callback prototype
 * @param[in]  elt_idx      Element index. The index comes from return value of @smacc_reg_element()
 * @param[in]  msg          access message pointer
 * @param[in]  src_addr     source address
 * @param[in]  appkey_idx   index of application key the message encrypted with
 * @param[in]  rssi         RSSI of the message packet, if the message comes from a proxy node, the rssi should be -127
 */
typedef void (*smacc_msg_ind_cbk_t)(uint8_t elt_idx, smacc_msg_t* msg, sm_addr_t src_addr, int32_t appkey_idx, int8_t rssi);

/**
 * Register element to access layer
 * @param[in]  mid_num      SIG-model ID total number
 * @param[in]  mid_arr      SIG-model ID array
 * @param[in]  vmid_num     vendor-model ID total number
 * @param[in]  vmid_arr     vendor-model ID array
 * @param[in]  msg_ind_cbk  callback function for access messages
 * @retval     the element index
 */
uint8_t smacc_reg_element(uint8_t mid_num, const sm_mid_t* const mid_arr,
                                    uint8_t vmid_num, const sm_vmid_t* const vmid_arr,
                                    smacc_msg_ind_cbk_t msg_ind_cbk);

/**
 * Get total element number
 * @retval     total element number
 */
uint8_t smacc_get_element_num(void);

/**
 * Get element address
 * @param[in]  elt_idx      element index
 * @param[out] addr         returned element address, shall not be NULL
 * @retval     SMACC_STATUS_SUCCESS success
 * @retval     SMACC_STATUS_FAIL    fail
 */
smacc_status_t smacc_get_element_addr(uint8_t elt_idx, sm_addr_t* addr);

/**
 * Get model identifiers
 * @param[in]  elt_idx      element index
 * @param[out] mid_num      returned SIG-model ID total number
 * @param[in]  mid_arr      returned SIG-model ID array
 * @param[in]  vmid_num     returned vendor-model ID total number
 * @param[in]  vmid_arr     returend vendor-model ID array
 */
void smacc_get_mid(uint8_t elt_idx, uint8_t* mid_num, sm_mid_t** mid_arr, uint8_t* vmid_num, sm_vmid_t** vmid_arr);

/**
 * Publish a message
 * @param[in]  elt_idx      index of element which contains the source model
 * @param[in]  model_idx    index of model, this index is the index of model array which is passed to 
 *                          @smacc_reg_element(), if the model is a SIG-model, the index shall be 0-based,
 *                          if the model is a vendor-model, the index shall be (mid_num)-based.
 * @param[in]  msg          published message
 * @retval     SMACC_STATUS_SUCCESS success
 * @retval     SMACC_STATUS_INVALID_MODEL invalid model
 * @retval     SMACC_STATUS_INVALID_ADDRESS invalid element
 */
smacc_status_t smacc_publish_msg(uint8_t elt_idx, uint8_t model_idx, smacc_msg_t* msg);

/**
 * Respond a message
 * @param[in]  elt_idx      index of element which contains the source model
 * @param[in]  dst          destination address which comes from parameter 'source address' of 
 *                          message indication callback
 * @param[in]  appkey_idx   index of applicaiton key to encrypt this message, -1 means device key
 * @param[in]  msg          response message
 * @retval     SMACC_STATUS_SUCCESS success
 * @retval     SMACC_STATUS_INVALID_ADDRESS invalid element
 */
smacc_status_t smacc_respond_msg(uint8_t elt_idx, sm_addr_t dst, int32_t appkey_idx, smacc_msg_t* msg);

/**
 * Unicast a message
 * @param[in]  elt_idx      index of element which contains the source model
 * @param[in]  dst          destination address
 * @param[in]  msg          unicasted message
 * @retval     SMACC_STATUS_SUCCESS success
 * @retval     SMACC_STATUS_INVALID_ADDRESS invalid element
 */
smacc_status_t smacc_dev_unicast_msg(uint8_t elt_idx, sm_addr_t dst, smacc_msg_t* msg);

#endif
