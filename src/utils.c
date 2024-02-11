#include "headers/utils.h"
#include "headers/logs.h"
#include "headers/routes.h"
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define VALUE_FIELD "\"valor\":"
#define VALUE_FIELD_SIZE 8
#define TYPE_FIELD "\"tipo\":"
#define TYPE_FIELD_SIZE 7
#define DESCRIPTION_FIELD "\"descricao\":"
#define DESCRIPTION_FIELD_SIZE 13

char *get_id_from_path(char *path) {
  regex_t regex;
  int reti = regcomp(&regex, "[0-9]+", REG_EXTENDED);
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
  Transacoes_Body *transacoes = malloc(sizeof(Transacoes_Body));
  char *value_str = strstr(json, VALUE_FIELD);
  if (value_str == NULL) {
    log_debug("Value not found...");
    return NULL;
  }

  transacoes->value = atof(value_str + VALUE_FIELD_SIZE);

  char *type_str = strstr(json, TYPE_FIELD);
  if (type_str == NULL) {
    log_debug("Type not found...");
    return NULL;
  }

  char *type_value = strstr(type_str + TYPE_FIELD_SIZE, "\"") + 1;
  if (type_value == NULL) {
    log_debug("Type value not found...");
    return NULL;
  }
  transacoes->type = *type_value;

  char *description_str = strstr(json, DESCRIPTION_FIELD);
  if (description_str == NULL) {
    log_debug("Description not found...");
    return NULL;
  }

  char *description_value =
      strstr(description_str + DESCRIPTION_FIELD_SIZE, "\"") + 1;
  if (description_value == NULL) {
    log_debug("Description value not found...");
    return NULL;
  }

  char *description_end = strstr(description_value, "\"");
  if (description_end == NULL) {
    log_debug("Description end not found...");
    return NULL;
  }

  int description_size = description_end - description_value;
  char *description = malloc(description_size + 1);
  strncpy(description, description_value, description_size);
  description[description_size] = '\0';
  transacoes->description = description;

  return transacoes;
}

char *get_environment_variable(char *variable_name) {
  char *resp = getenv(variable_name);

  if (!resp) {
    log_error("Error getting env");
    log_error(variable_name);
    exit(EXIT_FAILURE);
  }
}
