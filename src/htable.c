#include "headers/htable.h"
#include "headers/logs.h"
#include <stdlib.h>

int hash(char *key, int size) {
  int hash = 0;
  for (int i = 0; key[i] != '\0'; i++) {
    hash = (hash * key[i]) % size;
  }
  log_debug("Hashed new entry");
  char hash_str = (char)hash + '0';
  log_debug(&hash_str);
  return hash;
}

HashTable *hashtable_create(int size) {
  log_info("Creating hash table...");
  HashTable *htable = (HashTable *)malloc(sizeof(HashTable));
  htable->size = size;

  for (int i = 0; i < size; i++) {
    htable->table[i] = NULL;
  }

  return htable;
}

void hashtable_destroy(HashTable *htable) {
  log_info("Destroying hash table...");
  for (int i = 0; i < htable->size; i++) {
    if (htable->table[i] != NULL) {
      free(htable->table[i]);
    }
  }

  free(htable);
}

void hashtable_insert(HashTable *table, char *key, void *value) {
  log_info("New entry on hash table...");
  int index = hash(key, table->size);
  HashTable_Node *node = (HashTable_Node *)malloc(sizeof(HashTable_Node));
  node->key = key;
  node->value = value;

  if (table->count == table->size) {
    log_error("Table is full");
    return;
  }

  if (table->table[index] == NULL) {
    table->table[index] = node;
  } else {
    HashTable_Node *current = table->table[index];
    do {
      index = (index + 1) % table->size;
      if (table->table[index] == NULL) {
        table->table[index] = node;
        break;
      }
    } while (current != NULL);
  }

  table->count++;
  log_debug("Entry added to hash table");
}

int hashtable_find_node(HashTable *table, char *key) {
  log_info("Searching for entry on hash table...");
  int index = hash(key, table->size);
  if (table->table[index] != NULL) {
    if (table->table[index]->key == key) {
      log_debug("Entry found on hash table");
      return index;
    } else {
      HashTable_Node *current = table->table[index];
      do {
        index = (index + 1) % table->size;
        if (table->table[index] != NULL) {
          if (table->table[index]->key == key) {
            log_debug("Entry found on hash table");
            return index;
          }
        }
      } while (current != NULL);
    }
  }

  return -1;
}

void *hashtable_search(HashTable *table, char *key) {
  log_info("Searching for value...");
  int index = hashtable_find_node(table, key);
  if (index >= 0) {
    return table->table[index]->value;
  }

  return NULL;
}

char **hashtable_get_keys(HashTable *table) {
  log_info("Getting keys from hash table...");
  char **keys = (char **)malloc(table->count * sizeof(char *));
  int j = 0;
  for (int i = 0; i < table->size; i++) {
    if (table->table[i] != NULL) {
      keys[j] = table->table[i]->key;
      j++;
    }
  }

  return keys;
}

void **hashtable_get_values(HashTable *table) {
  log_info("Getting values from hash table...");
  void **values = (void **)malloc(table->count * sizeof(void *));
  int j = 0;
  for (int i = 0; i < table->size; i++) {
    if (table->table[i] != NULL) {
      values[j] = table->table[i]->value;
      j++;
    }
  }

  return values;
}

void hashtable_delete_entry(HashTable *table, char *key) {
  log_info("Deleting entry from hash table...");
  int index = hashtable_find_node(table, key);
  if (index >= 0) {
    free(table->table[index]);
    table->table[index] = NULL;
    table->count--;
  }
}
