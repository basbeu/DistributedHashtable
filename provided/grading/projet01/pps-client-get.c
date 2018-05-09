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

int main(void)
{

    client_t client;
    client_init_args_t args;                // correcteur astuce: vous pouvez utiliser la notation: args = {args, "Client"}:
    args.client = &client;
    args.name = "Client";
    if(client_init(args) != ERR_NONE) {
        printf("FAIL\n");
        return 0;
    }

    pps_key_t key = NULL;
    pps_value_t value = NULL;
    int j = 0;

    do {
        char key_temp[MAX_MSG_ELEM_SIZE];
        j = scanf("%s", key_temp);              // correcteur: vous ne vous assurez pas que scanf ne lit pas plus de MAX_MSG_ELEM_SIZE char (overflow possible) (-0.5pts)
        key = key_temp;

        if(j != -1) {
            error_code err = network_get(client, key, &value);

            if(err == ERR_NONE) {
                printf("OK %s\n", value);
            } else {
                printf("FAIL\n");
            }
        }
        while(!feof(stdin) && !ferror(stdin) && getc(stdin) != '\n');           // correcteur: vous ne libéré jamais value (allouée dans network_get) (-0.5pts)
    } while(!feof(stdin) && !ferror(stdin));

    client_end(&client);

    return 0;
}
