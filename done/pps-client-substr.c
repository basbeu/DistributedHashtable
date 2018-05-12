/**
 * @file pps-client-substr.c
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

#define REQUIRED_ARGS 4

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

    pps_key_t original_key = argv[0];
    pps_value_t original_value = NULL;

    int beginning = 0;
    size_t length = 0;
    sscanf(argv[1], "%d", &beginning);
    sscanf(argv[2], "%zu", &length);

    error_code err = network_get(client, original_key, &original_value);

    if(err == ERR_NONE) {
        if(beginning < 0) {
            beginning += strlen(original_value);
        }
        err = (beginning + length <= strlen(original_value)) ? ERR_NONE : ERR_BAD_PARAMETER;

        if(err == ERR_NONE) {
            size_t beginning_index = (size_t)beginning;

            char value_dest_temp[length];
            strncpy(value_dest_temp, &(original_value[beginning_index]), length);

            err = network_put(client, argv[3], value_dest_temp);

            if(err == ERR_NONE) {
                printf("OK\n");
            }
        }
    }

    if(err != ERR_NONE) {
        printf("FAIL\n");
    }

    client_end(&client);

    return 0;
}
