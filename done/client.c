/**
 * @file client.c
 * @brief
 *
 * @author Bastien Beuchat and Andrea Scalisi
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
	free(client->args);
	client->args = NULL;
    node_list_free(client->list_servers);
}

error_code client_init(client_init_args_t client_to_init)
{
    M_REQUIRE_NON_NULL(client_to_init.client);
	M_REQUIRE_NON_NULL(client_to_init.argv);
	
	char** init_ptr = *client_to_init.argv;
	client_to_init.client->name = *init_ptr[0];
	++(*client_to_init.argv);

	if(client_to_init.required_args > client_to_init.argc){
		return ERR_BAD_PARAMETER;
	}
		
	if(client_to_init.required_args != client_to_init.argc-1){
		client_to_init.client->args = parse_opt_args(client_to_init.max_opt_args, client_to_init.argv);
	}
	
	ptrdiff_t dp = (*client_to_init.argv - init_ptr);
	if(client_to_init.argc - dp < client_to_init.required_args){
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
