/**
 * @file hashtable.c
 * @brief 
 *
 * @author Bastien Beuchat and Andrea Scalisi
 */

#include "error.h"
#include "hashtable.h"

error_code add_Htable_value(Htable_t table, pps_key_t key, pps_value_t value){
	if(table == NULL){
		return ERR_BAD_PARAMETER;
	}
	
	table[hash_function(key, HTABLE_SIZE)] = value;
	 
	return ERR_NONE;
}

pps_value_t get_Htable_value(Htable_t table, pps_key_t key){
	return table[hash_function(key, HTABLE_SIZE)];
}

size_t hash_function(pps_key_t key, size_t table_size){
	if(table_size > HTABLE_SIZE){
		return 0;
	}
	
	return key % table_size;
}
