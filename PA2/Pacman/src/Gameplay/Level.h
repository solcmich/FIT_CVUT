//
// Created by solcmich on 5/24/19.
//

#ifndef PACMAN_LEVEL_H
#define PACMAN_LEVEL_H


#include <string>
#include "../Map/Map.h"
#include "Game.h"

/*!
 * Class holding the level and appropriate attributes
 */
class Level
{
public:
    Level(Game * game, const std::string &mapPath) : m_game(game), m_path(mapPath) {}

    bool Init();
    int GetScore() const {return m_score;};
    int GetPickedCoins() const {return m_coinsPicked;}
    Map * GetMap() const {return m_map;}

    void IncreaseScore(const int & value) { m_score += value;};
    void PickCoin(){m_coinsPicked++;}
    ~Level();

private:
    Map * m_map;
    int m_score;
    int m_coinsPicked;
    Game * m_game;
    std::string m_path;
};
#endif //PACMAN_LEVEL_H
