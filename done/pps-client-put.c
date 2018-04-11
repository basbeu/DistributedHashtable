/**
 * @file pps-launch-server.c
 * @brief
 *
 * @author Bastien Beuchat and Andrea Scalisi
 * @date 20 Mar 2018
 */
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h> // for memset()
 
 #include "system.h"
 #include "client.h"
 #include "network.h"
 #include "node.h"
 
 
 int main(void){
	client_t client;
	client_init_args_t args;
	args.client = &client;
	args.name = "Client";
	if(client_init(args) != ERR_NONE){
		printf("FAIL\n");
		return 0;
	}
	kv_pair_t pair;
	memset(&pair, 0, sizeof(pair));
	 
	//TODO init ??? 
	pps_key_t key = NULL;
	pps_value_t value = NULL;
	
	int j1 = 0;
	int j2 = 0;
 
	do{
		char* key_temp = NULL;
		j1 = scanf("%s", key_temp);
		key = key_temp;	
		if(j1 != -1){
			char* value_temp = NULL;
			j2 = scanf("%s", value_temp);
			value = value_temp;
			if(j2 != -1){
				pair.key = key;
				pair.value = value;
				debug_print("Trying to insert : %s, %s...\n", key, value);
				error_code err = network_put(client, pair.key, pair.value);
				if(err == ERR_NONE){
					printf("OK\n");
				}
				else{
					printf("FAIL\n");
				}		
			}	
		}
		while(!feof(stdin) && !ferror(stdin) && getc(stdin) != '\n');
		
	}while(!feof(stdin) && !ferror(stdin));
 
	client_end(&client);
	
	return 0;
 }

