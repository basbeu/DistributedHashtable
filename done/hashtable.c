/**
 * @file hashtable.c
 * @brief 
 *
 * @author Bastien Beuchat and Andrea Scalisi
 */

#include "error.h"
#include "hashtable.h"
#include <stdlib.h>
#include <string.h>
#include "config.h"

struct bucket_t{
		kv_pair_t pair;
		bucket_t* next;
};

error_code add_Htable_value(Htable_t table, pps_key_t key, pps_value_t value){
	M_REQUIRE_NON_NULL(table.bucket);
	M_REQUIRE_NON_NULL(key);
	M_REQUIRE_NON_NULL(value);
	
	size_t value_len = strnlen(value, MAX_MSG_ELEM_SIZE)+1;
	size_t key_len = strnlen(key, MAX_MSG_ELEM_SIZE)+1;
	
	char* val = NULL;
	strncpy(val, value, value_len);
	pps_value_t copied_value = val;;
	char* k = NULL;
	strncpy(k, key, key_len);	
	pps_key_t copied_key = k;
	
	
	size_t index = hash_function(key, table.size);
	bucket_t* b_temp = calloc(1, sizeof(bucket_t));
	b_temp->pair.key = NULL;
	b_temp->pair.value = NULL;
	b_temp->next = NULL;
	bucket_t insert;
	insert.pair.key = copied_key;
	insert.pair.value = copied_value;
	insert.next = NULL;
	b_temp = table.bucket[index].next;
	
	if(get_Htable_value(table, key) == NULL){

		while(b_temp->next != NULL){
			b_temp = b_temp->next;
		}
		
		*b_temp->next = insert;
	}
	else{
		while(b_temp->pair.key != key){
			b_temp = b_temp->next;
		}
		*b_temp = insert;
	}
		 
	return ERR_NONE;
}

pps_value_t get_Htable_value(Htable_t table, pps_key_t key){
	
	if(table.bucket == NULL || key == NULL){
		return NULL;
	}
	
	bucket_t b; 
	b.pair = table.bucket[hash_function(key, table.size)].pair;
	b.next = table.bucket[hash_function(key, table.size)].next;
	while(b.next != NULL){
		if(b.pair.key == key){
			return b.pair.value;
		}
		b.pair = b.next->pair;
		b.next = b.next->next;
	}
	
	if(b.pair.key == key){
		return b.pair.value;
	}
	else{
		return NULL;
	}
	 
}

Htable_t construct_Htable(size_t size){
	bucket_t * bucket = calloc(size, sizeof(bucket_t));
	Htable_t table;
	if(bucket != NULL){
		table.bucket = bucket;
		table.size = size;
		for(size_t i = 0; i < size; ++i){
			kv_pair_t pair;
			pair.key = NULL;
			pair.value = NULL;
			table.bucket[i].pair = pair;
			table.bucket[i].next = NULL;
		}
	}
	
	else{
		//TODO ERROR
	}	
	
	return table;
}

void delete_Htable_and_content(Htable_t* table){
	if(table != NULL){
		if(table->bucket != NULL){
			for(size_t i = 0; i < table->size; ++i){
			
				table->bucket[i].pair.key = NULL;
				table->bucket[i].pair.value = NULL;
				free(table->bucket[i].next);
				table->bucket[i].next = NULL;
			
			}
		table->size = 0;
		}
	}
}

size_t hash_function(pps_key_t key, size_t table_size){
    M_REQUIRE(table_size != 0, SIZE_MAX, "size == %d", 0);
    M_REQUIRE_NON_NULL_CUSTOM_ERR(key, SIZE_MAX);

    size_t hash = 0;
    const size_t key_len = strlen(key);
    for (size_t i = 0; i < key_len; ++i) {
        hash += (unsigned char) key[i];
        hash += (hash << 10);
        hash ^= (hash >>  6);
    }
    hash += (hash <<  3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash % table_size;
}
