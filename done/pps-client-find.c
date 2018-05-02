/**
 * @file pps-client-find.c
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

int main(void)
{

    client_t client;
    client_init_args_t args;
    args.client = &client;
    args.name = "Client_Get";
    if(client_init(args) != ERR_NONE) {
        printf("FAIL\n");
        return 0;
    }

    char* key = calloc(MAX_MSG_ELEM_SIZE, sizeof(char));
    pps_value_t value1 = NULL;
    pps_value_t value2 = NULL;
    error_code err1 = ERR_NONE;
    error_code err2 = ERR_NONE;
	int j1 = 0;
    int j2 = 0;

    do {
        //key = NULL;
        j1 = scanf("%s", key);
        if(j1 != -1) {
            err1 = network_get(client, key, &value1);
            
            //key = NULL;
            
            if(err1 == ERR_NONE){
				 while(!feof(stdin) && !ferror(stdin) && getc(stdin) != '\n');
				j2 = scanf("%s", key);
				if(j2 != -1) {
				err2 = network_get(client, key, &value2);              

                if(err2 == ERR_NONE) {
					int index = -1;
					int iterate = (strlen(value1) - strlen(value2));
					if(iterate > 0){
						for(size_t i = 0; i <= (strlen(value1) - strlen(value2)); ++i){
						
							if(strncmp(&value1[i], value2, strlen(value2)) == 0 && index == -1){
								
								index = i;
							}
						}					
					}
                    
                    printf("OK %d\n", index);
                    
                    
                } else {
                    printf("FAIL\n");
                }
            }
				
			}
			else {
                    printf("FAIL\n");
                }
        }
        while(!feof(stdin) && !ferror(stdin) && getc(stdin) != '\n');

    } while(!feof(stdin) && !ferror(stdin) && err1 == ERR_NONE && err2 == ERR_NONE);
    free(key);
    client_end(&client);
    return 0;
}

	

