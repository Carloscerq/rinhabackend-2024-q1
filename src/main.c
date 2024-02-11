#include "headers/llist.h"
#include "headers/logs.h"
#include "headers/server.h"
#include "headers/routes.h"
#include <netinet/in.h>
#include <pthread.h>
#include <regex.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

Route_Response *func_example_1(char *buffer) {
  log_info(buffer);
  Route_Response *response = malloc(sizeof(Route_Response));
  response->body = "func 1";
  response->status_code = "200";
  response->status_message = "OK";
  response->body_length = 30;

  return response;
}

Route_Response *func_example_2(char *buffer) {
  log_info(buffer);
  Route_Response *response = malloc(sizeof(Route_Response));
  response->body = "func 2";
  response->status_code = "200";
  response->status_message = "OK";
  response->body_length = 30;

  return response;
}

int main() {
  log_info("Configuring server...");
  Server_Configs *server_configs = server_configs_create(8080);

  log_info("Adding functions to server...");
  server_add_route(server_configs, "/clientes/[0-9]+/transacoes", func_example_1, METHOD_GET);
  server_add_route(server_configs, "/clientes/[0-9]+/extrato", func_example_2, METHOD_GET);

  log_info("Starting server...");
  server_start(server_configs);
}
