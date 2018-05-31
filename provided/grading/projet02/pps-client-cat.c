/**
 * @file pps-client-cat.c
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

int main(int argc, char* argv[])
{

    client_t client;

    if(client_init((client_init_args_t) {
    &client, SIZE_MAX, TOTAL_SERVERS | PUT_NEEDED | GET_NEEDED,
    (size_t) argc, &argv
    }) != ERR_NONE) {
        printf("FAIL\n");
        return 1;
    }

    size_t index = 0;
    char value[MAX_MSG_ELEM_SIZE];
    pps_value_t value_temp = NULL;
    error_code err = ERR_NONE;
    while((argv[index+1] != NULL) && (err == ERR_NONE)) {
        //correcteur: il faut vérifier que la concatenation de toutes les clefs ne fasse pas d'overflow (-0.5)
        err = network_get(client, argv[index], &value_temp);
        if(err == ERR_NONE) {
            strncpy(&value[strlen(value)], value_temp, strlen(value_temp));
        }
        //correcteur: il faut free value_temp qui est alloué à chaque tour par network get (-0.5)
        index++;
    }

    if(err != ERR_NONE) {
        printf("FAIL\n");
    } else {
        err = network_put(client, argv[index], value);
        if(err != ERR_NONE) {
            printf("FAIL\n");
        } else {
            printf("OK\n");
        }
    }
    //correcteur: il faut free la mémoire allouée (-0.5)
    client_end(&client);

    return 0;
}
