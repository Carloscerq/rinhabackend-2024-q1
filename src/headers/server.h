#pragma once
#include "llist.h"
#include "routes.h"
#define METHOD_GET "GET"
#define METHOD_POST "POST"
#define METHOD_PUT "PUT"
#define METHOD_DELETE "DELETE"
#define METHOD_PATCH "PATCH"

typedef struct Server_Configs {
  int socket_fd;
  int port;
  Linked_List *routes;
} Server_Configs;

typedef struct Server_Handle_Args {
  Linked_List *routes;
  int *client_fd;
} Server_Handle_Args;

Server_Configs *server_configs_create(int port);
void server_configs_destroy(Server_Configs *configs);
Linked_List_Node *server_get_route(Server_Configs *configs, char *route);
void server_start(Server_Configs *configs);
void server_stop(Server_Configs *configs);
void server_add_route(Server_Configs *configs, char *route, Route_Response *(*handler)(char *buffer),
                      char *method);
void *server_handle_request(void *args);
