//
// Created by willyy on 08/02/24.
//

#include "bt_tasks.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "c.h"
#include "uv.h"

typedef void (*foreach_cb)(const char *, size_t, const char *, size_t);
// 单例类定义
typedef struct {
    leveldb_t *db;
    leveldb_options_t *options;
    leveldb_readoptions_t *roptions;
    leveldb_writeoptions_t *woptions;
    foreach_cb bt_tasks_cb;
} SingletonKVDB;

// 静态实例和标志
static SingletonKVDB *instance = NULL;
static int initialized = 0;
const char *bt_tasks_path_str = "bt_tasks";
// 初始化单例
int leveldb_bt_task_init() {
    if (!initialized) {
        instance = (SingletonKVDB *)malloc(sizeof(SingletonKVDB));
        instance->options = leveldb_options_create();
        leveldb_options_set_create_if_missing(instance->options, 1);

        char tasks_folder[512];
        snprintf(tasks_folder, sizeof(tasks_folder), "%s", bt_tasks_path_str);
        char *err = NULL;
        instance->db = leveldb_open(instance->options, tasks_folder, &err);
        if (err != NULL) {
            fprintf(stderr, "Unable to open/create task db: %s\n", err);
            return 1;
        }
        instance->roptions = leveldb_readoptions_create();
        instance->woptions = leveldb_writeoptions_create();
        leveldb_writeoptions_set_sync(instance->woptions, 1);
        instance->bt_tasks_cb = NULL;
        initialized = 1;
    }
    return 0;
}

// 封装写入接口
static void leveldb_bt_task_put(const char *key, const char *value) {
    char *err = NULL;
    leveldb_put(instance->db, instance->woptions, key, strlen(key), value, strlen(value) + 1, &err);
    if (err != NULL) {
        fprintf(stderr, "Write failed: %s\n", err);
        exit(1);
    }
}

// 封装读取接口
static char *leveldb_bt_task_get(const char *key) {
    char *err = NULL;
    size_t read_len;
    char *value = leveldb_get(instance->db, instance->roptions, key, strlen(key), &read_len, &err);
    if (err != NULL) {
        fprintf(stderr, "Read failed: %s\n", err);
        exit(1);
    }
    return value;
}

// 遍历 LevelDB 中的所有键值对，并对每个键值对调用传入的处理函数
static void leveldb_bt_task_for_each(void (*processor)(const char *, size_t, const char *, size_t)) {
    leveldb_iterator_t *iter;
    iter = leveldb_create_iterator(instance->db, instance->roptions);
    for (leveldb_iter_seek_to_first(iter); leveldb_iter_valid(iter); leveldb_iter_next(iter)) {
        size_t key_len, value_len;
        const char *key = leveldb_iter_key(iter, &key_len);
        const char *value = leveldb_iter_value(iter, &value_len);
        processor(key, key_len, value, value_len);
    }
    leveldb_iter_destroy(iter);
}

// 清理资源
void leveldb_bt_task_destroy() {
    if (instance != NULL) {
        leveldb_close(instance->db);
        leveldb_options_destroy(instance->options);
        leveldb_readoptions_destroy(instance->roptions);
        leveldb_writeoptions_destroy(instance->woptions);
        free(instance);
        initialized = 0;
    }
}

// 封装删除接口
static int leveldb_bt_task_delete(const char *key) {
    char *err = NULL;
    leveldb_delete(instance->db, instance->woptions, key, strlen(key),&err);
    if (err != NULL) {
        fprintf(stderr, "Read failed: %s\n", err);
        return 1;
    }
    return 0;
}

void leveldb_bt_task_free(void* ptr){
    leveldb_free(ptr);
}

void package_init()
{
    leveldb_bt_task_init();
}

void package_tasks_put(const char *key, const char *value){
    leveldb_bt_task_put(key, value);
}

char *package_tasks_get(const char *key) {
    char *result = leveldb_bt_task_get(key);
    return result;
}

int package_tasks_delete(const char *key) {
    return leveldb_bt_task_delete(key);
}

void package_tasks_processor(const char *key, size_t key_len, const char *value, size_t value_len){
        instance->bt_tasks_cb(key, key_len, value, value_len);
}

void package_tasks_for_each(foreach_cb cb) {
    instance->bt_tasks_cb = cb;
    leveldb_bt_task_for_each(package_tasks_processor);
}

void package_destroy()
{
    leveldb_bt_task_destroy();
}