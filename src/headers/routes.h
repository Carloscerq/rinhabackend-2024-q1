#pragma once

typedef struct Route_Response {
  char *body;
  int body_length;
  char *status_message;
  char *status_code;
} Route_Response;

