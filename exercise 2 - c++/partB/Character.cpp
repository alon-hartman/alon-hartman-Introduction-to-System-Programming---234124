#include "Character.h"

using namespace mtm;
using namespace std;

Character::Character(Team team, int health, int ammo, int attack_range, int power, int movement_range, 
                        int attack_cost, int reload_amount) : 
    health(health), ammo(ammo), attack_range(attack_range), power(power), team(team), movement_range(movement_range),
    attack_cost(attack_cost), reload_amount(reload_amount)
{   }

Character::~Character() {  }

bool Character::isAlive() const
{
    return health > 0;
}

Team Character::getTeam() const
{
    return team;
}

void Character::takeDamage(int damage)
{
    health -= damage;
}

bool Character::withinAttackRange(const GridPoint& src, const GridPoint& dest) const
{
    if(GridPoint::distance(src, dest) > attack_range) {
        return false;
    }
    return true;
}

bool Character::withinMoveRange(const GridPoint& src,const  GridPoint& dest) const
{
    if(GridPoint::distance(src, dest) > movement_range) {
        return false;
    }
    return true;
}

void Character::reload()
{
    ammo += reload_amount;
}