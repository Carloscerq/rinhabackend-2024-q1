#include "headers/routes.h"
#include "headers/logs.h"
#include "headers/utils.h"
#include <stdlib.h>

Route_Response *route_transacoes(char *path, char *body, PGconn *database) {
  log_debug("Getting id from path...");
  char *id = get_id_from_path(path);

  if (!id) {
    log_error("Failed to get id from path");
    Route_Response *response = malloc(sizeof(Route_Response));
    response->body = "Failed to get id from path";
    response->status_code = "400";
    response->status_message = "Bad Request";
    response->body_length = 25;

    return response;
  }

  log_debug("Parsing from json...");
  Transacoes_Body *data = parse_transacoes_json(body);
  if (!data) {
    Route_Response *response = malloc(sizeof(Route_Response));
    response->body = "Failed to parse json";
    response->status_code = "400";
    response->status_message = "Bad Request";
    response->body_length = 20;

    return response;
  }

  Route_Response *response = malloc(sizeof(Route_Response));
  response->body = "func 1";
  response->status_code = "200";
  response->status_message = "OK";
  response->body_length = 6;

  return response;
}

Route_Response *route_extrato(char *path, char *body, PGconn *database) {
  log_debug("Getting id from path...");
  char *id = get_id_from_path(path);

  if (!id) {
    log_error("Failed to get id from path");
    Route_Response *response = malloc(sizeof(Route_Response));
    response->body = "Failed to get id from path";
    response->status_code = "400";
    response->status_message = "Bad Request";
    response->body_length = 25;

    return response;
  }

  Route_Response *response = malloc(sizeof(Route_Response));
  response->body = "func 2";
  response->status_code = "200";
  response->status_message = "OK";
  response->body_length = 30;

  return response;
}
