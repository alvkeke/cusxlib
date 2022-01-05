
#ifndef __LIB_SLIST_H__
#define __LIB_SLIST_H__

#include <sys/types.h>

struct slist {
    struct slist *next;
};
typedef struct slist *slist_t;


typedef void *(*slist_fn_alloc) (void);
typedef void (*slist_fn_free) (slist_t node);
typedef int (*slist_fn_fill) (slist_t node, void *param);
typedef int (*slist_fn_node_do) (slist_t node, void *param);
typedef int (*slist_fn_check) (slist_t node, void *param);

/**
 *  create a list node, can be used as a list head
 *  @param n_alloc: function be used to allocate memory for new node, can NOT be NULL;
 *  @param n_free: function be used to free memory for failed, can NOT be NULL;
 *  @param n_fill: function to fill the new node, can be NULL;
 *  @param param_fill: pointer to the parameter that be used by `nfill`;
 *  @return: pointer of the new list, NULL for failed;
 */
slist_t slist_node_create(slist_fn_alloc n_alloc, slist_fn_free n_free, slist_fn_fill n_fill, void *param_fill);

/**
 *  destory a the list from node that passin, and release the memory with `dmfree`
 *  @param list: list need to be destory;
 *  @param nfree: function be used to free memory
 */
void slist_destory(slist_t list, slist_fn_free n_free);

/**
 *  add a node to the end of a slist;
 *  @param list: the list need to be operated
 *  @param new_node: the node need to be add;
 *  @param is_exist: func be used to check if node is exist, current-node will be passin as `param`. skip checking if NULL;
 *  @return: 0 for success, other for failed;
 */
int slist_node_add(slist_t list, slist_t new_node, slist_fn_check is_exist);

/**
 * @brief add a node in the front of the list
 * 
 * @param list : the old list, pointer
 * @param new_node : new node
 * @return int 0 for success, other for failed;
 */
int slist_node_add_head(slist_t *list, slist_t new_node);

/**
 *  remove an node from a list;
 *  @param list: the list need to be operated
 *  @param n_free: used to release memory
 *  @param is_target: used to check if a node need to be remove;
 *  @param param: parameter be passin `is_target`
 *  @return: 0 for success, others for failed;
 */
int slist_node_del(slist_t *list, slist_fn_free n_free, slist_fn_check is_target, void *param);

/**
 *  ifind a node in a list;
 *  @param list: list want to search
 *  @param is_target: used to check if a node is the target;
 *  @param param: be passin `is_target`
 *  @return pointer of the node, NULL for failed;
 */
slist_t slist_node_find(slist_t list, slist_fn_check is_target, void *param);

/**
 *  modify content of an node
 *  @param list: the list need to be operate
 *  @param n_set: used to set value in node
 *  @param is_target: used to check if it's the node to be modified
 *  @param param_set: be passin `n_set`
 *  @param param_check: be passin `is_target`
 *  @return: 0 for success, other for failed;
 */
int slist_node_set(slist_t list, slist_fn_fill n_set, slist_fn_check is_target, void *param_set, void *param_check);


/**
 *  get count of the entries in the list;
 *  @param list: the list
 *  @return: count of nodes
 */
size_t slist_node_count(slist_t list);

/**
 *  go through the list, and operate the node with userdefine function `fn`
 *  the traversal will stop if `fn` return a non-zero value;
 *  @param list: the list need to be traversal
 *  @param n_do: userdefine function, will be called for every node entry
 *  @param param: userdefine parameter, will be passin `n_do`
 *  @return: userdefine return value. if non-zero, the traversal was stop by user;
 */
int slist_traversal(slist_t list, slist_fn_node_do n_do, void *param);

#endif  // __LIB_SLIST_H__
