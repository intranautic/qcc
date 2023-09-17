#include <stddef.h>
#include <stdlib.h>

#include "qcc/list.h"


List* list_create(void* value) {
  List* list = calloc(1, sizeof(List));
  list->value = value;
  list->next = NULL;
  return list;
}

void list_destroy(List* list) {
  List* tmp;
  if (list) {
    while (list) {
      tmp = list;
      list = list->next;
      free(tmp);
    }
  }
  return;
}

int list_insert(List* list, int index, void* value) {
  List* tmp, *new;
  if (list) {
    int i;
    for (tmp = list, i = 0; i < index; ++i) {
      if (tmp->next)
        tmp = tmp->next;
    }
    new = list_create(value);
    new->next = tmp->next;
    tmp->next = new;
    return i;
  }
  return -1;
}

int list_remove(List* list, int index) {
  List* tmp;
  if (list) {
    int i;
    for (tmp = list, i = 0; i < index; ++i) {
      if (tmp->next)
        tmp = tmp->next;
    }
    tmp->next = tmp->next->next;
    return i;
  }
  return -1;
}

void* list_retrieve(List* list, int index) {
  List* tmp;
  if (list) {
    int i;
    for (tmp = list, i = 0; i < index; ++i) {
      if (tmp->next)
        tmp = tmp->next;
    }
    return tmp->value;
  }
  // could not retrieve entry
  return (void *)-1;
}

int list_fpush(List** list, void* value) {
  if (!list)
    return -1;

  if (*list) {
    List* tmp = list_create(value);
    tmp->next = *list;
    *list = tmp;
  } else
    *list = list_create(value);

  return 0;
}

void* list_fpop(List** list) {
  List* tmp;
  void* value;
  if (list && *list) {
    tmp = *list;
    *list = (*list)->next;
    value = tmp->value;
    free(tmp);
    return value;
  }
  return (void *)-1; // failed to pop
}

int list_push(List* list, void* value) {
  List* tmp;
  if (list) {
    tmp = list;
    while (tmp->next)
      tmp = tmp->next;
    tmp->next = list_create(value);
    return 0;
  }
  return -1;
}

void* list_pop(List* list, void* value) {
  List* tmp;
  void* save;
  if (list) {
    while (list->next->next)
      list = list->next;
    tmp = list->next;
    save = tmp->value;
    list->next = NULL;
    free(tmp);
    return save;
  }
  return (void *)-1;
}

void* list_top(List* list) {
  return (list)
    ? list->value
    : NULL;
}

int list_length(List* list) {
  List* tmp;
  if (list) {
    tmp = list;
    int len;
    for (len=1; tmp->next; ++len)
      tmp = tmp->next;
    return len;
  }
  return 0;
}

