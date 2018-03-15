#pragma once

/**
 * @file client.h
 * @brief Client definition and related functions
 *
 * @author Valérian Rousset
 */

#include <stddef.h> // for size_t

#include "error.h"     // for error_code type
#include "node.h"      // in week 5
#include "node_list.h" // weeks 6 to 10
#include "args.h"      // weeks 10 and after
#include "ring.h"      // weeks 11 and after

/**
 * @brief client state
 */
/* TODO WEEK 05:
 * Définir ici le type client_t
 * (et supprimer ces trois lignes de commentaire).
 */

/**
 * @brief client_init function arguments.
 *        To be defined in week 05 and THEN UPDATED in week 10.
 */
/* TODO WEEK 05:
 * Définir ici le type client_init_args_t
 * (et supprimer ces trois lignes de commentaire).
 */

/**
 * @brief does all the work to be done at the end of life of a client
 * @param client the client to end
 */
void client_end(client_t *client);

/**
 * @brief does all the work to be done at the beginning of life of a client
 * @param client the client to initialize
 * @return some error code, if any.
 */
error_code client_init(client_init_args_t);
