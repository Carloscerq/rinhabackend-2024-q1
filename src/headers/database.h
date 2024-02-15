#pragma once
#include <libpq-fe.h>

typedef struct Cliente {
  int credito;
  int saldo;
} Cliente;

typedef struct Transacao {
  char operacao;
  int quantidade;
  char *descricao;
  char *cliente_id;
  char *timestamp;
} Transacao;

PGconn *connect_to_db();
PGresult *exec_query(PGconn *database, char *query);
Cliente *find_cliente_by_id(PGconn *database, char *id);
int update_cliente_saldo(PGconn *database, char *id, int saldo);
int create_transacao(PGconn *database, char *cliente_id, char operacao,
                     int quantidade, char *descricao);
int find_transacao_by_cliente_id(PGconn *database, char *cliente_id, int limit, Transacao *transacoes);
