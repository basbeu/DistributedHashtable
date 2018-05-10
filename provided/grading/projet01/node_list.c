/**
 * @file node_list.c
 * @brief
 *
 * @author Bastien Beuchat and Andrea Scalisi
 * @date 28 Mar 2018
 */
#include "node_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h> // for uint16_t
#include <string.h> // for memset()
#include "config.h"
#define ADD_LENGTH 15
#define DOUBLE_SIZE_OF_LIST 2

node_list_t* enlarge_list_of_nodes(node_list_t* list);

node_list_t *node_list_new()
{
    node_list_t* node_list_r = NULL;
    node_list_r = calloc(1 ,sizeof(node_list_t));                       // correcteur astuce: tout (et je dis bien TOUT) ce qui suis est inutile: calloc initialise la mémoire à 0 (et non NULL), ce qui veut dire que la size vaut 0, allocated vaut 0, et list_of_nodes vaut NULL. Vous auriez directement pu return ce calloc ;)
    if(node_list_r == NULL) {
        return NULL;
    } else {
        node_list_r->list_of_nodes = calloc(0, sizeof(node_t));         // correcteur: dans la majorité des implementations de calloc, si on essaie d'allouer 0 élément, alloc return NULL (-0.5pts)
        if(node_list_r->list_of_nodes == NULL) {
            return NULL;                                                // correcteur: dans ce cas vous ne libérez pas node_list_r (-0.5pts)
        } else {
            node_list_r->allocated = 0;
            node_list_r->size = 0;
            return node_list_r;
        }
    }
}
node_list_t *get_nodes()
{
    node_list_t* complete_list = node_list_new();           // correcteur: vous ne testez pas la valeur de retour de node_list_new (-0.5pts)
    node_t temp_node;

    FILE* server_list_file = NULL;
    server_list_file = fopen(PPS_SERVERS_LIST_FILENAME, "r");
    char address[ADD_LENGTH];                               // correcteur: vous oubliez la place pour le char '\0' (-0.5pts)
    (void)memset(&address, 0, ADD_LENGTH);                  // correcteur: faux! address est DEJA un pointeur (sur le premier character) (-0.5pts)
    uint16_t port = 0;
    if(server_list_file == NULL) {
        return NULL;
    } else {
        while(fscanf(server_list_file, "%s", address) == 1 && fscanf(server_list_file, "%" SCNu16, &port) == 1 &&       // correcteur: pourquoi ne pas "scanfer" en une fois les deux valeurs?
            !feof(server_list_file) && !ferror(server_list_file)) {                                                     // correcteur: vous ne contraignez pas scanf à ne lire au maximum 15 character dans address (overflow possible) (-0.5pts)
            debug_print("Adress is : %s, port : %" PRIu16 "\n", address, port);
            if(port <= 0 || port > 65535) {                                                                             // correcteur: c'est (presque) la définition d'un uint16_t (-0.5pts)
                fclose(server_list_file);
                return NULL;
            } else {

                error_code err = node_init(&temp_node, address, port, 0);
                if(err != ERR_NONE) {
                    fclose(server_list_file);
                    return NULL;
                } else {                                        // correcteur: vous avez défini node_list_add pour ce genre de cas il me semble ? (-0.5pts)
                    complete_list->list_of_nodes = realloc(complete_list->list_of_nodes, (complete_list->allocated+1)*sizeof(node_t));
                    complete_list->list_of_nodes[complete_list->size] = temp_node;
                    ++complete_list->allocated;
                    ++complete_list->size;
                }
            }

        }
        fclose(server_list_file);
        return complete_list;
    }


}

error_code node_list_add(node_list_t *list, node_t node)
{
    M_REQUIRE_NON_NULL(list);
    M_REQUIRE_NON_NULL(list->list_of_nodes);                    // correcteur: comme votre fonction node_list_new n'aloue pas de place list->list_of_nodes, ce dernier sera NULL quand list->size == 0, donc ce test est incorrecte (-0.5pts) 
    while(list->size >= list->allocated) {
        if(enlarge_list_of_nodes(list) == NULL) {
            return ERR_NOMEM;
        }
    }
    list->list_of_nodes[list->size] = node;
    ++list->size;
    return ERR_NONE;
}


void node_list_free(node_list_t *list)
{
    if((list != NULL) && (list->list_of_nodes != NULL)) {
        free(list->list_of_nodes);                              // correcteur: vous devriez appeler node_end sur chaque node (pas grave ici car node_end ne fait rien pour l'instant, mais pensez-y)
        list->list_of_nodes = NULL;
        list->size = 0;
        list->allocated = 0;
        free(list);
        list = NULL;                                            // correcteur astuce: cette affectation n'a aucun effet (list est local à node_list_free)
    }
}

node_list_t* enlarge_list_of_nodes(node_list_t* list)
{
    node_list_t* result = list;                                     // correcteur astuce: copier list n'a ici aucun intérêt
    if(result != NULL) {
        node_t* const old_list_of_nodes = result->list_of_nodes;
        result->allocated*=DOUBLE_SIZE_OF_LIST;                     // correcteur: comme votre fonction node_list_new initialise allocated à 0, vous ne pourrez JAMAIS enlarge votre node_list de cette manière (-0.5pts)
        if((result->allocated > SIZE_MAX / sizeof(node_t)) ||       // correcteur: vous devriez également vérifier que allocated*DOUBLE_SIZE_OF_LIST n'overflow pas (avant de le multiplier) (-0.5pts)
           ((result->list_of_nodes = realloc(result->list_of_nodes,
                                             result->allocated * sizeof(node_t))) == NULL)) {
            result->list_of_nodes = old_list_of_nodes;
            result->allocated/=DOUBLE_SIZE_OF_LIST;
            result = NULL;
        }
    }
    return result;
}
