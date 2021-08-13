#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "tournament.h"
#include "map.h"
#include "list.h"
#include "game.h"
#include "player.h"

struct tournament_t {
    int tournament_id;
    List games;
    bool status;
    char* tournament_location;
    int winner_id;
    int max_games_allowed;
    List players_list;
    int removed_players_counter;
};


// HELPER FUNCTIONS START

/**
 * tournamentAddPlayer: stupid function to add or update a player's data. You only insert players when adding games
 *                      so there is always information regarding the result of the game. assumes player_id is valid.
 *
 * @param tournament - target tournament.
 * @param player_id - the player to add or update
 * @param win - true if he won.
 * @param lose - true if he lost.
 * @param draw  - true if there was a draw.
 * @return
 *   TOURNAMENT_NULL_ARGUMENTS if one of the arguments is NULL.
 *   TOURNAMENT_INVALID_ARGUMENTS if win, lose and draw are all false or if more than one are true.
 *   TOUNAMENT_SUCCESS otherwise.
*/
static TournamentError tournamentAddPlayer(Tournament tournament, int player_id, bool win, bool lose, bool draw)
{
    if(tournament == NULL)
    {
        return TOURNAMENT_NULL_ARGUMENTS;
    }
    if((win+lose+draw == 0) || (win+lose+draw > 1) || player_id <= 0)
    {
        return TOURNAMENT_INVALID_ARGUMENTS;
    }
    List iterator = tournament->players_list;
    if(listGetData(iterator) == NULL)  // no players in list, add the first
    {
        Player player = playerCreate(player_id, win, lose, draw);
        if(player == NULL)
        {
            return TOURNAMENT_OUT_OF_MEMORY;
        }
        listSet(iterator, player);
        return TOURNAMENT_SUCCESS;
    }
    while(iterator)  // at least one player in list
    {
        Player player = listGetData(iterator);
        if(player->id == player_id)
        {
            player->wins += win;
            player->losses += lose;
            player->draws += draw;
            return TOURNAMENT_SUCCESS;
        }
        iterator = iterator->next;
    }
    // player does not exist in players_list. create him.
    Player player = playerCreate(player_id, win, lose, draw);
    List new_node = listAdd(tournament->players_list);
    if(new_node == NULL)
    {
        return TOURNAMENT_OUT_OF_MEMORY;
    }
    listSet(new_node, player);  // no copying here so no need to free player.
    return TOURNAMENT_SUCCESS;
}

// runs through all games in a tournament and calculates for each player attending: wins, losses and draws.
static TournamentError tournamentUpdatePlayersList(Tournament tournament)
{
    List new_players_list = listCreate((freeListDataElement)playerDestroy, (copyListDataElement)playerCopy);
    if(new_players_list == NULL)
    {
        return TOURNAMENT_OUT_OF_MEMORY;
    }
    listDestroy(tournament->players_list);
    tournament->players_list = new_players_list;
    List iterator = tournament->games;
    while(iterator)
    {
        Game game = listGetData(iterator);
        if(game == NULL)  // no games in tournament
        {
            return TOURNAMENT_SUCCESS;
        }
        Winner winner = getWinner(game);
        tournamentAddPlayer(tournament, getPlayer1ID(game), winner == FIRST_PLAYER, 
                            winner == SECOND_PLAYER, winner == DRAW);
        tournamentAddPlayer(tournament, getPlayer2ID(game), winner == SECOND_PLAYER, 
                            winner == FIRST_PLAYER, winner == DRAW);
        iterator = iterator->next;
    }
    return TOURNAMENT_SUCCESS;
}

// HELPER FUNCTIONS END

Tournament createTournament(int tournament_id, const char* tournament_location, unsigned int max_games)
{
    Tournament tournament = malloc(sizeof(*tournament));
    if(tournament == NULL)
    {
        return NULL;
    }
    tournament->tournament_id = tournament_id;
    tournament->tournament_location = malloc(sizeof(char) * (strlen(tournament_location)+1));
    if(tournament->tournament_location == NULL)
    {
        tournamentDestroy(tournament);
        return NULL;
    }
    strcpy(tournament->tournament_location, tournament_location);
    
    tournament->games = listCreate((freeListDataElement)gameDestroy, (copyListDataElement)gameCopy);
    if(tournament->games == NULL)
    {
        tournamentDestroy(tournament);
        return NULL;
    }

    tournament->winner_id = NO_WINNER;
    tournament->max_games_allowed = max_games;
    tournament->status = IN_PROCCESS;
    tournament->removed_players_counter = 0;
    tournament->players_list = listCreate((freeListDataElement)playerDestroy, (copyListDataElement)playerCopy);
    if(tournament->players_list == NULL)
    {
        tournamentDestroy(tournament);
        return NULL;
    }
    return tournament;
}

void tournamentDestroy(Tournament tournament)
{
    if(tournament == NULL)
    {
        return;
    }
    listDestroy(tournament->games);
    listDestroy(tournament->players_list);
    free(tournament->tournament_location);
    free(tournament);

}

TournamentError tournamentAddGame(Tournament tournament, int first_player, int second_player,
                                  Winner winner, int play_time)
{
    Game game = gameCreate(first_player, second_player, winner, play_time);
    if(game == NULL)
    {
        return TOURNAMENT_OUT_OF_MEMORY;
    }
    List new_list = listAdd(tournament->games);
    if(new_list == NULL)
    {
        return TOURNAMENT_OUT_OF_MEMORY;
    }
    listSet(new_list, game);
    // adding the players to the players map
    tournamentAddPlayer(tournament, first_player, winner == FIRST_PLAYER, winner == SECOND_PLAYER, winner == DRAW);
    tournamentAddPlayer(tournament, second_player, winner == SECOND_PLAYER, winner == FIRST_PLAYER, winner == DRAW);
    return TOURNAMENT_SUCCESS;
}

int getTournamentWinnerID(Tournament tournament)
{
    if(tournament == NULL)
    {
        return -1;
    }
    return tournament->winner_id;
}

void setTournamentWinnerID(Tournament tournament, int winner_id)
{
    tournament->winner_id = winner_id;
}

TournamentStatus getTournamentStatus(Tournament tournament)
{
    return tournament->status;
}

void setTournamentStatus(Tournament tournament, TournamentStatus status)
{
    tournament->status = status;
}

int getTournamentMaxGamesAllowed(Tournament tournament)
{
    return tournament->max_games_allowed;
}

bool doesTournamentHasGames(Tournament tournament)
{
    return listGetData(tournament->games) != NULL;
}

List getTournamentPlayersList(Tournament tournament)
{
    return tournament->players_list;
}

int tournamentRemovePlayer(Tournament tournament, int player_id)
{
    if(tournament == NULL)
    {
        return 0;
    }
    List iterator = tournament->games;
    int instances_removed = 0;
    while(iterator != NULL)
    {
        Game game = listGetData(iterator);
        if(game == NULL)  // no games in tournament
        {
            return 0;
        }
        if(getPlayer1ID(game) == player_id)
        {
            setPlayer1(game, PLAYER_REMOVED);
            if(getTournamentStatus(tournament) == IN_PROCCESS)
            {
                setWinner(game, SECOND_PLAYER);
            }
            instances_removed += 1;
        }
        else if(getPlayer2ID(game) == player_id)
        {
            setPlayer2(game, PLAYER_REMOVED);
            if(getTournamentStatus(tournament) == IN_PROCCESS)
            {
                setWinner(game, FIRST_PLAYER);
            }
            instances_removed += 1;
        }
        iterator = iterator->next;
    }
    // assuming no memory errors because its hard to take care of.
    tournamentUpdatePlayersList(tournament);
    if(instances_removed > 0) {
        tournament->removed_players_counter++;
    }
    return instances_removed;
}

Tournament tournamentCopy(Tournament tournament)
{
    if(tournament == NULL)
    {
        return NULL;
    }
    Tournament new_tournament = createTournament(tournament->tournament_id, tournament->tournament_location, 
                                                 tournament->max_games_allowed);
    new_tournament->status = tournament->status;
    new_tournament->winner_id = tournament->winner_id;
    if(listGetData(tournament->games) != NULL)  // there are games to copy
    {
        ListError result = listCopy(tournament->games, new_tournament->games);
        if(result == LIST_MEMORY_ERROR)
        {
            tournamentDestroy(new_tournament);
            return NULL;
        }
    }
    ListError result = listCopy(tournament->players_list, new_tournament->players_list);
    if(result == LIST_MEMORY_ERROR)
    {
        tournamentDestroy(new_tournament);
        return NULL;
    }
    return new_tournament;
}

bool doesGameExist(Tournament tournament, int player1, int player2)
{ 
    List iterator = tournament->games;
    if(listGetData(iterator) == NULL)  // no games in tournament
    {
        return false;
    }
    while(iterator)
    {
        if ((getPlayer1ID(iterator->data) == player1 && getPlayer2ID(iterator->data) == player2) ||
            (getPlayer1ID(iterator->data) == player2 && getPlayer2ID(iterator->data) == player1))
        {
            return true;
        }
        iterator = iterator->next;
    }
    return false;
}

int tournamentCountGames(Tournament tournament, int player_id)
{
    if(tournament == NULL || player_id <= 0)
    {
        return 0;
    }

    int games_counter = 0;
    List iterator = tournament->games;
    if(listGetData(iterator) == NULL)  // no games in tournament
    {
        return 0;
    }
    while(iterator)
    {
        if(getPlayer1ID(iterator->data) == player_id || getPlayer2ID(iterator->data) == player_id)
        {
            games_counter += 1;
        }
        iterator = iterator->next;
    }
    return games_counter;
}

int tournamentCalculateGameTime(Tournament tournament, int player_id)
{
    if(tournament == NULL || player_id <= 0)
    {
        return 0;
    }
    int total_game_time = 0;
    List games_iterator = tournament->games;
    while(games_iterator)
    {
        Game game = listGetData(games_iterator);
        if(game == NULL)  // no games in tournament
        {
            return 0;
        }
        if(getPlayer1ID(game) == player_id || getPlayer2ID(game) == player_id)
        {
            total_game_time += getGameTime(game);
        }
        games_iterator = games_iterator->next;
    }
    return total_game_time;
}

TournamentError getTournamentStatistics(Tournament tournament, int* longest_game_time, double* average_game_time,
                                        int* number_of_games, int* number_of_players)
{
    *number_of_players = tournament->removed_players_counter;
    *number_of_players += (listGetData(tournament->players_list) != NULL) * listGetSize(tournament->players_list);
    // ^ number of nodes in the list but there is always at least 1 because of stupid implementation.
    *number_of_games = 0;
    *longest_game_time = 0;
    *average_game_time = 0;
    List iterator = tournament->games;
    if(listGetData(iterator) == NULL)  // no games in tournament, avoid dividing by zero.
    {
        *number_of_players = 0;
        return TOURNAMENT_SUCCESS;
    }
    while(iterator)
    {
        Game game = listGetData(iterator);
        if(game != NULL)
        {
            *number_of_games += 1;
            int game_time = getGameTime(game);
            *longest_game_time = *longest_game_time > game_time ? *longest_game_time : game_time;
            *average_game_time += game_time;
        }
        iterator = iterator->next;
    }
    *average_game_time = *average_game_time/(double)(*number_of_games);
    return TOURNAMENT_SUCCESS;
}

char* getTournamentLocation(Tournament tournament)
{
    return tournament->tournament_location;
}

// bool tournamentDoesPlayerExist(Tournament tournament, int player_id)
// {
//     // assuming tournament and player_id are valid
//     List iterator = tournament->players_list;
//     while(iterator)
//     {
//         Player player = listGetData(iterator);
//         if(player == NULL)
//         {
//             return false;
//         }
//         else if(player->id == player_id)
//         {
//             return true;
//         }
//     }
//     return false;
// }