#include "headers/utils.h"
#include "headers/logs.h"
#include "headers/routes.h"
#include <cjson/cJSON.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUMBER_REGEX "[0-9]+"

char *get_id_from_path(char *path) {
  regex_t regex;
  int reti = regcomp(&regex, NUMBER_REGEX, REG_EXTENDED);
  if (reti) {
    log_error("Failed to compile regex");
    return NULL;
  }

  regmatch_t matches[1];
  reti = regexec(&regex, path, 1, matches, 0);
  if (reti) {
    log_error("Failed to execute regex");
    return NULL;
  }

  // return the match
  int match_size = matches[0].rm_eo - matches[0].rm_so;
  char *id = malloc(match_size + 1);
  strncpy(id, path + matches[0].rm_so, match_size);
  id[match_size] = '\0';

  regfree(&regex);
  return id;
}

Transacoes_Body *parse_transacoes_json(char *json) {
  Transacoes_Body *resp = malloc(sizeof(Transacoes_Body));
  cJSON *root = cJSON_Parse(json);

  if (root == NULL) {
    log_error("Failed to parse JSON");
    cJSON_Delete(root);
    return NULL;
  }

  cJSON *valor = cJSON_GetObjectItemCaseSensitive(root, "valor");
  cJSON *tipo = cJSON_GetObjectItemCaseSensitive(root, "tipo");
  cJSON *descricao = cJSON_GetObjectItemCaseSensitive(root, "descricao");

  if (valor == NULL || tipo == NULL || descricao == NULL ||
      !cJSON_IsNumber(valor) || !cJSON_IsString(tipo) ||
      !cJSON_IsString(descricao)) {
    log_error("Failed to parse JSON");
    cJSON_Delete(root);
    return NULL;
  }

  resp->value = valor->valuedouble;
  resp->type = strdup(tipo->valuestring)[0];
  resp->description = strdup(descricao->valuestring);
  cJSON_Delete(root);

  return resp;
}

char *get_environment_variable(char *variable_name) {
  char *resp = getenv(variable_name);

  if (!resp) {
    log_error("Error getting env");
    log_error(variable_name);
    exit(EXIT_FAILURE);
  }
}
