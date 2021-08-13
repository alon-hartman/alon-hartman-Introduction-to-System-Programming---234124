#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "map.h"
#include "list.h"
#include "pair.h"

struct Map_t {
    List head;
    compareMapKeyElements compareKeyElements;
    List iterator;
};

// HELPER FUNCTIONS START

/**
* mapAdd: adds a new key:data elements to map while keeping the map ordered.
* assumes key does not exist in map.
*
* @param map - The map to add to.
* @param key - the key to add.
* @param data - the data which is accociated to the key.
* 		comparison function.
* @return
* 	MAP_OUT_OF_MEMORY - if fails to allocate memory.
*   MAP_SUCCESS - the paired elements had been inserted successfully.
*/
static MapResult mapAdd(Map map, MapKeyElement key, MapDataElement data)
{
    if(mapGetSize(map) == 0) // adding first element
    {
        pairSet((Pair)(listGetData(map->head)), data, key);
        return MAP_SUCCESS;
    }

    // there is a pair to add
    Pair pair = pairCreate(((Pair)(listGetData(map->head)))->copyDataFunc, 
                            ((Pair)(listGetData(map->head)))->copyKeyFunc,
                            ((Pair)(listGetData(map->head)))->freeDataFunc, 
                            ((Pair)(listGetData(map->head)))->freeKeyFunc);
    if(pair == NULL)
    {
        return MAP_OUT_OF_MEMORY;
    }
    // pairSet(pair, data, key);

    // check if we need to insert before the first element
    if(map->compareKeyElements(key, ((Pair)(listGetData(map->head)))->key) < 0)
    {
        if(listAdd(map->head) == NULL)
        {
            pairFree(pair);
            return MAP_OUT_OF_MEMORY;
        }

        Pair current_data = pairCopy(listGetData(map->head));
        listSet(map->head->next, current_data);
        listSet(map->head, pair);

        pairSet((Pair)(listGetData(map->head)), data, key);
        return MAP_SUCCESS;
    }

    List iterator = map->head;
    while(iterator)
    {
        if(iterator->next == NULL || map->compareKeyElements(key, ((Pair)(listGetData(iterator->next)))->key) < 0)
        {
            if(listAdd(iterator) == NULL)
            {
                pairFree(pair);
                return MAP_OUT_OF_MEMORY;
            }
            listSet(iterator->next, pair);
            pairSet((Pair)(iterator->next->data), data, key);
            return MAP_SUCCESS;
        }
        iterator = iterator->next;
    }
    return MAP_SUCCESS; // should never reach
}
// HELPER FUNCTIONS END

Map mapCreate(copyMapDataElements copyDataElement,
              copyMapKeyElements copyKeyElement,
              freeMapDataElements freeDataElement,
              freeMapKeyElements freeKeyElement,
              compareMapKeyElements compareKeyElements)
{
    if(!copyDataElement || !copyKeyElement || !freeDataElement || !freeKeyElement || !compareKeyElements)
    {
        return NULL;
    }

    Map map = malloc(sizeof(*map));
    if(map == NULL)
    {
        return NULL;
    }

    map->head = listCreate((freeListDataElement)&pairFree, (copyListDataElement)&pairCopy);
    if(map->head == NULL)
    {
        mapDestroy(map);
        return NULL;
    }
    Pair pair = pairCreate(copyDataElement, copyKeyElement, freeDataElement, freeKeyElement);
    if(pair == NULL)
    {
        mapDestroy(map);
        return NULL;
    }
    ListError result = listSet(map->head, pair);
    if(result != LIST_SUCCESS)
    {
        free(map);
        pairFree(pair);
        return NULL;
    }

    map->iterator = map->head;  // maybe mapGetFirst(map)

    map->compareKeyElements = compareKeyElements;

    return map;
}

void mapDestroy(Map map)
{
    if(map == NULL)
    {
        return;
    }
    listDestroy(map->head);
    free(map);
}

Map mapCopy(Map map)
{
    if(map == NULL)
    {
        return NULL;
    }
    Map new_map = mapCreate(((Pair)(listGetData(map->head)))->copyDataFunc, 
                            ((Pair)(listGetData(map->head)))->copyKeyFunc, 
                            ((Pair)(listGetData(map->head)))->freeDataFunc, 
                            ((Pair)(listGetData(map->head)))->freeKeyFunc, 
                            map->compareKeyElements);
    if(new_map == NULL)
    {
        return NULL;
    }
    ListError result = listCopy(map->head, new_map->head);
    if(result == LIST_MEMORY_ERROR)
    {
        return NULL;
    }
    if(new_map->head == NULL)  // neccessary?
    {
        mapDestroy(new_map);
        return NULL;
    }   
    new_map->compareKeyElements = map->compareKeyElements;
    return new_map;
}

int mapGetSize(Map map)
{
    if(map == NULL)
    {
        return -1;
    }
    int counter = 0;
    List iterator = map->head;

    // counting keys
    while(iterator != NULL && listGetData(iterator) != NULL && ((Pair)(listGetData(iterator)))->key != NULL)
    {
        counter++;
        iterator = iterator->next;
    }
    return counter;
}

bool mapContains(Map map, MapKeyElement element)
{
    if(map == NULL || element == NULL || map->head->data == NULL /*empty list*/)
    {
        return false;
    }
    if(mapGet(map, element) != NULL)
    {
        return true;
    }
    return false;
}


MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement)
{
    if(map == NULL || keyElement == NULL || dataElement == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    // search for existing key
    List iterator = map->head;
    while(iterator && ((Pair)listGetData(iterator))->key)  // while there are keys to search for
    {
        if(map->compareKeyElements(((Pair)listGetData(iterator))->key, keyElement) == 0)
        {
            pairSet((Pair)(listGetData(iterator)), dataElement, keyElement);
            return MAP_SUCCESS;
        }
        iterator = iterator->next;
    }
    // didn't found, need to add a new key
    return mapAdd(map, keyElement, dataElement);
}

MapDataElement mapGet(Map map, MapKeyElement keyElement)
{
    if(map == NULL || keyElement == NULL || mapGetSize(map) == 0)
    {
        return NULL;
    }
    List iterator = map->head;
    while(iterator)
    {
        if(map->compareKeyElements(((Pair)(listGetData(iterator)))->key, keyElement) == 0)
        {
            return ((Pair)(listGetData(iterator)))->data;
        }
        iterator = iterator->next;
    }
    return NULL;
}

MapResult mapRemove(Map map, MapKeyElement keyElement)
{
    if(map == NULL || keyElement == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    MapDataElement first_key = mapGetFirst(map);
    if(first_key == NULL) // no keys in map
    {
        return MAP_ITEM_DOES_NOT_EXIST;
    }

    if(map->compareKeyElements(first_key, keyElement) == 0)
    {
        if(listGetSize(map->head) > 1) // list has at least two elements - switch heads.
        {
            map->head->freeDataElement(listGetData(map->head));
            List temp = map->head;
            map->head = map->head->next;
            free(temp);
        }
        else
        {
            pairSet((Pair)(listGetData(map->head)), NULL, NULL);  // we want to keep the first node
        }
        ((Pair)(listGetData(map->head)))->freeKeyFunc(first_key);
        return MAP_SUCCESS;
    }
    ((Pair)(listGetData(map->iterator)))->freeKeyFunc(first_key);

    List iterator = map->head;
    while(iterator->next)
    {
        if(map->compareKeyElements((((Pair)(listGetData(iterator->next)))->key), keyElement) == 0)
        {
            List temp = iterator->next;
            map->head->freeDataElement((Pair)(temp->data));
            iterator->next = temp->next;
            free(temp);
            return MAP_SUCCESS;
        }
        iterator = iterator->next;
    }
    return MAP_ITEM_DOES_NOT_EXIST;
}

MapKeyElement mapGetFirst(Map map)
{
    if(map->head == NULL)
    {
        return NULL;
    }
    map->iterator = map->head;
    return ((Pair)(listGetData(map->iterator)))->copyKeyFunc(((Pair)(listGetData(map->iterator)))->key);
}

MapKeyElement mapGetNext(Map map)
{
    if(map->iterator->next == NULL)
    {
        return NULL;
    }
    map->iterator = map->iterator->next;
    return ((Pair)(listGetData(map->iterator)))->copyKeyFunc(((Pair)(listGetData(map->iterator)))->key);
}

MapResult mapClear(Map map)
{
    if(map == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    listDestroy(map->head->next);
    pairSet((Pair)listGetData(map->head), NULL, NULL);  // need to keep first pair
    return MAP_SUCCESS;
}