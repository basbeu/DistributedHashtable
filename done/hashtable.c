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
		kv_pair_t pair;
		bucket_t* next;
};

void bucket_free(bucket_t* bucket);

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
	
	
	size_t index = hash_function(key, table.size);
	bucket_t* b_temp;

	
	b_temp = &table.bucket[index];
	while(b_temp != NULL && b_temp->pair.key != NULL){
		if(strncmp(b_temp->pair.key, key, MAX_MSG_ELEM_SIZE) == 0){
			b_temp->pair.key = copied_key;
			b_temp->pair.value = copied_value;
			
			return ERR_NONE;
		}
		if(b_temp->next != NULL){
			b_temp = b_temp->next;
		}
		else{
			bucket_t* insert = calloc(1, sizeof(bucket_t));
			insert->next = NULL;
			kv_pair_t pair;
			pair.key = copied_key;
			pair.value = copied_value;
			insert->pair = pair;
			b_temp->next = insert;
			return ERR_NONE;
		}
	}
	if(b_temp != NULL){
		b_temp->pair.key = copied_key;
		b_temp->pair.value = copied_value;
	}
	
 
	return ERR_NONE;
}

pps_value_t get_Htable_value(const Htable_t table, pps_key_t key){
	
	if(table.bucket == NULL || key == NULL){
		return NULL;
	}
	
	bucket_t* b = &table.bucket[hash_function(key, table.size)]; 
	
	while(b->next != NULL){
		if(strncmp(b->pair.key, key, MAX_MSG_ELEM_SIZE) == 0){
			return b->pair.value;
		}
		b = b->next;
	}

	if(b->pair.key != NULL && strncmp(b->pair.key, key, MAX_MSG_ELEM_SIZE) == 0){
		return b->pair.value;
	}
	else{
		return NULL;
	}
	 
}

Htable_t construct_Htable(size_t size){
	Htable_t table;
	table.size = 0;
	table.bucket = NULL;
	
	if(size > 0){
		table.bucket = calloc(size, sizeof(bucket_t));
		
		if(table.bucket != NULL){
			table.size = size;
			for(size_t i = 0; i < size; ++i){
				kv_pair_t pair;
				pair.key = NULL;
				pair.value = NULL;
				
				table.bucket[i].pair = pair;
				table.bucket[i].next = NULL;
			}
		}	
	}
	
	return table;
}

void delete_Htable_and_content(Htable_t* table){
	if(table != NULL){
		if(table->bucket != NULL){
			for(size_t i = 0; i < table->size; ++i){
				
				bucket_free(&table->bucket[i]);
			}
			free(table->bucket);
			table->bucket = NULL;
		}

	table->size = 0;
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

void kv_pair_free(kv_pair_t *kv){
	if(kv->key != NULL){
		free_const_ptr(kv->key );
		kv->key = NULL;
	}
	if(kv->value != NULL){
		free_const_ptr(kv->value); 
		kv->value = NULL;
	}
}

void bucket_free(bucket_t* bucket){
	if(bucket != NULL){
		kv_pair_free(&bucket->pair);
		bucket_t* b_curr = NULL;
		bucket_t* b_next = bucket->next;
		while(b_next != NULL){
			b_curr = b_next;
			b_next = b_curr->next;
			free(b_curr);
			b_curr = NULL;
		}
	}
}
