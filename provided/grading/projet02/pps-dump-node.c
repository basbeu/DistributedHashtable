/**
 * @file pps-dump-node.c
 * @date 25 Avr 2018
 */

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "system.h"
#include "hashtable.h"
#include "node.h"
#include "config.h"
#include "client.h"

#define ADD_LENGTH 15
#define TIMEOUT 1
#define REQUIRED_ARGS 2

int main(int argc, char* argv[])
{
    uint16_t port = 0;


    client_t client;
    //correcteur: pas besoin de client ici, c'est un utilitaire de debug et on a pas besoin de créer toute la liste de noeud (-1)
    if(client_init((client_init_args_t) {
    &client, REQUIRED_ARGS, 0,
    (size_t) argc, &argv
    }) != ERR_NONE) {
        printf("FAIL\n");
        return 1;
    }

    int j = sscanf(argv[1],"%" SCNu16, &port); //correcteur: en scannant dans un uint16_t on cast les ports négatifs (erreur) en port positif (valide) (-0.5)
    if(j == -1) {
        printf("FAIL\n");
        return 1;
    }

    node_t node;
    node_init(&node, argv[0], port, 0);

    //send the dump request
    //correcteur: "" ou '\0' != "\0" (-0.5)
    sendto(client.socket, "/0", 1, 0, (struct sockaddr *) &node.srv_addr, sizeof(node.srv_addr));

    char in_msg[MAX_MSG_SIZE]; //correcteur: il manque le + 1 pour le \0 (-0.5)
    (void)memset(in_msg, '\0', MAX_MSG_SIZE);
    socklen_t addr_len = sizeof(node.srv_addr);

    ssize_t in_msg_len = recvfrom(client.socket, &in_msg, sizeof(in_msg), 0,
                                  (struct sockaddr *)&node.srv_addr, &addr_len);

    //correcteur: il faut utiliser ntohl pour récupérer le nombre de clef (et il ne faudrait JAMAIS utiliser atoi, cette fonction comporte pleins de bug)
    char nb_pair_s[4];
    strncpy(nb_pair_s, in_msg, 4);
    size_t nb_pair= atoi(nb_pair_s);
    size_t all_pair_size = nb_pair;
    kv_pair_t all_pair[all_pair_size];

    size_t index_pair = 0;
    size_t index_msg = 4;
    while(nb_pair > 0 && in_msg_len != -1) {
        //correcteur: mauvaise gestion lorsqu'il y a plusieurs messages UDP, le 2ème (en tout cas) message UDP est mal déchiffré et la valeur à 4 bytes de trop (-1)
        while(index_msg < in_msg_len) {
            char* key = calloc(MAX_MSG_ELEM_SIZE, sizeof(char));
            char* value = calloc(MAX_MSG_ELEM_SIZE, sizeof(char));

            if(key != NULL && value != NULL) {
                strncpy(key, &(in_msg[index_msg]),MAX_MSG_ELEM_SIZE);
                index_msg += strlen(key)+1;
                strncpy(value, &(in_msg[index_msg]),MAX_MSG_ELEM_SIZE);
                index_msg+= strlen(value)+1;

                all_pair[index_pair].key = key;
                all_pair[index_pair].value = value;

                --nb_pair;
                ++index_pair;
            }
        }
        in_msg_len = recvfrom(client.socket, &in_msg, sizeof(in_msg), 0,
                              (struct sockaddr *)&node.srv_addr, &addr_len);
        index_msg = 0;
    }

    if(nb_pair != 0) {
        printf("FAIL\n");
    }

    for(size_t i = 0; i < all_pair_size; ++i) {
        printf("%s = %s\n",all_pair[i].key, all_pair[i].value);
    }

    for(size_t i = 0; i < all_pair_size; ++i) {
        kv_pair_free(&all_pair[i]);
    }
    node_end(&node);
    return 0;
}
