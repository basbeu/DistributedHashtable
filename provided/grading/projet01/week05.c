/**
 * @file week05.c
 * @brief test code for week 05
 *
 * @author Bastien Beuchat
 * @date 19 Mar 2018
 */

#include <stdio.h> // for puts(). to be removed when no longer needed.

#include <check.h>

#include "tests.h"
#include "node.h"
#include "config.h"
#include "client.h"
#include "network.h"

START_TEST(node_init_exception)
{
	node_t* node = NULL;
	error_code code = node_init(node, PPS_DEFAULT_IP, PPS_DEFAULT_PORT, 0);
	ck_assert_bad_param(code);	
}
END_TEST

START_TEST(client_init_exception)
{
	node_t* node = NULL;
	client_init_args_t client_to_init;
	
	client_to_init.client = NULL;
	client_to_init.name = "test";
	
	error_code code = client_init(client_to_init);
	ck_assert_bad_param(code);	
}
END_TEST

START_TEST(network_compile)
{
	pps_key_t key;
	pps_value_t value;
	
	node_t* node = NULL;
	client_init_args_t client_to_init;
	client_t client;
	client_to_init.client = &client;
	client_to_init.name = "test";
	
	network_get(client, key, &value);
	network_put(client, key, value);
	ck_assert_int_eq(1,1);
}
END_TEST

Suite *hashtable_suite()
{

    Suite *s = suite_create("week05");

    TCase *tc_node = tcase_create("node");
    suite_add_tcase(s, tc_node);

    tcase_add_test(tc_node, node_init_exception);

	TCase *tc_client = tcase_create("client");
    suite_add_tcase(s, tc_client);
	
	tcase_add_test(tc_client, client_init_exception);
	
	TCase *tc_network = tcase_create("network");
    suite_add_tcase(s, tc_network);
	
	tcase_add_test(tc_network, network_compile);
	
    return s;
}

TEST_SUITE(hashtable_suite)
