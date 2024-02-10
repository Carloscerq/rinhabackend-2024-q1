#pragma once
#include "llist.h"
#define METHOD_GET "GET"

typedef struct Server_Configs {
  int socket_fd;
  int port;
  Linked_List *routes;
} Server_Configs;

typedef struct Server_Handle_Args {
  Linked_List *routes;
  int *client_fd;
} Server_Handle_Args;

typedef struct Server_Response {
  char *response;
  int response_length;
  char *status_message;
  char *status_code;
} Server_Response;

Server_Configs *server_configs_create(int port);
void server_configs_destroy(Server_Configs *configs);
Linked_List_Node *server_get_route(Server_Configs *configs, char *route);
void server_start(Server_Configs *configs);
void server_stop(Server_Configs *configs);
void server_add_route(Server_Configs *configs, char *route, void *callback,
                      char *method);
void *server_handle_request(void *args);
