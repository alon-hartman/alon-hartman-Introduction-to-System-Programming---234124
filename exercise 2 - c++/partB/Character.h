#ifndef CHARACTER_H
#define CHARACTER_H

#include "Auxiliaries.h"
#include "Exceptions.h"
#include <vector>
#include <memory>
#include "Exceptions.h"


namespace mtm{
    class Character
    {
        protected:
            int health;
            int ammo;
            const int attack_range;
            const int power;
            const Team team;
            const int movement_range;
            const int attack_cost;
            const int reload_amount;
            typedef std::vector<std::vector<std::shared_ptr<Character>>> Board;

        public:
            /**
             * Character: constructor for Character. can't be used by user as it is an abstract class
             *
             * @param team - the team of the new Character (Team::POWERLIFTERS or Team::CROSSFITTERS)
             * @param health - starting health of the character.
             * @param ammo - starting ammo of the character.
             * @param attack_range - attack_range of the character.
             * @param power - power of the character (his attack strength).
             * @param movement_range - his movement range. default 0 as it is different for each Character.
             * @param attack_cost - how much ammo he loses upon attacking. 
             *                      default 0 as it is different for each Character.
             * @param reload_amount - the amount of ammo the Character recieves upon reloading. different for each
             *                        character.
             *
             * @return 
            */
            Character(Team Team, int health, int ammo, int attack_range, 
                        int power, int movement_range=0, int attack_cost=0, int reload_amount=0);
            /**
             * Character(const Character&): copy constructor for Character.
             *
             * @param Character - a reference to an existing Character
             *
             * @return - a deep copy of the given Character
            */
            Character(const Character& other) = default;
            // destructor
            virtual ~Character();

            /**
             * clone: creates a deep-copy of the current Character and returns a pointer to it
             *
             * @return - a deep copy of the given Character (Medic, Sniper or Soldier)
            */
            virtual Character* clone() const = 0;
            
            /**
             * withinAttackRange: checks if dest is within attack range of the Character at src 
             *
             * @param src - a reference to GridPoint object holding the coordinates of the source tile
             * @param dest - a reference to GridPoint object holding the coordinates of the destination tile
             *
             * @return - true if the destination tile is within the Character's attack range and false otherwise.
            */
            virtual bool withinAttackRange(const GridPoint& src, const GridPoint& dest) const;
            /**
             * withinMoveRange: checks if dest is within movement range of the Character at src 
             *
             * @param src - a reference to GridPoint object holding the coordinates of the source tile
             * @param dest - a reference to GridPoint object holding the coordinates of the destination tile
             *
             * @return - true if the destination tile is within the Character's movement range and false otherwise.
            */
            virtual bool withinMoveRange(const GridPoint& src, const  GridPoint& dest) const;

            /** attack: attacks the target destination tile, according to each Character limitations and abilities.
             *          the attack costs ammo equal to the Characters attack_cost value.
             * @param src - a reference to GridPoint object holding the coordinates of the source tile
             * @param dest - a reference to GridPoint object holding the coordinates of the destination tile
             * @param board - a reference to Board (a vector of vectors of shared_ptr) on which the game is played.
             * 
             * @return - void.
             */ 
            virtual void attack(const GridPoint& src, const GridPoint& dest, Board& board) = 0;

            /** 
             * reload: gives the current Character ammo, the amount of ammo is different for each Character and is 
             *          equal to the reload_amount value.
             * @return - void.
            */
            virtual void reload();

            /** isAlive: checks if the health of current Character is 0 or less
             * @return - true if the Character is alive (health>0), false otherwise.
             */
            bool isAlive() const;

            /** getTeam: returns the team current Character is a part of
             * @return - Team::POWERLIFTERS or Team:CROSSFITTERS.
             */
            Team getTeam() const;

            /** takeDamage: changes the health of current Character by <damage>
             * @param damage - the amount of health to be added/subtracted from the Character's health
             * @return - void
            */
            void takeDamage(int damage);  // should be protected?
            
            /** getCharacterSymbol: returns a symbol which represents the current 
             *                      Character as according to instructions 
             * @return - S/s for powerlifter/crossfitere Soldier
             *           M/m for powerlifter/crossfitere Medic
             *           N/n for powerlifter/crossfitere Sniper
            */
            virtual char getCharacterSymbol() const = 0;
    };
}
#endif