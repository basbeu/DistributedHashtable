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

#define REQUIRED_ARGS 0

int main(int argc, char* argv[])
{

    ssize_t out_msg_len = 0;
    ssize_t in_msg_len = 0;
    client_t client;

    if(client_init((client_init_args_t) {
    &client, REQUIRED_ARGS, 0,
    (size_t) argc, &argv
    }) != ERR_NONE) {
        printf("FAIL\n");
        return 1;
    }

    for(size_t i = 0; i < client.list_servers->size; ++ i) {
        node_t node = client.list_servers->list_of_nodes[i];

        out_msg_len = sendto(client.socket, NULL, 0, 0, (struct sockaddr *) &node.srv_addr, sizeof(node.srv_addr));
        //correcteur: pas beson de tester la valeur de retour de sendto dans ce cas
        if(out_msg_len != -1) {
            char temp_value;
            in_msg_len = recv(client.socket, &temp_value, 1, 0);

            if(in_msg_len == 0) {
                printf("%s %" PRIu16 " OK\n", node.ip, node.port);
            } else {
                printf("%s %" PRIu16 " FAIL\n", node.ip, node.port);
            }
        }
    }


    return 0;
}
