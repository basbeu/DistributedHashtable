/**
 * @file pps-client-cat.c
 * @brief
 *
 * @author Bastien Beuchat and Andrea Scalisi
 * @date 02 Mai 2018
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
    args.name = "Client_Cat";
    if(client_init(args) != ERR_NONE) {
        printf("FAIL\n");
        return 0;
    }

    pps_key_t key = NULL;
    char* value = calloc(MAX_MSG_ELEM_SIZE, sizeof(char));
    pps_value_t value_temp = NULL;
    int j = 0;
    int last_was_ok = 1;

    do {
        char key_temp[MAX_MSG_ELEM_SIZE];
        j = scanf("%s", key_temp);
        key = key_temp;
		value_temp = NULL;
        if(j != -1) {
            error_code err = network_get(client, key, &value_temp);

            if(err == ERR_NONE && last_was_ok != 0) {
				
				strncpy(&value[strlen(value)], value_temp, strlen(value_temp));
                
            } else {
				 while(!feof(stdin) && !ferror(stdin) && getc(stdin) != '\n');
				 j = scanf("%s", key_temp);
				key = key_temp;
				value_temp = NULL;
				
				if(j != -1){
					last_was_ok = 0;
				}
				else{
					error_code err = network_put(client, key, value);
					if(err != ERR_NONE){
						printf("OK\n");
					}
					else{
						printf("FAIL\n");
					}
					
				}  
            }
        }
        else{
			printf("FAIL\n");
		}
        while(!feof(stdin) && !ferror(stdin) && getc(stdin) != '\n');
    } while(!feof(stdin) && !ferror(stdin));
	
    client_end(&client);

	return 0;
}
