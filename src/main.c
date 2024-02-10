#include "headers/llist.h"
#include "headers/logs.h"
#include "headers/server.h"
#include <netinet/in.h>
#include <pthread.h>
#include <regex.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

char *func_example_1(int *client_fd, char *buffer) {
  log_info(buffer);
  char *response = "HTTP/1.1 200 OK";

  log_info("Here is the client_fd:");
  send(*client_fd, response, 100, 0);
  return response;
}

void func_example_2() { log_info("This is a void function 2"); }

int main() {
  log_info("Configuring server...");
  Server_Configs *server_configs = server_configs_create(8080);

  log_info("Adding functions to server...");
  server_add_route(server_configs, "/", func_example_1, METHOD_GET);

  log_info("Starting server...");
  server_start(server_configs);
}
