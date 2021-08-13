#ifndef GAME_H
#define GAME_H

#include <memory>
#include "Exceptions.h"
#include "Character.h"
#include "Auxiliaries.h"
#include <vector>
#include "Soldier.h"
#include "Sniper.h"
#include "Medic.h"
#include <iostream>

namespace mtm { class Game; }

std::ostream& operator<<(std::ostream& stream, const mtm::Game& game);

namespace mtm {
    typedef std::vector<std::vector<std::shared_ptr<Character>>> Board;

    class Game
    {
        private:
            int height;
            int width;
            Board board;
            /** isOutOfArray : helper function to determine of a given coordinate is within the game boundaries*/
            bool isOutOfArray(const GridPoint& coordinates) const;
        public:
            /** Game constructor: creates a new Game with board of size height x width 
             * @param height - an integer holding the height of the board.
             * @param width - an integer holding the width of the board
             * @throws IllegalArgument - if height or width are negative.
             */        
            Game(int height, int width);
            /** Game copy constructor: creates a deep copy of the given (reference to) Game */
            Game(const Game& other);
            // destructor
            ~Game() = default;

            /** operator= for Game. Cleans the game at the left side of the equals sign and copies the Game at the
             *  right side of the sign to it.
             */
            Game& operator=(const Game& other);

            /** addCharacter: adds a the given Character to the game at the given coordinates.
             * @param coordinates - a reference to a GridPoint object with the coordinates of where the created
             *                      Character will be placed on the board.
             * @param Character - a shared_ptr to the new Character which will be placed on the board.
             * 
             * @throws IllegalCell - if the cell is out of bounds of the board.
             * @throws CellOccupied - if there is another Character in the same coordinates.
             * 
             * @returns - void
            */
            void addCharacter(const GridPoint& coordinates, std::shared_ptr<Character> Character);

            /** makeCharacter: creates a new Character of a given type (Soldier, Medic or Sniper).
             * @param type - CharacterType enum of the wanted character (Soldier, Medic or Sniper).
             * @param team - the team of which the new Character will belong to 
             *                  (Team::POWERLIFTERS or Team::Crossfitters)
             * @param health - the character's starting health.
             * @param ammo - the character's starting ammo
             * @param range - the character's attack range.
             * @param power - the character's power.
             * 
             * @throws IllegalArgument - if health ןד negative or if ammo, power or range are not positive.
             * 
             * @returns - a shared_ptr to the newly created Character.
             * 
            */
            static std::shared_ptr<Character> makeCharacter(CharacterType type, Team team, units_t health,
                                                                units_t ammo, units_t range, units_t power);
            /** move: moves the character at src_coordinates to dst_coordinates.
             * @param src_coordinates - the coordinates of the tile where the desired character is placed.
             * @param dst_coordinates - the coordinates of the tile where the desired character will be moved to.
             * 
             * @throws IllegalCell - if one of the coordinates is out of bound of the board.
             * @throws CellEmpty - if no character is placed at src_coordinates.
             * @throws MoveTooFar - if the distance between dst_coordinates and src_coordinates is larger than
             *          the character's (at src_coordinates) movement range.
             * @throws CellOccupied - if there is a character placed at dst_coordinates.
             * 
             * @returns - void.
            */                   
			void move(const GridPoint& src_coordinates, const GridPoint& dst_coordinates);  

           /** attack: attacks the dst_coordinates tile with the character at src_coordinates.
            * @param src_coordinates - the coordinates of the tile containing the attacker.
            * @param dst_coordinates - the coordinates of the tile to be attacked.
            * 
            * @throws IllegalCell - if either src_coordinates or dst_coordinates are out of bound of the board.
            * @throws CellEmpty - if there is no character placed at src_coordinates.
            * @throws OutOfRange - if dst_coordinates is not in range of the character at src_coordinates
            * @throws OutOfAmmo - if the attacker doesn't have enough ammo to attack.
            * @throws IllegalTarget - if the attacker cannot attack the dst_coordinates tile 
            *          (different for each Character)
           */
            void attack(const GridPoint& src_coordinates, const GridPoint& dst_coordinates);

            /** reload: give the character at src_coordinates ammo. Each character receives a different amount.
             * @param coordinates - the coordinates of the desired character to reload.
             * 
             * @throws IllegalCell - if coordinates is out of bound of the board.
             * @throws CellEmpty - if there is no character at src_coordinates.
             * 
             * @returns - void.
            */
            void reload(const GridPoint& coordinates);
			
			
            /** isOver: checks if one the teams has no players alive
            * @param winningTeam - pointer to possible result, if pointer is null
                                   no result will be set in param
            * 
            * @return - if the true if the game is over, false otherwise
            */
            bool isOver(Team* winningTeam = nullptr) const;

            /** operator<<: prints the current state of the game board:
             *              each character has a letter attributed to it: s for soldier, m for medic and n for sniper.
             *              if the character is in team POWERLIFTERS, then the letter will appear as a capital letter
             *              and for team CROSSFITTERS the letter will remain in lower case.
             * @param stream - the output stream.
             * @param game - the game to be printed.
             * @returns the given stream after printing the game to it.
            */
            friend std::ostream& ::operator<<(std::ostream& stream, const Game& game);
    };
}
#endif