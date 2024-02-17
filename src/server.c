#include "headers/server.h"
#include "headers/llist.h"
#include "headers/logs.h"
#include "headers/routes.h"
#include "headers/utils.h"
#include <netinet/in.h>
#include <pthread.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define BUFFER_SIZE 1024
#define MAX_CONNECTIONS 10
#define PORT 8080
#define METHOD_SIZE 10
#define PATH_SIZE 256
#define PROTOCOL_SIZE 10

Server_Configs *server_configs_create(int port, PGconn *db_conn) {
  Server_Configs *server_configs = malloc(sizeof(Server_Configs));
  server_configs->port = port;
  server_configs->routes = llist_create();
  server_configs->database = db_conn;

  return server_configs;
}

void server_configs_destroy(Server_Configs *server_configs) {
  free(server_configs);
}

void server_stop(Server_Configs *server_configs) {
  close(server_configs->socket_fd);
}

void server_add_route(Server_Configs *configs, char *route,
                      Route_Response *(*handler)(char *path, char *body,
                                                 PGconn *database),
                      char *method) {
  llist_add(configs->routes, route, handler, method);
}

void *server_handle_request(void *args) {
  Server_Handle_Args *server_args = (Server_Handle_Args *)args;
  int socket_fd = *server_args->client_fd;
  log_info("Handling request");

  char buffer[BUFFER_SIZE], method[METHOD_SIZE], path[PATH_SIZE],
      protocol[PROTOCOL_SIZE];
  char *body_start;
  int valread = read(*server_args->client_fd, buffer, BUFFER_SIZE);

  Route_Response *response = NULL;

  sscanf(buffer, "%s %s %s", method, path, protocol);
  body_start = strstr(buffer, "\r\n\r\n") + 4;
  Linked_List_Node *current = server_args->routes->head;
  regex_t path_regex, method_regex;
  while (current != NULL) {

    int path_reti = regcomp(&path_regex, current->path, REG_EXTENDED);
    int method_reti = regcomp(&method_regex, current->method, REG_EXTENDED);
    if (path_reti || method_reti) {
      log_error("Could not compile regex");
      exit(EXIT_FAILURE);
    }

    path_reti = regexec(&path_regex, path, 0, NULL, 0);
    method_reti = regexec(&method_regex, method, 0, NULL, 0);
    if (!path_reti && !method_reti) {
      response = current->handler(path, body_start, server_args->database);
      break;
    }

    current = current->next;
  }

  regfree(&path_regex);
  regfree(&method_regex);

  if (response == NULL) {
    response = create_route_response(404, "Not Found", "Not Found", "text/plain");
  }

  char resp[BUFFER_SIZE * 2];
  snprintf(resp, BUFFER_SIZE * 2,
           "HTTP/1.1 %s %s\r\ncontent-type: %s\r\ncontent-length: "
           "%d\r\n\r\n%s",
           response->status_code, response->status_message,
           response->content_type, response->body_length, response->body);
  send(*server_args->client_fd, resp, BUFFER_SIZE * 2, 0);
  free(response);
  free(server_args);
  close(socket_fd);

  // This will prevent the buffer from being used after the function returns
  buffer[0] = '\0';

  return NULL;
}

void server_start(Server_Configs *server_configs) {
  int server_fd;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);

  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Forcefully attaching socket to the port 8080
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Forcefully attaching socket to the port 8080
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  int *new_socket = malloc(sizeof(int));
  while (1) {
    if ((*new_socket = accept(server_fd, (struct sockaddr *)&address,
                              (socklen_t *)&addrlen)) < 0) {
      perror("accept");
      exit(EXIT_FAILURE);
    }

    pthread_t thread;
    Server_Handle_Args *args = malloc(sizeof(Server_Handle_Args));
    args->client_fd = new_socket;
    args->routes = server_configs->routes;
    args->database = server_configs->database;
    pthread_create(&thread, NULL, server_handle_request, (void *)args);
    pthread_detach(thread);
  }
}
