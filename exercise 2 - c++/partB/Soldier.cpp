#include "Soldier.h"
#include "Auxiliaries.h"
#include <vector>
#include <memory>
#include "Exceptions.h"

using namespace mtm;
using namespace std;

Soldier::Soldier(Team team, int health, int ammo, int range, int power, int movement_range, int attack_cost,
                    int reload_amount) : 
                Character(team, health, ammo, range, power, movement_range, attack_cost, reload_amount) {   }
Soldier::Soldier(const Soldier& other) : 
    Character(other.team, other.health, other.ammo, other.attack_range, 
              other.power, other.movement_range, other.attack_cost, other.reload_amount) {  }
Soldier::~Soldier() {    }

Soldier* Soldier::clone() const
{
    Soldier* new_soldier = new Soldier(*this);
    return new_soldier;
}

void Soldier::attack(const GridPoint& src, const GridPoint& dest, Board& board)
{
    // out of range
    if(withinAttackRange(src, dest) == false) {
        throw OutOfRange();
    }
    // out of ammo
    if(this->ammo < attack_cost)
    {
        throw OutOfAmmo();
    }
    /** TODO: put in independant function? */
    if(src.col != dest.col && src.row != dest.row) {  // not in the same row or col
        throw IllegalTarget();
    }
    
    GridPoint cell(0,0);
    for(vector<shared_ptr<Character>> vec : board)
    {
        for(shared_ptr<Character> character : vec)
        {
            if(character != nullptr && character->getTeam() != team &&
                    GridPoint::distance(cell, dest) <= ceil(attack_range/3.0f))
            {
                if(cell == dest) {
                    character->takeDamage(this->power);
                } else {
                    character->takeDamage(ceil(power/2.0));
                }
                if(character->isAlive() == false) {
                    character == nullptr;  // shared_ptr should delete character.
                    board[cell.row][cell.col] = nullptr;  // just in case
                }
            }
            cell.col++;
        }
        cell.col = 0;
        cell.row++;
    }
    ammo -= attack_cost;
}

char Soldier::getCharacterSymbol() const
{
    return (team == POWERLIFTERS ?  'S' : 's');
}