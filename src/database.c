#include "headers/database.h"
#include "headers/logs.h"
#include "headers/utils.h"
#include <stdlib.h>
#define DB_CONNECT_BUFFER_SIZE 256
#define MAX_QUERY_SIZE 1000

PGconn *connect_to_db() {
  char *dbname = get_environment_variable("DATABASE_NAME");
  char *dbuser = get_environment_variable("DATABASE_USER");
  char *dbpass = get_environment_variable("DATABASE_PASSWORD");
  char *dbhost = get_environment_variable("DATABASE_HOST");
  char *dbport = get_environment_variable("DATABASE_PORT");
  char dbconnect[DB_CONNECT_BUFFER_SIZE];
  snprintf(dbconnect, DB_CONNECT_BUFFER_SIZE,
           "dbname=%s user=%s password=%s host=%s port=%s", dbname, dbuser,
           dbpass, dbhost, dbport);

  PGconn *conn = PQconnectdb(dbconnect);

  if (PQstatus(conn) != CONNECTION_OK) {
    log_error("Connection to database failed");
    log_error(PQerrorMessage(conn));
    PQfinish(conn);
    exit(EXIT_FAILURE);
    return NULL;
  }

  log_info("Connected to the database");
  return conn;
}

Cliente *find_cliente_by_id(PGconn *database, char *id) {
  char str_cmplt[MAX_QUERY_SIZE];
  snprintf(str_cmplt, MAX_QUERY_SIZE,
           "select credito, saldo from clientes where id = %s", id);

  PGresult *result = PQexec(database, str_cmplt);
  ExecStatusType res_status = PQresultStatus(result);
  if (res_status != PGRES_TUPLES_OK) {
    log_debug("User not found");

    PQclear(result);
    return NULL;
  }

  Cliente *resp = malloc(sizeof(Cliente));
  resp->credito = atoi(PQgetvalue(result, 0, 0));
  resp->saldo = atoi(PQgetvalue(result, 0, 1));
  return resp;
}

int update_cliente_saldo(PGconn *database, char *id, int saldo) {
  char str_cmplt[MAX_QUERY_SIZE];
  snprintf(str_cmplt, MAX_QUERY_SIZE,
           "update clientes set saldo = %i where id = %s", saldo, id);

  PGresult *update_result = PQexec(database, str_cmplt);
  ExecStatusType update_res_status = PQresultStatus(update_result);
  PQclear(update_result);
  if (update_res_status != PGRES_COMMAND_OK) {
    log_error("Failed to update saldo");
    return -1;
  }

  return 0;
}

int create_transacao(PGconn *database, char *cliente_id, char operacao,
                     int quantidade, char *descricao) {
  char str_cmplt[MAX_QUERY_SIZE];
  snprintf(
      str_cmplt, MAX_QUERY_SIZE,
      "insert into transacoes (client_id, operacao, quantidade, descricao) "
      "values (%s, '%c', %d, '%s')",
      cliente_id, operacao, quantidade, descricao);

  PGresult *insert_result = PQexec(database, str_cmplt);
  ExecStatusType insert_res_status = PQresultStatus(insert_result);
  PQclear(insert_result);
  if (insert_res_status != PGRES_COMMAND_OK) {
    return -1;
  }

  return 0;
}
