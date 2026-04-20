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

//=================sdk api=================
typedef enum {
    BIND_NODE_TYPE_DEFAULT             = 0,        // 无
    BIND_NODE_TYPE_PROVIDER            = 1 << 0,   // 0001
    BIND_NODE_TYPE_INDEXER             = 1 << 1,   // 0010
    BIND_NODE_TYPE_EXTRA1              = 1 << 2,   // 0100
    BIND_NODE_TYPE_EXTRA2              = 1 << 3    // 1000
} BindTypes;

void vs_show_self();
void vs_register_node();
void vs_bind_node(BindTypes node_type);
void vs_chain_msg(char *content);
void vs_transfer_amount(char *to_id, char *amount);

void vs_get_wallet(char *key);
void vs_get_self_wallet();
void vs_mpt_value(char *key);
void vs_generate_cert(char *cert_passwd);
void vs_download_file_from_url(char *url);
void vs_download_file_make_url(char *id, char *file_name_ori, char *to_id);
void vs_chat_send(char *target_id, char *msg);
void vs_dropbox_send(char *target_id, char *msg);
void vs_dropbox_peek();
void vs_dropbox_pop();

void vs_id_del_nickname(char *node_id);
void vs_id_set_nickname(char *node_id, char *node_nickname);
void vs_id_info(char *id);
void vs_ids_all();
void vs_ids_hint();
void vs_ids_neighbour();

void vs_task_get(char *mid);
void vs_task_sync();
void vs_task_clean();
void vs_task_list();

void vs_file_encrypt(char *key, char *file);
void vs_file_decrypt(char *key, char *file, char *out);
void vs_bt_get_status(char *target);
void vs_bt_add_magnet(char *target);
void vs_bt_add_seed(char *target);
void vs_show_block_record_by_id(char *author_id);
void vs_show_block_record_self();

void vs_secret_encrypt(char *node_id, char *in);
void vs_secret_decrypt(char *node_id, char *in);

void vs_node_connect(char *node_info, char *node_pubkey);
void vs_node_connect_by_image(char *node_info_image_path);
void vs_disconnect_node(char *id);
void vs_add_file(char *src_path);
void vs_ls_files();
void vs_ls_rawdata(char *block_uid, char *block_msg_hash);
void vs_req_block(char *block_num);
void vs_req_block_rawdata(char *msg_id, char *node_id);
void vs_generate_connect_image(char *image_path);

void vs_rag_query(char *callback_id, char *query_string);
void vs_rag_ingest(char *file_path);
void vs_idx_search(char *answer_id, char *desc);
void vs_idx_embed(char *provider_id, char *desc);
void vs_show_blacklist();
void vs_version();
void vs_work();
void vs_core_id();
void vs_exit();

#endif //LIBUV_TCP_UDP_APP_IGNIS_H
