#include "headers/logs.h"
#include "headers/htable.h"
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

  HashTable *ht = hashtable_create(10);
  hashtable_insert(ht, "key1", *func_example_1);
  hashtable_insert(ht, "key2", *func_example_2);
  void *func = hashtable_search(ht, "key1");
  ((void (*)())func)();

  char **keys = (char **)malloc(10 * sizeof(char *));
  keys = hashtable_get_keys(ht);

  for (int i = 0; i < 10; i++) {
    if (keys[i] != NULL) {
      log_info(keys[i]);
    }
  }

  hashtable_delete_entry(ht, "key1");

  char **keys2 = (char **)malloc(10 * sizeof(char *));
  keys2 = hashtable_get_keys(ht);
  for (int i = 0; i < 10; i++) {
    if (keys2[i] != NULL) {
      log_info(keys2[i]);
    }
  }

  void *func2 = hashtable_search(ht, "key1");
  if (func2 == NULL) {
    log_info("Function not found");
  }

}
