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

error_code send_request(node_t node, const int socket, pps_key_t key, pps_value_t* value, const size_t size_data)
{
    M_REQUIRE_NON_NULL(key);

    error_code error = ERR_NONE;

    ssize_t out_msg_len = sendto(socket, key, size_data, 0,(struct sockaddr *)&node.srv_addr, sizeof(node.srv_addr));           // correcteur: vous devez tester si sendto a marché AVANT d'attendre de recv quelque chose (-0.5pts)

    char* temp_value = calloc(MAX_MSG_ELEM_SIZE, sizeof(char));                                 // correcteur: vous ne vérifiez pas la valeur de retour de calloc (-0.5pts)
    ssize_t in_msg_len = recv(socket, temp_value, MAX_MSG_ELEM_SIZE, 0);                        // correcteur: vuos devez utiliser recvfrom (et ses deux derniers paramêtres) pour tester si c'est le bon serveur qui vous a répondu (-1pts)

    if (out_msg_len == -1 || in_msg_len == -1 || (strncmp(temp_value, "\0", 1) == 0 && in_msg_len != 0)) {  // correcteur: strncmp ne marchera pas dans ce cas (voir le cours sur les chaine de character pour une explication) (-0.5pts)

        error = ERR_NETWORK;                                                                                // correcteur: vous ne vérifiez pas assez précisement ce qui s'est mal passé (ERR_NOT_FOUND pour un get par exemple) (-1pts)
    }

    *value = temp_value;

    return error;
}

error_code network_get(client_t client, pps_key_t key, pps_value_t* value)
{                                                                               // correcteur: vous devriez vérifier la validité des arguments (-0.5pts)
    error_code err = ERR_NETWORK;
    for(size_t i = 0; i < client.list_servers->size && err != ERR_NONE; ++i) {
        err = send_request(client.list_servers->list_of_nodes[i], client.socket, key, value, strlen(key));      // correcteur: vous devez vous arrêter dès qu'une requête a marché (-0.5pts)
    }                                                                                                           // correcteur: cette erreur provoque aussi des fuite de mémoire, car value (alouée dans send_request) n'est jamais free (-0.5pts)

    return err;
}


char* prepare_msg(pps_key_t key, pps_value_t value, size_t* size_msg)
{
    size_t size_key = strlen(key);
    size_t size_value = strlen(value);
    *size_msg = size_key + size_value + 1;


    char* out_msg = calloc(*size_msg, sizeof(char));                // correcteur: vous ne vérifiez pas la valeur de retour de malloc
    strncpy(out_msg, key, size_key);
    out_msg[size_key + 1] = '\0';
    strncpy(&out_msg[size_key+1], value, size_value);
    return out_msg;
}


error_code network_put(client_t client, pps_key_t key, pps_value_t value)
{
    if(strlen(key) > MAX_MSG_ELEM_SIZE || strlen(value) > MAX_MSG_ELEM_SIZE) {      // correcteur astuce: voir la macro M_EXIT_IF_TOO_LONG
        return ERR_BAD_PARAMETER;
    }

    char* out_msg = NULL;
    size_t size_msg = 0;
    out_msg = prepare_msg(key, value, &size_msg);

    error_code err = ERR_NONE;
    for(size_t i = 0; i < client.list_servers->size; ++i) {
        error_code ans = send_request(client.list_servers->list_of_nodes[i], client.socket, out_msg, &value, size_msg);
        if(ans != ERR_NONE) {
            err = ans;
        }
    }
                                                                                    // correcteur: vous ne libérez pas out_msg (-0.5pts)
    return err;
}
