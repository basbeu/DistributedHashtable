/**
 * @file ring.c
 * @date 14 Mai 2018
 */
#include <string.h>
#include <openssl/sha.h> 
#include <inttypes.h> // for uint16_t

#include "ring.h"
#include "node.h"
#include "config.h"
#define ADD_LENGTH 15


ring_t *ring_alloc(){
	return node_list_new();
}


error_code ring_init(ring_t *ring){
	node_list_t* temp = get_nodes();	
	if(ring != NULL && temp != NULL){
		ring->list_of_nodes = temp->list_of_nodes;
		ring->size = temp->size;
		ring->allocated = temp->allocated;
		node_list_sort((node_list_t*)ring, node_cmp_sha); 
		debug_print("Size of the ring in ring_init : %zu", ring->size);
		return ERR_NONE;
	}else{
		return ERR_NOMEM;
	}
}



void ring_free(ring_t *ring){
	node_list_free(ring);
}

int list_contains_server(node_list_t* list, node_t node){
	for(size_t i = 0; i < list->size; ++i){
		if(node_cmp_server_addr(&list->list_of_nodes[i], &node) == 0){
			return 1;
		}
	}
	
	return 0;
}

node_list_t *ring_get_nodes_for_key(const ring_t *ring, size_t wanted_list_size, pps_key_t key){
	node_list_t* list = node_list_new();
	unsigned char sha[SHA_DIGEST_LENGTH];
	
	if(list != NULL){
		for(size_t i = 0; i < ring->size;++i){
			//cast to remove warning
			if(strncmp((char*)ring->list_of_nodes[i].sha, (char*)SHA1((const unsigned char *)key,strlen(key), sha), SHA_DIGEST_LENGTH) > 0
				&& !list_contains_server(list, ring->list_of_nodes[i])){
				node_list_add(list, ring->list_of_nodes[i]);
			}
		}
	}
	
	return list;
}
