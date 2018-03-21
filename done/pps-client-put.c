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
	client_init(args);
	kv_pair_t pair;
	memset(&pair, 0, sizeof(pair));
	 
	pps_key_t key = 0;
	pps_value_t value = 0;
 
	do{
		int j1 = 0;
		int j2 = 0;
		j1 = scanf("%c", &key);
		j2 = scanf("%d", &value);
		if(j1 != 1 || j2 != 1){
			printf("FAIL\n");
		}
		else{
			pair.key = key;
			pair.value = value;
			network_put(client, pair.key, pair.value);
			printf("OK\n");
		}
		
	}while(!ferror(stdin));
 
	client_end(client);
	
	return 0;
 }

