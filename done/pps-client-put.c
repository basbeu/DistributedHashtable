/**
 * @file pps-client-put.c
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

#define REQUIRED_ARGS 2

int main(int argc, char* argv[])
{
    client_t client;

    if(client_init((client_init_args_t) {
    &client, REQUIRED_ARGS, TOTAL_SERVERS | PUT_NEEDED,
    (size_t) argc, &argv
    }) != ERR_NONE) {
		debug_print("GROS FAIL", 0);
        printf("FAIL\n");
        return 1;
    }
    error_code err = network_put(client, argv[0], argv[1]);

    if(err == ERR_NONE) {
        printf("OK\n");
    } else {
		debug_print("ERR_NETWORK", 0);
        printf("FAIL\n");
    }
    client_end(&client);

    return 0;
}

