/**
 * @file pps-client-substr.c
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
    args.name = "Client_Substr";
    if(client_init(args) != ERR_NONE) {
        printf("FAIL\n");
        return 0;
    }

    pps_key_t original_key = NULL;
    pps_value_t original_value = NULL;
    int j = 0;

	int beginning = 0;
	size_t length = 0;
    do {
		error_code err = ERR_NONE;
        char key_orig_temp[MAX_MSG_ELEM_SIZE];
        j = scanf("%s", key_orig_temp);
        if(j != -1){
			original_key = key_orig_temp;

			j = scanf("%d", &beginning);
			if(j!= -1){
				j = scanf("%zu", &length);
				if(j!= -1){
					err = (length >= 0) ? ERR_NONE : ERR_BAD_PARAMETER;
					
					char key_dest_temp[MAX_MSG_ELEM_SIZE];
					j = scanf("%s", key_dest_temp);
					
					
					if(j != -1) {
						err = network_get(client, original_key, &original_value);
						
						if(err == ERR_NONE) {
							if(beginning < 0){
								beginning += strlen(original_value);
							}
							err = (beginning + length <= strlen(original_value)) ? ERR_NONE : ERR_BAD_PARAMETER;
							
							if(err == ERR_NONE){
								size_t beginning_index = (size_t)beginning;
							
								char value_dest_temp[length];
								strncpy(value_dest_temp, &(original_value[beginning_index]), length);                
								
								err = network_put(client, key_dest_temp, value_dest_temp);
								
								if(err == ERR_NONE){
									printf("OK\n");
								}
							}
						}
						
						if(err != ERR_NONE){
							printf("FAIL\n");
						}
					}
				}
			}
		}
        while(!feof(stdin) && !ferror(stdin) && getc(stdin) != '\n');
    } while(!feof(stdin) && !ferror(stdin));

    client_end(&client);

    return 0;
}
