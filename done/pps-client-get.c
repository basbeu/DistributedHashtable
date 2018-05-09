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

int main(int argc, char* argv[])
{

    client_t client;
    
    if(client_init((client_init_args_t) { &client, 1, TOTAL_SERVERS | GET_NEEDED, 
                                           (size_t) argc, &argv }) != ERR_NONE) {
        debug_print("NO INIT", 0);
        printf("FAIL\n");
        return 0;
    }
    pps_value_t value = NULL;

    error_code err = network_get(client, argv[0], &value);

    if(err == ERR_NONE) {
		printf("OK %s\n", value);
    } else {
		//debug_print("Error network", 0);
         printf("FAIL\n");
    }
    client_end(&client);

    return 0;
}
