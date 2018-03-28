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

void client_end(client_t *client){
	//node_end(&client->node);
	node_list_free(client->list_servers);
}

error_code client_init(client_init_args_t client_to_init){
	M_REQUIRE_NON_NULL(client_to_init.client);
	M_REQUIRE_NON_NULL(client_to_init.name);
	
	//node_t node;
	//error_code err = node_init(&node, PPS_DEFAULT_IP, PPS_DEFAULT_PORT, 0);
	
	
	//client_to_init.client->node = node;
	
	client_to_init.client->list_servers = get_nodes();
	if(client_to_init.client->list_servers != NULL){
		client_to_init.client->name = client_to_init.name;
		client_to_init.client->socket = get_socket(1);
		debug_print("client init\n",0);
		return ERR_NONE;
	}else{
		return ERR_NOT_FOUND;
	}
	//return err;
}
