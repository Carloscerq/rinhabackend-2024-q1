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
#define MAX_CONNECTIONS 10
#define PORT 8080

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

Server_Response *func_example_(char *buffer) {
  log_info(buffer);
  char *response = "OK ----- This is the response from the server\nHello world";

  log_info("Here is the client_fd:");
  Server_Response *server_response = malloc(sizeof(Server_Response));
  server_response->response = response;
  server_response->response_length = strlen(response);
  server_response->status_code = "200";
  server_response->status_message = "ok";
  return server_response;
}

void *server_handle_request(void *args) {
  Server_Handle_Args *server_args = (Server_Handle_Args *)args;
  int socket_fd = *server_args->client_fd;
  log_info("Handling request");

  char buffer[BUFFER_SIZE];
  int valread = read(*server_args->client_fd, buffer, BUFFER_SIZE);
  log_info("Request handled");

  Server_Response *response = func_example_(buffer);

  char resp[BUFFER_SIZE * 2];
  snprintf(resp, BUFFER_SIZE * 2,
           "http/1.1 %s %s\r\ncontent-type: text/plain\r\ncontent-length: "
           "%d\r\n\r\n%s",
           response->status_code, response->status_message,
           response->response_length, response->response);
  send(*server_args->client_fd, resp, BUFFER_SIZE * 2, 0);
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
