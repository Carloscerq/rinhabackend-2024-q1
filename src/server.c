#include "headers/server.h"
#include "headers/logs.h"
#include "stdlib.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define BUFFER_SIZE 1024
#define MAX_CONNECTIONS 3

Server_Configs *server_configs_create(int port) {
  Server_Configs *server_configs = malloc(sizeof(Server_Configs));
  server_configs->port = port;

  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_fd < 0) {
    log_error("Error opening socket");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    log_error("Error binding socket");
    exit(EXIT_FAILURE);
  }

  if (listen(socket_fd, MAX_CONNECTIONS) < 0) {
    log_error("Error listening on socket");
    exit(EXIT_FAILURE);
  }

  server_configs->socket_fd = socket_fd;
  return server_configs;
}

void server_configs_destroy(Server_Configs *server_configs) {
  free(server_configs);
}

void server_stop(Server_Configs *server_configs) {
  close(server_configs->socket_fd);
}

void server_start(Server_Configs *server_configs) {
  while (1) {
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int *client_fd = malloc(sizeof(int));

    if ((*client_fd = accept(server_configs->socket_fd,
                             (struct sockaddr *)&client_address,
                             &client_address_len)) < 0) {
      log_error("Error accepting connection");
      exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE] = {0};
    int valread = read(*client_fd, buffer, BUFFER_SIZE);
    log_info(buffer);
  }
}
