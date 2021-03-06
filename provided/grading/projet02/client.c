/**
 * @file client.c
 * @date 20 Mar 2018
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"
#include "system.h"
#include "config.h"
#include "args.h"

#define TIMEOUT_CLIENT 1

#define DEFAULT_N 3
#define DEFAULT_W 2
#define DEFAULT_R 2

void client_end(client_t *client)
{
    if(client != NULL) {

        if(client->args != NULL) {
            free(client->args);
            client->args = NULL;
        }

        node_list_free(client->list_servers);
    }

}

error_code client_init(client_init_args_t client_to_init)
{
    M_REQUIRE_NON_NULL(client_to_init.client);
    M_REQUIRE_NON_NULL(client_to_init.argv);

    char** init_ptr = *client_to_init.argv;
    client_to_init.client->name = *init_ptr[0];
    ++(*client_to_init.argv);

    if(client_to_init.required_args > client_to_init.argc && client_to_init.required_args != SIZE_MAX) {
        return ERR_BAD_PARAMETER;
    }

    if(client_to_init.required_args < client_to_init.argc-1 || client_to_init.required_args == SIZE_MAX) {
        client_to_init.client->args = parse_opt_args(client_to_init.max_opt_args, client_to_init.argv);
    } else {
        client_to_init.client->args = calloc(1, sizeof(args_t));
        if(client_to_init.client->args == NULL) {
            return ERR_NOMEM;
        }
    }
    //correcteur: dans le cas ou N,W ou R est égal à 0 il faudrait quitter retourner une erreur (-0.5)
    if(client_to_init.client->args->N == 0) {
        client_to_init.client->args->N = DEFAULT_N;
    }
    if(client_to_init.client->args->W == 0) {
        client_to_init.client->args->W = DEFAULT_W;
    }
    if(client_to_init.client->args->R == 0) {
        client_to_init.client->args->R = DEFAULT_R;
    }

    client_to_init.client->list_servers = get_nodes();
    if(client_to_init.client->list_servers != NULL && client_to_init.client->list_servers->size != 0) {

        //correcteur: dans ces 3 cas aussi il faut retourner une erreur
        if(client_to_init.client->list_servers->size < client_to_init.client->args->N) {
            client_to_init.client->args->N = client_to_init.client->list_servers->size ;
        }

        if(client_to_init.client->args->R > client_to_init.client->args->N) {
            client_to_init.client->args->R = client_to_init.client->args->N;
        }

        if(client_to_init.client->args->W > client_to_init.client->args->N) {
            client_to_init.client->args->W = client_to_init.client->args->N;
        }

        //correcteur: argv_size de util.h peut être utilisé ici
        ptrdiff_t dp = (*client_to_init.argv - init_ptr);
        if(client_to_init.argc - dp != client_to_init.required_args && client_to_init.required_args != SIZE_MAX) {
            //correcteur: avant de quitter il faut libérer la mémoire (free_node_list entre autre) (-0.5)
            return ERR_BAD_PARAMETER;
        }

        if((client_to_init.client->socket = get_socket(TIMEOUT_CLIENT)) == -1) {
            return ERR_NETWORK;
        }

        return ERR_NONE;
    } else {
        return ERR_NOT_FOUND;
    }
}
