#include "headers/logs.h"
#include "headers/llist.h"
#include "headers/server.h"
#include <stdlib.h>

void func_example_1() {
  log_info("This is a void function");
}

void func_example_2() {
  log_info("This is a void function 2");
}

int main() {
  log_info("Configuring server...");
  Server_Configs *server_configs = server_configs_create(8080);

  log_info("Adding functions to server...");
  server_add_route(server_configs, "/", func_example_1, METHOD_GET);

  log_info("Starting server...");
  server_start(server_configs);
}
