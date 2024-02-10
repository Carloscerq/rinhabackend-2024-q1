#include "headers/logs.h"
#include "headers/llist.h"
#include "headers/server.h"
#include <stdlib.h>

void func_example_1() {
  log_info("This is a void function");
}

void func_example_2() {
  log_info("This is a void function 2");
}

int main() {
  log_info("Starting server...");
  log_info("This is a info message");
  log_error("This is a error message");
  log_debug("This is a debug message");

  Linked_List *llist = llist_create();
  log_info("Adding functions to linked list");
  llist_add(llist, "path1", func_example_1);
  llist_add(llist, "path2", func_example_2);
  log_info("Getting function from linked list");
  Linked_List_Node *node = llist_get(llist, "path1");
  log_info("Executing function from linked list");
  ((void (*)())node->callback)();

  Server_Configs *server_configs = server_configs_create(8080);
  server_start(server_configs);
}
