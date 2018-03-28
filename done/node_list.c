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
#include <string.h> // for memset()
#include "config.h"
#define N 10
#define ADD_LENGTH 15

node_list_t *node_list_new(){
	node_list_t* nodeList = NULL;
	nodeList = calloc(1 ,sizeof(node_list_t));
	if(nodeList == NULL){
		return NULL;
	}
	else{
		nodeList->listOfNodes = calloc(N, sizeof(node_t));
		if(nodeList->listOfNodes == NULL){
			return NULL;
		}
		else{
			nodeList->allocated = N;
			nodeList->size = 0;
			return nodeList;			
		}
	}	
}
node_list_t *get_nodes(){
	node_list_t* completeList;
	node_t toInit;
	
	FILE* serverListFile = NULL;
	serverListFile = fopen(PPS_SERVERS_LIST_FILENAME, "r");
	char address[ADD_LENGTH];
	(void)memset(&address, 0, ADD_LENGTH);
	uint16_t port = 0;
	if(serverListFile == NULL){
			return NULL;
	}
	else{
		while(!feof(serverList) && !ferror(serverList)){
			fscanf(serverList, "%s", address);
			fscanf(serverList, "%d", &port);
			if(port <= 0 || port > 65535){
				return NULL;
			}
			else{
				
				error_code err = node_init(
				if(err != ERR_NONE){
					return NULL;
				}
				else{
				 //ADD TO COMPLETELIST
				}
			}
			
		}
		
		fclose(serverListFile);
	}
	
	
}
/*
error_code node_list_add(node_list_t *list, node_t node){

}
void node_list_free(node_list_t *list){

}*/
