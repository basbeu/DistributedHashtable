/**
 * @file node.c
 * @brief
 *
 * @date 20 Mar 2018
 */

#include "system.h"
#include "node.h"
#include <stdlib.h>
#define ADD_LENGTH 15

error_code node_init(node_t *node, const char *ip, uint16_t port, size_t _unused node_id)
{
    M_REQUIRE_NON_NULL(node);
    M_REQUIRE_NON_NULL(ip);
    
    error_code err = get_server_addr(ip, port, &node->srv_addr);
    node->ip = strdup(ip);
    if(node->ip == NULL){
		return ERR_NOMEM;
	}
    
    node->port = port;

    return err;
}

void node_end(node_t *node)
{	
	if(node != NULL){
		node->port = 0;
		if(node->ip != NULL){
			free_const_ptr(node->ip);
			node->ip = NULL;
		}
	}
}
