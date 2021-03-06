/**
 * @file network.c
 * @date 20 Mar 2018
 */
#include <stdlib.h>
#include <string.h>

#include "network.h"
#include "system.h"
#include "config.h"

#define TIMEOUT 1


error_code send_request(node_t node, const int socket, pps_key_t key, pps_value_t* value, const size_t size_data)
{
    M_REQUIRE_NON_NULL(key);

    error_code error = ERR_NONE;

    ssize_t out_msg_len = sendto(socket, key, size_data, 0,(struct sockaddr *)&node.srv_addr, sizeof(node.srv_addr));
    
    if(out_msg_len == -1) {
        debug_print("ERR_NETWORK", 0);
        error = ERR_NETWORK;
    }

    return error;
}

error_code get_answer(const int socket, pps_value_t* value)
{
    char* temp_value = calloc(MAX_MSG_ELEM_SIZE, sizeof(char));

    error_code error = ERR_NONE;

    if(temp_value == NULL) {
        return ERR_NOMEM;
    }
    struct sockaddr_in temp_srv_addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    ssize_t in_msg_len = recvfrom(socket, temp_value, MAX_MSG_ELEM_SIZE, 0, (struct sockaddr *)&temp_srv_addr, &addr_len);

    if (in_msg_len == -1) {
        //debug_print("ERR_NETWORK", 0);
        error = ERR_NETWORK;
    } else if(strncmp(temp_value, "\0", 1) == 0 && in_msg_len != 0) {
        //debug_print("ERR_NOT_FOUND", 0);
        error = ERR_NOT_FOUND;
    }

    *value = temp_value;
    return error;
}

pps_value_t increment_counter(pps_value_t counter, size_t* R_reached, size_t R)
{
    char* c = calloc(1, sizeof(char));

    if(c != NULL) {
        strncpy(c,counter, 1);
        ++c[0];

        *R_reached = c[0] == R;
    }

    return c;
}

error_code network_get(client_t client, pps_key_t key, pps_value_t* value)
{
    M_REQUIRE_NON_NULL(key);

    if(strlen(key) > MAX_MSG_ELEM_SIZE) {
        return ERR_BAD_PARAMETER;
    }

    Htable_t quorum = construct_Htable(client.args->N);
    if(quorum.size == 0 || quorum.bucket == NULL) {
        return ERR_NOMEM;
    }


    int socket = get_socket(TIMEOUT);
    if(socket == -1) {
        return ERR_NETWORK;                    // correcteur: dans ces cas d'erreur vous oubliez de libérez la table (-1pts)
    }

    error_code err = ERR_NONE;
    size_t R_reached = 0;

    node_list_t* nodes_for_key = ring_get_nodes_for_key(client.list_servers, client.args->N, key);
    for(size_t i = 0; i < client.args->N && R_reached == 0 && i < nodes_for_key->size; ++i) {
        err = send_request(nodes_for_key->list_of_nodes[i], socket, key, value, strlen(key));

    }
    for(size_t i = 0; i < client.args->N && R_reached == 0 && i < nodes_for_key->size; ++i) {
        err = get_answer(socket, value);

        if(err == ERR_NONE) {
            pps_value_t counter_value = get_Htable_value(quorum, *value);

            if(counter_value!= NULL) {
                pps_value_t value_temp = increment_counter(counter_value,&R_reached, client.args->R );
                if(value_temp == NULL) {
                    return ERR_NOMEM;
                }
                add_Htable_value(quorum, *value, value_temp);
                free_const_ptr(value_temp);
            } else {
                add_Htable_value(quorum, *value, increment_counter("\x00",&R_reached, client.args->R ));
            }
                                // correcteur: vous oubliez de free counter_value (alloué par get_Htable_value) (-0.5pts)
        }
    }

    delete_Htable_and_content(&quorum);

    if(R_reached == 0) {
        value = NULL;
        return ERR_NOT_FOUND;
    }

    return err;
}


char* prepare_msg(pps_key_t key, pps_value_t value, size_t* size_msg)
{
    size_t size_key = strlen(key);
    size_t size_value = strlen(value);
    *size_msg = size_key + size_value + 1;


    char* out_msg = calloc(*size_msg, sizeof(char));
    if(out_msg != NULL) {
        strncpy(out_msg, key, size_key);
        out_msg[size_key + 1] = '\0';
        strncpy(&out_msg[size_key+1], value, size_value);
    }

    return out_msg;
}


error_code network_put(client_t client, pps_key_t key, pps_value_t value)
{
    if(strlen(key) > MAX_MSG_ELEM_SIZE || strlen(value) > MAX_MSG_ELEM_SIZE) {
        return ERR_BAD_PARAMETER;
    }

    int socket = get_socket(TIMEOUT);
    if(socket == -1) {
        return ERR_NETWORK;
    }

    char* out_msg = NULL;
    size_t size_msg = 0;
    out_msg = prepare_msg(key, value, &size_msg);                    // correcteur: testez la valeur de retour (-0.5pts)

    int write_counter = 0;

    node_list_t* nodes_for_key = ring_get_nodes_for_key(client.list_servers, client.args->N, key);                    // correcteur: testez la valeur de retour (-0.5pts)
    error_code ans = ERR_NONE;
	
    for(size_t i = 0; i < nodes_for_key->size && i < client.args->N; ++i) {
        ans = send_request(nodes_for_key->list_of_nodes[i], socket, out_msg, &value, size_msg);
    }

    for(size_t i = 0; i < nodes_for_key->size && i < client.args->N && write_counter<client.args->W ; ++i) {
        ans = get_answer(socket,&value);
        if(ans == ERR_NONE) {
            ++write_counter;                    // correcteur: même remarque (-1pts)
        }
    }

    free(out_msg);                    // correcteur: vous oubliez de libérer nodes_for_key (-0.5pts)
    return write_counter >= client.args->W ? ERR_NONE : ERR_NETWORK;
}
