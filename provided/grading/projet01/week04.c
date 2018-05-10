/**
 * @file week04.c
 * @brief test code for hashtables
 *
 * @author Valérian Rousset & Jean-Cédric Chappelier
 * @date 02 Oct 2017
 */

#include <stdio.h> // for puts(). to be removed when no longer needed.

#include <check.h>

#include "tests.h"
#include "hashtable.h"

START_TEST(add_value_does_retrieve_same_value)
{
    Htable_t table;

	const pps_key_t cle = 'c';
	const pps_value_t valeur_mise = 42;
	add_Htable_value(table, cle, valeur_mise);

	const pps_value_t valeur_lue = get_Htable_value(table, cle);

	ck_assert_int_eq(valeur_mise, valeur_lue);
	
	
}
END_TEST

START_TEST(hash_value_bigger_than_HTABLE_SIZE)
{
	const pps_key_t cle = 'c';
	ck_assert_int_eq(0,hash_function(cle, 8000));
}
END_TEST

START_TEST(add_Htable_value_null)
{
	const pps_key_t cle = 'c';
	const pps_value_t valeur_mise = 42;
	error_code code =  add_Htable_value(NULL, cle, valeur_mise);
	ck_assert_bad_param(code);
}
END_TEST


Suite *hashtable_suite()
{

    Suite *s = suite_create("hashtable.h");

    TCase *tc_ht = tcase_create("hashtable");
    suite_add_tcase(s, tc_ht);

    tcase_add_test(tc_ht, add_value_does_retrieve_same_value);
	tcase_add_test(tc_ht, hash_value_bigger_than_HTABLE_SIZE);
	tcase_add_test(tc_ht,add_Htable_value_null);
	
    return s;
}

TEST_SUITE(hashtable_suite)
