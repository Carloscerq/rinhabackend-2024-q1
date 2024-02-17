#include "headers/routes.h"
#include "headers/database.h"
#include "headers/logs.h"
#include "headers/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_RESP_SIZE 5000

Route_Response *route_transacoes(char *path, char *body, PGconn *database) {
  log_debug("Getting id from path...");
  char *id = get_id_from_path(path);

  if (!id) {
    log_debug("Failed to get id from path");
    return create_route_response(400, "Bad Request",
                                 "Failed to get id from path", "text/plain");
  }

  log_debug("Parsing from json...");
  Transacoes_Body *data = parse_transacoes_json(body);
  if (!data) {
    return create_route_response(400, "Bad Request", "Failed to parse json",
                                 "text/plain");
  }

  log_debug("Executing query...");

  Cliente *user = find_cliente_by_id(database, id);
  if (!user) {
    log_debug("User not found");
    return create_route_response(404, "Not Found", "User not found",
                                 "text/plain");
  }

  if (data->type != 'c' && data->type != 'd') {
    log_debug("Invalid transaction type");
    return create_route_response(400, "Bad Request", "Invalid transaction type",
                                 "text/plain");
  }

  int new_saldo = user->saldo - data->value;
  if (new_saldo < -user->credito && data->type == 'd') {
    log_debug("Insufficient funds");
    return create_route_response(422, "Bad Request", "Insufficient funds",
                                 "text/plain");
  }

  if (update_cliente_saldo(database, id, new_saldo)) {
    log_error("Failed to update saldo");
    Route_Response *response = malloc(sizeof(Route_Response));
    return create_route_response(500, "Internal Server Error",
                                 "Failed to update saldo", "text/plain");

    return response;
  }

  log_debug("Adding transaction to database...");
  if (create_transacao(database, id, data->type, data->value,
                       data->description)) {
    log_error("Failed to add transaction to database");
    return create_route_response(500, "Internal Server Error",
                                 "Failed to add transaction to database",
                                 "text/plain");
  }

  log_debug("Saldo updated successfully");

  char str_cmplt[MAX_RESP_SIZE];
  // Get the json response
  snprintf(str_cmplt, MAX_RESP_SIZE, "{\"saldo\": %d, \"limite\": %d}",
           new_saldo, user->credito);
  return create_route_response(200, "OK", str_cmplt, "application/json");
}

Route_Response *route_extrato(char *path, char *body, PGconn *database) {
  log_debug("Getting id from path...");
  char *id = get_id_from_path(path);

  if (!id) {
    log_debug("Failed to get id from path");
    return create_route_response(404, "Bad Request", "Id is missing",
                                 "text/plain");
  }

  Cliente *user = find_cliente_by_id(database, id);
  if (!user) {
    log_debug("User not found");
    return create_route_response(404, "Not Found", "User not found",
                                 "text/plain");
  }

  log_debug("Getting transactions from cliente...");
  Transacao transactions[10];
  int amount_of_transactions =
      find_transacao_by_cliente_id(database, id, 10, transactions);

  char str_cmplt[MAX_RESP_SIZE];

  struct timespec spec;
  clock_gettime(CLOCK_REALTIME, &spec);
  struct tm timeinfo;
  gmtime_r(&spec.tv_sec, &timeinfo);
  char iso_time[30];
  strftime(iso_time, sizeof(iso_time), "%Y-%m-%dT%H:%M:%S", &timeinfo);
  sprintf(iso_time + 19, ".%06ldZ", spec.tv_nsec / 1000);

  sprintf(str_cmplt,
          "{\"saldo\": {\"total\": %d, \"data_extrato\": \"%s\", "
          "\"limite\": %d}, \"ultimas_transacoes\": [",
          user->saldo, iso_time, user->credito);


  for (int i = 0; i < amount_of_transactions; i++) {
    snprintf(str_cmplt + strlen(str_cmplt), MAX_RESP_SIZE - strlen(str_cmplt),
             "{\"valor\": %d, \"tipo\": \"%c\", \"descricao\": \"%s\", "
             "\"realizada_em\": \"%s\"},",
             transactions[i].quantidade, transactions[i].operacao,
             transactions[i].descricao, transactions[i].timestamp);
  }

  int overwrite_comma = -1;
  if (amount_of_transactions == 0) {
    overwrite_comma = 0;
  }
  // Overwrite the last comma
  snprintf(str_cmplt + strlen(str_cmplt) - overwrite_comma,
           MAX_RESP_SIZE - strlen(str_cmplt), "]}");

  return create_route_response(200, "OK", str_cmplt, "application/json");
}
