/**
 * @file hashtable.c
 * @brief
 *
 */

#include "error.h"
#include "hashtable.h"
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include <stdio.h>
#include "util.h"

#define ENLARGE_FACTOR 2

struct bucket_t {
    kv_pair_t pair;
    bucket_t* next;
};

void bucket_free(bucket_t* bucket);
kv_list_t* kv_list_new();
error_code kv_list_add(kv_list_t *list, kv_pair_t pair);
kv_list_t* kv_list_enlarge(kv_list_t* list);


error_code add_Htable_value(Htable_t table, pps_key_t key, pps_value_t value)
{
    M_REQUIRE_NON_NULL(table.bucket);
    M_REQUIRE_NON_NULL(key);
    M_REQUIRE_NON_NULL(value);

    pps_value_t copied_value = strdup(value);
    if(copied_value == NULL) {
        return ERR_NOMEM;
    }
    pps_key_t copied_key = strdup(key);

    if(copied_key == NULL) {
        free_const_ptr(copied_value);
        return ERR_NOMEM;
    }


    size_t index = hash_function(key, table.size);
    bucket_t* b_temp;


    b_temp = &table.bucket[index];
    while(b_temp != NULL && b_temp->pair.key != NULL) {
        if(strncmp(b_temp->pair.key, key, MAX_MSG_ELEM_SIZE) == 0) {
            kv_pair_free(&b_temp->pair);
            b_temp->pair.key = copied_key;
            b_temp->pair.value = copied_value;

            return ERR_NONE;
        }
        if(b_temp->next != NULL) {
            b_temp = b_temp->next;
        } else {
            bucket_t* insert = calloc(1, sizeof(bucket_t));
            if(insert == NULL) {
                free_const_ptr(copied_value);
                free_const_ptr(copied_key);
                return ERR_NOMEM;
            }
            insert->next = NULL;
            insert->pair = (kv_pair_t) {
                copied_key, copied_value
            };
            b_temp->next = insert;
            return ERR_NONE;
        }
    }
    if(b_temp != NULL) {
        kv_pair_free(&b_temp->pair);
        b_temp->pair.key = copied_key;
        b_temp->pair.value = copied_value;
    }


    return ERR_NONE;
}

pps_value_t get_Htable_value(const Htable_t table, pps_key_t key)
{

    if(table.bucket == NULL || key == NULL) {
        return NULL;
    }

    bucket_t* b = &table.bucket[hash_function(key, table.size)];

    while(b->next != NULL) {
        if(strncmp(b->pair.key, key, MAX_MSG_ELEM_SIZE) == 0) {
            pps_value_t copied_value = strdup(b->pair.value);
            return copied_value;
        }
        b = b->next;
    }

    if(b->pair.key != NULL && strncmp(b->pair.key, key, MAX_MSG_ELEM_SIZE) == 0) {
        pps_value_t copied_value = strdup(b->pair.value);
        return copied_value;
    } else {
        return NULL;
    }
}

Htable_t construct_Htable(size_t size)
{
    Htable_t table;
    table.size = 0;
    table.bucket = NULL;

    if(size > 0) {
        table.bucket = calloc(size, sizeof(bucket_t));

        if(table.bucket != NULL) {
            table.size = size;
        }
    }

    return table;
}

void delete_Htable_and_content(Htable_t* table)
{
    if(table != NULL) {
        if(table->bucket != NULL) {
            for(size_t i = 0; i < table->size; ++i) {

                bucket_free(&table->bucket[i]);
            }
            free(table->bucket);
            table->bucket = NULL;
        }

        table->size = 0;
    }
}

size_t hash_function(pps_key_t key, size_t table_size)
{
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

void kv_pair_free(kv_pair_t *kv)
{
    if(kv != NULL) {
        if(kv->key != NULL) {
            free_const_ptr(kv->key );
            kv->key = NULL;
        }
        if(kv->value != NULL) {
            free_const_ptr(kv->value);
            kv->value = NULL;
        }
    }
}

void bucket_free(bucket_t* bucket)
{
    if(bucket != NULL) {
        kv_pair_free(&bucket->pair);
        bucket_t* b_curr = NULL;
        bucket_t* b_next = bucket->next;
        while(b_next != NULL) {
            b_curr = b_next;
            b_next = b_curr->next;
            kv_pair_free(&b_curr->pair);
            free(b_curr);
            b_curr = NULL;
        }
    }
}

kv_list_t* kv_list_new()
{
    kv_list_t* list = NULL;
    list = calloc(1 ,sizeof(kv_list_t));

    if(list == NULL) {
        return NULL;
    } else {
        list->list_pair = calloc(1, sizeof(kv_pair_t));

        if(list->list_pair == NULL) {
            free(list);
            return NULL;
        }

        list->allocated = 1;
        list->size = 0;
        return list;
    }
}

kv_list_t* kv_list_enlarge(kv_list_t* list)
{
    if(list != NULL) {
        kv_pair_t* const old_list_pair = list->list_pair;

        if(list->allocated < SIZE_MAX / ENLARGE_FACTOR) {
            list->allocated*=ENLARGE_FACTOR;
            if((list->allocated > SIZE_MAX / sizeof(kv_pair_t)) ||
               ((list->list_pair = realloc(list->list_pair,
                                           list->allocated * sizeof(kv_pair_t))) == NULL)) {
                list->list_pair = old_list_pair;
                list->allocated/=ENLARGE_FACTOR;
                list = NULL;
            }
        }
    }
    return list;
}


error_code kv_list_add(kv_list_t *list, kv_pair_t pair)
{
    M_REQUIRE_NON_NULL(list);
    M_REQUIRE_NON_NULL(list->list_pair);
    while(list->size >= list->allocated) {
        if(kv_list_enlarge(list) == NULL) {
            return ERR_NOMEM;
        }
    }

    kv_pair_t copied_pair;

    copied_pair.key = strdup(pair.key);
    if(copied_pair.key == NULL) {
        return ERR_NOMEM;
    }
    copied_pair.value = strdup(pair.value);
    if(copied_pair.value == NULL) {
        free_const_ptr(copied_pair.key);
        return ERR_NOMEM;
    }
    list->list_pair[list->size] = copied_pair;
    ++list->size;
    return ERR_NONE;
}

kv_list_t *get_Htable_content(Htable_t table)
{
    kv_list_t* list = kv_list_new();

    if(list != NULL) {
        bucket_t* temp_bucket = NULL;
        for(size_t i = 0; i < table.size; ++i) {
            temp_bucket = &table.bucket[i];

            while(temp_bucket != NULL && temp_bucket->pair.key != NULL) {
                if(kv_list_add(list,temp_bucket->pair) != ERR_NONE) {
                    kv_list_free(list);
                    return NULL;
                }
                temp_bucket = temp_bucket->next;
            }

        }
    }


    return list;
}

void kv_list_free(kv_list_t *list)
{
    if(list != NULL && list->list_pair != NULL) {
        for(size_t i = 0; i < list->size; ++i) {
            if(list->list_pair[i].key != NULL) {
                free_const_ptr(list->list_pair[i].key);
            }
            if(list->list_pair[i].value != NULL) {
                free_const_ptr(list->list_pair[i].value);
            }
        }
        free(list->list_pair);
        list->list_pair = NULL;
        free(list);
        list =NULL;
    }
}

