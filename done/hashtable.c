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
#include <stdio.h>
#include "util.h"

struct bucket_t{
		kv_pair_t* pair;
		bucket_t* next;
};

error_code add_Htable_value(Htable_t table, pps_key_t key, pps_value_t value){
	M_REQUIRE_NON_NULL(table.bucket);
	M_REQUIRE_NON_NULL(key);
	M_REQUIRE_NON_NULL(value);
	
	size_t value_len = strnlen(value, MAX_MSG_ELEM_SIZE)+1;
	size_t key_len = strnlen(key, MAX_MSG_ELEM_SIZE)+1;
	
	char* val = calloc(value_len, sizeof(char));
	strncpy(val, value, value_len);
	pps_value_t copied_value = val;
	char* k = calloc(key_len, sizeof(char));
	strncpy(k, key, key_len);	
	pps_key_t copied_key = k;
	kv_pair_t final_pair;// = calloc(1, sizeof(kv_pair_t));
	final_pair.value = copied_value;
	final_pair.key = copied_key;
	
	size_t index = hash_function(key, table.size);
	bucket_t* b_temp;

	
	b_temp = &table.bucket[index];
	while(b_temp->pair->key != NULL){
		if(strncmp(b_temp->pair->key, key, MAX_MSG_ELEM_SIZE) == 0){
			/*b_temp->pair->key = copied_key;
			b_temp->pair->value = copied_value;*/
			b_temp->pair = &final_pair;
			
			return ERR_NONE;
		}
		b_temp = b_temp->next;
	}
	/*b_temp->pair->key = copied_key;
	b_temp->pair->value = copied_value;*/
	b_temp->pair = &final_pair;
	
	
	free(val);
	free(k);	 
	return ERR_NONE;
}

pps_value_t get_Htable_value(Htable_t table, pps_key_t key){
	
	if(table.bucket == NULL || key == NULL){
		return NULL;
	}
	
	bucket_t* b = &table.bucket[hash_function(key, table.size)]; 

	while(b->next != NULL){
		if(strncmp(b->pair->key, key, MAX_MSG_ELEM_SIZE) == 0){
			return b->pair->value;
		}
		b = b->next;
	}

	if(b->pair != NULL && b->pair->key != NULL && strncmp(b->pair->key, key, MAX_MSG_ELEM_SIZE) == 0){
		return b->pair->value;
	}
	else{
		return NULL;
	}
	 
}

Htable_t construct_Htable(size_t size){
	Htable_t table;
	table.bucket = calloc(size, sizeof(bucket_t));
	
	if(table.bucket != NULL){
		table.size = size;
		for(size_t i = 0; i < size; ++i){
			kv_pair_t* pair = calloc(1, sizeof(kv_pair_t));
			pair->key = NULL;
			pair->value = NULL;
			/*kv_pair_t pair;
			pair.key = NULL;
			pair.value = NULL;*/
			
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
			
				table->bucket[i].pair->key = NULL;
				table->bucket[i].pair->value = NULL;
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

int main(void){
	Htable_t table = construct_Htable(256);
	
	char* v = calloc(10, 1);
	strncpy(v, "SAMARCH", 10);
	pps_value_t value = v;
	
	char* k = calloc(10, 1);
	strncpy(k, "25", 10);
	pps_key_t key = k;
	printf("%zu\n", hash_function(key, 256));
	//add_Htable_value(table, key, value);

	bucket_t bucket;
	bucket.pair = calloc(1, sizeof(kv_pair_t));
	bucket.pair->key = calloc(10, 1);
	bucket.pair->value = calloc(10,1);
	bucket.next = calloc(1, sizeof(bucket_t));
	bucket.pair->value = value;
	bucket.pair->key = key;
	
	//table.bucket[hash_function(key, 256)] = bucket;
	//table.bucket[0] = bucket;
	add_Htable_value(table, key, value);
	pps_value_t val = get_Htable_value(table, key);
	
	//pps_value_t val = table.bucket[0].pair.value;
	if(val !=NULL){
		printf("%s\n", val);
	}
	else{
		printf("NULL\n");
	}
	
	free_const_ptr(bucket.pair->value);
	free_const_ptr(bucket.pair->key);
	free(bucket.next);
	free(v);
	free(k);
	
	return 1;

}


