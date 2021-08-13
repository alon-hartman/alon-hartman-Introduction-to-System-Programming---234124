#include "Game.h"
#include "Auxiliaries.h"
#include "Character.h"
#include <vector>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <cstring>

using namespace std;
using namespace mtm;

Game::Game(int height, int width) : height(height), width(width)
{
    if(height <=0 || width <= 0) {
        throw IllegalArgument();
    }
    for(int i=0; i<height; i++) {
        vector<shared_ptr<Character>> temp;
        for(int j=0; j<width; j++) {
            temp.push_back(shared_ptr<Character>(nullptr));
        }
        this->board.push_back(temp);
    }
}

Game::Game(const Game& other) : height(other.height), width(other.width)
{
    for(int i=0; i < height; i++) {
        vector<shared_ptr<Character>> temp;
        for(int j=0; j < width; j++) {
            if(other.board[i][j] != nullptr) {
                Character* copy = other.board[i][j]->clone();
                temp.push_back(shared_ptr<Character>(copy));
            }
            else {
                temp.push_back(shared_ptr<Character>(nullptr));
            }
        }
        this->board.push_back(temp);
    }
}

bool Game::isOutOfArray(const GridPoint& coordinates) const
{
    if(coordinates.row >= height || coordinates.row < 0 || coordinates.col >= width || coordinates.col < 0)
    {
        return true;
    }
    return false;
}

void Game::move(const GridPoint& src_coordinates, const GridPoint& dst_coordinates)
{
    if(isOutOfArray(src_coordinates) || isOutOfArray(dst_coordinates)) {
        throw IllegalCell();
    }
    if(board[src_coordinates.row][src_coordinates.col] == nullptr) {
        throw CellEmpty();
    }

    if(board[src_coordinates.row][src_coordinates.col]->withinMoveRange(src_coordinates, dst_coordinates) == false) {
        throw MoveTooFar();
    }
    if(!(this->board[dst_coordinates.row][dst_coordinates.col] == nullptr)) {
        throw CellOccupied();
    }
    this->board[dst_coordinates.row][dst_coordinates.col] = board[src_coordinates.row][src_coordinates.col];
    this->board[src_coordinates.row][src_coordinates.col] = nullptr;
}

Game& Game::operator=(const Game& other)
{
    if(&other == this || &other == nullptr)
    {
        return *this;
    }
    board.clear();
    this->height = other.height;
    this->width = other.width;
    for(int i=0; i < height; i++) {
        vector<shared_ptr<Character>> temp;
        for(int j=0; j < width; j++) {
            Character* copy = other.board[i][j]->clone();
            temp.push_back(shared_ptr<Character>(copy));
        }
        this->board.push_back(temp);
    }
    return *this;
}

shared_ptr<Character> Game::makeCharacter(CharacterType type, Team team, 
                                            units_t health, units_t ammo, units_t range, units_t power)
{
    if(health <= 0 || ammo < 0 || range < 0 || power < 0)
    {
        throw IllegalArgument();
    }
    if(CharacterType::SOLDIER == type)
    {
        return shared_ptr<Character>(new Soldier(team, health, ammo, range, power));
    }
    else if(CharacterType::MEDIC == type)
    {
        return shared_ptr<Character>(new Medic(team, health, ammo, range, power));    
    }
    else  //(CharacterType::SNIPER == type)
    {
        return shared_ptr<Character>(new Sniper(team, health, ammo, range, power));    
    }
}

void Game::addCharacter(const GridPoint& coordinates, shared_ptr<Character> Character)
{
    if(isOutOfArray(coordinates))
    {
        throw IllegalCell();
    }
    if(board[coordinates.row][coordinates.col] != nullptr)
    {
        throw CellOccupied();
    }
    board[coordinates.row][coordinates.col] = Character;
}

void Game::reload(const GridPoint& coordinates)
{
    if(isOutOfArray(coordinates))// coordinates.row > height || coordinates.row < 0 || coordinates.col > width || coordinates.col < 0)
    {
        throw IllegalCell();
    }
    if(board[coordinates.row][coordinates.col] == nullptr)
    {
        throw CellEmpty();
    }
    board[coordinates.row][coordinates.col]->reload();
}

bool Game::isOver(Team* winningTeam) const
{
    int crossfitters_alive = 0, powerlifters_alive = 0;
    bool write_to_ptr = !(winningTeam == nullptr);
    for(vector<shared_ptr<Character>> vec : board) {
        for(shared_ptr<Character> character : vec) {
            if(character != nullptr && character->isAlive() == true) {
                if(character->getTeam() == Team::CROSSFITTERS) {
                    crossfitters_alive++;
                    continue;
                }
                powerlifters_alive++;
            }
        }
    }
    if((crossfitters_alive == 0 && powerlifters_alive == 0) || (crossfitters_alive > 0 && powerlifters_alive > 0)) {
        return false;
    }
    if(crossfitters_alive == 0) {
        if(write_to_ptr && winningTeam != NULL) {
            *winningTeam = Team::POWERLIFTERS;
        }
    }
    if(powerlifters_alive == 0) {
        if(write_to_ptr && winningTeam != NULL) {
            *winningTeam = Team::CROSSFITTERS;
        }
    }
    return true;   
}

void Game::attack(const GridPoint& src_coordinates, const GridPoint& dst_coordinates)
{
    if(isOutOfArray(src_coordinates) || isOutOfArray(dst_coordinates)) {
        throw IllegalCell();
    }
    if(board[src_coordinates.row][src_coordinates.col] == nullptr) {
        throw CellEmpty();
    }
    board[src_coordinates.row][src_coordinates.col]->attack(src_coordinates, dst_coordinates, board);
}



ostream& operator<<(ostream& stream, const Game& game)
{
    int height = game.board.size();
    int width = game.board[0].size();
    string str;
    str.reserve(width * height);
    // for(vector<shared_ptr<Character>> vec : game.board)
    for(int i=0; i<height; ++i)
    {
        // for(shared_ptr<Character> character : vec)
        for(int j=0; j<width; ++j)
        {
            if(game.board[i][j] != nullptr) {
                str.push_back(game.board[i][j]->getCharacterSymbol());
            } else {
                str.push_back(' ');
            }
        }     
    }
    printGameBoard(stream, &str[0], &(str.back()) + 1, width);
    return stream;
}




