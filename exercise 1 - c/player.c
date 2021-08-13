#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "player.h"

Player playerCreate(int id, int wins, int losses, int draws)
{
    Player player = malloc(sizeof(*player));
    if(player == NULL || wins < 0 || losses < 0 || draws < 0)
    {
        return NULL;
    }
    player->id = id;
    player->wins = wins;
    player->losses = losses;
    player->draws = draws;
    return player;
}

Player playerCopy(Player src_player)
{
    if(src_player == NULL)
    {
        return NULL;
    }
    Player new_player = playerCreate(src_player->id, src_player->wins, src_player->losses, src_player->draws);
    return new_player;  // NULL if allocation failed.
}

void playerDestroy(Player player)
{
    free(player);
}

int getTotalGamesPlayed(Player player)
{
    if(player == NULL)
    {
        return 0;
    }
    return player->wins + player->losses + player->draws;
}

double playerGetLevel(Player player)
{
    // assuming totalGamesPlayed(player) > 0
    double level = (double)(6*player->wins - 10*player->losses + 2*player->draws)/(double)(getTotalGamesPlayed(player));
    int sign = level >= 0 ? 1 : -1;
    return (double)((int)((level + sign*0.004)*100))/100.0;
}
int playerGetID(Player player)
{
    return player->id;
}

/** TODO: neccessary?
 * perhaps only when inserting a removed player.
 * the question is wethear it's enough to compare players only by their ids.
 */ 
// bool playerCompare(Player player1, Player player2)
// {
//     if(player1->id == player2->id)
// }