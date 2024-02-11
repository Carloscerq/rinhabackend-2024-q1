#include "headers/database.h"
#include "headers/logs.h"
#include "headers/utils.h"
#include <stdlib.h>
#define DB_CONNECT_BUFFER_SIZE 256

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

