#ifndef _SM_MOD_MSG_H_
#define _SM_MOD_MSG_H_

#define SMMOD_MSG_OPCODE_APPKEY_ADD                                             {0x00, 0x00, 0x00}
#define SMMOD_MSG_OPCODE_APPKEY_UPDATE                                          {0x01, 0x00, 0x00}
#define SMMOD_MSG_OPCODE_DEV_COMPOSITION_DATA_STATUS                            {0x02, 0x00, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_MODEL_PUBLICATION_SET                           {0x03, 0x00, 0x00}
#define SMMOD_MSG_OPCODE_HEALTH_CURRENT_STATUS                                  {0x04, 0x00, 0x00}
#define SMMOD_MSG_OPCODE_HEALTH_FAULT_STATUS                                    {0x05, 0x00, 0x00}
#define SMMOD_MSG_OPCODE_HEARTBEAT_PUBLICATION_STATUS                           {0x06, 0x00, 0x00}
#define SMMOD_MSG_OPCODE_APPKEY_DELETE                                          {0x80, 0x00, 0x00}
#define SMMOD_MSG_OPCODE_APPKEY_GET                                             {0x80, 0x01, 0x00}
#define SMMOD_MSG_OPCODE_APPKEY_LIST                                            {0x80, 0x02, 0x00}
#define SMMOD_MSG_OPCODE_APPKEY_STATUS                                          {0x80, 0x03, 0x00}
#define SMMOD_MSG_OPCODE_HEALTH_ATTENTION_GET                                   {0x80, 0x04, 0x00}
#define SMMOD_MSG_OPCODE_HEALTH_ATTENTION_SET                                   {0x80, 0x05, 0x00}
#define SMMOD_MSG_OPCODE_HEALTH_ATTENTION_SET_UNACKNOWLEDGED                    {0x80, 0x06, 0x00}
#define SMMOD_MSG_OPCODE_HEALTH_ATTENTION_STATUS                                {0x80, 0x07, 0x00}
#define SMMOD_MSG_OPCODE_DEV_COMPOSITION_DATA_GET                               {0x80, 0x08, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_BEACON_GET                                      {0x80, 0x09, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_BEACON_SET                                      {0x80, 0x0A, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_BEACON_STATUS                                   {0x80, 0x0B, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_DEFAULT_TTL_GET                                 {0x80, 0x0C, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_DEFAULT_TTL_SET                                 {0x80, 0x0D, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_DEFAULT_TTL_STATUS                              {0x80, 0x0E, 0x00}
#define SMMOD_MSG_OPCODE_FRIEND_GET                                             {0x80, 0x0F, 0x00}
#define SMMOD_MSG_OPCODE_FRIEND_SET                                             {0x80, 0x10, 0x00}
#define SMMOD_MSG_OPCODE_FRIEND_STATUS                                          {0x80, 0x11, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_PROXY_GET                                       {0x80, 0x12, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_PROXY_SET                                       {0x80, 0x13, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_PROXY_STATUS                                    {0x80, 0x14, 0x00}
#define SMMOD_MSG_OPCODE_KEY_REFRESH_PHASE_GET                                  {0x80, 0x15, 0x00}
#define SMMOD_MSG_OPCODE_KEY_REFRESH_PHASE_SET                                  {0x80, 0x16, 0x00}
#define SMMOD_MSG_OPCODE_KEY_REFRESH_PHASE_STATUS                               {0x80, 0x17, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_MODEL_PUBLICATION_GET                           {0x80, 0x18, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_MODEL_PUBLICATION_STATUS                        {0x80, 0x19, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_MODEL_PUBLICATION_VIRTUAL_ADDR_SET              {0x80, 0x1A, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_MODEL_SUBSCRIPTION_ADD                          {0x80, 0x1B, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_MODEL_SUBSCRIPTION_DELETE                       {0x80, 0x1C, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_MODEL_SUBSCRIPTION_DELETE_ALL                   {0x80, 0x1D, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_MODEL_SUBSCRIPTION_OVERWRITE                    {0x80, 0x1E, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_MODEL_SUBSCRIPTION_STATUS                       {0x80, 0x1F, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_MODEL_SUBSCRIPTION_VIRTUAL_ADDR_ADD             {0x80, 0x20, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_MODEL_SUBSCRIPTION_VIRTUAL_ADDR_DELETE          {0x80, 0x21, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_MODEL_SUBSCRIPTION_VIRTUAL_ADDR_OVERWRITE       {0x80, 0x22, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_NETWORK_TRANSMIT_GET                            {0x80, 0x23, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_NETWORK_TRANSMIT_SET                            {0x80, 0x24, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_NETWORK_TRANSMIT_STATUS                         {0x80, 0x25, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_RELAY_GET                                       {0x80, 0x26, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_RELAY_SET                                       {0x80, 0x27, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_RELAY_STATUS                                    {0x80, 0x28, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_MODEL_SUBSCRIPTION_GET                          {0x80, 0x29, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_MODEL_SUBSCRIPTION_LIST                         {0x80, 0x2A, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_VENDOR_MODEL_SUBSCRIPTION_GET                   {0x80, 0x2B, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_VENDOR_MODEL_SUBSCRIPTION_LIST                  {0x80, 0x2C, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_LOW_POWER_NODE_POLLTIMEOUT_GET                  {0x80, 0x2D, 0x00}
#define SMMOD_MSG_OPCODE_CONFIG_LOW_POWER_NODE_POLLTIMEOUT_STATUS               {0x80, 0x2E, 0x00}
#define SMMOD_MSG_OPCODE_HEALTH_FAULT_CLEAR                                     {0x80, 0x2F, 0x00}
#define SMMOD_MSG_OPCODE_HEALTH_FAULT_CLEAR_UNACKNOWLEDGED                      {0x80, 0x30, 0x00}
#define SMMOD_MSG_OPCODE_HEALTH_FAULT_GET                                       {0x80, 0x31, 0x00}
#define SMMOD_MSG_OPCODE_HEALTH_FAULT_TEST                                      {0x80, 0x32, 0x00}
#define SMMOD_MSG_OPCODE_HEALTH_FAULT_TEST_UNACKNOWLEDGED                       {0x80, 0x33, 0x00}
#define SMMOD_MSG_OPCODE_HEALTH_PERIOD_GET                                      {0x80, 0x34, 0x00}
#define SMMOD_MSG_OPCODE_HEALTH_PERIOD_SET                                      {0x80, 0x35, 0x00}
#define SMMOD_MSG_OPCODE_HEALTH_PERIOD_SET_UNACKNOWLEDGED                       {0x80, 0x36, 0x00}
#define SMMOD_MSG_OPCODE_HEALTH_PERIOD_STATUS                                   {0x80, 0x37, 0x00}
#define SMMOD_MSG_OPCODE_HEARTBEAT_PUBLICATION_GET                              {0x80, 0x38, 0x00}
#define SMMOD_MSG_OPCODE_HEARTBEAT_PUBLICATION_SET                              {0x80, 0x39, 0x00}
#define SMMOD_MSG_OPCODE_HEARTBEAT_SUBSCRIPTION_GET                             {0x80, 0x3A, 0x00}
#define SMMOD_MSG_OPCODE_HEARTBEAT_SUBSCRIPTION_SET                             {0x80, 0x3B, 0x00}
#define SMMOD_MSG_OPCODE_HEARTBEAT_SUBSCRIPTION_STATUS                          {0x80, 0x3C, 0x00}
#define SMMOD_MSG_OPCODE_MODEL_APP_BIND                                         {0x80, 0x3D, 0x00}
#define SMMOD_MSG_OPCODE_MODEL_APP_STATUS                                       {0x80, 0x3E, 0x00}
#define SMMOD_MSG_OPCODE_MODEL_APP_UNBIND                                       {0x80, 0x3F, 0x00}
#define SMMOD_MSG_OPCODE_NETKEY_ADD                                             {0x80, 0x40, 0x00}
#define SMMOD_MSG_OPCODE_NETKEY_DELETE                                          {0x80, 0x41, 0x00}
#define SMMOD_MSG_OPCODE_NETKEY_GET                                             {0x80, 0x42, 0x00}
#define SMMOD_MSG_OPCODE_NETKEY_LIST                                            {0x80, 0x43, 0x00}
#define SMMOD_MSG_OPCODE_NETKEY_STATUS                                          {0x80, 0x44, 0x00}
#define SMMOD_MSG_OPCODE_NETKEY_UPDATE                                          {0x80, 0x45, 0x00}
#define SMMOD_MSG_OPCODE_NODE_IDENTITY_GET                                      {0x80, 0x46, 0x00}
#define SMMOD_MSG_OPCODE_NODE_IDENTITY_SET                                      {0x80, 0x47, 0x00}
#define SMMOD_MSG_OPCODE_NODE_IDENTITY_STATUS                                   {0x80, 0x48, 0x00}
#define SMMOD_MSG_OPCODE_NODE_RESET                                             {0x80, 0x49, 0x00}
#define SMMOD_MSG_OPCODE_NODE_RESET_STATUS                                      {0x80, 0x4A, 0x00}
#define SMMOD_MSG_OPCODE_SIG_MODEL_APP_GET                                      {0x80, 0x4B, 0x00}
#define SMMOD_MSG_OPCODE_SIG_MODEL_APP_LIST                                     {0x80, 0x4C, 0x00}
#define SMMOD_MSG_OPCODE_VENDOR_MODEL_APP_GET                                   {0x80, 0x4D, 0x00}
#define SMMOD_MSG_OPCODE_VENDOR_MODEL_APP_LIST                                  {0x80, 0x4E, 0x00}


#define SMMOD_MSG_OPCODE_GENERIC_ONOFF_GET                                      {0x82, 0x01, 0x00}
#define SMMOD_MSG_OPCODE_GENERIC_ONOFF_SET                                      {0x82, 0x02, 0x00}
#define SMMOD_MSG_OPCODE_GENERIC_ONOFF_SET_UNACK                                {0x82, 0x03, 0x00}
#define SMMOD_MSG_OPCODE_GENERIC_ONOFF_STATUS                                   {0x82, 0x04, 0x00}

#define SMMOD_MSG_OPCODE_LIGHT_GET                                              {0xA0, 0x00, 0x00}
#define SMMOD_MSG_OPCODE_LIGHT_SET                                              {0xA0, 0x01, 0x00}
#define SMMOD_MSG_OPCODE_LIGHT_STATUS                                           {0xA0, 0x02, 0x00}
#define SMMOD_MSG_OPCODE_POWER_GET                                              {0xA0, 0x03, 0x00}
#define SMMOD_MSG_OPCODE_POWER_SET                                              {0xA0, 0x04, 0x00}
#define SMMOD_MSG_OPCODE_POWER_STATUS                                           {0xA0, 0x05, 0x00}
#define SMMOD_MSG_OPCODE_HUE_GET                                          		{0xA0, 0x06, 0x00}
#define SMMOD_MSG_OPCODE_HUE_SET                                          		{0xA0, 0x07, 0x00}
#define SMMOD_MSG_OPCODE_HUE_STATUS                                       		{0xA0, 0x08, 0x00}
#define SMMOD_MSG_OPCODE_LIGHTNESS_GET                                         	{0xA0, 0x09, 0x00}
#define SMMOD_MSG_OPCODE_LIGHTNESS_SET                                          {0xA0, 0x0A, 0x00}
#define SMMOD_MSG_OPCODE_LIGHTNESS_STATUS                                       {0xA0, 0x0B, 0x00}
#define SMMOD_MSG_OPCODE_SATURATION_GET                                         {0xA0, 0x0C, 0x00}
#define SMMOD_MSG_OPCODE_SATURATION_SET                                         {0xA0, 0x0D, 0x00}
#define SMMOD_MSG_OPCODE_SATURATION_STATUS                                      {0xA0, 0x0E, 0x00}

#define SMMOD_MSG_OPCODE_BELONGED_GROUP_ADD                                     {0xA2, 0x02, 0x00}
#define SMMOD_MSG_OPCODE_BELONGED_GROUP_DEL                                     {0xA2, 0x03, 0x00}
#define SMMOD_MSG_OPCODE_MEMBER_NUM_GET                                         {0xA2, 0x04, 0x00}
#define SMMOD_MSG_OPCODE_MEMBER_ADD                                             {0xA2, 0x05, 0x00}
#define SMMOD_MSG_OPCODE_MEMBER_DEL                                             {0xA2, 0x06, 0x00}
#define SMMOD_MSG_OPCODE_GROUP_MEMBERS_GET                                      {0xA2, 0x07, 0x00}
#define SMMOD_MSG_OPCODE_GROUP_MEMBERS_DEL                                      {0xA2, 0x08, 0x00}
#define SMMOD_MSG_OPCODE_ALL_MEMBERS_GET                                        {0xA2, 0x09, 0x00}
#define SMMOD_MSG_OPCODE_ALL_MEMBERS_DEL                                        {0xA2, 0x0A, 0x00}
#define SMMOD_MSG_OPCODE_MANAGER_STATUS                                         {0xA2, 0x0B, 0x00}
#define SMMOD_MSG_OPCODE_MEMBER_STATUS                                          {0xA2, 0x0C, 0x00}
#define SMMOD_MSG_OPCODE_BELONGED_GROUP_STATUS                                  {0xA2, 0x0D, 0x00}

#define SMMOD_MSG_OPCODE_NET_TIME_GET                                           {0xA3, 0x00, 0x00}
#define SMMOD_MSG_OPCODE_NET_TIME_STATUS                                        {0xA3, 0x01, 0x00}

#define SMMOD_MSG_OPCODE_ADD_MEMBER												{0xB0, 0x00, 0x00}
#define SMMOD_MSG_OPCODE_DEL_MEMBER												{0xB0, 0x01, 0x00}
#define SMMOD_MSG_OPCODE_START_BROADCAST										{0xB0, 0x02, 0x00}
#define SMMOD_MSG_OPCODE_BROADCAST												{0xB0, 0x03, 0x00}
#define SMMOD_MSG_OPCODE_CTRL_STATUS											{0xB0, 0x04, 0x00}
#define SMMOD_MSG_OPCODE_GET_GROUP												{0xB0, 0x05, 0x00}
#define SMMOD_MSG_OPCODE_GET_GROUP_STATUS										{0xB0, 0x06, 0x00}

#define SMMOD_MSG_OPCODE_SEND_DATA												{0xB1, 0x00, 0x00}
#define SMMOD_MSG_OPCODE_SEND_STATUS											{0xB1, 0x01, 0x00}

#define SMMOD_MSG_PACK_KEYIDX(appkey_idx, netkey_idx)           (((uint32_t)(appkey_idx) & 0x00000FFF) | (((uint32_t)(netkey_idx) & 0x00000FFF) << 12))
#define SMMOD_MSG_UNPACK_KEYIDX(key_idx, appkey_idx, netkey_idx)                \
            do {                                                                \
                appkey_idx = ((uint32_t)key_idx) & 0x00000FFF;                  \
                netkey_idx = ((uint32_t)key_idx >> 12) & 0x00000FFF;            \
            } while(0)

typedef struct
{
    uint8_t                                     beacon;
} smmod_msg_config_beacon_set_t, smmod_msg_config_beacon_status_t;

typedef struct
{
    uint8_t*                                    data;
} smmod_msg_dev_composition_data_status_t;

typedef struct
{
    uint8_t                                     identify;
} smmod_msg_config_identify_set_t, smmod_msg_config_identify_status_t;

typedef struct
{
    uint8_t                                     ttl;
} smmod_msg_config_default_ttl_set_t, smmod_msg_config_default_ttl_status_t;

typedef struct
{
    uint8_t                                     proxy;
} smmod_msg_config_proxy_set_t, smmod_msg_config_proxy_status_t;

typedef struct
{
    uint8_t                                     relay;
} smmod_msg_config_relay_set_t, smmod_msg_config_relay_status_t;

typedef struct
{
    sm_addr_t                                   elt_addr;
    union
    {
        sm_mid_t                                mid;
        sm_vmid_t                               vmid;
    } id;
} smmod_msg_config_model_publication_get_t;

typedef struct
{
    sm_addr_t                                   elt_addr;
    sm_addr_t                                   pub_addr;
    uint16_t                                    appkey_idx;
    uint8_t                                     pub_ttl;
    uint8_t                                     pub_period;
    union
    {
        sm_mid_t                                mid;
        sm_vmid_t                               vmid;
    } id;
} smmod_msg_config_model_publication_set_t;

typedef struct
{
    sm_addr_t                                   elt_addr;
    sm_virt_addr_lab_t                          full_virt_addr;
    uint16_t                                    appkey_idx;
    uint8_t                                     pub_ttl;
    uint8_t                                     pub_period;
    union
    {
        sm_mid_t                                mid;
        sm_vmid_t                               vmid;
    } id;
} smmod_msg_config_model_publication_virtual_addr_set_t;

typedef struct
{
    uint8_t                                     status;
    sm_addr_t                                   elt_addr;
    sm_addr_t                                   pub_addr;
    uint16_t                                    appkey_idx;
    uint8_t                                     pub_ttl;
    uint8_t                                     pub_period;
    union
    {
        sm_mid_t                                mid;
        sm_vmid_t                               vmid;
    } id;
} smmod_msg_config_model_publication_status_t;

typedef struct
{
    sm_addr_t                                   elt_addr;
    sm_addr_t                                   pub_addr;
    union
    {
        sm_mid_t                                mid;
        sm_vmid_t                               vmid;
    } id;
} smmod_msg_config_model_subscription_add_t, 
  smmod_msg_config_model_subscription_delete_t,
  smmod_msg_config_model_subscription_overwrite_t;

typedef struct
{
    sm_addr_t                                   elt_addr;
    sm_virt_addr_lab_t                          full_virt_addr;
    union
    {
        sm_mid_t                                mid;
        sm_vmid_t                               vmid;
    } id;
} smmod_msg_config_model_subscription_virtual_addr_add_t, 
  smmod_msg_config_model_subscription_virtual_addr_delete_t,
  smmod_msg_config_model_subscription_virtual_addr_overwrite_t;

typedef struct
{
    sm_addr_t                                   elt_addr;
    union
    {
        sm_mid_t                                mid;
        sm_vmid_t                               vmid;
    } id;
} smmod_msg_config_model_subscription_delete_all_t;

typedef struct
{
    uint8_t                                     status;
    sm_addr_t                                   elt_addr;
    sm_addr_t                                   addr;
    union
    {
        sm_mid_t                                mid;
        sm_vmid_t                               vmid;
    } id;
} smmod_msg_config_model_subscription_status_t;

typedef struct
{
    sm_addr_t                                   elt_addr;
    sm_mid_t                                    mid;
} smmod_msg_config_model_subscription_get_t;

typedef struct
{
    uint8_t                                     status;
    sm_addr_t                                   elt_addr;
    sm_mid_t                                    mid;
    sm_addr_t*                                  addr_list;
    uint16_t                                    addr_num;
} smmod_msg_config_model_subscription_list_t;

typedef struct
{
    sm_addr_t                                   elt_addr;
    sm_vmid_t                                   vmid;
} smmod_msg_config_vendor_model_subscription_get_t;

typedef struct
{
    uint8_t                                     status;
    sm_addr_t                                   elt_addr;
    sm_vmid_t                                   vmid;
    sm_addr_t*                                  addr_list;
    uint16_t                                    addr_num;
} smmod_msg_config_vendor_model_subscription_list_t;

typedef struct
{
    uint16_t                                    netkey_idx;
    uint8_t                                     netkey[SM_KEY_SIZE];
} smmod_msg_netkey_add_t, smmod_msg_netkey_update_t;

typedef struct
{
    uint16_t                                    netkey_idx;
} smmod_msg_netkey_delete_t;

typedef struct
{
    uint8_t                                     status;
    uint16_t                                    netkey_idx;
} smmod_msg_netkey_status_t;

typedef struct
{
    uint16_t                                    netkey_idx;
    uint16_t                                    appkey_idx;
    uint8_t                                     appkey[SM_KEY_SIZE];
} smmod_msg_appkey_add_t, smmod_msg_appkey_update_t;

typedef struct
{
    uint16_t                                    netkey_idx;
    uint16_t                                    appkey_idx;
} smmod_msg_appkey_delete_t;

typedef struct
{
    uint8_t                                     status;
    uint16_t                                    netkey_idx;
    uint16_t                                    appkey_idx;
} smmod_msg_appkey_status_t;

typedef struct
{
    uint16_t                                    netkey_idx;
} smmod_msg_appkey_get_t;

typedef struct
{
    uint8_t                                     status;
    uint16_t                                    netkey_idx;
    uint16_t*                                   appkey_idx_list;
    uint16_t                                    appkey_idx_num;
} smmod_msg_appkey_list_t;

typedef struct
{
    uint8_t                                     discoverable;
} smmod_msg_node_discoverable_set_t, smmod_msg_node_discoverable_status;

typedef struct
{
    sm_addr_t                                   elt_addr;
    uint16_t                                    appkey_idx;
    union
    {
        sm_mid_t                                mid;
        sm_vmid_t                               vmid;
    } id;
} smmod_msg_model_app_bind_t, smmod_msg_model_app_unbind_t;

typedef struct
{
    uint8_t                                     status;
    sm_addr_t                                   elt_addr;
    uint16_t                                    appkey_idx;
    union
    {
        sm_mid_t                                mid;
        sm_vmid_t                               vmid;
    } id;
} smmod_msg_model_app_status_t;

typedef struct
{
    sm_addr_t                                   elt_addr;
    sm_mid_t                                    mid;
} smmod_msg_model_app_get_t;

typedef struct
{
    uint8_t                                     status;
    sm_addr_t                                   elt_addr;
    sm_mid_t                                    mid;
    uint16_t*                                   appkey_idx_list;
    uint16_t                                    appkey_idx_num;
} smmod_msg_model_app_list_t;

typedef struct
{
    sm_addr_t                                   elt_addr;
    sm_vmid_t                                   vmid;
} smmod_msg_vendor_model_app_get_t;

typedef struct
{
    uint8_t                                     status;
    sm_addr_t                                   elt_addr;
    sm_vmid_t                                   vmid;
    uint16_t*                                   appkey_idx_list;
    uint16_t                                    appkey_idx_num;
} smmod_msg_vendor_model_app_list_t;

typedef struct
{
    uint8_t                                     friend;
} smmod_msg_config_friend_set_t, smmod_msg_config_friend_status_t;

typedef struct
{
    uint8_t                                     phase;
} smmod_msg_config_key_refresh_set_t, smmod_msg_config_key_refresh_status_t;


typedef struct
{
    uint8_t                                     power;
}smmod_msg_power_set_t;

typedef struct
{
    uint8_t                                     status;
    uint8_t                                     power;
}smmod_msg_power_status_t;

typedef struct
{
    uint32_t                                    power_delay_timer;
    uint32_t                                    power_on_timer;
    uint32_t                                    power_off_timer;
}smmod_msg_power_aux_set_t;

typedef struct
{
    uint8_t                                     status;
    uint32_t                                    power_delay_timer;
    uint32_t                                    power_on_timer;
    uint32_t                                    power_off_timer;
}smmod_msg_power_aux_status_t;


typedef struct
{
    uint8_t                                     status;
    uint8_t                                     lightness;
    uint16_t                                    color_temp;
    uint32_t                                    color;
}smmod_msg_light_status_t;

typedef struct
{
    uint8_t                                     lightness;
}smmod_msg_lightness_set_t;

typedef struct
{
    uint8_t                                     status;
    uint8_t                                     lightness;
}smmod_msg_lightness_status_t;

typedef struct
{
    uint16_t                                    color_temp;
}smmod_msg_color_temp_set_t;

typedef struct
{
    uint8_t                                     status;
    uint16_t                                    color_temp;
}smmod_msg_color_temp_status_t;

typedef struct
{
    uint32_t                                    color;
}smmod_msg_color_set_t;

typedef struct
{
    uint8_t                                     status;
    uint32_t                                    color;
}smmod_msg_color_status_t;


typedef struct smmod_msg_member_data_tag
{
    sm_addr_t                                   member_addr;
    uint32_t                                    power_delay_timer;
    uint32_t                                    power_on_timer;
    uint32_t                                    power_off_timer;
    uint8_t                                     power;
    uint8_t                                     lightness;
    uint16_t                                    color_temp;
    uint32_t                                    color;
}smmod_msg_member_data_t;

typedef struct smmod_msg_net_time_get_tag
{
    uint32_t                                    time_stamp;
} smmod_msg_net_time_get_t;

typedef struct smmod_msg_net_time_status_tag
{
    uint32_t                                    net_time;    // network time
    uint32_t                                    time_stamp;
} smmod_msg_net_time_status_t;


#endif
