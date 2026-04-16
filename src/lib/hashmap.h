//
// Created by willyy on 20/02/24.
//

#ifndef GIGS_HASHMAP_H
#define GIGS_HASHMAP_H

#include "uthash.h"

// 定义哈希表项结构体
typedef struct HashItem {
    char *key;            // 键
    char *value;          // 值
    UT_hash_handle hh;    // uthash句柄
} HashItem;

// 定义哈希表结构体
typedef struct HashTable {
    HashItem *hashitem;  // 指向哈希表项的指针
} HashTable;

void hm_init(HashTable *ht);

void hm_putItem(HashTable *ht, char *key, char *value);

char *hm_getItem(HashTable *ht, char *key);

void hm_removeItem(HashTable *ht, char *key);

void hm_for_each(HashTable *ht, void (*f)(char *, char *));

void hm_free(HashTable *ht);

int hm_test_main();
#endif //GIGS_HASHMAP_H
