#include "Sniper.h"
#include "Auxiliaries.h"
#include "Character.h"
#include <vector>
#include <memory>
#include "Exceptions.h"

using namespace mtm;
using namespace std;

Sniper::Sniper(Team team, int health, int ammo, int range, int power, int movement_range,
        int attack_cost, int reload_amount) :
        Character(team, health, ammo, range, power, movement_range, attack_cost, reload_amount) , attacks_fired(0) {}

Sniper::Sniper(const Sniper& other) : 
    Character(other.team, other.health, other.ammo, other.attack_range, other.power, other.movement_range,
              other.attack_cost, other.reload_amount), attacks_fired(other.attacks_fired) {  }

Sniper::~Sniper() {   }

Sniper* Sniper::clone() const 
{
    Sniper* new_sniper = new Sniper(*this);
    return new_sniper;
}

bool Sniper::withinAttackRange(const GridPoint& src,const GridPoint& dest) const
{
    if(GridPoint::distance(src, dest) > attack_range ||
             GridPoint::distance(src, dest) < ceil(attack_range/2.0f)) {  // not in attack range
        return false;
    }
    return true;
}

void Sniper::attack(const GridPoint& src, const GridPoint& dest, Board& board)
{
   if(this->withinAttackRange(src, dest) == false) {
        throw OutOfRange();
    } 
    if(this->ammo < attack_cost) {
        throw OutOfAmmo();
    }
    shared_ptr<Character> target = board[dest.row][dest.col];
    if(target == nullptr || target->getTeam() == this->team) {
        throw IllegalTarget();
    }
    attacks_fired++;
    if(attacks_fired % 3 == 0) {
        target->takeDamage(power*2);
    } else {
        target->takeDamage(power);
    }
    // CHECK IF TARGET LIVES
    if(target->isAlive() == false) {
        target = nullptr;
        board[dest.row][dest.col] = nullptr;
    }
    ammo -= attack_cost;
}

char Sniper::getCharacterSymbol() const
{
    return (team == POWERLIFTERS ?  'N' : 'n');
}
