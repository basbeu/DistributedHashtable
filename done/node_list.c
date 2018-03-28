/**
 * @file node_list.c
 * @brief 
 *
 * @author Bastien Beuchat and Andrea Scalisi
 * @date 28 Mar 2018
 */
#include "node_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h> // for uint16_t
#include <string.h> // for memset()
#include "config.h"
#define ADD_LENGTH 15
#define DOUBLE_SIZE_OF_LIST 2

node_list_t* enlarge_list_of_nodes(node_list_t* list);

node_list_t *node_list_new(){
	node_list_t* node_list_r = NULL;
	node_list_r = calloc(1 ,sizeof(node_list_t));
	if(node_list_r == NULL){
		return NULL;
	}
	else{
		node_list_r->list_of_nodes = calloc(0, sizeof(node_t));
		if(node_list_r->list_of_nodes == NULL){
			return NULL;
		}
		else{
			node_list_r->allocated = 0;
			node_list_r->size = 0;
			return node_list_r;			
		}
	}	
}
node_list_t *get_nodes(){
	node_list_t* complete_list = node_list_new();
	node_t temp_node;
	
	FILE* server_list_file = NULL;
	server_list_file = fopen(PPS_SERVERS_LIST_FILENAME, "r");
	char address[ADD_LENGTH];
	(void)memset(&address, 0, ADD_LENGTH);
	uint16_t port = 0;
	if(server_list_file == NULL){
			return NULL;
	}
	else{
		while(!feof(server_list_file) && !ferror(server_list_file)){
			fscanf(server_list_file, "%s", address);
			fscanf(server_list_file, "%" SCNu16, &port);
			if(port <= 0 || port > 65535){
				return NULL;
			}
			else{
				
				error_code err = node_init(&temp_node, address, port, 0);
				if(err != ERR_NONE){
					return NULL;
				}
				else{
					complete_list->list_of_nodes = realloc(complete_list->list_of_nodes, complete_list->allocated+1); 
					 complete_list->list_of_nodes[complete_list->size] = temp_node;
					 ++complete_list->allocated;
					 ++complete_list->size;
				}
			}
			
		}
		return complete_list;
		
		fclose(server_list_file);
	}
	
	
}

error_code node_list_add(node_list_t *list, node_t node){
	M_REQUIRE_NON_NULL(list);
	M_REQUIRE_NON_NULL(list->list_of_nodes);
	while(list->size >= list->allocated){
		if(enlarge_list_of_nodes(list) == NULL){
			return ERR_NOMEM;
		}
	}
	list->list_of_nodes[list->size] = node;
	++list->size;
	return ERR_NONE;
}


void node_list_free(node_list_t *list){
	if((list != NULL) && (list->list_of_nodes != NULL)){
		free(list->list_of_nodes);
		list->list_of_nodes = NULL;
		list->size = 0;
		list->allocated = 0;
		free(list);
		list = NULL;	
	}
}

node_list_t* enlarge_list_of_nodes(node_list_t* list){
	node_list_t* result = list;
	if(result != NULL){
		node_t* const old_list_of_nodes = result->list_of_nodes;
		result->allocated*=DOUBLE_SIZE_OF_LIST;
		if((result->allocated > SIZE_MAX / sizeof(node_t)) ||
			((result->list_of_nodes = realloc(result->list_of_nodes,
										result->allocated * sizeof(node_t))) == NULL)){
			result->list_of_nodes = old_list_of_nodes;
			result->allocated/=DOUBLE_SIZE_OF_LIST;
			result = NULL;
		}	
	}
	return result;
}
