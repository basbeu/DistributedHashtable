/**
 * @file client.c
 * @brief
 *
 * @date 20 Mar 2018
 */

#include "client.h"
#include "system.h"
#include "config.h"
#include "args.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define TIMEOUT_CLIENT 1

void client_end(client_t *client)
{	
	if(client != NULL){
		
		if(client->args != NULL){
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

	if(client_to_init.required_args > client_to_init.argc && client_to_init.required_args != SIZE_MAX){
		debug_print("Too few args : required : %zu, actual :  %zu", client_to_init.required_args, client_to_init.argc);
		return ERR_BAD_PARAMETER;
	}
		
	if(client_to_init.required_args < client_to_init.argc-1 || client_to_init.required_args == SIZE_MAX){
		debug_print("Ok init parse", 0);
		client_to_init.client->args = parse_opt_args(client_to_init.max_opt_args, client_to_init.argv);
		debug_print("N : %zu", client_to_init.client->args->N);
	}
	else{
		client_to_init.client->args = calloc(1, sizeof(args_t));
		client_to_init.client->args->N = 3;
		client_to_init.client->args->W = 2;
		client_to_init.client->args->R = 2;
		
	}
	
	ptrdiff_t dp = (*client_to_init.argv - init_ptr);
	debug_print("ptrdiff : %d", dp);
	if(client_to_init.argc - dp != client_to_init.required_args && client_to_init.required_args != SIZE_MAX){
		debug_print("Not enough args left", 0);
		return ERR_BAD_PARAMETER;
	}
	
	
    client_to_init.client->list_servers = get_nodes();
    if(client_to_init.client->list_servers != NULL) {
		
        if((client_to_init.client->socket = get_socket(TIMEOUT_CLIENT)) == -1) {
            return ERR_NETWORK;
        }
        
        debug_print("client init\n",0);
        return ERR_NONE;
    } else {
        return ERR_NOT_FOUND;
    }
}
