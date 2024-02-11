#include "headers/llist.h"
#include "stdlib.h"
#include <string.h>

Linked_List *llist_create() {
  Linked_List *list = (Linked_List *)malloc(sizeof(Linked_List));
  list->head = NULL;
  list->size = 0;
  return list;
}

void llist_destroy(Linked_List *list) {
  for (int i = 0; i < list->size; i++) {
    llist_remove(list, 0);
  }

  free(list);
}

void llist_add(Linked_List *list, char *path, Route_Response *(*handler)(char *buffer), char *method) {
  Linked_List_Node *node = (Linked_List_Node *)malloc(sizeof(Linked_List_Node));
  node->path = path;
  node->handler = handler;
  node->next = list->head;
  node->method = method;
  list->head = node;
}

void llist_remove(Linked_List *list, char *path) {
  Linked_List_Node *current = list->head;
  Linked_List_Node *previous = NULL;

  while (current != NULL) {
    if (current == NULL) {
      return;
    }

    if (strcmp(path, current->path) == 0) {
      if (previous != NULL) {
        previous->next = current->next;
      } else {
        list->head = current->next;
      }

      free(current);
      return;
    }

    previous = current;
    current = current->next;
  }
}

Linked_List_Node *llist_get(Linked_List *list, char *path) {
  Linked_List_Node *current = list->head;

  while (current != NULL) {
    if (strcmp(path, current->path) == 0) {
      return current;
    }

    current = current->next;
  }

  return NULL;
}

Linked_List_Node **llist_list_paths(Linked_List *list) {
  Linked_List_Node **paths =
      (Linked_List_Node **)malloc(sizeof(Linked_List_Node *) * list->size);
  Linked_List_Node *current = list->head;

  for (int i = 0; i < list->size; i++) {
    paths[i] = current;
    current = current->next;
  }

  return paths;
}
