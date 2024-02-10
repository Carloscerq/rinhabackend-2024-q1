#include "headers/server.h"
#include "headers/llist.h"
#include "headers/logs.h"
#include <netinet/in.h>
#include <pthread.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define BUFFER_SIZE 1024
#define MAX_CONNECTIONS 3
#define PORT 8080
#define RESPONSE                                                               \
  "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: "            \
  "12\r\n\r\nHello, World"
Server_Configs *server_configs_create(int port) {
  Server_Configs *server_configs = malloc(sizeof(Server_Configs));
  server_configs->port = port;
  server_configs->routes = llist_create();

  return server_configs;
}

void server_configs_destroy(Server_Configs *server_configs) {
  free(server_configs);
}

void server_stop(Server_Configs *server_configs) {
  close(server_configs->socket_fd);
}

void server_add_route(Server_Configs *configs, char *route, void *callback,
                      char *method) {
  llist_add(configs->routes, route, callback, method);
}

void *server_handle_request(void *args) {
  Server_Handle_Args *server_args = (Server_Handle_Args *)args;
  int socket_fd = *server_args->client_fd;
  log_info("Handling request");
  send(socket_fd, RESPONSE, 100, 0);
  return NULL;

  char buffer[BUFFER_SIZE] = {0};
  int valread = read(*server_args->client_fd, buffer, BUFFER_SIZE);

  Linked_List_Node *node = server_args->routes->head;
  while (node != NULL) {
    regex_t path_regex, method_regex;
    if (regcomp(&path_regex, node->path, 0) != 0 ||
        regcomp(&method_regex, node->method, 0) != 0) {
      log_error("Error compiling regex");
      exit(EXIT_FAILURE);
    }

    int path_match = regexec(&path_regex, buffer, 0, NULL, 0);
    int method_match = regexec(&method_regex, buffer, 0, NULL, 0);

    if (path_match != 0 || method_match != 0) {
      log_debug("Route does not match request, trying next route");
      node = node->next;
      continue;
    }

    if (path_match == 0 && method_match == 0) {
      void (*callback)(int *, char *) = node->callback;
      callback(server_args->client_fd, buffer);
      free(server_args);
      return NULL;
    }

    if (path_match == 0) {
      log_error("Method not allowed");
      return NULL;
    }
  }

  log_debug("No route found for request");
  send(*server_args->client_fd, "HTTP/1.1 404 Not Found\n", 22, 0);

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
    pthread_create(&thread, NULL, server_handle_request, (void *)args);
    pthread_detach(thread);
  }
}

