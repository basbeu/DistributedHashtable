/**
 * @file network.c
 * @brief 
 *
 * @author Bastien Beuchat and Andrea Scalisi
 * @date 20 Mar 2018
 */
#include "network.h"

error_code network_get(client_t client, pps_key_t key, pps_value_t *value){
	unsigned int out_msg;
    //out_msg = htonl(key);
	sendto(client.socket, &key, 1, 0,(struct sockaddr *)&client.node.srv_addr, sizeof(client.node.srv_addr));
	
    ssize_t in_msg_len = recv(client.socket, value, sizeof(value), 0);
    //*value = ntohl(*value);
}

error_code network_put(client_t client, pps_key_t key, pps_value_t value){
	//unsigned long msg = key;
	//msg <<= 32;
	//msg = msg|value;
	unsigned char out_msg[5];
	out_msg[0] = key;
	debug_print("network key : %d value : %d", key,value);

    value = htonl(value);
    
	debug_print("network key : %d value : %d", key,value);
    for(size_t i = 0; i < 4; ++i){
		out_msg[i+1] = (char) value;
		value >>= 8;
	} 
    
    
    
	sendto(client.socket, &out_msg, 5, 0,(struct sockaddr *)&client.node.srv_addr, sizeof(client.node.srv_addr));
	
	ssize_t in_msg_len = recv(client.socket, value, sizeof(value), 0);
}

