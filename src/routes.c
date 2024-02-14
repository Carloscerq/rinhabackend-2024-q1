#include "headers/routes.h"
#include "headers/database.h"
#include "headers/logs.h"
#include "headers/utils.h"
#include <stdio.h>
#include <stdlib.h>
#define MAX_RESP_SIZE 1000

Route_Response *route_transacoes(char *path, char *body, PGconn *database) {
  log_debug("Getting id from path...");
  char *id = get_id_from_path(path);

  if (!id) {
    log_debug("Failed to get id from path");
    return create_route_response(400, "Bad Request",
                                 "Failed to get id from path");
  }

  log_debug("Parsing from json...");
  Transacoes_Body *data = parse_transacoes_json(body);
  if (!data) {
    return create_route_response(400, "Bad Request", "Failed to parse json");
  }

  log_debug("Executing query...");

  Cliente *user = find_cliente_by_id(database, id);
  if (!user) {
    log_debug("User not found");
    return create_route_response(404, "Not Found", "User not found");
  }

  if (data->type != 'c' && data->type != 'd') {
    log_debug("Invalid transaction type");
    return create_route_response(400, "Bad Request",
                                 "Invalid transaction type");
  }

  int new_saldo = user->saldo - data->value;
  if (new_saldo < -user->credito && data->type == 'd') {
    log_debug("Insufficient funds");
    return create_route_response(422, "Bad Request", "Insufficient funds");
  }

  if (update_cliente_saldo(database, id, new_saldo)) {
    log_error("Failed to update saldo");
    Route_Response *response = malloc(sizeof(Route_Response));
    return create_route_response(500, "Internal Server Error",
                                 "Failed to update saldo");

    return response;
  }

  log_debug("Adding transaction to database...");
  if (create_transacao(database, id, data->type, data->value,
                       data->description)) {
    log_error("Failed to add transaction to database");
    return create_route_response(500, "Internal Server Error",
                                 "Failed to add transaction to database");
  }

  log_debug("Saldo updated successfully");

  char str_cmplt[MAX_RESP_SIZE];
  // Get the json response
  snprintf(str_cmplt, MAX_RESP_SIZE, "{\"saldo\": %d, \"limite\": %d}",
           new_saldo, user->credito);
  return create_route_response(200, "OK", str_cmplt);
}

Route_Response *route_extrato(char *path, char *body, PGconn *database) {
  log_debug("Getting id from path...");
  char *id = get_id_from_path(path);

  if (!id) {
    log_debug("Failed to get id from path");
    return create_route_response(404, "Bad Request", "Id is missing");
  }

  Cliente *user = find_cliente_by_id(database, id);
  if (!user) {
    log_debug("User not found");
    return create_route_response(404, "Not Found", "User not found");
  }

  return create_route_response(200, "OK", "OK");
}
