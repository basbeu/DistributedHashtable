/**
 * @file pps-launch-server.c
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

    Htable_t table = construct_Htable(HTABLE_SIZE);

    if(table.size == 0 || table.bucket == NULL) {
        debug_print("Unable to create a Hashtable",0);
        return 1;
    }

    int socket = get_socket(TIMEOUT);
    if(socket == -1) {
        debug_print("Unable to get a socket", 0);
        return 1;
    }

    char address[ADD_LENGTH + 1];
    (void)memset(address, 0, ADD_LENGTH);
    uint16_t port = 0;

    int i = 0;
    int j = 0;
    do {
        printf("IP port? ");
        i = scanf("%15s", address);
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
                                      (struct sockaddr *) &cli_addr, &addr_len);
        if(in_msg_len != -1) {
            debug_print("received",0);
            size_t out_msg_len = 0;

            //dump request
            if(in_msg[0]== '\0' && in_msg_len == 1) {
                debug_print("DUMP REQUEST\n", 0);
                //MAX_MSG_SIZE
                kv_list_t* list_of_pairs = get_Htable_content(table);
                if(list_of_pairs != NULL) {

                    debug_print("list_of_pairs != NULL\n", 0);
                    kv_pair_t elem_insert;
                    size_t elem_size = 0;
                    size_t elem_key_size = 0;
                    size_t elem_value_size = 0;
                    char out_msg[MAX_MSG_SIZE];
                    (void)memset(&out_msg, '\0', MAX_MSG_SIZE);
                    sprintf(out_msg, "%d", (int)list_of_pairs->size);
                    out_msg_len += 4;


                    for(size_t k = 0; k < list_of_pairs->size; ++ k) {
                        elem_insert = list_of_pairs->list_pair[k];
                        elem_key_size = strlen(elem_insert.key);
                        elem_value_size = strlen(elem_insert.value);

                        elem_size = elem_key_size + elem_value_size + 1;
                        if(out_msg_len + elem_size < MAX_MSG_SIZE) {
                            //INSERT
                            strncpy(&(out_msg[out_msg_len]), elem_insert.key, elem_key_size);
                            strncpy(&(out_msg[out_msg_len+elem_key_size+1]), elem_insert.value, elem_value_size);
                            out_msg_len += elem_size+1;
                        } else {
                            send_answer(socket, out_msg, out_msg_len-1, &cli_addr, addr_len);
                            out_msg_len = 0;
                            (void)memset(&out_msg, '\0', MAX_MSG_SIZE);
                            strncpy(&(out_msg[out_msg_len]), elem_insert.key, elem_key_size);
                            strncpy(&(out_msg[out_msg_len+elem_key_size+1]), elem_insert.value, elem_value_size);
                            out_msg_len += elem_size+1;

                        }
                    }

                    send_answer(socket, out_msg, out_msg_len-1, &cli_addr, addr_len);
                    kv_list_free(list_of_pairs);
                }
            } else if(in_msg_len == 0) {
                //ping
                send_answer(socket, NULL, out_msg_len, &cli_addr, addr_len);

            }

            else {
                //put or write request
                kv_pair_t pair = decompose_msg(in_msg, in_msg_len);
                debug_print("%s %s", pair.key, pair.value);
                pps_value_t out_msg = NULL;

                //Writing request
                if(strlen(pair.value) != 0) {
                    if( add_Htable_value(table, pair.key, pair.value) == ERR_NONE) {
                        send_answer(socket, out_msg, out_msg_len, &cli_addr, addr_len);
                    }
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
        }
    }

    delete_Htable_and_content(&table);
    return 0;
}

void send_answer(const int socket, pps_value_t out_msg, const size_t out_msg_len, const struct sockaddr_in* const cli_addr, socklen_t addr_len)
{
    debug_print("%s", out_msg);
    sendto(socket, out_msg, out_msg_len, 0, (struct sockaddr *) cli_addr, addr_len);
}


kv_pair_t decompose_msg(char* const msg, const size_t size_msg)
{
    size_t key_size = strlen(msg);
    return (kv_pair_t) {
        msg, &(msg[key_size+1])
    };
}
