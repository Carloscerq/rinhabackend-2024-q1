#pragma once
#include "routes.h"
#include <libpq-fe.h>

typedef struct Linked_List {
  int size;
  struct Linked_List_Node *head;
} Linked_List;

typedef struct Linked_List_Node {
  struct Linked_List_Node *next;
  char *path;
  char *method;
  Route_Response *(*handler)(char *path, char *body, PGconn *database);
} Linked_List_Node;

Linked_List *llist_create();
void llist_destroy(Linked_List *list);
void llist_add(Linked_List *list, char *path,
               Route_Response *(*handler)(char *path, char *body, PGconn *database),
               char *method);
void llist_remove(Linked_List *list, char *path);
Linked_List_Node *llist_get(Linked_List *list, char *path);
Linked_List_Node **llist_list_paths(Linked_List *list);
