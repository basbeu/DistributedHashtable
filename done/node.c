/**
 * @file node.c
 * @brief
 *
 * @author Bastien Beuchat and Andrea Scalisi
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
    /*char* addr = calloc(strnlen(ip, ADD_LENGTH), sizeof(char));
    strncpy(addr, ip, ADD_LENGTH);*/
    // struct sockaddr_in srv_addr;
    
    
    
    //error_code err = get_server_addr(ip, port, &srv_addr);
    //node->srv_addr = srv_addr;
    error_code err = get_server_addr(ip, port, &node->srv_addr);
    node->ip = strdup(ip);
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
