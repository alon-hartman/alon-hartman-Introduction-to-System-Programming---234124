#include <stdbool.h>
#include <stdlib.h>
#include "game.h"


struct game_t {
    int player_1;
    int player_2;
    Winner winner;
    int game_time;
};


Game gameCreate(int player_1, int player_2, Winner winner, int game_time)
{
    Game game = malloc(sizeof(*game));
    if(game == NULL)
    {
        return NULL;
    }
    game->player_1 = player_1;
    game->player_2 = player_2;
    game->winner = winner;
    game->game_time = game_time;
    return game;
}

Game gameCopy(Game game)
{
    Game copy = gameCreate(game->player_1, game->player_2, 
                           game->winner, game->game_time);
    if(copy == NULL)
    {
        return NULL;
    }
    return copy;
}

void gameDestroy(Game game)
{
    free(game);
}

int getPlayer1ID(Game game)
{
    return game->player_1;
}

int getPlayer2ID(Game game)
{
    return game->player_2;
}

Winner getWinner(Game game)
{
    return game->winner;
}

int getGameTime(Game game)
{
    return game->game_time;
}

void setPlayer1(Game game, int player1_id)
{
    game->player_1 = player1_id;
}

void setPlayer2(Game game, int player2_id)
{
    game->player_2 = player2_id;
}

void setWinner(Game game, Winner winner)
{
    game->winner = winner;
}