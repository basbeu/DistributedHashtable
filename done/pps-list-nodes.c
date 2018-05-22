/**
 * @file pps-list-nodes.c
 * @date 25 Apr 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include "system.h"

#include "node_list.h"
#include "client.h"
#include "network.h"
#include <inttypes.h> // for uint16_t
#include "config.h"

#define TIMEOUT 1
#define REQUIRED_ARGS 0

void print_sha(unsigned char code[]){
	if(code != NULL){
		printf("(");
		for(int i = 0; i < SHA_DIGEST_LENGTH; ++i){
			printf("%02x", code[i]);
		}
		printf(")");
	}
}

int main(int argc, char* argv[])
{

    ssize_t out_msg_len = 0;
    ssize_t in_msg_len = 0;
    int socket = get_socket(TIMEOUT);
    client_t client;

    if(client_init((client_init_args_t) {
    &client, REQUIRED_ARGS, 0,
    (size_t) argc, &argv
    }) != ERR_NONE) {
        printf("FAIL\n");
        return 1;
    }

	node_list_sort(client.list_servers, node_cmp_server_addr);
    for(size_t i = 0; i < client.list_servers->size; ++ i) {
        node_t node = client.list_servers->list_of_nodes[i];

        out_msg_len = sendto(socket, NULL, 0, 0, (struct sockaddr *) &node.srv_addr, sizeof(node.srv_addr));

       /* if(out_msg_len != -1) {
            char temp_value;
            in_msg_len = recv(client.socket, &temp_value, 1, 0);

			printf("%s %" PRIu16" ", node.ip, node.port);
            print_sha(node.sha);
            if(in_msg_len == 0) {
                printf(" OK\n");
            } else {
                printf(" FAIL\n");
            }
        }*/
    }
    
    for(size_t i = 0; i < client.list_servers->size; ++ i) {
		node_t node = client.list_servers->list_of_nodes[i];
		char temp_value;
        in_msg_len = recv(socket, &temp_value, 1, 0);

		printf("%s %" PRIu16" ", node.ip, node.port);
        print_sha(node.sha);
        if(in_msg_len == 0) {
            printf(" OK\n");
        } else {
            printf(" FAIL\n");
        }
	}


    return 0;
}
