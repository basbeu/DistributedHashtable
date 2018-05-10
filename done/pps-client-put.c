/**
 * @file pps-client-put.c
 * @brief
 *
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
    
     if(client_init((client_init_args_t) { &client, 2, TOTAL_SERVERS | PUT_NEEDED, 
                                           (size_t) argc, &argv }) != ERR_NONE) {
        printf("FAIL\n");
        return 1;
    }    
    error_code err = network_put(client, argv[0], argv[1]);

    if(err == ERR_NONE) {
      printf("OK\n");
    }else {
      printf("FAIL\n");
	}
    client_end(&client);

    return 0;
}

