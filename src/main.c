#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "xbc.h"
#include "cJSON.h"
#include <unistd.h>
#include "lib/bt_tasks.h"

// 函数：从路径读取文件内容并返回字符串
char* read_file_to_string(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "无法打开文件: %s\n", path);
        return NULL;
    }

    // 移动指针到文件末尾以获取文件大小
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc(length + 1);
    if (buffer) {
        fread(buffer, 1, length, file);
        buffer[length] = '\0'; // 确保字符串以空字符结尾
    }

    fclose(file);
    return buffer;
}

void foreach_cb(const char *key, size_t key_len, const char *value, size_t value_len)
{
    char *db_key = malloc(key_len + 1);
    memcpy(db_key, key, key_len);
    db_key[key_len] = 0;
    char *db_value = malloc(value_len + 1);
    memcpy(db_value, value, value_len);
    db_value[value_len] = 0;
    printf("key: %s, value: %s\n", db_key, db_value);

    char cmd[256];
    snprintf(cmd, sizeof(cmd), "cmd bt status %s", db_key);
    run_cmd(cmd);

    free(db_key);
    free(db_value);
}

// 定时器线程函数
void* timer_thread(void* arg) {
    int interval = *(int*)arg;
    while (1) {
        sleep(interval);
        // 这里先检查是否有bt package task
        package_tasks_for_each(foreach_cb);
        run_cmd("cmd dropbox pop");
    }
    return NULL;
}

void show_self_info()
{
    run_cmd("cmd self");
}

static void rm_bt_tasks() {
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "rm -rf -- 'bt_tasks'");
    system(cmd);
}

void cleanup(void) {
    rm_bt_tasks();
    run_cmd("cmd exit");
}

void log_handler(const log_cb_t* log_data) {
    cJSON *root = cJSON_Parse(log_data->log);
    if (cJSON_IsObject(root))
    {

        int type = (int)cJSON_GetNumberValue(cJSON_GetObjectItem(root, "type"));
        if (type != 36)
        {
            printf("TYPE: %s, NUM: %d\n", loginfo(type), type);
        }
        // printf("TYPE: %s, NUM: %d\n", loginfo(type), type);
        cJSON *msg = cJSON_GetObjectItem(root, "msg");
        if (cJSON_IsString(msg) && type != 19)
        {
            if (strcasecmp(msg->valuestring, "dropbox pop msg: (null)") == 0)
            {
                return;
            }
            printf("MSG: %s\n", msg->valuestring);
        }
        cJSON *api = cJSON_GetObjectItem(root, "api");
        if (NULL!=api && cJSON_IsString(api))
        {
            if (type == 19) // dropbox pop
            {
                cJSON* tmp = cJSON_Parse(cJSON_GetStringValue(api));
                char *selfid = cJSON_GetStringValue(cJSON_GetObjectItem(tmp, "id"));
                char *self_info = cJSON_GetStringValue(cJSON_GetObjectItem(tmp, "info"));
                cJSON *j = cJSON_Parse(self_info);

                printf("API:\n id: %s\n info: %s\n", selfid, cJSON_Print(j));

                cJSON_Delete(tmp);
            }

            if (type == 18)
            {
                cJSON* tmp = cJSON_Parse(cJSON_GetStringValue(api));
                int state = (int)cJSON_GetNumberValue(cJSON_GetObjectItem(tmp, "state"));
                int progress = (int)cJSON_GetNumberValue(cJSON_GetObjectItem(tmp, "progress"));
                char *infohash = cJSON_GetStringValue(cJSON_GetObjectItem(tmp, "infohash"));

                if (state == 2 && progress == 100)
                {
                    char *pack = package_tasks_get(infohash);
                    char pack_path[256];
                    snprintf(pack_path, sizeof(pack_path), "vshome/dat/self/%s", pack);
                    char *pack_path_ = strdup(pack_path);
                    char *json_raw = read_file_to_string(pack_path_);
                    if (json_raw == NULL) return;
                    cJSON *json_obj = cJSON_Parse(json_raw);
                    char *summary = cJSON_GetStringValue(cJSON_GetObjectItem(json_obj,"summary"));
                    char *provider = cJSON_GetStringValue(cJSON_GetObjectItem(json_obj,"provider"));

                    char cmd[256];
                    snprintf(cmd, sizeof(cmd), "cmd idx embed \'%s\' \'%s\'", provider, summary);
                    run_cmd(cmd);
                    package_tasks_delete(infohash);
                    cJSON_Delete(json_obj);
                    free(json_raw);
                }
                cJSON_Delete(tmp);
            }
            if (type == 36) // dropbox msg
            {
                cJSON* obj = cJSON_Parse(cJSON_GetStringValue(api));
                char *record = cJSON_GetStringValue(cJSON_GetObjectItem(obj, "record"));
                cJSON* record_obj = cJSON_Parse(record);
                char *search_from_id = cJSON_GetStringValue(cJSON_GetObjectItem(record_obj, "nid"));
                cJSON* msg_obj = cJSON_GetObjectItem(record_obj, "msg");
                if (msg_obj!=NULL && cJSON_IsString(msg_obj))
                {
                    cJSON* idx_obj = cJSON_Parse(cJSON_GetStringValue(msg_obj));
                    cJSON* vs1984x_obj = cJSON_GetObjectItem(idx_obj,"vs1984x");
                    if (vs1984x_obj != NULL && cJSON_IsString(vs1984x_obj) && strcasecmp(cJSON_GetStringValue(vs1984x_obj), "002") == 0)
                    {
                        char *search = cJSON_GetStringValue(cJSON_GetObjectItem(idx_obj, "search"));
                        char cmd[4096];
                        snprintf(cmd, sizeof(cmd), "cmd idx search \'%s\' \'%s\'", search_from_id, search);
                        char *cmd_ = strdup(cmd);
                        run_cmd(cmd_);
                        free(cmd_);
                    }
                    cJSON_Delete(idx_obj);
                }
                cJSON_Delete(record_obj);
                cJSON_Delete(obj);
            }
            if (type == 50)
            {
                cJSON* obj = cJSON_Parse(cJSON_GetStringValue(api));
                char *raw_data = cJSON_GetStringValue(cJSON_GetObjectItem(obj, "raw_data"));
                cJSON* raw_data_obj = cJSON_Parse(raw_data);
                cJSON* content_obj = cJSON_GetObjectItem(raw_data_obj, "content");
                if (content_obj!=NULL && cJSON_IsString(content_obj))
                {
                    cJSON* bt_obj = cJSON_Parse(cJSON_GetStringValue(content_obj));
                    cJSON* vs1984x_obj = cJSON_GetObjectItem(bt_obj,"vs1984x");
                    if (vs1984x_obj != NULL && cJSON_IsString(vs1984x_obj) && strcasecmp(cJSON_GetStringValue(vs1984x_obj), "001") == 0)
                    {
                        char *infohash = cJSON_GetStringValue(cJSON_GetObjectItem(bt_obj, "infohash"));
                        char magnet_url[128];
                        snprintf(magnet_url, sizeof(magnet_url), "magnet:?xt=urn:btih:%s", infohash);
                        char *bt_pack = cJSON_GetStringValue(cJSON_GetObjectItem(bt_obj, "pack"));
                        char *bt_magnet = strdup(magnet_url);
                        char cmd[256];
                        snprintf(cmd, sizeof(cmd), "cmd bt magnet %s", bt_magnet);
                        run_cmd(cmd);
                        package_tasks_put(infohash, bt_pack);
                        free(bt_magnet);
                    }
                    cJSON_Delete(bt_obj);
                }
                cJSON_Delete(raw_data_obj);
                cJSON_Delete(obj);
            }

            if (type == 11) //IDX_SRV_SEARCHING_ANSWER
            {
                cJSON* obj = cJSON_Parse(cJSON_GetStringValue(api));
                cJSON_AddStringToObject(obj,"vs1984x","005");
                char *callback_id = strdup(cJSON_GetStringValue(cJSON_GetObjectItem(obj, "callback_id")));
                char *obj_resp = cJSON_PrintUnformatted(obj);
                printf("obj_resp : \n%s\n", obj_resp);
                char cmd[4096];
                snprintf(cmd, sizeof(cmd), "cmd dropbox %s '%s'", callback_id, obj_resp);
                run_cmd(cmd);
                cJSON_Delete(obj);
                free(callback_id);
            }
            else
            {
                // printf("api : \n%s\n", api->valuestring);
            }
            printf("api : \n%s\n", api->valuestring);
        }
        cJSON_Delete(root);

        //============================ func ================================

        if (type == 25)
        {
            show_self_info();
        }

    }else {
        printf("SDK: %s\n", log_data->log);
    }
}

int rsx_init(int argc, char const *argv[]){
    node_log_cb(log_handler);
    return rsunxer(argc, argv);
}

int main(int argc, char const *argv[]) {
    atexit(cleanup);
    package_init();
    pthread_t tid;
    int seconds = 5;
    // 创建子线程跑定时任务
    pthread_create(&tid, NULL, timer_thread, &seconds);
    pthread_detach(tid); // 分离线程，让它自己结束
    rsx_init(argc, argv);
    return 0;
}