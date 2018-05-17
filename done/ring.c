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

node_list_t *get_ring_nodes(ring_t *complete_list)
{
    //node_list_t* complete_list = node_list_new();

    if(complete_list != NULL) {
        node_t temp_node;

        FILE* server_list_file = NULL;
        server_list_file = fopen(PPS_SERVERS_LIST_FILENAME, "r");
        char address[ADD_LENGTH+1];
        (void)memset(address, 0, ADD_LENGTH);
        uint16_t port = 0;
        size_t num_of_nodes = 0;
        if(server_list_file == NULL) {
            return NULL;
        } else {
            while(fscanf(server_list_file, "%15s", address) == 1 && fscanf(server_list_file, "%" SCNu16, &port) == 1 && fscanf(server_list_file, "%zu", &num_of_nodes) == 1 && !feof(server_list_file) && !ferror(server_list_file)) {
                debug_print("Adress is : %s, port : %" PRIu16 ", #of nodes : %d\n", address, port, num_of_nodes);
                //BEGIN AT 0 OR 1 ??
                for(size_t i = 1; i <= num_of_nodes; ++ i){
					error_code err = node_init(&temp_node, address, port, i);
					if(err != ERR_NONE) {
						fclose(server_list_file);
						return NULL;
					} else {
						node_list_add(complete_list, temp_node);
					}
				}
            }
            fclose(server_list_file);

        }
    }

    return complete_list;
}


error_code ring_init(ring_t *ring){
	get_ring_nodes(ring);
	if(ring != NULL){
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
	debug_print("TAILLE DE LA LISTE %zu", list->size);
	return list;
}
