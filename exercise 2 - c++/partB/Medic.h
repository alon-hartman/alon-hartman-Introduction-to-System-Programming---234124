#ifndef MEDIC_H
#define MEDIC_H

#include "Character.h"
#include "Auxiliaries.h"
#include <vector>
#include <memory>

namespace mtm {
    const int medic_movement_range = 5;
    const int medic_attack_cost = 1;
    const int medic_reload_amount = 5;
    class Medic : public Character
    {
        private:
        public:
            /** Medic constructor: same as Character constructor but initializes the movement_range, attack_cost
             *                      and reload_amount to the values which are suitable to the medic.
            */
            Medic(Team team, int health, int ammo, int range, int power, int movement_range=medic_movement_range,
                    int attack_cost=medic_attack_cost, int reload_amount=medic_reload_amount);
            /** Medic copy constructor: creates a deep copy of a given Medic.
            */
            Medic(const Medic& other);
            // destructor
            ~Medic();

            Medic* clone() const override;

            /** attack: if the target at the destination tile is an enemy, deals damage equal to the medic's power
             *          to the enemy. if the target is an ally, heals him for an amount equal to the medic's power.
             * @throws OutOfRange - if the destination tile isn't within attack range of the Medic.
             * @throws OutOfAmmo - if the Medic tries to attack a tile which does not have an ally and does not 
             *                      have enough ammo to attack (ammo < attack_cost).
             * @throws IllegalTarget - if the medic tries to attack himself or an empty tiles.
             
             * @returns - void.
             */
            virtual void attack(const GridPoint& src, const GridPoint& dest, Board& board) override;

            // void reload() override;

            // returns M if affiliated with Powerlifters and m otherwise.
            char getCharacterSymbol() const override;        
    };
}

#endif