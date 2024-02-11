#include "headers/routes.h"
#include "headers/logs.h"
#include <stdlib.h>

Route_Response *route_transacoes(char *path, char *body) {
  log_debug(path);
  log_debug(body);
  Route_Response *response = malloc(sizeof(Route_Response));
  response->body = "func 1";
  response->status_code = "200";
  response->status_message = "OK";
  response->body_length = 30;

  return response;
}

Route_Response *route_extrato(char *path, char *body) {
  log_debug(path);
  log_debug(body);
  Route_Response *response = malloc(sizeof(Route_Response));
  response->body = "func 2";
  response->status_code = "200";
  response->status_message = "OK";
  response->body_length = 30;

  return response;
}
