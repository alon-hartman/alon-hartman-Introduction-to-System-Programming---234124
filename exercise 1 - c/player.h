#ifndef _PLAYER_H
#define _PLAYER_H

typedef struct player_t {
    int id;
    int wins;
    int losses;
    int draws;
} *Player;

// creates a new player and initiallizes his statistics to the given statistics.
Player playerCreate(int id, int wins, int losses, int draws);

/* returns a copy of src_player.
* if src_player is NULL returns NULL. */
Player playerCopy(Player src_player);

// returns wins + losses + draws of a given player
int getTotalGamesPlayed(Player player);

// returns the level of a given player, rounded down to two decimal points.
double playerGetLevel(Player player);

// destroys a given player.
void playerDestroy(Player player);

// a getter function for player id.
int playerGetID(Player player);

#endif