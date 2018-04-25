/**
 * @file pps-list-nodes.c
 * @brief
 *
 * @author Bastien Beuchat and Andrea Scalisi
 * @date 25 Apr 2018
 */
 
 #include "node_list.h"
 #include "client.h"
 #include "hashtable.h"
 #include "network.h"
 #include <inttypes.h> // for uint16_t
 
 int main(void){
	
	node_list_t* list = get_nodes();
	pps_key_t = '\0';
	pps_value_t = '\0';
	err_code err = ERROR_NONE;
	
	for(size_t i = 0; i < list->size; ++ i){
		node_list_t* serv = node_list_new();
		node_list_add(serv, list->list_of_nodes(i));
		
		client_t client;
		client.name = "Client";
		if((client.socket = get_socket(1)) == -1){
			return 1;
		}
		client.list_servers = serv;		
		
		sendto(socket, NULL, 0, 0, (struct sockaddr *) cli_addr, addr_len);
		printf("%s %" PRIu16 " OK\n", address, port);
		printf("%s %" PRIu16 " FAIL\n", address, port);
		//network_put(client, key, value);
		

		
		client_end(&client);
		node_list_free(serv);
	
	}
	
	
	
 
 
	return 0;
 }
