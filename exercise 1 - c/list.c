#include <stdlib.h>
#include "list.h"

/** TODO:
 * listGetSize: always returns at least 1 which is a problem. is it more correct to count data elements?
 * other wise we could implement a dummy node. either way it will change implementation of the whole project.
 * remove - needs to be implemented in tournament.c since we can't compare data elements here.
 * or can we? no it is best from outside.
 * contains - same problem.
*/

List listCreate(freeListDataElement freeData, copyListDataElement copyData)
{
    List list = malloc(sizeof(*list));
    if(list == NULL)
    {
        return NULL;
    }
    list->freeDataElement = freeData;
    list->copyDataElement = copyData;
    list->data = NULL;
    list->next = NULL;
    return list;
}

ListError listSet(List list, dataElement data)
{
    if(list == NULL)
    {
        return LIST_NULL_ARGUMENT;
    }
    list->freeDataElement(list->data);
    list->data = data;
    return LIST_SUCCESS;
}

dataElement listGetData(List list)
{
    if(list == NULL)
    {
        return NULL;
    }
    return list->data;
}

int listGetSize(List list)
{
    List iterator = list;
    int counter = 0;
    while(iterator)
    {
        counter++;
        iterator = iterator->next;
    }
    return counter;
}

ListError listCopy(List src, List dest)
{
    if(src == NULL || dest == NULL)
    {
        return LIST_NULL_ARGUMENT;
    }

    List iterator = src;
    List head = dest;
    while(iterator)
    {
        src->freeDataElement(dest->data);  // making sure no memory leaks
        dest->data = src->copyDataElement(iterator->data);
        if(iterator->next)
        {
            dest->next = listCreate(src->freeDataElement, src->copyDataElement);
            if(dest->next == NULL)
            {
                listDestroy(head);
                return LIST_MEMORY_ERROR;
            }
            dest = dest->next;
        }
        iterator = iterator->next;
    }
    return LIST_SUCCESS;
}

List listAdd(List list)
{
    if(list == NULL)
    {
        return NULL;
    }
    if(list->data == NULL)  // empty node
    {
        return list;
    }
    List new_list = listCreate(list->freeDataElement, list->copyDataElement);
    if(new_list == NULL)
    {
        return NULL;
    }
    new_list->data = NULL;
    new_list->next = list->next;
    list->next = new_list;
    return new_list;
}

void listDestroy(List list)
{
    if(list == NULL)
    {
        return;
    }
    while(list)
    {
        List temp = list;
        list->freeDataElement(list->data);
        list = list->next;
        free(temp);
    }
}
