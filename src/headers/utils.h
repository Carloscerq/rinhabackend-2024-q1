#pragma once
#include "routes.h"

char *get_id_from_path(char *path);
Transacoes_Body *parse_transacoes_json(char *json);
char *get_environment_variable(char *variable_name);
Route_Response *create_route_response(int status, char *message, char *body);
