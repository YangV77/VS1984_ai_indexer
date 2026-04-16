//
// Created by willyy on 08/02/24.
//

#ifndef GIGS_LEVELDB_H
#define GIGS_LEVELDB_H
#include "stddef.h"

void package_init();

void package_tasks_put(const char *key, const char *value);

char *package_tasks_get(const char *key);

int package_tasks_delete(const char *key);

void package_tasks_for_each(void (*task)(const char *, size_t, const char *, size_t));

void package_destroy();

#endif //GIGS_LEVELDB_H
