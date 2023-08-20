#ifndef LIST_H_
#define LIST_H_

typedef struct list List;
struct list {
  void* value;
  List* next;
};

/* construction/destruction methods for list */
List* list_create(void* value);
void list_destroy(List* list);

/* insert/remove/retrieve entries at any arbitrary index of the list */
int list_insert(List* list, int index, void* value);
int list_remove(List* list, int index);
void* list_retrieve(List* list, int index);

/* push/pop operations from front of list O(1) */
int list_fpush(List** list, void* value);
void* list_fpop(List** list);

/* push/pop operations on back of list O(n) */
int list_push(List* list, void* value);
void* list_pop(List* list, void* value);

/* retrieve the length of the list */
int list_length(List* list);
#endif // LIST_H_