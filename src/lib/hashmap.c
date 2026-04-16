//
// Created by willyy on 20/02/24.
//

#include "hashmap.h"
// 初始化哈希表
void hm_init(HashTable *ht) {
    ht->hashitem = NULL;
}

// 向哈希表中添加或更新一个项
void hm_putItem(HashTable *ht, char *key, char *value) {
    HashItem *item;
    HASH_FIND_STR(ht->hashitem, key, item);  // 查找键对应的项
    if (item == NULL) {  // 如果项不存在，则添加新项
        item = (HashItem *)malloc(sizeof(HashItem));
        item->key = strdup(key);
        item->value = strdup(value);
        HASH_ADD_KEYPTR(hh, ht->hashitem, item->key, strlen(item->key), item);
    } else {  // 如果项已存在，则更新其值
        free(item->value);
        item->value = strdup(value);
    }
}

// 从哈希表中获取一个项的值
char *hm_getItem(HashTable *ht, char *key) {

    HashItem *item;
    HASH_FIND_STR(ht->hashitem, key, item);  // 查找键对应的项

    if (item == NULL) {
        return NULL;
    } else {
        return item->value;
    }
}

// 从哈希表中移除一个项
void hm_removeItem(HashTable *ht, char *key) {

    HashItem *item;
    HASH_FIND_STR(ht->hashitem, key, item);  // 查找键对应的项
    if (item != NULL) {
        HASH_DEL(ht->hashitem, item);  // 从哈希表中删除项
        free(item->key);
        free(item->value);
        free(item);
    }

}

// 遍历哈希表中的每一项，并对每一项执行给定的函数
void hm_for_each(HashTable *ht, void (*f)(char *, char *)) {

    HashItem *item, *tmp;
    HASH_ITER(hh, ht->hashitem, item, tmp) {
        f(item->key, item->value);  // 对每一项执行给定的函数
    }

}

// 释放哈希表占用的资源
void hm_free(HashTable *ht) {

    HashItem *current_item, *tmp;
    HASH_ITER(hh, ht->hashitem, current_item, tmp) {
        HASH_DEL(ht->hashitem, current_item);  // 从哈希表中删除项
        free(current_item->key);
        free(current_item->value);
        free(current_item);
    }

}