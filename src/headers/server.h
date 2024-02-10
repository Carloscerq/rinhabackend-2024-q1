#pragma once
#include "llist.h"

typedef struct Server_Configs {
  int socket_fd;
  int port;
  Linked_List *routes;
} Server_Configs;

Server_Configs *server_configs_create(int port);
void server_configs_destroy(Server_Configs *configs);
Linked_List_Node *server_get_route(Server_Configs *configs, char *route);
void server_start(Server_Configs *configs);
void server_stop(Server_Configs *configs);
