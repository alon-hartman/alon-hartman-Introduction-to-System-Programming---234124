#ifndef _TOUR_H
#define _TOUR_H

#include "game.h"
#include "map.h"
#include "list.h"

#define NO_WINNER -1

typedef struct tournament_t *Tournament;

typedef enum{
    TOURNAMENT_SUCCESS,
    TOURNAMENT_OUT_OF_MEMORY,
    TOURNAMENT_NULL_ARGUMENTS,
    TOURNAMENT_INVALID_ARGUMENTS
} TournamentError;

typedef enum {IN_PROCCESS=0,
              DONE} TournamentStatus;


/**
* gameCreate: Allocates a new tournament. Sets status to IN_PROCCESS and creates a Map of games.
*
* @param tournament_id - the tournament's id
* @param tournament_location - the tournament's location
*
* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new tournament in case of success.
*/
Tournament createTournament(int tournament_id, const char* tournament_location, unsigned int max_games);

/**
* tournamentDestroy: Deallocates an existing tournament. Clears all elements 
* including games by using the relevent free functions.
*
* @param tournament - Target tournament to be deallocated. If map is NULL nothing will be
* 		done
*/
void tournamentDestroy(Tournament tournament);

/**
 * tournamentAddGame: adds a game to the tournament.
 *
 * @param tournament - target tournament. must not be NULL.
 * @param first_player - first player in the game
 * @param second_player - second player in the game
 * @param winner - the winner of the game. must be of enum Winner.
 * @param play_time - time it took the game to complete.
 * @return
 *   NULL if allocation failed. TOURNAMENT_SUCCESS otherwise.
*/
TournamentError tournamentAddGame(Tournament tournament,int first_player, int second_player,
                                  Winner winner, int play_time);

/** tournamentemovePlayer: removes a given player from the touranment. his id is removed in all of the games he played
 * and if the tournament isn't over, his oppnent gets the win.
 * @param tournament - target tournament. must not be NULL.
 * @param player_id - id of the player to remove from the tournament.
 * @return
 *      number of games the player was removed from. 0 if allocation error occured. **/
int tournamentRemovePlayer(Tournament tournament, int player_id);

/** tournamentCopy: creates a new tournament, identical to given tournament.
 * @param tournament - target tournament. must not be NULL.
 * @return
 *      NULL if allocation error occured. Tournament otherwise. **/
Tournament tournamentCopy(Tournament tournament);

/** doesGameExist: checks if a game between player1 and player2 exists in the given tournament.
 * @param tournament - target tournament. must not be NULL.
 * @param player1 - first player in the game
 * @param player2 - second player in the game
 * @return
 *      true if a game with both player1 and player2 exists in the tournament, false otherwise. **/
bool doesGameExist(Tournament tournament, int player1, int player2);


/** endTournament: end a given tournament and determines the winner according to the instructions in the PDF.
 * @param tournament - target tournament. must not be NULL.
 * @return
 *      void **/
void endTournament(Tournament tournament);

/**
 * getTournamentWinnderID: returns the tournament winner's ID.
 *
 * @param tournament - target tournament.
 * @return
 *   -1 if a NULL was sent.
 *   The tournament winner's ID otherwise.
*/
int getTournamentWinnerID(Tournament tournament);

/**
 * setTournamentWinnderID: sets the tournament winner's ID.
 *
 * @param tournament - target tournament. must not be NULL.
 * @param winner_id - the winner's id. must be non negative.
 * @return
 *   void
*/
void setTournamentWinnerID(Tournament tournament, int winner_id);

/**
 * getTournamentStatus: returns the tournament's status. Assumes tournament is not NULL.
 *
 * @param tournament - target tournament. must not be NULL.
 * @return
 *   The tournament's status: IN_PROCCESS or DONE.
*/
TournamentStatus getTournamentStatus(Tournament tournament);

/**
 * getTournamentMaxGamesAllowed: returns the maximum amount of games a player is allowed to play in the tournament.
 *
 * @param tournament - target tournament. must not be NULL.
 * @return
 *   The max amount of game a player is allowed to play in the given tournament.
*/
int getTournamentMaxGamesAllowed(Tournament tournament);

/**
 * getTournamentLocation: returns the location of the tournament
 *
 * @param tournament - target tournament. must be non-NULL.
 * @return
 *   The location of the tournament
*/
char* getTournamentLocation(Tournament tournament);

/**
 * setTournamentStatus: sets the given tournament to the given status.
 *
 * @param tournament - target tournament. must not be NULL.
 * @param status - status to be given to the tournament: IN_PROCCESS or DONE
 * @return
 *   void
*/
void setTournamentStatus(Tournament tournament, TournamentStatus status);

/**
 * getTournamentPlayersMap: returns the players_map associated with the given tournament
 *
 * @param tournament - target tournament. must not be NULL.
 * @return
 *   tournament->players_map
*/
Map getTournamentPlayersMap(Tournament tournament);

// returns true if at least one game in tournament. false otherwise.
bool doesTournamentHasGames(Tournament tournament);

// counts the number of games player_id played in the given tournament.
int tournamentCountGames(Tournament tournament, int player_id);

/**
 * getTournamentStatistics: calculates the tournament's statistics. the results are returned through
 * the given pointers.
 * @return
 *  basically always CHESS_SUCCESS.
*/
TournamentError getTournamentStatistics(Tournament tournament, int* longest_game_time, double* average_game_time,
                                        int* number_of_games, int* number_of_players);

// calculates the avereage game time of a given player.
int tournamentCalculateGameTime(Tournament tournament, int player_id);

// getter function. returns tournament->players_list
List getTournamentPlayersList(Tournament tournament);

// checks if a player played in the tournament.
// bool tournamentDoesPlayerExist(Tournament tournament, int player_id);

#endif