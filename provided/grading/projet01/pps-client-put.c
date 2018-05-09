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
    client_init_args_t args;
    args.client = &client;
    args.name = "Client";
    if(client_init(args) != ERR_NONE) {
        printf("FAIL\n");
        return 0;
    }

    kv_pair_t pair;
    (void)memset(&pair, 0, sizeof(kv_pair_t));

    int j1 = 0;
    int j2 = 0;

    do {
        char key_temp[MAX_MSG_ELEM_SIZE];       // correcteur astuce: pas besoin de créer ce buffer dans le while
        j1 = scanf("%s", key_temp);             // correcteur: vous ne vous assurez pas que scanf ne lit pas plus de MAX_MSG_ELEM_SIZE char (overflow possible) (-0.5pts)
        if(j1 != -1) {                          // correcteur astuce: vous pouvez "scanfer" les deux "string" en même temps
            char value_temp[MAX_MSG_ELEM_SIZE];
            j2 = scanf("%s", value_temp);       // correcteur: vous ne vous assurez pas que scanf ne lit pas plus de MAX_MSG_ELEM_SIZE char (overflow possible) (-0.5pts)
                                                // correcteur: vous devriez vérifier qu'il ne reste pas de character dans le buffer (cela voudrait dire que l'utilisateur a rentré une clé (ou valeur) trop longue) (-0.5pts) (voir macro M_EXIT_IF_TRAILING_WITH_CODE)
            if(j2 != -1) {
                pair.key = key_temp;
                pair.value = value_temp;
                debug_print("Trying to insert : %s, %s...\n", pair.key, pair.value);

                error_code err = network_put(client, pair.key, pair.value);

                if(err == ERR_NONE) {
                    printf("OK\n");
                } else {
                    printf("FAIL\n");
                }
            }
        }
        while(!feof(stdin) && !ferror(stdin) && getc(stdin) != '\n');

    } while(!feof(stdin) && !ferror(stdin));

    client_end(&client);

    return 0;
}

