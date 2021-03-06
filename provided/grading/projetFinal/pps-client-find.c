/**
 * @file pps-client-find.c
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

#define REQUIRED_ARGS 2

int main(int argc, char* argv[])
{

    client_t client;

    if(client_init((client_init_args_t) {
    &client, REQUIRED_ARGS, TOTAL_SERVERS | GET_NEEDED | PUT_NEEDED,
    (size_t) argc, &argv
    }) != ERR_NONE) {
        printf("FAIL\n");
        return 1;
    }

    pps_value_t value1 = NULL;
    pps_value_t value2 = NULL;
    error_code err1 = ERR_NONE;
    error_code err2 = ERR_NONE;

    err1 = network_get(client, argv[0], &value1);
    if(err1 == ERR_NONE) {

        err2 = network_get(client, argv[1], &value2);
        if(err2 == ERR_NONE) {
			ptrdiff_t index = -1;
            
            char* find = strstr(value1, value2);
            if(find!=NULL){
				index = find - value1;
			}
            
            
            printf("OK %d\n", index);
        } else {
            printf("FAIL\n");
        }
    } else {
        printf("FAIL\n");
    }
	
	free_const_ptr(value1);
	free_const_ptr(value2);

    client_end(&client);
    return 0;
}



