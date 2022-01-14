
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "slist.h"


slist_t slist_node_create(slist_fn_alloc n_alloc, slist_fn_free n_free, slist_fn_fill n_fill, void *param_fill)
{
    slist_t node = (slist_t) n_alloc();
    if (!node) goto err_malloc;
    
    if (n_fill)
    {
        int ret = n_fill(node, param_fill);
        if (ret) goto err_fill;
    }

    node->next = NULL;

    return node;

err_fill:
    n_free(node);
err_malloc:
    return NULL;
}

void slist_destory(slist_t list, slist_fn_free n_free)
{
    while(list != NULL)
    {
        slist_t itr = list;
        list = list->next;
        n_free(itr);
    }
}

int slist_node_add(slist_t list, slist_t new_node, slist_fn_check is_exist)
{
    slist_t itr;
    for (itr = list; itr->next != NULL; itr = itr->next)
    {
        if (is_exist && is_exist(itr, (void*)new_node))
        {
            return -EEXIST;
        }
    }
    if (is_exist && is_exist(itr, (void*)new_node))
    {
        return -EEXIST;
    }

    itr->next = new_node;
    if (!itr->next) return -ENOMEM;

    return 0;
}

int slist_node_add_head(slist_t *list, slist_t new_node)
{
    new_node->next = *list;
    *list = new_node;
    return 0;
}

int slist_node_del(slist_t *list, slist_fn_free n_free, slist_fn_check is_target, void *param)
{
    slist_t *itr;
    slist_t tmp;

    for (itr = list; *itr; itr = &(*itr)->next)
    {
        tmp = *itr;

        if (is_target(tmp, param))
        {
            *itr = tmp->next;
            n_free(tmp);
            return 0;
        }
    }

    return -ENOENT;
}

slist_t slist_node_find(slist_t list, slist_fn_check is_target, void *param)
{
    slist_t itr;
    for (itr = list; itr; itr = itr->next)
    {
        if (is_target(itr, param))
        {
            return itr;
        }
    }
    
    return NULL;
}

int slist_node_set(slist_t list, slist_fn_fill n_set, slist_fn_check is_target, void *param_set, void *param_check)
{
    slist_t itr;
    for (itr = list; itr; itr = itr->next)
    {
        if (is_target(itr, param_check))
        {
            return n_set(itr, param_set);
        }
    }
    return -ENOENT;
}

size_t slist_node_count(slist_t list)
{
    int num = 0;

    slist_t itr;
    for (itr = list; itr; itr = itr->next)
    {
        num++;
    }

    return num;
}

int slist_traversal(slist_t list, slist_fn_node_do n_do, void *param)
{
    slist_t itr;
    for (itr = list; itr; itr = itr->next)
    {
        int ret = n_do(itr, param);
        if (ret) return ret;
    }

    return 0;
}

