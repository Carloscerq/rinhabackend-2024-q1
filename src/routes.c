#include "headers/routes.h"
#include "headers/logs.h"
#include "headers/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_QUERY_SIZE 1000

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

  char str_cmplt[MAX_QUERY_SIZE];
  snprintf(str_cmplt, MAX_QUERY_SIZE,
           "select credito, saldo from clientes where id = %s", id);

  log_debug("Executing query...");
  PGresult *result = PQexec(database, str_cmplt);
  ExecStatusType res_status = PQresultStatus(result);
  if (res_status != PGRES_TUPLES_OK) {
    log_error("Failed to execute query");
    PQclear(result);

    Route_Response *response = malloc(sizeof(Route_Response));
    response->body = "Failed to connect to database";
    response->status_code = "500";
    response->status_message = "Bad Request";
    response->body_length = 28;

    return response;
  }

  log_debug("Query executed successfully");
  int rows = PQntuples(result);

  if (!rows) {
    log_error("User not found");
    PQclear(result);

    Route_Response *response = malloc(sizeof(Route_Response));
    response->body = "User not found";
    response->status_code = "404";
    response->status_message = "Not Found";
    response->body_length = 14;

    return response;
  }

  int credito = atoi(PQgetvalue(result, 0, 0));
  int saldo = atoi(PQgetvalue(result, 0, 1));
  PQclear(result);

  if (data->type != 'c' && data->type != 'd') {
    printf("Type: %c\n", data->type);
    log_error("Invalid transaction type");

    Route_Response *response = malloc(sizeof(Route_Response));
    response->body = "Invalid transaction type";
    response->status_code = "400";
    response->status_message = "Bad Request";
    response->body_length = 24;

    return response;
  }
  printf("Saldo: %d\n", saldo);
  printf("Value: %d\n", data->value);
  int new_saldo = saldo - data->value;

  if (new_saldo < -credito && data->type == 'd') {
    log_error("Insufficient funds");

    Route_Response *response = malloc(sizeof(Route_Response));
    response->body = "Insufficient funds";
    response->status_code = "422";
    response->status_message = "Bad Request";
    response->body_length = 18;

    return response;
  }

  snprintf(str_cmplt, MAX_QUERY_SIZE,
           "update clientes set saldo = %d where id = %s", new_saldo, id);
  PGresult *update_result = PQexec(database, str_cmplt);
  ExecStatusType update_res_status = PQresultStatus(update_result);
  PQclear(update_result);

  if (update_res_status != PGRES_COMMAND_OK) {
    log_error("Failed to update saldo");

    Route_Response *response = malloc(sizeof(Route_Response));
    response->body = "Failed to update saldo";
    response->status_code = "500";
    response->status_message = "Internal Server Error";
    response->body_length = 23;

    return response;
  }

  log_debug("Saldo updated successfully");

  // Get the json response
  snprintf(str_cmplt, MAX_QUERY_SIZE, "{\"saldo\": %d, \"limite\": %d}",
           new_saldo, credito);

  Route_Response *response = malloc(sizeof(Route_Response));
  response->body = str_cmplt;
  response->status_code = "200";
  response->status_message = "OK";
  response->body_length = strlen(str_cmplt);

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
