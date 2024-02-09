#include "headers/logs.h"

int main() {
  log_info("Starting server...");
  log_info("This is a info message");
  log_error("This is a error message");
  log_debug("This is a debug message");
}
