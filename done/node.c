/**
 * @file node.c
 * @date 20 Mar 2018
 */

#include "system.h"
#include "node.h"
#include "error.h"
#include <stdlib.h>
#include <openssl/sha.h>
#include <stdio.h>
#define ADD_LENGTH 15
#define MAX_LEN_TO_HASH 50


error_code node_init(node_t *node, const char *ip, uint16_t port, size_t _unused node_id)
{
    M_REQUIRE_NON_NULL(node);
    M_REQUIRE_NON_NULL(ip);

    error_code err = get_server_addr(ip, port, &node->srv_addr);
    node->ip = strdup(ip);
    if(node->ip == NULL) {
        return ERR_NOMEM;
    }
    node->port = port;
	
	
    char sha[MAX_LEN_TO_HASH];
    (void)memset(sha, 0, MAX_LEN_TO_HASH);
    
    if(strncpy(sha, ip, strlen(ip)) != NULL){
		//debug_print("SHA1 after IP : %s", sha);
		sprintf(&sha[strlen(sha)], " %d ", port);
		//debug_print("SHA1 after PORT : %s", sha);
		sprintf(&sha[strlen(sha)], "%zu", node_id);
		debug_print("SHA1 TOBE : %s", sha);
		(void)memset(node->sha, 0, SHA_DIGEST_LENGTH);
		SHA1((const unsigned char*)sha, strlen(sha), node->sha);	
		//debug_print("SHA1 %s", node->sha);
	}
 
	else{
		err = ERR_NOMEM;
	}
	//debug_print("SHA1 %s", node->sha);
    return err;
}

void node_end(node_t *node)
{
    if(node != NULL) {
        node->port = 0;
        if(node->ip != NULL) {
            free_const_ptr(node->ip);
            node->ip = NULL;
        }
    }
}

//Cast to delete warnings
int node_cmp_sha(const node_t *first, const node_t *second){
	return strncmp((const char*)first->sha, (const char*)second->sha, SHA_DIGEST_LENGTH);
}

int node_cmp_server_addr(const node_t *first, const node_t *second){
	int srv_ip = strncmp(first->ip, second->ip, strlen(first->ip));
	if(srv_ip != 0){
		return srv_ip;
	}
	else{
		return first->port - second->port;
	}
}
