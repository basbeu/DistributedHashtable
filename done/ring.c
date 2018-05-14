/**
 * @file ring.c
 * @date 14 Mai 2018
 */
#include "ring.h"

ring_t *ring_alloc(){
	return node_list_new();
}

error_code ring_init(ring_t *ring){
	ring = get_nodes();
	if(ring != NULL){
		node_list_sort((node_list_t*)ring, node_cmp_sha); 
		return ERR_NONE;
	}else{
		return ERR_NOMEM;
	}
}

void ring_free(ring_t *ring){
	node_list_free(ring);
}
