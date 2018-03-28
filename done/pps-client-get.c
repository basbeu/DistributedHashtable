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
	 
	pps_key_t key = 0;
	pps_value_t value = 0;
	int j = 0;
 
	do{
		j = scanf("%c", &key);
		if(j != -1){
			error_code err = network_get(client, key, &value);
			if(err == ERR_NONE){
				printf("OK %d\n", value);
			}
			else{
				printf("FAIL\n");
			}			
		}	
		while(!feof(stdin) && !ferror(stdin) && getc(stdin) != '\n');
	}while(!feof(stdin) && !ferror(stdin));
 
	client_end(&client);
	
	return 0;
 }
