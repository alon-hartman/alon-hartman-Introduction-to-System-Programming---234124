#include "Medic.h"

using namespace std;
using namespace mtm;

Medic::Medic(Team team, int health, int ammo, int range, int power, 
             int movement_range, int attack_cost, int reload_amount) : 
    Character(team, health, ammo, range, power, movement_range, attack_cost, reload_amount) {   }
Medic::Medic(const Medic& other) : 
    Character(other.team, other.health, other.ammo, other.attack_range, 
              other.power, other.movement_range, other.attack_cost, other.reload_amount) {  }
Medic::~Medic() {   }

Medic* Medic::clone() const
{
    Medic* new_medic = new Medic(*this);
    return new_medic;
}   

void Medic::attack(const GridPoint& src, const GridPoint& dest, Board& board)
{
    if(withinAttackRange(src, dest) == false) {
        throw OutOfRange();
    }
    shared_ptr<Character> target = board[dest.row][dest.col];
    if(((target != nullptr && target->getTeam() != team) || (target == nullptr)) && ammo < attack_cost) {
        throw OutOfAmmo();
    }
    if(src == dest || target == nullptr) {
        throw IllegalTarget();
    }
    if(target->getTeam() == team) {
        target->takeDamage(-power);
    } else {
        target->takeDamage(power);
        this->ammo -= attack_cost;
        if(target->isAlive() == false) {
            target = nullptr;
            board[dest.row][dest.col] = nullptr;
        }
    }
}

char Medic::getCharacterSymbol() const
{
    return (team == POWERLIFTERS ?  'M' : 'm');
}