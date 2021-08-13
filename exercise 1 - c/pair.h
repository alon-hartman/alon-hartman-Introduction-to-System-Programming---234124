#ifndef _PAIR_H
#define _PAIR_H

typedef  void* PairDataElement;
typedef  void* PairKeyElement;
typedef PairDataElement(*copyDataElement)(PairDataElement);
typedef PairKeyElement(*copyKeyElement)(PairKeyElement);
typedef void(*freeDataElement)(PairDataElement);
typedef void(*freeKeyElement)(PairKeyElement);

struct pair_t
{
    PairDataElement data;
    PairKeyElement key;

    copyDataElement copyDataFunc;
    copyKeyElement copyKeyFunc;
    freeDataElement freeDataFunc;
    freeKeyElement freeKeyFunc;

};

typedef struct pair_t* Pair;

typedef enum{
    PAIR_SUCCESS,PAIR_NULL_ARGUMENT,PAIR_OUT_OF_MEMORY
}PairErrors;

/**
 * pairCreate: creates a new pair
 *
 * @param copyDataFunc - a copy function which receives and returns PairDataElement.
 * @param copyKeyFunc - a copy function which receives and returns PairDataElement.
 * @param freeDataFunc - a free function which receives PairDataElement and returns void.
 * @param freeKeyFunc - a free function which receives PairDataElement and returns void.
 * @param winner - enum Winner: FIRST_PLAYER, SECOND_PLAYER or DRAW
 * @return
 *   void
*/
Pair pairCreate(copyDataElement copyDataFunc, copyKeyElement copyKeyFunc, 
                freeDataElement freeDataFunc, freeKeyElement freeKeyFunc);

// sets the key and data elements of a given pair to the given data and key.
PairErrors pairSet(Pair pair, PairDataElement data, PairKeyElement key);

// destroys a given pair.
void pairFree(Pair pair);

// returns a copy of a given pair.
Pair pairCopy(Pair pair);

#endif