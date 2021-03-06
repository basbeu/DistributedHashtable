/**
 * @file network.c
 * @date 20 Mar 2018
 */
#include <stdlib.h>
#include <string.h>
#include "network.h"
#include "config.h"


error_code send_request(node_t node, const int socket, pps_key_t key, pps_value_t* value, const size_t size_data)
{
    M_REQUIRE_NON_NULL(key);

    error_code error = ERR_NONE;

    ssize_t out_msg_len = sendto(socket, key, size_data, 0,(struct sockaddr *)&node.srv_addr, sizeof(node.srv_addr));

    if(out_msg_len != -1) {
        char* temp_value = calloc(MAX_MSG_ELEM_SIZE, sizeof(char));

        if(temp_value == NULL) {
            return ERR_NOMEM;
        }

        //correcteur: il faut initialiser addr_len à sizeof(node.srv_addr) sinon le recvfrom fail quasi 1 fois sur 2 (-2)
        socklen_t addr_len;
        //correcteur: il faut passer une autre structure vide comme avant dernier argument et vérifier qu'après la fonction la mémoire
        //correcteur: corresponde à node.srv_addr. Il faut aussi vérifier que addr_len corresponde à la taille de la structure avant l'appel (-1)
        ssize_t in_msg_len = recvfrom(socket, temp_value, MAX_MSG_ELEM_SIZE, 0, (struct sockaddr *)&node.srv_addr, &addr_len);

        //correcteur: on est dans un if out_msg_len != 1 donc la première condition n'est pas utile
        if (out_msg_len == -1 || in_msg_len == -1) {
            error = ERR_NETWORK;
        } else if(strncmp(temp_value, "\0", 1) == 0 && in_msg_len != 0) {
            error = ERR_NOT_FOUND;
        }

        *value = temp_value;
    } else {
        error = ERR_NETWORK;
    }

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

    //correcteur: la macro EXIT_IF_MSG_TOO_LONG fait ça aussi
    if(strlen(key) > MAX_MSG_ELEM_SIZE) {
        return ERR_BAD_PARAMETER;
    }
    //correcteur: on peut utiliser client->args->N comme taille pour la hashtable
    Htable_t quorum = construct_Htable(HTABLE_SIZE);
    if(quorum.size == 0 || quorum.bucket == NULL) {
        return ERR_NOMEM;
    }

    error_code err = ERR_NONE;
    size_t R_reached = 0;
    for(size_t i = 0; i < client.args->N && R_reached == 0; ++i) {
        err = send_request(client.list_servers->list_of_nodes[i], client.socket, key, value, strlen(key));

        if(err == ERR_NONE) {
            pps_value_t counter_value = get_Htable_value(quorum, *value);

            if(counter_value!= NULL) {
                add_Htable_value(quorum, *value, increment_counter(counter_value,&R_reached, client.args->R ));
                //correcteur: il faut free la valeur de retour de increment_counter (allouée dans increment_counter)
                //correcteur: car add_Htable_value fait une copie (-0.5)
            } else {
                add_Htable_value(quorum, *value, increment_counter("\x00",&R_reached, client.args->R ));
            }
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
    if(strlen(key) > MAX_MSG_ELEM_SIZE || strlen(value) > MAX_MSG_ELEM_SIZE) {      //correcteur astuce: voir la macro M_EXIT_IF_TOO_LONG
        return ERR_BAD_PARAMETER;
    }

    char* out_msg = NULL;
    size_t size_msg = 0;
    out_msg = prepare_msg(key, value, &size_msg);

    int write_counter = 0;
    for(size_t i = 0; i < client.args->N; ++i) {
        error_code ans = send_request(client.list_servers->list_of_nodes[i], client.socket, out_msg, &value, size_msg);
        if(ans == ERR_NONE) {
            ++write_counter;
            //correcteur: dans le cas où write_counter >= N il faut quitter (-1) sinon il y aura trop de put effectué
        }
    }
    free(out_msg);
    return write_counter >= client.args->W ? ERR_NONE : ERR_NETWORK;
}
