#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "sm_config.h"
#include "sm_common.h"
#include "sm_db.h"
#include "sm_port.h"

#if (SM_DB_STORAGE == SM_DB_VIRTUAL)
typedef struct nrf_db_list_hdr
{
    struct nrf_db_list_hdr *next;
} nrf_db_list_hdr_t;

typedef struct nrf_db_list
{
    nrf_db_list_hdr_t *first;
    nrf_db_list_hdr_t *last;
} nrf_db_list_t;


#if (SM_PROVISIONER_SUPPORT)
typedef struct
{
    nrf_db_list_hdr_t           hdr;
    smdb_element_info_t         info;
} smdb_elt_info_node_t;

typedef struct
{
    nrf_db_list_hdr_t           hdr;
    sm_uuid_t                   uuid;
    sm_addr_t                   dev_addr;
    sm_uuid_t                   dev_info;
    uint8_t                     dev_key[SM_KEY_SIZE];
    uint32_t                    states;
    nrf_db_list_t               elt_info_list;
} smdb_dev_node_t;
#endif

typedef struct
{
    nrf_db_list_hdr_t           hdr;
    uint16_t                    idx;
    uint8_t                     appkey[SM_KEY_SIZE];
    uint8_t                     appid;
} smdb_appkey_node_t;

typedef struct
{
    nrf_db_list_hdr_t           hdr;
    uint16_t                    appkey_idx;
} smdb_appkey_idx_node_t;

typedef struct
{
    nrf_db_list_hdr_t           hdr;
    uint32_t                    idx;
    uint8_t                     netkey[SM_KEY_SIZE];
    uint8_t                     nid;
    uint8_t                     nwk_id[SM_NETWORK_ID_SIZE];
    uint8_t                     enckey[SM_KEY_SIZE];
    uint8_t                     prikey[SM_KEY_SIZE];
    uint8_t                     bconkey[SM_KEY_SIZE];
    nrf_db_list_t               appkey_idx_list;
} smdb_netkey_node_t;

typedef struct smdb_model_tag
{
    nrf_db_list_hdr_t               hdr;
    sm_vmid_t                       mid;        // if mid, use SMACC_MSG_MID2VMID
    sm_addr_t                       pub_addr;
    uint8_t                         ttl;
    uint8_t                         pub_period;
    uint16_t                        pub_appkey_idx;
    nrf_db_list_t                   subs_list;
    nrf_db_list_t                   boundkey_list;
} smdb_model_node_t;

typedef struct smdb_subscription_node_tag
{
    nrf_db_list_hdr_t               hdr;
    sm_addr_t                       addr;
} smdb_subscription_node_t;

typedef struct smdb_boundkey_node_tag
{
    nrf_db_list_hdr_t               hdr;
    uint16_t                        appkey_idx;
} smdb_boundkey_node_t;


typedef struct smdb_element_node_tag
{
    nrf_db_list_hdr_t               hdr;
    sm_addr_t                       elt_addr;
    nrf_db_list_t                  model_list;
} smdb_element_node_t;


typedef struct
{
    nrf_db_list_t               netkey_list;
    nrf_db_list_t               appkey_list;
    nrf_db_list_t               elt_list;
    sm_ivi_t                    cur_ivi;
	uint16_t                    netkey_idx;
    uint32_t                    seq;
    uint8_t                     dev_key[SM_KEY_SIZE];
    uint8_t                     attention_timer;
    uint8_t                     def_ttl;
    
#if (SM_PROVISIONER_SUPPORT)
    sm_addr_t                   alloc_addr;
    nrf_db_list_t               dev_list;
#endif
} smdb_t;

static smdb_t g_smdb_virtual;

static void _nrf_db_list_init(nrf_db_list_t *list)
{
    list->first = NULL;
    list->last = NULL;
}

static bool _nrf_db_list_is_empty(const nrf_db_list_t *const list)
{
    bool listempty;
    listempty = (list->first == NULL);
    return (listempty);
}

static void _nrf_db_list_push_back(nrf_db_list_t *list,
                       nrf_db_list_hdr_t *list_hdr)
{
    // check if list is empty
    if (_nrf_db_list_is_empty(list))
    {
        // list empty => pushed element is also head
        list->first = list_hdr;
    }
    else
    {
        // list not empty => update next of last
        list->last->next = list_hdr;
    }

    // add element at the end of the list
    list->last = list_hdr;
    list_hdr->next = NULL;
}

static void _nrf_db_list_push_front(nrf_db_list_t *list,
                        nrf_db_list_hdr_t *list_hdr)
{
    // check if list is empty
    if (_nrf_db_list_is_empty(list))
    {
        // list empty => pushed element is also head
        list->last = list_hdr;
    }

    // add element at the beginning of the list
    list_hdr->next = list->first;
    list->first = list_hdr;
}

static nrf_db_list_hdr_t *_nrf_db_list_pop_front(nrf_db_list_t *list)
{
    nrf_db_list_hdr_t *element;

    // check if list is empty
    element = list->first;
    if (element != NULL)
    {

        // The list isn't empty : extract the first element
        list->first = list->first->next;

        if(list->first == NULL)
        {
            list->last = list->first;
        }
    }
    return element;
}

static bool _nrf_db_list_extract(nrf_db_list_t *list, nrf_db_list_hdr_t *list_hdr, uint8_t nb_following)
{
    bool found = false;

    nrf_db_list_hdr_t *prev = NULL;
    nrf_db_list_hdr_t *curr = list->first;

    // Search for the element
    while(curr != NULL)
    {
        // Check element
        if(curr == list_hdr)
        {
            found = true;
            break;
        }

        // Move pointers
        prev = curr;
        curr = curr->next;
    }

    if(found)
    {
        nrf_db_list_hdr_t *last = curr;

        // Get the last element to extract
        for(int i = nb_following; i > 0; i--)
        {
            // Jump to next
            last = last->next;
        }

        // Check if the element is first
        if(prev == NULL)
        {
            // Extract element
            list->first = last->next;
        }
        else
        {
            // Extract element
            prev->next = last->next;
        }

        // Check if the element is last
        if(last == list->last)
        {
            // Update last pointer
            list->last = prev;
        }
    }

    return found;
}

static uint16_t _nrf_db_list_size(nrf_db_list_t *list)
{
    uint16_t count = 0;
    nrf_db_list_hdr_t *tmp_list_hdr = list->first;

    // browse list to count number of elements
    while (tmp_list_hdr != NULL)
    {
        tmp_list_hdr = tmp_list_hdr->next;
        count++;
    }

    return count;
}

static nrf_db_list_hdr_t *_nrf_db_list_pick(const nrf_db_list_t *const list)
{
    return(list->first);
}


static nrf_db_list_hdr_t *_nrf_db_list_next(const nrf_db_list_hdr_t *const list_hdr)
{
    return(list_hdr->next);
}

static smdb_netkey_node_t* _smdb_find_netkey_node(nrf_db_list_t* netkey_list, uint32_t netkey_idx)
{
    smdb_netkey_node_t* netkey_node;

    netkey_node = (smdb_netkey_node_t*)_nrf_db_list_pick(netkey_list);
    while (netkey_node != NULL)
    {
        if (netkey_node->idx == netkey_idx)
        {
            break;
        }
        netkey_node = (smdb_netkey_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)netkey_node);
    }
    return netkey_node;
}

static smdb_appkey_node_t* _smdb_find_appkey_node(nrf_db_list_t* appkey_list, uint16_t appkey_idx)
{
    smdb_appkey_node_t* appkey_node;

    appkey_node = (smdb_appkey_node_t*)_nrf_db_list_pick(appkey_list);
    while (appkey_node != NULL)
    {
        if (appkey_node->idx == appkey_idx)
        {
            break;
        }
        appkey_node = (smdb_appkey_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)appkey_node);
    }
    return appkey_node;
}

static smdb_appkey_idx_node_t* _smdb_find_appkey_idx_node(nrf_db_list_t* appkey_idx_list, uint16_t appkey_idx)
{
    smdb_appkey_idx_node_t* appkey_idx_node;

    appkey_idx_node = (smdb_appkey_idx_node_t*)_nrf_db_list_pick(appkey_idx_list);
    while (appkey_idx_node)
    {
        if (appkey_idx_node->appkey_idx == appkey_idx)
        {
            break;
        }
        appkey_idx_node = (smdb_appkey_idx_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)appkey_idx_node);
    }
    return appkey_idx_node;
}

static smdb_element_node_t* _smdb_find_element_node(nrf_db_list_t* elt_list, sm_addr_t elt_addr)
{
    smdb_element_node_t* elt_node;

    elt_node = (smdb_element_node_t*)_nrf_db_list_pick(elt_list);
    while (elt_node != NULL)
    {
        if (elt_node->elt_addr == elt_addr)
        {
            break;
        }
        elt_node = (smdb_element_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)elt_node);
    }
    return elt_node;
}

static smdb_model_node_t* _smdb_find_model_node(nrf_db_list_t* model_list, sm_vmid_t mid)
{
    smdb_model_node_t* mod_node;

    mod_node = (smdb_model_node_t*)_nrf_db_list_pick(model_list);
    while (mod_node != NULL)
    {
        if (mod_node->mid == mid)
        {
            break;
        }
        mod_node = (smdb_model_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)mod_node);
    }
    return mod_node;
}

static smdb_subscription_node_t* _smdb_find_subscription_node(nrf_db_list_t* subs_list, sm_addr_t subs_addr)
{
    smdb_subscription_node_t* subs_node;

    subs_node = (smdb_subscription_node_t*)_nrf_db_list_pick(subs_list);
    while (subs_node != NULL)
    {
        if (subs_node->addr == subs_addr)
        {
            break;
        }
        subs_node = (smdb_subscription_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)subs_node);
    }
    return subs_node;
}

static smdb_boundkey_node_t* _smdb_find_boundkey_node(nrf_db_list_t* boundkey_list, uint16_t appkey_idx)
{
    smdb_boundkey_node_t* boundkey_node;

    boundkey_node = (smdb_boundkey_node_t*)_nrf_db_list_pick(boundkey_list);
    while (boundkey_node != NULL)
    {
        if (boundkey_node->appkey_idx == appkey_idx)
        {
            break;
        }
        boundkey_node = (smdb_boundkey_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)boundkey_node);
    }
    return boundkey_node;
}

void smdb_init(void)
{
    g_smdb_virtual.seq = 0;
    g_smdb_virtual.cur_ivi = 0;
    g_smdb_virtual.def_ttl = 4;
    _nrf_db_list_init(&g_smdb_virtual.netkey_list);
    _nrf_db_list_init(&g_smdb_virtual.appkey_list);
#if (SM_PROVISIONER_SUPPORT)
    _nrf_db_list_init(&g_smdb_virtual.dev_list);
#endif
}

void smdb_reset(void)
{
    smdb_element_node_t* elt_node;
    smdb_model_node_t* mod_node;
    smdb_subscription_node_t* subs_node;
    smdb_boundkey_node_t* boundkey_node;
    smdb_appkey_node_t* appkey_node;
    smdb_netkey_node_t* netkey_node;
    smdb_appkey_idx_node_t* appkey_idx_node;
#if (SM_PROVISIONER_SUPPORT)
    smdb_dev_node_t* dev_node;
    smdb_elt_info_node_t* elt_info_node;
#endif

    while ((elt_node = (smdb_element_node_t*)_nrf_db_list_pop_front(&g_smdb_virtual.elt_list)) != NULL)
    {
        while ((mod_node = (smdb_model_node_t*)_nrf_db_list_pop_front(&elt_node->model_list)) != NULL)
        {
            while ((subs_node = (smdb_subscription_node_t*)_nrf_db_list_pop_front(&mod_node->subs_list)) != NULL)
            {
                smport_free(subs_node);
            }

            while ((boundkey_node = (smdb_boundkey_node_t*)_nrf_db_list_pop_front(&mod_node->boundkey_list)) != NULL)
            {
                smport_free(boundkey_node);
            } 
            
            smport_free(mod_node);
        }
        smport_free(elt_node);
    }

    while ((appkey_node = (smdb_appkey_node_t*)_nrf_db_list_pop_front(&g_smdb_virtual.appkey_list)) != NULL)
    {
        smport_free(appkey_node);
    } 

    while ((netkey_node = (smdb_netkey_node_t*)_nrf_db_list_pop_front(&g_smdb_virtual.netkey_list)) != NULL)
    {
        while ((appkey_idx_node = (smdb_appkey_idx_node_t*)_nrf_db_list_pop_front(&netkey_node->appkey_idx_list)) != NULL)
        {
            smport_free(appkey_idx_node);
        } 
        
        smport_free(netkey_node);
    } 

#if (SM_PROVISIONER_SUPPORT)
    while ((dev_node = (smdb_dev_node_t*)_nrf_db_list_pop_front(&g_smdb_virtual.dev_list)) != NULL)
    {
        while ((elt_info_node = (smdb_elt_info_node_t*)_nrf_db_list_pop_front(&dev_node->elt_info_list)) != NULL)
        {
            if (elt_info_node->info.mid_arr != NULL)
            {
                smport_free(elt_info_node->info.mid_arr);
            }
            if (elt_info_node->info.vmid_arr != NULL)
            {
                smport_free(elt_info_node->info.vmid_arr);
            }
            smport_free(elt_info_node);
        }
        smport_free(dev_node);
    }
#endif

    sm_memset(&g_smdb_virtual, 0x00, sizeof(g_smdb_virtual));
}

uint16_t smdb_store_element(uint16_t idx, sm_addr_t elt_addr)
{
    smdb_element_node_t* elt_node;

    elt_node = smport_malloc(sizeof(smdb_element_node_t), SM_MEM_RETENTION);
    SM_ASSERT_ERR(elt_node);

    elt_node->elt_addr = elt_addr;
    _nrf_db_list_init(&elt_node->model_list);

    _nrf_db_list_push_back(&g_smdb_virtual.elt_list, (nrf_db_list_hdr_t*)elt_node);

    return _nrf_db_list_size(&g_smdb_virtual.elt_list);
}

uint16_t smdb_get_element_num(void)
{
    return _nrf_db_list_size(&g_smdb_virtual.elt_list);
}

void smdb_store_publication(sm_addr_t elt_addr, sm_vmid_t mid, sm_addr_t pub_addr, uint16_t appkey_idx, uint8_t ttl, uint8_t pub_period)
{
    smdb_element_node_t* elt_node;
    smdb_model_node_t* mod_node;

    elt_node = _smdb_find_element_node(&g_smdb_virtual.elt_list, elt_addr);
    if (elt_node != NULL)
    {
        mod_node = _smdb_find_model_node(&elt_node->model_list, mid);
        if (mod_node == NULL)
        {
            mod_node = smport_malloc(sizeof(smdb_model_node_t), SM_MEM_RETENTION);
            SM_ASSERT_ERR(mod_node);
            _nrf_db_list_init(&mod_node->subs_list);
            _nrf_db_list_init(&mod_node->boundkey_list);
            mod_node->mid = mid;
            mod_node->pub_addr = pub_addr;
            mod_node->pub_appkey_idx = appkey_idx;
            mod_node->ttl = ttl;
            mod_node->pub_period = pub_period;
            _nrf_db_list_push_back(&elt_node->model_list, (nrf_db_list_hdr_t*)mod_node);
        }
        else
        {
            mod_node->mid = mid;
            mod_node->pub_addr = pub_addr;
            mod_node->pub_appkey_idx = appkey_idx;
            mod_node->ttl = ttl;
            mod_node->pub_period = pub_period;
        }
    }
}

smdb_err_t smdb_load_publication(sm_addr_t elt_addr, sm_vmid_t mid, sm_addr_t* pub_addr, uint16_t* appkey_idx, uint8_t* ttl, uint8_t* pub_period)
{
    smdb_element_node_t* elt_node;
    smdb_model_node_t* mod_node;
    smdb_err_t err = SMDB_SUCCESS;

    elt_node = _smdb_find_element_node(&g_smdb_virtual.elt_list, elt_addr);
    if (elt_node != NULL)
    {
        mod_node = _smdb_find_model_node(&elt_node->model_list, mid);
        if (mod_node == NULL)
        {
            err = SMDB_RESOURCE_NOT_FOUND;
            return err;
        }
        if (pub_addr != NULL)
        {
            *pub_addr = mod_node->pub_addr;
        }
        if (appkey_idx != NULL)
        {
            *appkey_idx = mod_node->pub_appkey_idx;
        }
        if (ttl != NULL)
        {
            *ttl = mod_node->ttl;
        }
        if (pub_period != NULL)
        {
            *pub_period = mod_node->pub_period;
        }
    }
    else
    {
        err = SMDB_RESOURCE_NOT_FOUND;
    }
    return err;
}

void smdb_store_subscription(sm_addr_t elt_addr, sm_vmid_t mid, sm_addr_t subs_addr)
{
    smdb_element_node_t* elt_node;
    smdb_model_node_t* mod_node;
    smdb_subscription_node_t* subs_node;

    elt_node = _smdb_find_element_node(&g_smdb_virtual.elt_list, elt_addr);
    if (elt_node != NULL)
    {
        mod_node = _smdb_find_model_node(&elt_node->model_list, mid);
        if (mod_node == NULL)
        {
            mod_node = smport_malloc(sizeof(smdb_model_node_t), SM_MEM_RETENTION);
            SM_ASSERT_ERR(mod_node);
            mod_node->mid = mid;
            _nrf_db_list_init(&mod_node->subs_list);
            _nrf_db_list_init(&mod_node->boundkey_list);
            _nrf_db_list_push_back(&elt_node->model_list, (nrf_db_list_hdr_t*)mod_node);
        }
        
        if (mod_node != NULL)
        {
            subs_node = smport_malloc(sizeof(smdb_subscription_node_t), SM_MEM_RETENTION);
            SM_ASSERT_ERR(subs_node);
            subs_node->addr = subs_addr;
            _nrf_db_list_push_back(&mod_node->subs_list, (nrf_db_list_hdr_t*)subs_node);
        }
    }
}

bool smdb_check_subscription(sm_addr_t elt_addr, sm_addr_t subs_addr)
{
    bool check = false;
    smdb_element_node_t* elt_node;
    smdb_model_node_t* mod_node;
    smdb_subscription_node_t* subs_node;

    elt_node = _smdb_find_element_node(&g_smdb_virtual.elt_list, elt_addr);
    if (elt_node != NULL)
    {
        mod_node = (smdb_model_node_t*)_nrf_db_list_pick(&elt_node->model_list);
        while (mod_node != NULL)
        {
            subs_node = _smdb_find_subscription_node(&mod_node->subs_list, subs_addr);
            if (subs_node != NULL)
            {
                check = true;
                break;
            }
            mod_node = (smdb_model_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)mod_node);
        }
    }
    return check;
}

smdb_err_t smdb_load_subscription(sm_addr_t elt_addr, sm_vmid_t mid, sm_addr_t** subs_list, uint16_t* subs_num)
{
    smdb_element_node_t* elt_node;
    smdb_model_node_t* mod_node;
    smdb_subscription_node_t* subs_node;
    uint16_t i = 0;
    smdb_err_t err = SMDB_SUCCESS; 

    elt_node = _smdb_find_element_node(&g_smdb_virtual.elt_list, elt_addr);
    if (elt_node != NULL)
    {
        mod_node = _smdb_find_model_node(&elt_node->model_list, mid);
        if (mod_node != NULL)
        {
            *subs_num = _nrf_db_list_size(&mod_node->subs_list);
            *subs_list = smport_malloc(sizeof(sm_addr_t) * (*subs_num), SM_MEM_NON_RETENTION);
            SM_ASSERT_ERR(*subs_list);

            subs_node = (smdb_subscription_node_t*)_nrf_db_list_pick(&mod_node->subs_list);
            while (subs_node != NULL)
            {
                *(*subs_list + i) = subs_node->addr;
                i++;
                subs_node = (smdb_subscription_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)subs_node);
            }
        }
        else
        {
            err = SMDB_RESOURCE_NOT_FOUND;
        }
    }
    else
    {
        err = SMDB_RESOURCE_NOT_FOUND;
    }
    return err;
}

smdb_err_t smdb_remove_subscription(sm_addr_t elt_addr, sm_vmid_t mid, sm_addr_t subs_addr)
{
    smdb_element_node_t* elt_node;
    smdb_model_node_t* mod_node;
    smdb_subscription_node_t* subs_node;
    smdb_err_t err = SMDB_SUCCESS; 

    elt_node = _smdb_find_element_node(&g_smdb_virtual.elt_list, elt_addr);
    if (elt_node != NULL)
    {
        mod_node = _smdb_find_model_node(&elt_node->model_list, mid);
        if (mod_node != NULL)
        {
            subs_node = _smdb_find_subscription_node(&mod_node->subs_list, subs_addr);
            if (subs_node != NULL)
            {
                _nrf_db_list_extract(&mod_node->subs_list, (nrf_db_list_hdr_t*)subs_node, 0);
                smport_free(subs_node);
            }
            else
            {
                err = SMDB_RESOURCE_NOT_FOUND;
            }
        }
        else
        {
            err = SMDB_RESOURCE_NOT_FOUND;
        }
    }
    else
    {
        err = SMDB_RESOURCE_NOT_FOUND;
    }
    return err;
}

smdb_err_t smdb_remove_subscription_all(sm_addr_t elt_addr, sm_vmid_t mid)
{
    smdb_element_node_t* elt_node;
    smdb_model_node_t* mod_node;
    smdb_subscription_node_t* subs_node;
    smdb_err_t err = SMDB_SUCCESS; 

    elt_node = _smdb_find_element_node(&g_smdb_virtual.elt_list, elt_addr);
    if (elt_node != NULL)
    {
        mod_node = _smdb_find_model_node(&elt_node->model_list, mid);
        if (mod_node != NULL)
        {
            while ((subs_node = (smdb_subscription_node_t*)_nrf_db_list_pop_front(&mod_node->subs_list)) != NULL)
            {
                smport_free(subs_node);
            }
        }
        else
        {
            err = SMDB_RESOURCE_NOT_FOUND;
        }
    }
    else
    {
        err = SMDB_RESOURCE_NOT_FOUND;
    }
    return err;
}

smdb_err_t smdb_store_model_boundkey(sm_addr_t elt_addr, sm_vmid_t mid, uint16_t appkey_idx)
{
    smdb_element_node_t* elt_node;
    smdb_model_node_t* mod_node;
    smdb_boundkey_node_t* boundkey_node;
    smdb_err_t err = SMDB_SUCCESS;

    elt_node = _smdb_find_element_node(&g_smdb_virtual.elt_list, elt_addr);
    if (elt_node != NULL)
    {
        mod_node = _smdb_find_model_node(&elt_node->model_list, mid);
        if (mod_node != NULL)
        {
            boundkey_node = _smdb_find_boundkey_node(&mod_node->boundkey_list, appkey_idx);
            if (boundkey_node == NULL)
            {
                boundkey_node = smport_malloc(sizeof(smdb_boundkey_node_t), SM_MEM_RETENTION);
                SM_ASSERT_ERR(boundkey_node);
                boundkey_node->appkey_idx = appkey_idx;
                _nrf_db_list_push_back(&mod_node->boundkey_list, (nrf_db_list_hdr_t*)boundkey_node);
            }
            else
            {
                err = SMDB_KEY_IDX_ALREADY_EXIST;
            }
        }
        else
        {
            err = SMDB_RESOURCE_NOT_FOUND;
        }
    }
    else
    {
        err = SMDB_RESOURCE_NOT_FOUND;
    }
    return err;
}

smdb_err_t smdb_load_model_boundkey(sm_addr_t elt_addr, sm_vmid_t mid, uint16_t** appkey_idx_list, uint16_t* boundkey_idx_num)
{
    smdb_element_node_t* elt_node;
    smdb_model_node_t* mod_node;
    smdb_boundkey_node_t* boundkey_node;
    smdb_err_t err = SMDB_SUCCESS;
    uint16_t i;

    elt_node = _smdb_find_element_node(&g_smdb_virtual.elt_list, elt_addr);
    if (elt_node != NULL)
    {
        mod_node = _smdb_find_model_node(&elt_node->model_list, mid);
        if (mod_node != NULL)
        {
            *boundkey_idx_num = _nrf_db_list_size(&mod_node->boundkey_list);
            if (*boundkey_idx_num != 0)
            {
                *appkey_idx_list = smport_malloc(sizeof(uint16_t*) * (*boundkey_idx_num), SM_MEM_NON_RETENTION);
                SM_ASSERT_ERR(*appkey_idx_list);
                i = 0;
                boundkey_node = (smdb_boundkey_node_t*)_nrf_db_list_pick(&mod_node->boundkey_list);
                while (boundkey_node != NULL)
                {
                    *(*appkey_idx_list + i) = boundkey_node->appkey_idx;
                    i++;
                    boundkey_node = (smdb_boundkey_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)boundkey_node);
                }
            }
            
        }
        else
        {
            err = SMDB_RESOURCE_NOT_FOUND;
        }
    }
    else
    {
        err = SMDB_RESOURCE_NOT_FOUND;
    }
    return err;
}

smdb_err_t smdb_remove_model_boundkey(sm_addr_t elt_addr, sm_vmid_t mid, uint16_t appkey_idx)
{
    smdb_element_node_t* elt_node;
    smdb_model_node_t* mod_node;
    smdb_boundkey_node_t* boundkey_node;
    smdb_err_t err = SMDB_SUCCESS;

    elt_node = _smdb_find_element_node(&g_smdb_virtual.elt_list, elt_addr);
    if (elt_node != NULL)
    {
        mod_node = _smdb_find_model_node(&elt_node->model_list, mid);
        if (mod_node != NULL)
        {
            boundkey_node = _smdb_find_boundkey_node(&mod_node->boundkey_list, appkey_idx);
            if (boundkey_node != NULL)
            {
                _nrf_db_list_extract(&mod_node->boundkey_list, (nrf_db_list_hdr_t*)boundkey_node, 0);
                smport_free(boundkey_node);
            }
            else
            {
                err = SMDB_RESOURCE_NOT_FOUND;
            }
        }
        else
        {
            err = SMDB_RESOURCE_NOT_FOUND;
        }
    }
    else
    {
        err = SMDB_RESOURCE_NOT_FOUND;
    }
    return err;
}

bool smdb_check_model_boundkey(sm_addr_t elt_addr, sm_vmid_t mid, uint16_t appkey_idx)
{
    smdb_element_node_t* elt_node;
    smdb_model_node_t* mod_node;
    smdb_boundkey_node_t* boundkey_node;

    elt_node = _smdb_find_element_node(&g_smdb_virtual.elt_list, elt_addr);
    if (elt_node != NULL)
    {
        mod_node = _smdb_find_model_node(&elt_node->model_list, mid);
        if (mod_node != NULL)
        {
            boundkey_node = _smdb_find_boundkey_node(&mod_node->boundkey_list, appkey_idx);
            if (boundkey_node != NULL)
            {
                return true;
            }
        }
    }
    return false;
}

sm_addr_t smdb_load_dev_address(void)
{
    sm_addr_t addr = SM_ADDR_UNASSIGNED;
    smdb_element_node_t* elt_node;

    elt_node = (smdb_element_node_t*)_nrf_db_list_pick(&g_smdb_virtual.elt_list);
    if (elt_node != NULL)
    {
        addr = elt_node->elt_addr;
    }

    return addr;
}

void smdb_store_devkey(uint8_t* dev_key)
{
    sm_memcpy(g_smdb_virtual.dev_key, dev_key, SM_KEY_SIZE);
}

uint8_t* smdb_load_devkey(void)
{
    uint8_t* key;
    
    key = smport_malloc(SM_KEY_SIZE, SM_MEM_NON_RETENTION);
    sm_memcpy(key, g_smdb_virtual.dev_key, SM_KEY_SIZE);

    return key;
}

smdb_err_t smdb_store_appkey(uint16_t netkey_idx, uint16_t appkey_idx, uint8_t* app_key, uint8_t aid)
{
    smdb_netkey_node_t* netkey_node;
    smdb_appkey_node_t* appkey_node;
    smdb_appkey_idx_node_t* appkey_idx_node;

    netkey_node = _smdb_find_netkey_node(&g_smdb_virtual.netkey_list, netkey_idx);
    
    if (netkey_node == NULL)
    {
        return SMDB_RESOURCE_NOT_FOUND;
    }

    if (_nrf_db_list_size(&g_smdb_virtual.appkey_list) >= SMDB_MAX_APPKEY_NUM)
    {
        return SMDB_FULL;
    }
    appkey_node = smport_malloc(sizeof(smdb_appkey_node_t), SM_MEM_RETENTION);
    SM_ASSERT_ERR(appkey_node);
    appkey_node->idx = appkey_idx;
    if (app_key != NULL)
    {
        sm_memcpy(appkey_node->appkey, app_key, SM_KEY_SIZE);
    }
    appkey_node->appid = aid;
    _nrf_db_list_push_back(&g_smdb_virtual.appkey_list, (nrf_db_list_hdr_t*)appkey_node);

    appkey_idx_node = smport_malloc(sizeof(smdb_appkey_idx_node_t), SM_MEM_RETENTION);
    SM_ASSERT_ERR(appkey_idx_node);
    appkey_idx_node->appkey_idx = appkey_idx;
    _nrf_db_list_push_back(&netkey_node->appkey_idx_list, (nrf_db_list_hdr_t*)appkey_idx_node);

    return SMDB_SUCCESS;
}

uint8_t* smdb_load_appkey(uint16_t appkey_idx, uint8_t* aid, uint16_t* netkey_idx)
{
    smdb_appkey_node_t* appkey_node;
    smdb_netkey_node_t* netkey_node;
    smdb_appkey_idx_node_t* appkey_idx_node;
    uint8_t* key = NULL;
    bool found = false;

    appkey_node = _smdb_find_appkey_node(&g_smdb_virtual.appkey_list, appkey_idx);    

    if (appkey_node != NULL)
    {
        key = smport_malloc(SM_KEY_SIZE, SM_MEM_NON_RETENTION);
        sm_memcpy(key, appkey_node->appkey, SM_KEY_SIZE);
        *aid = appkey_node->appid;

        netkey_node = (smdb_netkey_node_t*)_nrf_db_list_pick(&g_smdb_virtual.netkey_list);
        while (netkey_node != NULL && found == false)
        {
            appkey_idx_node = (smdb_appkey_idx_node_t*)_nrf_db_list_pick(&netkey_node->appkey_idx_list);
            while (appkey_idx_node != NULL)
            {
                if (appkey_idx_node->appkey_idx == appkey_idx)
                {
                    *netkey_idx = netkey_node->idx;
                    found = true;
                    break;
                }
                appkey_idx_node = (smdb_appkey_idx_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)appkey_idx_node);
            }
            netkey_node = (smdb_netkey_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)netkey_node);
        }
    }
    return key;
}

uint8_t* smdb_load_appkey_at_pos(uint16_t pos, uint16_t* appkey_idx, uint8_t* aid, uint16_t* netkey_idx)
{
    smdb_appkey_node_t* appkey_node;
    smdb_netkey_node_t* netkey_node;
    smdb_appkey_idx_node_t* appkey_idx_node;
    uint8_t* key = NULL;
    bool found = false;

    appkey_node = (smdb_appkey_node_t*)_nrf_db_list_pick(&g_smdb_virtual.appkey_list);
    while (appkey_node != NULL)
    {
        if (pos == 0)
        {
            break;
        }
        pos--;
        appkey_node = (smdb_appkey_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)appkey_node);
    }
    

    if (appkey_node != NULL)
    {
        key = smport_malloc(SM_KEY_SIZE, SM_MEM_NON_RETENTION);
        sm_memcpy(key, appkey_node->appkey, SM_KEY_SIZE);
        *aid = appkey_node->appid;

        *appkey_idx = appkey_node->idx;

        netkey_node = (smdb_netkey_node_t*)_nrf_db_list_pick(&g_smdb_virtual.netkey_list);
        while (netkey_node != NULL && found == false)
        {
            appkey_idx_node = (smdb_appkey_idx_node_t*)_nrf_db_list_pick(&netkey_node->appkey_idx_list);
            while (appkey_idx_node != NULL)
            {
                if (appkey_idx_node->appkey_idx == appkey_node->idx)
                {
                    *netkey_idx = netkey_node->idx;
                    found = true;
                    break;
                }
                appkey_idx_node = (smdb_appkey_idx_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)appkey_idx_node);
            }
            netkey_node = (smdb_netkey_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)netkey_node);
        }
    }
    return key;
}
uint16_t smdb_get_appkey_num(void)
{
    return _nrf_db_list_size(&g_smdb_virtual.appkey_list);
}

smdb_err_t smdb_remove_appkey(uint16_t netkey_idx, uint16_t appkey_idx)
{
    smdb_netkey_node_t* netkey_node;
    smdb_appkey_node_t* appkey_node;
    smdb_appkey_idx_node_t* appkey_idx_node;
    smdb_err_t err = SMDB_SUCCESS;

    appkey_node = _smdb_find_appkey_node(&g_smdb_virtual.appkey_list, appkey_idx);
    if (appkey_node != NULL)
    {
        _nrf_db_list_extract(&g_smdb_virtual.appkey_list, (nrf_db_list_hdr_t*)appkey_node, 0);
        smport_free(appkey_node);

        netkey_node = _smdb_find_netkey_node(&g_smdb_virtual.netkey_list, netkey_idx);
        appkey_idx_node = _smdb_find_appkey_idx_node(&netkey_node->appkey_idx_list, appkey_idx);
        if (appkey_idx_node != NULL)
        {
            _nrf_db_list_extract(&netkey_node->appkey_idx_list, (nrf_db_list_hdr_t*)appkey_idx_node, 0);
            smport_free(appkey_idx_node);
        }
    }
    else
    {
        err = SMDB_RESOURCE_NOT_FOUND;
    }
    
    
    return err;
}

bool smdb_update_appkey(uint16_t netkey_idx, uint16_t appkey_idx, uint8_t* app_key, uint8_t aid)
{
    //smdb_netkey_node_t* netkey_node;
    smdb_appkey_node_t* appkey_node;
    bool ret = false;

    appkey_node = _smdb_find_appkey_node(&g_smdb_virtual.appkey_list, appkey_idx);
    if (appkey_node != NULL)
    {
        if (app_key != NULL)
        {
            sm_memcpy(appkey_node->appkey, app_key, SM_KEY_SIZE);
        }
        appkey_node->appid = aid;
    }
    return ret;
}

smdb_err_t smdb_get_appkey_idx(uint16_t netkey_idx, uint16_t** appkey_idx_list, uint16_t* idx_num)
{
    smdb_netkey_node_t* netkey_node;
    smdb_appkey_idx_node_t* appkey_idx_node;
    smdb_err_t err = SMDB_SUCCESS;
    uint16_t i = 0;

    netkey_node = _smdb_find_netkey_node(&g_smdb_virtual.netkey_list, netkey_idx);
    if (netkey_node != NULL)
    {
        *idx_num = _nrf_db_list_size(&netkey_node->appkey_idx_list);
        if (*idx_num > 0)
        {
            *appkey_idx_list = smport_malloc(sizeof(uint16_t) * (*idx_num), SM_MEM_NON_RETENTION);
            SM_ASSERT_ERR(*appkey_idx_list);

            appkey_idx_node = (smdb_appkey_idx_node_t*)_nrf_db_list_pick(&netkey_node->appkey_idx_list);
            while (appkey_idx_node != NULL)
            {
                *(*appkey_idx_list + i) = appkey_idx_node->appkey_idx;
                i++;
                appkey_idx_node = (smdb_appkey_idx_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)appkey_idx_node);
            }
        }
    }
    else
    {
        err = SMDB_RESOURCE_NOT_FOUND;
    }

    return err;
}

smdb_err_t smdb_store_netkey(uint32_t netkey_idx, uint8_t* net_key, uint8_t nid, uint8_t* nwk_id, 
                                                uint8_t* enc_key, uint8_t* pri_key, uint8_t* bcon_key)
{
    smdb_netkey_node_t* netkey_node;

    if (_nrf_db_list_size(&g_smdb_virtual.netkey_list) >= SMDB_MAX_APPKEY_NUM)
    {
        return SMDB_FULL;
    }

    netkey_node = smport_malloc(sizeof(smdb_netkey_node_t), SM_MEM_RETENTION);
    SM_ASSERT_ERR(netkey_node);
    _nrf_db_list_init(&netkey_node->appkey_idx_list);

    netkey_node->idx = netkey_idx;
    
    if (net_key != NULL)
    {
        sm_memcpy(netkey_node->netkey, net_key, SM_KEY_SIZE);
    }
    
    netkey_node->nid = nid;
    
    if (nwk_id != NULL)
    {
        sm_memcpy(netkey_node->nwk_id, nwk_id, SM_NETWORK_ID_SIZE);
    }
    if (enc_key != NULL)
    {
        sm_memcpy(netkey_node->enckey, enc_key, SM_KEY_SIZE);
    }
    if (pri_key != NULL)
    {
        sm_memcpy(netkey_node->prikey, pri_key, SM_KEY_SIZE);
    }
    if (bcon_key != NULL)
    {
        sm_memcpy(netkey_node->bconkey, bcon_key, SM_KEY_SIZE);
    }

    _nrf_db_list_push_back(&g_smdb_virtual.netkey_list, (nrf_db_list_hdr_t*)netkey_node);

    return SMDB_SUCCESS;
}

uint8_t* smdb_load_netkey(uint32_t idx, uint8_t* nid, uint8_t** nwk_id, uint8_t** enc_key, uint8_t** pri_key, uint8_t** bcon_key)
{
    smdb_netkey_node_t* netkey_node;
    uint8_t* key = NULL;

    netkey_node = _smdb_find_netkey_node(&g_smdb_virtual.netkey_list, idx);
    if (netkey_node != NULL)
    {
        key = smport_malloc(SM_KEY_SIZE, SM_MEM_NON_RETENTION);
        SM_ASSERT_ERR(key);
        sm_memcpy(key, netkey_node->netkey, SM_KEY_SIZE);

        if (nid != NULL)
        {
            *nid = netkey_node->nid;
        }

        if (nwk_id != NULL)
        {
            *nwk_id = smport_malloc(SM_NETWORK_ID_SIZE, SM_MEM_NON_RETENTION);
            SM_ASSERT_ERR(*nwk_id);
            sm_memcpy(*nwk_id, netkey_node->nwk_id, SM_NETWORK_ID_SIZE);
        }

        if (enc_key != NULL)
        {
            *enc_key = smport_malloc(SM_KEY_SIZE, SM_MEM_NON_RETENTION);
            SM_ASSERT_ERR(*enc_key);
            sm_memcpy(*enc_key, netkey_node->enckey, SM_KEY_SIZE);
        }

        if (pri_key != NULL)
        {
            *pri_key = smport_malloc(SM_KEY_SIZE, SM_MEM_NON_RETENTION);
            SM_ASSERT_ERR(*pri_key);
            sm_memcpy(*pri_key, netkey_node->prikey, SM_KEY_SIZE);
        }

        if (bcon_key != NULL)
        {
            *bcon_key = smport_malloc(SM_KEY_SIZE, SM_MEM_NON_RETENTION);
            SM_ASSERT_ERR(*bcon_key);
            sm_memcpy(*bcon_key, netkey_node->bconkey, SM_KEY_SIZE);
        }
    }
    else
    {
        if (nwk_id != NULL)
            *nwk_id = NULL;
        if (enc_key != NULL)
            *enc_key = NULL;
        if (pri_key != NULL)
            *pri_key = NULL;
        if (bcon_key != NULL)
            *bcon_key = NULL;
    }
    
    return key;
}

uint8_t* smdb_load_netkey_at_pos(uint16_t pos, uint32_t* netkey_idx, uint8_t* nid, uint8_t** nwk_id, uint8_t** enc_key, uint8_t** pri_key, uint8_t** bcon_key)
{
    smdb_netkey_node_t* netkey_node;
    uint8_t* key = NULL;
    uint16_t n;

    netkey_node = (smdb_netkey_node_t*)_nrf_db_list_pick(&g_smdb_virtual.netkey_list);
    n = 0;
    while (netkey_node != NULL)
    {
        if (n == pos)
        {
            break;
        }
        n++;
        netkey_node = (smdb_netkey_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)netkey_node);
    }
    
    if (netkey_node != NULL)
    {
        if (netkey_idx != NULL)
        {
            *netkey_idx = netkey_node->idx;
        }
        
        key = smport_malloc(SM_KEY_SIZE, SM_MEM_NON_RETENTION);
        SM_ASSERT_ERR(key);
        sm_memcpy(key, netkey_node->netkey, SM_KEY_SIZE);

        if (nid != NULL)
        {
            *nid = netkey_node->nid;
        }

        if (nwk_id != NULL)
        {
            *nwk_id = smport_malloc(SM_NETWORK_ID_SIZE, SM_MEM_NON_RETENTION);
            SM_ASSERT_ERR(*nwk_id);
            sm_memcpy(*nwk_id, netkey_node->nwk_id, SM_NETWORK_ID_SIZE);
        }

        if (enc_key != NULL)
        {
            *enc_key = smport_malloc(SM_KEY_SIZE, SM_MEM_NON_RETENTION);
            SM_ASSERT_ERR(*enc_key);
            sm_memcpy(*enc_key, netkey_node->enckey, SM_KEY_SIZE);
        }

        if (pri_key != NULL)
        {
            *pri_key = smport_malloc(SM_KEY_SIZE, SM_MEM_NON_RETENTION);
            SM_ASSERT_ERR(*pri_key);
            sm_memcpy(*pri_key, netkey_node->prikey, SM_KEY_SIZE);
        }

        if (bcon_key != NULL)
        {
            *bcon_key = smport_malloc(SM_KEY_SIZE, SM_MEM_NON_RETENTION);
            SM_ASSERT_ERR(*bcon_key);
            sm_memcpy(*bcon_key, netkey_node->bconkey, SM_KEY_SIZE);
        }
    }
    
    return key;
}

uint16_t smdb_get_netkey_num(void)
{
    return _nrf_db_list_size(&g_smdb_virtual.netkey_list);
}

bool smdb_remove_netkey(uint32_t idx)
{
    smdb_netkey_node_t* netkey_node;
    smdb_appkey_idx_node_t* appkey_idx_node;
    bool ret = false;

    netkey_node = (smdb_netkey_node_t*)_nrf_db_list_pick(&g_smdb_virtual.netkey_list);
    while (netkey_node != NULL)
    {
        if (netkey_node->idx == idx)
        {
            _nrf_db_list_extract(&g_smdb_virtual.netkey_list, (nrf_db_list_hdr_t*)netkey_node, 0);
            while ((appkey_idx_node = (smdb_appkey_idx_node_t*)_nrf_db_list_pop_front(&netkey_node->appkey_idx_list)) != NULL)
            {
                smport_free(appkey_idx_node);
            }
            
            smport_free(netkey_node);
            ret = true;
            break;
        }
        netkey_node = (smdb_netkey_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)netkey_node);
    }
    return ret;
}

bool smdb_update_netkey(uint32_t idx, uint8_t* net_key, uint8_t nid, uint8_t* nwk_id, uint8_t* enc_key, uint8_t* pri_key, uint8_t* bcon_key)
{
    smdb_netkey_node_t* netkey_node;
    bool ret = false;

    netkey_node = _smdb_find_netkey_node(&g_smdb_virtual.netkey_list, idx);
    if (netkey_node != NULL)
    {
        if (net_key != NULL)
        {
            sm_memcpy(netkey_node->netkey, net_key, SM_KEY_SIZE);
        }
        
        netkey_node->nid = nid;
        
        if (nwk_id != NULL)
        {
            sm_memcpy(netkey_node->nwk_id, nwk_id, SM_NETWORK_ID_SIZE);
        }
        if (enc_key != NULL)
        {
            sm_memcpy(netkey_node->enckey, enc_key, SM_KEY_SIZE);
        }
        if (pri_key != NULL)
        {
            sm_memcpy(netkey_node->prikey, pri_key, SM_KEY_SIZE);
        }
        if (bcon_key != NULL)
        {
            sm_memcpy(netkey_node->bconkey, bcon_key, SM_KEY_SIZE);
        }
        ret = true;
    }
    return ret;
}

void smdb_revoke_netkey(uint32_t idx)
{
    smdb_netkey_node_t *old_netkey_node, *new_netkey_node;

    old_netkey_node = _smdb_find_netkey_node(&g_smdb_virtual.netkey_list, idx);
    new_netkey_node = _smdb_find_netkey_node(&g_smdb_virtual.netkey_list, (idx | SMDB_KEY_REFRESH_FLAG));

    sm_memcpy(&new_netkey_node->appkey_idx_list, &old_netkey_node->appkey_idx_list, sizeof(nrf_db_list_t));

    co_list_extract(&g_smdb_virtual.netkey_list, (struct co_list_hdr*)old_netkey_node, 0);
    smport_free(old_netkey_node);

    new_netkey_node->idx &= ~(SMDB_KEY_REFRESH_FLAG);
}

smdb_err_t smdb_store_kr_netkey(uint32_t netkey_idx, uint8_t* net_key, uint8_t nid, uint8_t* nwk_id, uint8_t* enc_key, uint8_t* pri_key, uint8_t* bcon_key)
{
    netkey_idx |= SMDB_KEY_REFRESH_FLAG;
    
    return smdb_store_netkey(netkey_idx, net_key, nid, nwk_id, enc_key, pri_key, bcon_key);
}

bool smdb_update_kr_netkey(uint32_t idx, uint8_t* net_key, uint8_t nid, uint8_t* nwk_id, uint8_t* enc_key, uint8_t* pri_key, uint8_t* bcon_key)
{
    idx |= SMDB_KEY_REFRESH_FLAG;

    return smdb_update_netkey(idx, net_key, nid, nwk_id, enc_key, pri_key, bcon_key);
}

uint8_t* smdb_load_kr_netkey(uint32_t idx, uint8_t* nid, uint8_t** nwk_id, uint8_t** enc_key, uint8_t** pri_key, uint8_t** bcon_key)
{
    idx |= SMDB_KEY_REFRESH_FLAG;
    
    return smdb_load_netkey(idx, nid, nwk_id, enc_key, pri_key, bcon_key);
}

void smdb_set_primary_netkey_idx(uint16_t idx)
{
    g_smdb_virtual.netkey_idx = idx;
}

uint16_t smdb_get_primary_netkey_idx(void)
{
    return g_smdb_virtual.netkey_idx;
}


void smdb_store_seq(uint32_t seq)
{
    g_smdb_virtual.seq = seq;
}

uint32_t smdb_load_seq(void)
{
    return g_smdb_virtual.seq;
}

void smdb_update_ivi(sm_ivi_t new_ivi)
{
    g_smdb_virtual.cur_ivi = new_ivi;
}

sm_ivi_t smdb_get_ivi(void)
{
    return g_smdb_virtual.cur_ivi;
}

uint8_t smdb_load_attention_timer(void)
{
    return g_smdb_virtual.attention_timer;
}

void smdb_store_attention_timer(uint8_t att_timer)
{
    g_smdb_virtual.attention_timer = att_timer;
}

uint8_t smdb_load_default_ttl(void)
{
    return g_smdb_virtual.def_ttl;
}

void smdb_store_default_ttl(uint8_t ttl)
{
    g_smdb_virtual.def_ttl = ttl;
}

#if (SM_PROVISIONER_SUPPORT)
void smdb_store_allocated_address(sm_addr_t addr_alloc)
{
    g_smdb_virtual.alloc_addr = addr_alloc;
}

sm_addr_t smdb_load_allocated_address(void)
{
    return g_smdb_virtual.alloc_addr;
}

void smdb_store_provisioned_node(sm_uuid_t* uuid, sm_addr_t dev_addr, uint8_t *dev_key)
{
    smdb_dev_node_t* dev_node;

    dev_node = smport_malloc(sizeof(smdb_dev_node_t), SM_MEM_RETENTION);
    SM_ASSERT_ERR(dev_node);
    sm_memcpy(&dev_node->uuid, uuid, sizeof(sm_uuid_t));
    sm_memcpy(&dev_node->dev_key, dev_key, SM_KEY_SIZE);
    dev_node->dev_addr = dev_addr;
    dev_node->states = 0;
    _nrf_db_list_init(&dev_node->elt_info_list);

    _nrf_db_list_push_back(&g_smdb_virtual.dev_list, (nrf_db_list_hdr_t*)dev_node);
}

void smdb_remove_provisioned_node(sm_addr_t dev_addr)
{
    smdb_dev_node_t* dev_node;
    smdb_elt_info_node_t *elt_node;

    dev_node = (smdb_dev_node_t*)_nrf_db_list_pick(&g_smdb_virtual.dev_list);
    while (dev_node != NULL)
    {
        if (dev_node->dev_addr == dev_addr)
        {
            _nrf_db_list_extract(&g_smdb_virtual.dev_list, (nrf_db_list_hdr_t*)dev_node, 0);

            while((elt_node = (smdb_elt_info_node_t*)_nrf_db_list_pop_front(&dev_node->elt_info_list)) != NULL)
            {
                if (elt_node->info.mid_arr)
                {
                    smport_free(elt_node->info.mid_arr);
                }
                if (elt_node->info.vmid_arr)
                {
                    smport_free(elt_node->info.vmid_arr);
                }
                smport_free(elt_node);
            }
            smport_free(dev_node);
            break;
        }
        dev_node = (smdb_dev_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)dev_node);
    }
}

sm_uuid_t* smdb_get_node_uuid_by_dev_addr(sm_addr_t dev_addr)
{
    smdb_dev_node_t* dev_node;
    sm_uuid_t* uuid = NULL;
    
    dev_node = (smdb_dev_node_t*)_nrf_db_list_pick(&g_smdb_virtual.dev_list);
    while (dev_node != NULL)
    {
        if (dev_node->dev_addr == dev_addr)
        {
            uuid = &dev_node->uuid;
            break;
        }
        dev_node = (smdb_dev_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)dev_node);
    }
    return uuid;
}

sm_addr_t smdb_get_node_dev_addr_by_uuid(sm_uuid_t* uuid)
{
    smdb_dev_node_t* dev_node;
    
    dev_node = (smdb_dev_node_t*)_nrf_db_list_pick(&g_smdb_virtual.dev_list);
    while (dev_node != NULL)
    {
        if (memcmp(&dev_node->uuid, uuid, sizeof(sm_uuid_t)) == 0)
        {
            return dev_node->dev_addr;
        }
        dev_node = (smdb_dev_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)dev_node);
    }
    return SM_ADDR_UNASSIGNED;
}

sm_addr_t smdb_get_node_dev_addr_by_idx(uint16_t idx)
{
    smdb_dev_node_t* dev_node;
    
    dev_node = (smdb_dev_node_t*)_nrf_db_list_pick(&g_smdb_virtual.dev_list);
    while (dev_node != NULL)
    {
        if (idx-- == 0)
        {
            return dev_node->dev_addr;
        }
        dev_node = (smdb_dev_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)dev_node);
    }
    return SM_ADDR_UNASSIGNED;
}

uint16_t smdb_get_provisioned_node_number(void)
{
    return _nrf_db_list_size(&g_smdb_virtual.dev_list);
}

void smdb_store_device_info(sm_addr_t dev_addr, sm_device_info_t* dev_info)
{
    smdb_dev_node_t* dev_node;

    dev_node = (smdb_dev_node_t*)_nrf_db_list_pick(&g_smdb_virtual.dev_list);
    while (dev_node != NULL)
    {
        if (dev_node->dev_addr == dev_addr)
        {
            sm_memcpy(&dev_node->dev_info, dev_info, sizeof(sm_device_info_t));
            break;
        }
        dev_node = (smdb_dev_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)dev_node);
    }
}

void smdb_load_device_info(sm_addr_t dev_addr, sm_device_info_t* dev_info)
{
    smdb_dev_node_t* dev_node;

    dev_node = (smdb_dev_node_t*)_nrf_db_list_pick(&g_smdb_virtual.dev_list);
    while (dev_node != NULL)
    {
        if (dev_node->dev_addr == dev_addr)
        {
            sm_memcpy(dev_info, &dev_node->dev_info, sizeof(sm_device_info_t));
            break;
        }
        dev_node = (smdb_dev_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)dev_node);
    }
}

void smdb_store_element_info(sm_addr_t dev_addr, sm_addr_t elt_addr, uint8_t mid_num, uint8_t vmid_num,
                                             sm_mid_t* mid_arr, sm_vmid_t* vmid_arr)
{
    smdb_dev_node_t* dev_node;
    smdb_elt_info_node_t *elt_node;

    dev_node = (smdb_dev_node_t*)_nrf_db_list_pick(&g_smdb_virtual.dev_list);
    while (dev_node != NULL)
    {
        if (dev_node->dev_addr == dev_addr)
        {
            elt_node = smport_malloc(sizeof(smdb_elt_info_node_t), SM_MEM_RETENTION);
            SM_ASSERT_ERR(elt_node);
            elt_node->info.elt_addr = elt_addr;
            elt_node->info.mid_num = mid_num;
            elt_node->info.vmid_num = vmid_num;
            if (mid_num > 0)
            {
                elt_node->info.mid_arr = smport_malloc(mid_num * sizeof(sm_mid_t), SM_MEM_RETENTION);
                SM_ASSERT_ERR(elt_node->info.mid_arr);
                sm_memcpy(elt_node->info.mid_arr, mid_arr, mid_num * sizeof(sm_mid_t));
            }
            else
            {
                elt_node->info.mid_arr = NULL;
            }
            if (vmid_num > 0)
            {
                elt_node->info.vmid_arr = smport_malloc(vmid_num * sizeof(sm_vmid_t), SM_MEM_RETENTION);
                SM_ASSERT_ERR(elt_node->info.vmid_arr);
                sm_memcpy(elt_node->info.vmid_arr, vmid_arr, vmid_num * sizeof(sm_vmid_t));
            }
            else
            {
                elt_node->info.vmid_arr = NULL;
            }
            _nrf_db_list_push_back(&dev_node->elt_info_list, (nrf_db_list_hdr_t*)elt_node);
            break;
        }
        dev_node = (smdb_dev_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)dev_node);
    }
}

uint16_t smdb_get_element_info_num(sm_addr_t dev_addr)
{
    smdb_dev_node_t* dev_node;

    dev_node = (smdb_dev_node_t*)_nrf_db_list_pick(&g_smdb_virtual.dev_list);
    while (dev_node != NULL)
    {
        if (dev_node->dev_addr == dev_addr)
        {
            return _nrf_db_list_size(&dev_node->elt_info_list);
        }
        dev_node = (smdb_dev_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)dev_node);
    }
    
    return 0;
}

smdb_element_info_t* smdb_load_element_info_by_idx(sm_addr_t dev_addr, uint8_t elt_idx)
{
    smdb_dev_node_t* dev_node;
    smdb_elt_info_node_t *elt_node;
    smdb_element_info_t* elt_info = NULL;

    dev_node = (smdb_dev_node_t*)_nrf_db_list_pick(&g_smdb_virtual.dev_list);
    while (dev_node != NULL)
    {
        if (dev_node->dev_addr == dev_addr)
        {
            break;
        }
        dev_node = (smdb_dev_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)dev_node);
    }

    if (dev_node != NULL)
    {
        elt_node = (smdb_elt_info_node_t*)_nrf_db_list_pick(&dev_node->elt_info_list);
        if (elt_node != NULL)
        {
            while (elt_idx >= 0)
            {
                if (elt_idx == 0)
                {
                    elt_info = smport_malloc(sizeof(smdb_element_info_t), SM_MEM_NON_RETENTION);
                    sm_memcpy(elt_info, &(elt_node->info), sizeof(smdb_element_info_t));
                    break;
                }
                else
                {
                    elt_node = (smdb_elt_info_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)elt_node);
                    elt_idx--;
                }
            }
        }
    }
    
    return elt_info;
}

uint8_t* smdb_load_node_devkey(sm_addr_t dev_addr)
{
    smdb_dev_node_t* dev_node;
    uint8_t* key;

    dev_node = (smdb_dev_node_t*)_nrf_db_list_pick(&g_smdb_virtual.dev_list);
    while (dev_node != NULL)
    {
        if (dev_node->dev_addr == dev_addr)
        {
            key = smport_malloc(SM_KEY_SIZE, SM_MEM_NON_RETENTION);
            sm_memcpy(key, dev_node->dev_key, SM_KEY_SIZE);
            return key;
        }
        dev_node = (smdb_dev_node_t*)_nrf_db_list_next((nrf_db_list_hdr_t*)dev_node);
    }
    return NULL;
}

#endif

#endif

