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
 #include "config.h"
 
 
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
	(void)memset(&pair, 0, sizeof(kv_pair_t));
	
	int j1 = 0;
	int j2 = 0;
 
	do{
		char key_temp[MAX_MSG_ELEM_SIZE];
		j1 = scanf("%s", key_temp);
		if(j1 != -1){
			char value_temp[MAX_MSG_ELEM_SIZE];
			j2 = scanf("%s", value_temp);

			if(j2 != -1){
				pair.key = key_temp;
				pair.value = value_temp;
				debug_print("Trying to insert : %s, %s...\n", pair.key, pair.value);
				
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

