#ifndef LIST_H
#define LIST_H

typedef void* dataElement;

typedef void (*freeListDataElement)(dataElement);
typedef dataElement (*copyListDataElement)(dataElement);

struct list_t {
    dataElement data;
    freeListDataElement freeDataElement;
    copyListDataElement copyDataElement;
    struct list_t *next;
};

typedef struct list_t *List;

typedef enum {
    LIST_SUCCESS, LIST_MEMORY_ERROR, LIST_NULL_ARGUMENT
} ListError;

// creates a new list node. returns NULL on memory allocation error.
List listCreate(freeListDataElement freeData, copyListDataElement copyData);

// sets data value of a given list. returns LIST_NULL_ARGUMENT if list is NULL.
ListError listSet(List list, dataElement data);

// returns the data element of given list (NOT a copy).
dataElement listGetData(List list);

// returns a copy of a given list. returns NULL if the list is empty or if there
// was a memory allocation error.
ListError listCopy(List src, List dest);

// adds another list after list
List listAdd(List list);

// returns the amount of nodes in the list.
int listGetSize(List list);

// ListError listDelete(List list); //?

// destroys all nodes of a list, then frees the list.
void listDestroy(List list);

#endif