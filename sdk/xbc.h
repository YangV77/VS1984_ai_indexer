//
// Created by willyy on 31/01/24.
//

#ifndef LIBUV_TCP_UDP_APP_IGNIS_H
#define LIBUV_TCP_UDP_APP_IGNIS_H

#include <stddef.h>

typedef struct XbcInstance XbcInstance;

typedef struct {
    size_t value_length;
    void* value;
    size_t key_length;
    void* key;
} callback_data_t;

typedef struct {
    size_t log_length;
    char* log;
} log_cb_t;

typedef struct {
    size_t detail_length;
    char* detail;
} dbgr_cb_t;

typedef void (*callback_handler_t)(const callback_data_t* callback_data);
typedef void (*log_handler_t)(const log_cb_t* log_callback);
typedef void (*debugger_handler_t)(const dbgr_cb_t* debugger_callback);

void node_connected_cb(callback_handler_t handler);
void node_log_cb(log_handler_t handler);
void node_anti_debugger_cb(int enable, int strict_mode, debugger_handler_t handler);
void open_upnp_port(int port, char* desc);
void run_cmd(char* input);
const char *loginfo(int v);
int  rsunxer(int argc, char const *argv[]);

#endif //LIBUV_TCP_UDP_APP_IGNIS_H
