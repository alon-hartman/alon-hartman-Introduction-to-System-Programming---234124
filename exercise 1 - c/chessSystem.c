#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "chessSystem.h"
#include "map.h"
#include "tournament.h"
#include "game.h"
#include "list.h"
#include "player.h"

struct chess_system_t
{
    Map tournaments_map;
};

// HELPER FUNCTIONS 

/** Function to be used for copying an int as a key to the map */
static MapKeyElement copyInt(MapKeyElement n) {
    if (!n) {
        return NULL;
    }
    int *copy = malloc(sizeof(*copy));
    if (!copy) {
        return NULL;
    }
    *copy = *(int *) n;
    return copy;
}

/** Function to be used for copying a double as a key to the map */
static MapKeyElement copyDouble(MapKeyElement n) {
    if (!n) {
        return NULL;
    }
    double *copy = malloc(sizeof(*copy));
    if (!copy) {
        return NULL;
    }
    *copy = *(double *) n;
    return copy;
}

/** Functions to be used by the map for freeing elements */
static void freeInt(MapKeyElement n) {
    free(n);
}
static void freeDouble(MapKeyElement n) {
    free(n);
}

/** Function to be used by the map for comparing key elements 
 *  @return
 *      positive integer if key1 > key2, 0 if key1 = key2, negarive integer otherwise.
*/
static int compareInt(MapKeyElement key1, MapKeyElement key2) {
    return *(int*)key1 - *(int*)key2;
}

/** isValidLocation: checks if the location string is valid as per instructions.
 * @param location - the string to be validated.
 *  @return
 *      true if location if valid, false otherwise.
*/
static bool isValidLocation(const char* location)
{
    int length = strlen(location);
    if(length == 0 || (location[0] >= 'A' && location[0] <= 'Z') == false)
    {
        return false;
    }
    for(int i=1; i<length; i++)
    {
        if(((location[i] >= 'a' && location[i] <= 'z') || location[i] == ' ') == false)
        {
            return false;
        }
    }
    return true;
}

/** Function to be used for comparing two players' score.
 * @param player1 - Player struct of first player to compare.
 * @param player2 - Player struct of seconf player to compare.
 * @return 
 *      positive integer if player1's score is lower than player2's score.
 *      negative integer if player1's score is higher than player2's score.
 *      same score for two players is impossible.**/
static int comparePlayersScore(Player player1, Player player2)
{
    unsigned int player1_score = 2*player1->wins + player1->draws;
    unsigned int player2_score = 2*player2->wins + player2->draws;
    if(player1_score == player2_score)
    {
        if(player1->losses == player2->losses)
        {
            if(player1->wins == player2->wins)
            {
                return player1->id - player2->id;  // decide by lowest id
            }
            return player2->wins - player1->wins;  // decide by highest amount of wins
        }
        return player1->losses - player2->losses;  // decide by lowest amount of losses
    }
    return player2_score - player1_score;  // decide by highest score
}

/** Function to be used for comparing two players' level.
 * @param player1 - Player struct of first player to compare.
 * @param player2 - Player struct of seconf player to compare.
 * @return 
 *      positive integer if player1's level is lower than player2's level.
 *      negative integer if player1's level is higher than player2's level.
 *      if levels are equal, returns positive integer if player1's id is higher 
 *      than player2's id and negative integer otherwise. **/
static int comparePlayersLevel(Player player1, Player player2)
{
    double player1_level = playerGetLevel(player1);
    double player2_level = playerGetLevel(player2);
    if(player1_level == player2_level) 
    {
        return player1->id - player2->id;
    }
    return (player2_level > player1_level) ? 1 : -1;
}

/** chessCreateAllPlayersMap: ugly function which creates a map containing all players across all tournaments,
 * with statistics such as wins, losses and draws.
 * @param chess - the chess system.
 * @return 
 *      a NULL if chess is NULL or if an allocation error occured. A Map containing all players across all tournaments
 *      otherwise. **/
static Map chessCreateAllPlayersMap(ChessSystem chess)
{
    if(chess == NULL)
    {
        return NULL;
    }
    Map all_players = mapCreate((copyMapDataElements)playerCopy, copyInt, 
                                (freeMapDataElements)playerDestroy, freeInt, compareInt);
    if(all_players == NULL)
    {
        return NULL;
    }
    MAP_FOREACH(int*, tournament_id, chess->tournaments_map)
    {
        Tournament tournament = mapGet(chess->tournaments_map, tournament_id);
        List players_iterator = getTournamentPlayersList(tournament);
        while(players_iterator)
        {
            Player player_tournament = listGetData(players_iterator);
            if(player_tournament == NULL)  // no players in tournament
            {
                break;  // end while, go to next tournament
            }
            Player player_all = mapGet(all_players, &(player_tournament->id));
            if(player_all == NULL)  // add player to the map.
            {
                if(mapPut(all_players, &(player_tournament->id), player_tournament) == MAP_OUT_OF_MEMORY)
                {
                    freeInt(tournament_id);
                    mapDestroy(all_players);
                    return NULL;
                }
            }
            else  // player already exists in map, update his values.
            {
                player_all->wins += player_tournament->wins;
                player_all->losses += player_tournament->losses;
                player_all->draws += player_tournament->draws;
            }
            players_iterator = players_iterator->next;
        }
        freeInt(tournament_id);
    }
    return all_players;
}
// HELPER FUNCTIONS END

// IMPLEMENTATION STARTS HERE
ChessSystem chessCreate()
{
    ChessSystem chess = malloc(sizeof(*chess));
    if(chess == NULL)
    {
        return NULL;
    }

    chess->tournaments_map = mapCreate((copyMapDataElements)tournamentCopy, copyInt, 
                                       (freeMapDataElements)tournamentDestroy, freeInt, compareInt);
    if(chess->tournaments_map == NULL)
    {
        chessDestroy(chess);
        return NULL;
        
    }

    return chess;
}

void chessDestroy(ChessSystem chess)
{
    if(chess == NULL)
    {
        return;
    }
    mapDestroy(chess->tournaments_map);
    free(chess);
}

ChessResult chessAddTournament(ChessSystem chess, int tournament_id,  // unsigned int?
                                int max_games_per_player, const char* tournament_location)
{
    if(chess == NULL || tournament_location == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(tournament_id <= 0)
    {
        return CHESS_INVALID_ID;
    }
    if(mapContains(chess->tournaments_map, &tournament_id) == true)
    {
        return CHESS_TOURNAMENT_ALREADY_EXISTS;
    }
    if(isValidLocation(tournament_location) == false)
    {
        return CHESS_INVALID_LOCATION;
    }
    if(max_games_per_player <= 0)
    {
        return CHESS_INVALID_MAX_GAMES;
    }

    Tournament new_tournament = createTournament(tournament_id, tournament_location, max_games_per_player);
    if(new_tournament == NULL)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    MapResult result = mapPut(chess->tournaments_map, &tournament_id, new_tournament);
    tournamentDestroy(new_tournament);
    if(result == MAP_OUT_OF_MEMORY)
    {
        return CHESS_OUT_OF_MEMORY;
    }

    return CHESS_SUCCESS;
}

ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player,
                         int second_player, Winner winner, int play_time)
{
    if(chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(tournament_id <= 0 || (first_player == second_player) || first_player <= 0 || second_player <= 0)
    {
        return CHESS_INVALID_ID;
    }
    if(mapContains(chess->tournaments_map, &tournament_id) == false)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }

    Tournament tournament = mapGet(chess->tournaments_map, &tournament_id);
    if(getTournamentStatus(tournament) == DONE)
    {
        return CHESS_TOURNAMENT_ENDED;
    }
    if(doesGameExist(tournament, first_player, second_player) == true)
    {
        return CHESS_GAME_ALREADY_EXISTS;
    }
    if(play_time < 0)
    {
        return CHESS_INVALID_PLAY_TIME;
    }
    int max_games = getTournamentMaxGamesAllowed(tournament);
    if(tournamentCountGames(tournament, first_player) == max_games || 
       tournamentCountGames(tournament, second_player) == max_games)
    {
        return CHESS_EXCEEDED_GAMES;
    }

    TournamentError result = tournamentAddGame(tournament, first_player, second_player, winner, play_time);
    if(result == TOURNAMENT_OUT_OF_MEMORY)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    
    return CHESS_SUCCESS;
}

ChessResult chessRemoveTournament(ChessSystem chess, int tournament_id)
{
    if(chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if(tournament_id <= 0)
    {
        return CHESS_INVALID_ID;
    }
    if(mapContains(chess->tournaments_map, &tournament_id) == false)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }

    mapRemove(chess->tournaments_map, &tournament_id);
    return CHESS_SUCCESS;
}

ChessResult chessRemovePlayer(ChessSystem chess, int player_id)
{
    if(chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(player_id <= 0)
    {
        return CHESS_INVALID_ID;
    }
    int instances_removed = 0;

    MAP_FOREACH(int*, tournament_id, chess->tournaments_map)
    {
        Tournament tournament_iterator = mapGet(chess->tournaments_map, tournament_id);
        instances_removed += tournamentRemovePlayer(tournament_iterator, player_id);
        freeInt(tournament_id);
    }
    if(instances_removed == 0) // not a very good approach
    {
        return CHESS_PLAYER_NOT_EXIST;
    }
    return CHESS_SUCCESS;
}

ChessResult chessEndTournament(ChessSystem chess, int tournament_id)
{
    if(chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(tournament_id <= 0)
    {
        return CHESS_INVALID_ID;
    } 
    if(mapContains(chess->tournaments_map, &tournament_id) == false)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    Tournament tournament = mapGet(chess->tournaments_map, &tournament_id); 
    if(getTournamentStatus(tournament) == DONE)
    {
        return CHESS_TOURNAMENT_ENDED;
    }

    if(doesTournamentHasGames(tournament) == false)  // no games in tournament
    {
        return CHESS_NO_GAMES;
    }

    List players_iterator = getTournamentPlayersList(tournament);
    
    if(listGetData(players_iterator) == NULL) // // no players in tournament
    {
        setTournamentStatus(tournament, DONE);
        return CHESS_SUCCESS;  // or CHESS_NULL_ARGUMENT? either way it won't be checked
    }
    Map players_score = mapCreate(copyInt, (copyMapKeyElements)playerCopy, freeInt, 
                                 (freeMapKeyElements)playerDestroy, (compareMapKeyElements)comparePlayersScore);
    while(players_iterator)
    {
        Player player = listGetData(players_iterator);
        mapPut(players_score, player, &(player->id));
        players_iterator = players_iterator->next;
    }

    setTournamentStatus(tournament, DONE);
    int* winner_id = mapGetFirst(players_score);
    setTournamentWinnerID(tournament, *winner_id);
    freeInt(winner_id);  // mapGetFirst returns a copy of Player
    mapDestroy(players_score);
    return CHESS_SUCCESS;
}

double chessCalculateAveragePlayTime (ChessSystem chess, int player_id, ChessResult* chess_result)
{
    if(chess == NULL)
    {
        *chess_result = CHESS_NULL_ARGUMENT;
        return 0;
    }
    if(player_id <= 0)
    {
        *chess_result = CHESS_INVALID_ID;
        return 0;
    }
    // need to return CHESS_PLAYER_NOT_EXIST
    int total_play_time = 0, total_games_played = 0;
    MAP_FOREACH(int*, tournament_iterator, chess->tournaments_map)
    {
        Tournament tournament = mapGet(chess->tournaments_map, tournament_iterator);
        total_play_time += tournamentCalculateGameTime(tournament, player_id);
        total_games_played += tournamentCountGames(tournament, player_id);
        freeInt(tournament_iterator);
    }
    if(total_games_played == 0)  // did not appear in any games
    {
        *chess_result = CHESS_PLAYER_NOT_EXIST;
        return 0;
    }
    *chess_result = CHESS_SUCCESS;
    return (double)total_play_time/(double)total_games_played;
}

ChessResult chessSavePlayersLevels(ChessSystem chess, FILE* file)
{
    if(chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    Map all_players = chessCreateAllPlayersMap(chess);
    if(all_players == NULL)
    {
        return CHESS_OUT_OF_MEMORY;
    }

    Map player_level_map = mapCreate(copyDouble, (copyMapKeyElements)playerCopy, freeDouble, 
                                    (freeMapKeyElements)playerDestroy, (compareMapKeyElements)comparePlayersLevel);
    if(player_level_map == NULL)
    {
        return CHESS_OUT_OF_MEMORY;
    }

    MAP_FOREACH(int*, player_id, all_players)
    {
        Player player = mapGet(all_players, player_id);
        if(getTotalGamesPlayed(player) > 0) {  // only players with games matter for statistics
            double level = playerGetLevel(player);
            mapPut(player_level_map, player, &level);
        }
        freeInt(player_id);
    }
    MAP_FOREACH(Player, player, player_level_map)
    {
        double* level = mapGet(player_level_map, player);
        int result = fprintf(file, "%d %.2f\n", (int)player->id, (double)*level);
        playerDestroy(player);
        if(result <= 0)  // error while writing
        {
            mapDestroy(all_players);
            mapDestroy(player_level_map);
            return CHESS_SAVE_FAILURE;
        }
    }
    mapDestroy(all_players);
    mapDestroy(player_level_map);
    return CHESS_SUCCESS;
}

ChessResult chessSaveTournamentStatistics (ChessSystem chess, char* path_file)
{
    if(chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    char* open_method = "w";
    
    int longest_game_time, number_of_games, number_of_players, tournaments_ended_counter = 0;
    double average_game_time;
    MAP_FOREACH(int*, tournament_id, chess->tournaments_map)
    {
        Tournament tournament = mapGet(chess->tournaments_map, tournament_id);
        if(getTournamentStatus(tournament) == DONE)
        {
            FILE* stream = fopen(path_file, open_method);  // here becuase of error returning order:
            open_method = "a";                        // CHESS_NO_TOURNAMENTS_ENDED returns before CHESS_SAVE_FAILURE.
            if(stream == NULL)                   
            {
                return CHESS_SAVE_FAILURE;
            }
            tournaments_ended_counter++;
            getTournamentStatistics(tournament, &longest_game_time, 
                                    &average_game_time, &number_of_games, &number_of_players);
            int result = fprintf(stream, "%d\n%d\n%.2f\n%s\n%d\n%d\n",
                                 getTournamentWinnerID(tournament), longest_game_time, average_game_time, 
                                 getTournamentLocation(tournament), number_of_games, number_of_players);
            fclose(stream);  // we open and close more than I would like to. but this follows the instructions easily.
            if(result <= 0)  // writing falied
            {
                freeInt(tournament_id);
                return CHESS_SAVE_FAILURE;
            }
        }
        freeInt(tournament_id);
    }
    if(tournaments_ended_counter == 0)
    {
        return CHESS_NO_TOURNAMENTS_ENDED;
    }
    return CHESS_SUCCESS;
}