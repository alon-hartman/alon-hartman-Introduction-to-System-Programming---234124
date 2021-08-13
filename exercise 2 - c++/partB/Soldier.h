#ifndef SOLDIER_H
#define SOLDIER_H

#include "Auxiliaries.h"
#include "Character.h"
#include <memory>

namespace mtm {  
    const int soldier_movement_range = 3;
    const int soldier_attack_cost = 1;
    const int soldier_reload_amount = 3;
    class Soldier : public Character
    {
        public:
            /** Soldier constructor: same as Character constructor but initializes the movement_range, attack_cost
             *                      and reload_amount to the values which are suitable to the Soldier.
            */
            Soldier(Team team, int health, int ammo, int range, int power, int movement_range=soldier_movement_range,
                    int attack_cost=soldier_attack_cost, int reload_amount=soldier_reload_amount);
            /** Soldier copy constructor: creates a deep copy of a given Soldier.
            */
            Soldier(const Soldier& other);
            // destructor
            ~Soldier();

            Soldier* clone() const override;

            /** attack: if the destination tile is valid, then attacks the given destination tile and deals
             *          damage equal to his power to the destination cell and damage equal to half of his power
             *          (rounded up) to cells within one third of his attack range (rounded up).
             * @throws OutOfRange - if the destination tile isn't within attack range of the Soldier.
             * @throws OutOfAmmo - if the Soldier doesn't have enough ammo to attack (ammo < attack_cost)
             * @throws IllegalTarget - if the Soldier tries to attack tiles which are not in the same column or row
             *                          as himself
             
             * @returns - void.
             */
            void attack(const GridPoint& src, const GridPoint& dest, Board& board) override;

            // returns S if Soldier is in Powerlifters team and s otherwise
            char getCharacterSymbol() const override;
    };
}

#endif