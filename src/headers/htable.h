#pragma once

typedef struct HashTable_Node {
  char *key;
  void *value;
} HashTable_Node;

typedef struct HashTable {
  int size;
  int count;
  HashTable_Node **table;
} HashTable;

int hash(char *key, int size);
HashTable *hashtable_create(int size);
void hashtable_destroy(HashTable *table);
void hashtable_insert(HashTable *table, char *key, void *value);
int hashtable_find_node(HashTable *table, char *key);
void *hashtable_search(HashTable *table, char *key);
char **hashtable_get_keys(HashTable *table);
void **hashtable_get_values(HashTable *table);
void hashtable_delete_entry(HashTable *table, char *key);
