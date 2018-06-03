/**
 * @file pps-client-get.c
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
#include "util.h"

#define REQUIRED_ARGS 1

int main(int argc, char* argv[])
{

    client_t client;

    if(client_init((client_init_args_t) {
    &client, REQUIRED_ARGS, TOTAL_SERVERS | GET_NEEDED,
    (size_t) argc, &argv
    }) != ERR_NONE) {
        printf("FAIL\n");
        return 1;
    }
    pps_value_t value = NULL;

    error_code err = network_get(client, argv[0], &value);

    if(err == ERR_NONE) {
        printf("OK %s\n", value);
    } else {
        printf("FAIL\n");
    }

    free_const_ptr(value);
    client_end(&client);

    return 0;
}
