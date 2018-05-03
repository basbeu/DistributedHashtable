/**
 * @file network.c
 * @brief
 *
 * @author Bastien Beuchat and Andrea Scalisi
 * @date 20 Mar 2018
 */
#include <stdlib.h>
#include <string.h>
#include "network.h"
#include "config.h"

#define R 2
#define N 3
#define W 2

error_code send_request(node_t node, const int socket, pps_key_t key, pps_value_t* value, const size_t size_data)
{
    M_REQUIRE_NON_NULL(key);

    error_code error = ERR_NONE;

    ssize_t out_msg_len = sendto(socket, key, size_data, 0,(struct sockaddr *)&node.srv_addr, sizeof(node.srv_addr));

    char* temp_value = calloc(MAX_MSG_ELEM_SIZE, sizeof(char));
    ssize_t in_msg_len = recv(socket, temp_value, MAX_MSG_ELEM_SIZE, 0);

    if (out_msg_len == -1 || in_msg_len == -1 || (strncmp(temp_value, "\0", 1) == 0 && in_msg_len != 0)) {

        error = ERR_NETWORK;
    }

    *value = temp_value;

    return error;
}

pps_value_t increment_counter(pps_value_t counter, int* R_reached){
	char* c = calloc(1, sizeof(char));
	strncpy(c,counter, 1);
	++c[0];
	
	*R_reached = c[0] == R;
	
	return c;
}

error_code network_get(client_t client, pps_key_t key, pps_value_t* value)
{
	Htable_t quorum = construct_Htable(HTABLE_SIZE);
	
	error_code err = ERR_NONE;
	int R_reached = 0;
    for(size_t i = 0; i < N && R_reached == 0; ++i) {
        err = send_request(client.list_servers->list_of_nodes[i], client.socket, key, value, strlen(key));
		
		if(err == ERR_NONE){
			pps_value_t counter_value = get_Htable_value(quorum, *value);
			
			if(counter_value!= NULL){
				add_Htable_value(quorum, *value, increment_counter(counter_value,&R_reached )); 
			}else{
				add_Htable_value(quorum, *value, increment_counter("\x00",&R_reached ));
			}
		}
		
    }

	delete_Htable_and_content(&quorum);
    
    if(R_reached == 0){
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
    strncpy(out_msg, key, size_key);
    out_msg[size_key + 1] = '\0';
    strncpy(&out_msg[size_key+1], value, size_value);
    return out_msg;
}


error_code network_put(client_t client, pps_key_t key, pps_value_t value)
{
    if(strlen(key) > MAX_MSG_ELEM_SIZE || strlen(value) > MAX_MSG_ELEM_SIZE) {
        return ERR_BAD_PARAMETER;
    }

    char* out_msg = NULL;
    size_t size_msg = 0;
    out_msg = prepare_msg(key, value, &size_msg);

    int write_counter = 0;
    for(size_t i = 0; i < N; ++i) {
        error_code ans = send_request(client.list_servers->list_of_nodes[i], client.socket, out_msg, &value, size_msg);
        if(ans == ERR_NONE){
			++write_counter;
		}
    }

    return write_counter >= W ? ERR_NONE : ERR_NETWORK;
}
