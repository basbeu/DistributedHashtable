#pragma once

/**
 * @file node_list.h
 * @brief Parsing and handling of a list of nodes (= list of servers until week 11).
 *        This data structure is required only from week 06.
 *
 * @author Valérian Rousset
 */

#include <netinet/in.h>
#include <stddef.h>

#include "node.h"

/**
 * @brief list of nodes
 */
struct node_list {
    /* TODO WEEK 06:
     * Définir proprement le type node_list_t
     */
    char useless_to_be_removed_in_week_06;
};
typedef struct node_list node_list_t;

/**
 * @brief creates a new, empty, node_list_t
 * @return (a pointer to) the new list of nodes
 */
node_list_t *node_list_new();

/**
 * @brief parse the PPS_SERVERS_LIST_FILENAME file and return the corresponding list of nodes
 * @return the list of nodes initialized from the server file (PPS_SERVERS_LIST_FILENAME)
 */
node_list_t *get_nodes();

/**
 * @brief add a node to a list of nodes
 * @param list list of nodes where to add to (modified)
 * @param node the node to be added
 * @return some error code indicating whether addition fails or not
 */
error_code node_list_add(node_list_t *list, node_t node);

/**
 * @brief sort node list according to node comparison function
 * @param list list of nodes to be sorted
 * @param comparator node comparison function
 */
void node_list_sort(node_list_t *list, int (*comparator)(const node_t *, const node_t *));

/**
 * @brief free the given list of nodes
 * @param list list of nodes to clean
 */
void node_list_free(node_list_t *list);
