#include <stdlib.h>
#include "pair.h"

Pair pairCreate(copyDataElement copyDataFunc, copyKeyElement copyKeyFunc, 
                freeDataElement freeDataFunc, freeKeyElement freeKeyFunc)
{
    Pair pair = malloc(sizeof(*pair));
    if(pair == NULL)
    {
        return NULL;
    }
    pair->key = NULL;
    pair->data = NULL;
    pair->copyDataFunc = copyDataFunc;
    pair->copyKeyFunc = copyKeyFunc;
    pair->freeDataFunc = freeDataFunc;
    pair->freeKeyFunc = freeKeyFunc;
    return pair;
}

PairErrors pairSet(Pair pair, PairDataElement data, PairKeyElement key)
{
    if(pair == NULL)
    {
        return PAIR_NULL_ARGUMENT;
    }
    pair->freeDataFunc(pair->data);
    pair->freeKeyFunc(pair->key);
    pair->data = pair->copyDataFunc(data);
    pair->key = pair->copyKeyFunc(key);
    return PAIR_SUCCESS;
}

void pairFree(Pair pair)
{
    if(pair == NULL)
    {
        return;
    }
    pair->freeDataFunc(pair->data);
    pair->freeKeyFunc(pair->key);
    free(pair);
}

Pair pairCopy(Pair pair)
{
    if(pair == NULL)
    {
        return NULL;
    }
    Pair new_pair = pairCreate(pair->copyDataFunc, pair->copyKeyFunc, pair->freeDataFunc, pair->freeKeyFunc);
    if(new_pair == NULL)
    {
        return NULL;
    }
    pairSet(new_pair, pair->data, pair->key);
    return new_pair;
}

