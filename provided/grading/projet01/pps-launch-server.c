/**
 * @file pps-launch-server.c
 * @brief
 *
 * @author Bastien Beuchat and Andrea Scalisi
 * @date 20 Mar 2018
 */

// standard includes (printf, exit, ...)
#include <stdio.h>
#include <stdlib.h>
#include <errno.h> // for errno
#include <string.h> // for memset()

// for basic socket communication
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "system.h"
#include "config.h"
#include "hashtable.h"
#include <inttypes.h> // for uint16_t
#include <string.h> // for memset()

#define ADD_LENGTH 15
#define TIMEOUT 0

void send_answer(int socket, pps_value_t response, size_t out_msg_len, const struct sockaddr_in* const cli_addr, socklen_t addr_len);

kv_pair_t decompose_msg(char* msg, size_t size_msg);


int main(void)
{

    Htable_t table = construct_Htable(HTABLE_SIZE);             // correcteur: vous devriez vérifier si construct_Htable a bien marché (-0.5pts)

    debug_print("size of : %zu", sizeof(table));

    int socket = get_socket(TIMEOUT);
    if(socket == -1) {
        debug_print("Unable to get a socket", 0);
        return 1;
    }

    char address[ADD_LENGTH];                                   // correcteur: vous avez oublié la place pour le char '\0' (-0.5pts)
    (void)memset(&address, 0, ADD_LENGTH);
    uint16_t port = 0;

    printf("IP port? ");
    int i = 0;
    int j = 0;
    do {
        i = scanf("%s", address);                               // correcteur: vous devez limiter la longueur de la chaine acceptée pas scanf (sinon overflow possible) (-0.5pts)
        j = scanf("%" SCNu16, &port);
        while(!feof(stdin) && !ferror(stdin) && getc(stdin) != '\n');
    } while(i != 1 && j != 1);


    error_code err = bind_server(socket, address, port);
    if(err != ERR_NONE) {
        debug_print("Unable to bind server", 0);
        return 1;
    }

    debug_print("\nListening on : %s, port : %" PRIu16 "\n", address, port);

    while(1) {
        debug_print("Waiting for request", 0);
        struct sockaddr_in cli_addr;
        socklen_t addr_len = sizeof(cli_addr);
        memset(&cli_addr, 0, addr_len);

        char in_msg[MAX_MSG_SIZE];
        (void)memset(&in_msg, '\0', MAX_MSG_SIZE);

        ssize_t in_msg_len = recvfrom(socket, &in_msg, sizeof(in_msg), 0,
                                      (struct sockaddr *) &cli_addr, &addr_len);        // correcteur: vous ne vérifiez pas la valeur de retour de recvfrom (-0.5pts)
        debug_print("received",0);

        kv_pair_t pair = decompose_msg(in_msg, in_msg_len);

        debug_print("%s %s", pair.key, pair.value);

        pps_value_t out_msg = NULL;
        size_t out_msg_len = 0;

        //Writing request
        if(strlen(pair.value) != 0) {
            add_Htable_value(table, pair.key, pair.value);                          // correcteur: vous ne vérifiez pas la valeur de retour de add_Htable_value (-0.5pts)
            send_answer(socket, out_msg, out_msg_len, &cli_addr, addr_len);
        }

        //Reading request
        else {
            out_msg = get_Htable_value(table, pair.key);
            if(out_msg != NULL) {
                out_msg_len = strlen(out_msg);
            } else {
                out_msg_len = 1;
                char const temp_out_msg[1] = "\0";
                out_msg = temp_out_msg;
            }
            send_answer(socket, out_msg, out_msg_len, &cli_addr, addr_len);
        }
    }

    delete_Htable_and_content(&table);
    return 0;
}

void send_answer(const int socket, pps_value_t out_msg,const size_t out_msg_len, const struct sockaddr_in* const cli_addr, socklen_t addr_len)
{
    debug_print("%s", out_msg);
    sendto(socket, out_msg, out_msg_len, 0, (struct sockaddr *) cli_addr, addr_len);                    // correcteur: vous ne vérifiez pas la valeur de retour de sento (-0.5pts)
}


kv_pair_t decompose_msg(char* const msg, const size_t size_msg)
{
    size_t key_size = strlen(msg);
    char* key = calloc(key_size+1, sizeof(char));                       // correcteur: vous ne vérifiez pas la valeur de retour des deux calloc (-1pts)
    strncpy(key, msg, key_size);                                        // correcteur astuce: vous n'avez pas besoin de copier la valeur et la clé, un pointeur vers le début de la chaine suffis

    size_t value_size = size_msg - key_size;
    char* value = calloc(value_size + 1, sizeof(char));
    strncpy(value, &msg[key_size + 1], value_size);

    kv_pair_t pair;
    pair.key = key;
    pair.value = value;

    return pair;
}
