/** 
 * @file             sm_ui.h 
 * @brief            User interfaces
 * @details 	     This file defines the basic operations for a mesh node. Callers 
 *                   could organize, manage or access a mesh network utilizing these
 *                   interfaces.
 * @author           chenjian
 * @date             2017.09.01
 * @version          1.0.0.0 
 * @par              Copyright (c): BLX corp. 2017-2018
 */

#ifndef _SM_UI_H_
#define _SM_UI_H_

/**
 * Notices returned by mesh core
 */
typedef enum
{
    SMUI_NOTICE_NET_CREATED,
    SMUI_NOTICE_NET_JOINT,
    SMUI_NOTICE_BACK_TO_NET,
    SMUI_NOTICE_NEW_DEVICE_DETECTED,
    SMUI_NOTICE_ATTENTION,
    SMUI_NOTICE_READ_PUBKEY_OOB,
    SMUI_NOTICE_SHOW_AUTH_VALUE,
    SMUI_NOTICE_REQ_AUTH_VALUE,
    SMUI_NOTICE_SEC_KEY_UPDATED,
    SMUI_NOTICE_NEW_DEVICE_ADDED,
    SMUI_NOTICE_CONFIG_DONE,
    SMUI_NOTICE_TOPOLOGICAL_RELATION,
} smui_notice_t;

/**
 * Secure key type
 */
enum
{
    SMUI_KEYTYPE_NETKEY,
    SMUI_KEYTYPE_APPKEY,
};

enum
{
    SMUI_AUTH_TYPE_OUT_OOB,
    SMUI_AUTH_TYPE_IN_OOB,
};

/**
 * Authentication actions
 */
enum
{
    SMUI_AUTH_ACT_OUTPUT_BLINK        = 0,
    SMUI_AUTH_ACT_OUTPUT_BEPP         = 1,
    SMUI_AUTH_ACT_OUTPUT_VIBRATE      = 2,
    SMUI_AUTH_ACT_OUTPUT_NUMERIC      = 3,
    SMUI_AUTH_ACT_OUTPUT_ALPHANUMERIC = 4,

    SMUI_AUTH_ACT_INPUT_PUSH          = 0,
    SMUI_AUTH_ACT_INPUT_TWIST         = 1,
    SMUI_AUTH_ACT_INPUT_NUMBER        = 2,
    SMUI_AUTH_ACT_INPUT_ALPHANUMERIC  = 3,
};

/**
 * Configuration operations
 */
enum
{
    SMUI_CONF_OP_UNKOWN               = 0,
        
    SMUI_CONF_OP_ADD_APPKEY,                    ///< admin adds a application key to node
    SMUI_CONF_OP_SET_MOD_PUBLICATION,           ///< admin sets publish paramters to node model
    SMUI_CONF_OP_ADD_MOD_SUBSCRIPTION,          ///< admin adds subscription list to node model
    SMUI_CONF_OP_BIND_MOD_APPKEY,               ///< admin binds a application with node model
};

enum
{
    SMUI_OOB_INFO_OTHER                          = 0,
    SMUI_OOB_INFO_ELECTONIC_URI                  = 1,
    SMUI_OOB_INFO_2D_MACHINE_READABLE_CODE       = 2,
    SMUI_OOB_INFO_BAR_CODE                       = 3,
    SMUI_OOB_INFO_NFC                            = 4,
    SMUI_OOB_INFO_NUMBER                         = 5,
    SMUI_OOB_INFO_STRING                         = 6,
    ///< 7 - 10 reserved for future use
    SMUI_OOB_INFO_ON_BOX                         = 11,
    SMUI_OOB_INFO_INSIDE_BOX                     = 12,
    SMUI_OOB_INFO_ON_PIECE_PAPER                 = 13,
    SMUI_OOB_INFO_INSIDE_MANUAL                  = 14,
    SMUI_OOB_INFO_ON_DEVICE                      = 15
};

/**
 * Paramter of notice @SMUI_NOTICE_NET_CREATED
 */
typedef struct
{
    bool                        success;
} smui_net_created_param_t;

/**
 * Paramter of notice @SMUI_NOTICE_NET_JOINT
 */
typedef struct
{
    bool                        success;
} smui_net_joint_param_t;

/**
 * Paramter of notice @SMUI_NOTICE_BACK_TO_NET
 */
typedef struct
{
    bool                        success;
} smui_back_to_net_param_t;

/**
 * Paramter of notice @SMUI_NOTICE_NEW_DEVICE_DETECTED
 */
typedef struct
{
    sm_uuid_t                   uuid;                   ///< uuid of detected device
    sm_bdaddr_t                 bd;                     ///< bluetooth address
    bool                        pb_flag;                ///< true: pb-gatt, false: pb-adv
    uint8_t*                    ext_data;               ///< extended data contained in device's beacon
    uint8_t                     ext_data_len;           ///< extended data length
    uint8_t                     rssi;                   ///< RSSI of of device's beacon
} smui_new_device_detected_param_t;

/**
 * Paramter of notice @SMUI_NOTICE_ATTENTION
 */
typedef struct
{
    uint8_t                     duration;               ///< provision attention time in seconds, 0 means attention off
} smui_attention_param_t;

/**
 * Paramter of notice @SMUI_NOTICE_SEC_KEY_UPDATED
 */
typedef struct
{
    uint8_t                     key_type;               ///< @SMUI_KEYTYPE_NETKEY / @SMUI_KEYTYPE_APPKEY
    uint16_t                    idx;                    ///< key index
} smui_key_updated_param_t;

/**
 * Paramter of notice @SMUI_NOTICE_SHOW_AUTH_VALUE
 */
typedef struct
{
    uint8_t                     auth_type;              ///< @SMUI_AUTH_TYPE_OUT_OOB / @SMUI_AUTH_TYPE_IN_OOB
    uint8_t                     auth_act;               ///< reference SMUI_AUTH_ACT_xxx
    uint8_t                     auth_size;              ///< authentication data size
    uint8_t*                    data;                   ///< authentication data
} smui_show_auth_value_param_t;

/**
 * Paramter of notice @SMUI_NOTICE_REQ_AUTH_VALUE
 */
typedef struct
{
    uint8_t                     auth_type;              ///< @SMUI_AUTH_TYPE_OUT_OOB / @SMUI_AUTH_TYPE_IN_OOB
    uint8_t                     auth_act;               ///< reference SMUI_AUTH_ACT_xxx
    uint8_t                     auth_size;              ///< authentication data size
} smui_request_auth_value_param_t;

/**
 * Paramter of notice @SMUI_NOTICE_NEW_DEVICE_ADDED
 */
typedef struct
{
    bool                        success;                ///< provision success or not
    sm_device_info_t            dev_info;               ///< device information 
    sm_addr_t                   dev_addr;               ///< device address
} smui_new_device_added_param_t;

/**
 * Paramter of notice @SMUI_NOTICE_CONFIG_DONE
 */
typedef struct
{
    sm_addr_t                   addr;                   ///< device address
    bool                        success;                ///< success or not
    uint8_t                     conf_op;                ///< configuration operation, reference SMUI_CONF_OP_xxx
} smui_config_done_param_t;

typedef struct smui_topological_relation_tag
{
    sm_addr_t                   neighbor_addr;
    int8_t                      rssi;
} smui_topological_arc_t;

typedef struct smui_topology_relation_tag
{
    sm_addr_t                   node_addr;
    uint16_t                    arc_num;
    smui_topological_arc_t*     arc;
} smui_topology_relation_t;

typedef struct
{
    uint16_t                    relation_num;
    smui_topology_relation_t*   relations;
} smui_topology_relation_param_t;

/**
 * callback function prototype
 * @param[out]  notice    notified by mesh core
 * @param[out]  param     notice parameter
 */
typedef void (*smui_cbk_t)(smui_notice_t notice, void* param);

/**
 * Initialize mesh core
 */
void smui_init(void);

/**
 * Reset mesh core
 */
void smui_reset(void);

/**
 * Register a callback function to mesh core
 * @param[in]  cbk       callback function pointer
 */
void smui_reg_callback(smui_cbk_t cbk);

/**
 * Request to join a mesh network
 * This action leads to broadcast a beacon
 * @param[in]  timeout  action timeout, 0 for infinite
 * @param[in]  oob_info out of band information, see @SMUI_OOB_INFO_xxx
 */
void smui_join_net(uint32_t timeout, uint16_t oob_info);

/**
 * Wait to back a network
 * @param[in]  timeout  waiting timeout
 */
void smui_back_to_net(uint32_t timeout);

/**
 * Enable device as a proxy server
 * This device will be a proxy server connected by a client such as mobile phone
 */
void smui_enable_proxy(void);

/**
 * Set authentication data to mesh core
 * When input OOB authentication selected, use this function to pass the authentication data
 * which size should tally with the size requested by notice @SMUI_NOTICE_REQ_AUTH_VALUE 
 * @param[in]  size      size of authentication data
 * @param[in]  value     authentication data
 */
void smui_set_auth_value(uint8_t size, uint8_t* value);
#if (SM_PROVISIONER_SUPPORT)
/**
 * Create a mesh main network
 * The creator becomes provisioner after network created. A node shall create one network at most.
 * Notice @SMUI_NOTICE_NET_CREATED tells the creating result.
 */
void smui_create_net(void);

/**
 * Create a sub-net
 * The creator should also be the provisioner of the main network. A main network could support 65535
 * sub-nets at most, meanwhile depending on system resource. Notice @SMUI_NOTICE_NET_CREATED tells 
 * the creating result.
 */
void smui_create_subnet(void);

/**
 * Provisioner grants a new device's joining network proposal.
 * The parameters could be obtained via notice @SMUI_NOTICE_NEW_DEVICE_DETECTED.
 * @param[in]  uuid      proposer uuid
 * @param[in]  bd        proposer bluetooth address
 * @param[in]  pb_flag   provision bearer type, true: pb-gatt, false: pb-adv
 */
void smui_grant_new_proposer(sm_uuid_t* uuid, sm_bdaddr_t* bd, bool pb_flag);

/**
 * Provisioner sets OOB public key to mesh core
 * When OOB authentication selected, this funciton is used to pass the public key
 * @param[in]  x          X-coordinate of public key, 32 bytes
 * @param[in]  y          Y-coordinate of public key, 32 bytes
 */
void smui_set_oob_pubkey(uint8_t* x, uint8_t* y);

/**
 * Administrator generates a new application key
 * An application key should be bound with a network key.
 * @param[in]  netkey_idx index of network key which new application key bound with
 * @retval     new application key index
 */
uint16_t smui_new_appkey(uint16_t netkey_idx);

/**
 * Administrator add an application key to a node
 * The reuslt of this operation would be returned via notice @SMUI_NOTICE_CONFIG_DONE
 * with parameter @SMUI_CONF_OP_ADD_APPKEY.
 * @param[in]  dev_addr   device address of target node
 * @param[in]  netkey_idx index of network key which application key bound with
 * @param[in]  appkey_idx applicaiton key index
 */
void smui_add_device_appkey(sm_addr_t dev_addr, uint16_t netkey_idx, uint16_t appkey_idx);

/**
 * Administrator sets publish paramters to a node model
 * The result of this operation would be returned via notice @SMUI_NOTICE_CONFIG_DONE
 * with parameter @SMUI_CONF_OP_SET_MOD_PUBLICATION.
 * @param[in]  dev_addr    device address of target node
 * @param[in]  elt_addr    element address which contains tha model
 * @param[in]  mid         model identifier, if it is a SIG-model, transfer its id
 *                         to vendor-model format fristly utilizing @SMACC_MSG_MID2VMID()
 * @param[in]  pub_addr    publish address which the message published to
 * @param[in]  appkey_idx  index of application key which the published message encrypted with
 * @param[in]  ttl         time to live of the published message
 */
void smui_set_device_mod_publication(sm_addr_t dev_addr, sm_addr_t elt_addr, sm_vmid_t mid, 
                                                        sm_addr_t pub_addr, uint16_t appkey_idx, uint8_t ttl);

/**
 * Administrator adds a subscribe address to a node model
 * The result of this operation would be returned via notice @SMUI_NOTICE_CONFIG_DONE
 * with parameter @SMUI_CONF_OP_ADD_MOD_SUBSCRIPTION.
 * @param[in]  dev_addr    device address of target node
 * @param[in]  elt_addr    element address which contains tha model
 * @param[in]  mid         model identifier, if it is a SIG-model, transfer its id
 *                         to vendor-model format fristly utilizing @SMACC_MSG_MID2VMID()
 * @param[in]  subs_addr   subscribed address
 */
void smui_add_device_mod_subscription(sm_addr_t dev_addr, sm_addr_t elt_addr, sm_vmid_t mid, sm_addr_t subs_addr);

/**
 * Administrator binds an application key with model
 * The result of this operation would be returned via notice @SMUI_NOTICE_CONFIG_DONE
 * with parameter @SMUI_CONF_OP_BIND_MOD_APPKEY.
 * @param[in]  dev_addr    device address of target node
 * @param[in]  elt_addr    element address which contains tha model
 * @param[in]  mid         model identifier, if it is a SIG-model, transfer its id
 *                         to vendor-model format fristly utilizing @SMACC_MSG_MID2VMID()
 * @param[in]  appkey_idx  application key index
 */
void smui_bind_device_mod_appkey(sm_addr_t dev_addr, sm_addr_t elt_addr, sm_vmid_t mid, uint16_t appkey_idx);
#endif
#if (SM_LOW_POWER_NODE_SUPPORT)
/**
 * Enable low power mode
 * The device will be set to low power node after enabled. A LPN shall not be a friend
 * node or a relay node simultaneously.
 * @param[in]  avg_msg_num_per_sec average message number per second when entering low 
 *                                 power mode
 * @param[in]  poll_timeout        the communication interval between LPN and FN, the 
 *                                 friendship would be terminated when message missing
 *                                 within 3 times of poll timeout
 * @param[in]  min_subs_size       minimum subscription list size
 */
void smui_enable_low_power_mode(uint8_t avg_msg_num_per_sec, uint32_t poll_timeout, uint8_t min_subs_size);
#endif
#if (SM_TOPOLOGY_DETECT_SUPPORT)
void smui_detect_topology(bool adaptive_param_mode, bool topology_report);
#endif
#endif

