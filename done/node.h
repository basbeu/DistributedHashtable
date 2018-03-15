#pragma once

/**
 * @file node.h
 * @brief Node (= server until week 11) definition and related functions
 *
 * @author Valérian Rousset
 */

#include <netinet/in.h>
#include <openssl/sha.h>

#include "error.h"
#include "util.h" // for _unused macro

/**
 * @brief node data structure
 */
/* TODO WEEK 05:
 * Définir ici le type node_t
 * (et supprimer ces trois lignes de commentaire).
 */

/**
 * @brief node initialization function
 * @param node node to be initalized (modified)
 * @param ip server IP address
 * @param port server port
 * @param node_id after week 11 (included), specify the server node id in a key ring. Unused before week 11.
 * @return some error code
 */
error_code node_init(node_t *node, const char *ip, uint16_t port, size_t _unused node_id);

/**
 * @brief all what needs to be done when a node is removed.
 *        Actually useless (=empty) in the current version of the project, but remains here as a placeholder
 * @param node the node that is removed, passed by reference as it might be modified.
 */
void node_end(node_t *node);

/**
 * @brief tool function to sort nodes according to their SHA
 * @param two nodes to be compared
 * @return <0 if first node comes first, 0 if equal and >0 is second node comes first
 */
int node_cmp_sha(const node_t *first, const node_t *second);

/**
 * @brief tool function to sort node according to their SERVER address
 * @param two nodes to be compared
 * @return <0 if server of first node comes first, 0 if equal and >0 is sever of second node comes first
 */
int node_cmp_server_addr(const node_t *first, const node_t *second);
