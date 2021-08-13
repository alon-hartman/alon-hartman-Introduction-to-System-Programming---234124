#ifndef _GAME_H
#define _GAME_H

#define PLAYER_REMOVED -1

typedef struct game_t *Game;

typedef enum{
    GAME_SUCCESS,
    GAME_OUT_OF_MEMORY,
    GAME_NULL_ARGUMENTS,
    GAME_INVALID_ARGUMENTS
} GameErrors;

#ifndef _CHESSSYSTEM_H
typedef enum {
    FIRST_PLAYER,
    SECOND_PLAYER,
    DRAW
} Winner;
#endif

/**
* gameCreate: Allocates a new game.
*
* @param player1 - player 1 id
* @param player2 - player 2 id
* @param winner - FIRST_PLAYER, SECOND_PLAYER or DRAW
* @param game_time - length of the game in seconds.
*
* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new game in case of success.
*/
Game gameCreate(int player_1, int player_2, Winner winner, int game_time);

/**
* gameDestroy: Deallocates an existing game. Clears all elements by using the
* stored free functions.
*
* @param game - Target game to be deallocated. If game is NULL nothing will be
* 		done
*/
void gameDestroy(Game game);

/**
* gameCopy: Creates a copy of target game.
*
* @param game - Target game.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A Game containing the same elements as game otherwise.
*/
Game gameCopy(Game game);

/**
 * getPlayer1ID: returns player 1's ID.
 *
 * @param game - target game.
 * @return
 *   -1 if a NULL was sent.
 *   Player 1's ID otherwise.
*/
int getPlayer1ID(Game game);

/**
 * getPlayer2ID: returns player 2's ID.
 *
 * @param game - target game.
 * @return
 *   -1 if a NULL was sent.
 *   Player 2's ID otherwise.
*/
int getPlayer2ID(Game game);

/**
 * getWinner: returns the winner of the game (enum Winner)
 *
 * @param game - target game. must be non-NULL.
 * @return
 *   FIRST_PLAYER if a first_player won.
 *   SECOND_PLAYER if a second_player won.
 *   DRAW otherwise.
*/
Winner getWinner(Game game);

/**
 * getGameTime: returns the game time in seconds. Assumes game is not NULL.
 *
 * @param game - target game.
 * @return
 *   PLAYER_1, PLAYER_2 or DRAW.
*/
int getGameTime(Game game);

/**
 * setPlayer1: lets the user to set player1 to any int.
 *
 * @param game - target game.
 * @param player1_id - id to be set as player1
 * @return
 *   void
*/
void setPlayer1(Game game, int player1_id);

/**
 * setPlayer2: lets the user to set player1 to any int.
 *
 * @param game - target game.
 * @param player1_id - id to be set as player2
 * @return
 *   void
*/
void setPlayer2(Game game, int player2_id);

/**
 * setWinner: lets the user to set a winner for the game
 *
 * @param game - target game.
 * @param winner - enum Winner: FIRST_PLAYER, SECOND_PLAYER or DRAW
 * @return
 *   void
*/
void setWinner(Game game, Winner winner);

#endif