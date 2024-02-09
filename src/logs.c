#include "headers/logs.h"
#include <stdio.h>
#include <time.h>
#define FORMATED_TIME_SIZE 25

void log_data(const char *type, const char *message) {
  time_t now;
  time(&now);
  char formatted_time[FORMATED_TIME_SIZE];
  strftime(formatted_time, FORMATED_TIME_SIZE, "%Y-%m-%d %H:%M:%S", localtime(&now));
  printf("%s [%s]: %s\n", formatted_time, type, message);
}

void log_error(const char *message) {
  log_data("ERROR", message);
}

void log_info(const char *message) {
  log_data("INFO", message);
}

void log_debug(const char *message) {
  log_data("DEBUG", message);
}

