#include "headers/logs.h"
#include "headers/server.h"
#include "headers/database.h"
#include <netinet/in.h>
#include <pthread.h>
#include <regex.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  log_info("Configuring server...");

  Server_Configs *server_configs = server_configs_create(8080, connect_to_db());
  log_info("Adding functions to server...");
  server_add_route(server_configs, "/clientes/[0-9]+/transacoes$",
                   route_transacoes, METHOD_POST);
  server_add_route(server_configs, "/clientes/[0-9]+/extrato$", route_extrato,
                   METHOD_GET);

  log_info("Starting server...");
  server_start(server_configs);
}
