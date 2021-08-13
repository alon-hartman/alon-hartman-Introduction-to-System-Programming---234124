#ifndef SNIPER_H
#define SNIPER_H

#include "Character.h"
#include "Auxiliaries.h"
#include <vector>
#include <memory>

namespace mtm {
    const int sniper_movement_range = 4;
    const int sniper_attack_cost = 1;
    const int sniper_reload_amount = 2;
    class Sniper : public Character
    {
        private:
            int attacks_fired;
        public:
            /** Sniper constructor: same as Character constructor but initializes the movement_range, attack_cost
             *                      and reload_amount to the values which are suitable to the Sniper.
            */
            Sniper(Team team, int health, int ammo, int range, int power, int movement_range=sniper_movement_range,
                    int attack_cost=sniper_attack_cost, int reload_amount=sniper_reload_amount);
            /** Sniper copy constructor: creates a deep copy of a given Sniper.
            */
            Sniper(const Sniper& other);
            // destructor
            ~Sniper();

            Sniper* clone() const override;

            /** withinAttackRange: a cell is within attack range of the Sniper if the distance 
             *                     between the sniper and the cell is at least half his attack 
             *                     range (rounded up) and at most his attack range.  
            */
            bool withinAttackRange(const GridPoint& src,const GridPoint& dest) const override;

            /** attack: attacks the target at the destination tile and deals damage equal to his power. 
             *          every third attack deals damage equal to twice his power.
             * @throws OutOfRange - if the destination tile isn't within attack range of the Sniper.
             * @throws OutOfAmmo - if the Sniper doesn't have enough ammo to attack (ammo < attack_cost)
             * @throws IllegalTarget - if the Sniper tries to attack allies or an empty tiles.
             
             * @returns - void.
             */
            void attack(const GridPoint& src, const GridPoint& dest, Board& board) override;
            // void reload() override;

            // returns N for Powerlifter Sniper and n for Crossfitter Sniper.
            char getCharacterSymbol() const override;
    };
}

#endif