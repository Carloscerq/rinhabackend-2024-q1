#pragma once
#include "routes.h"

char *get_id_from_path(char *path);
Transacoes_Body *parse_transacoes_json(char *json);
