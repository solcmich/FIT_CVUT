//
// Created by solcmich on 5/24/19.
//

#ifndef PACMAN_CLYDE_H
#define PACMAN_CLYDE_H


#include "Ghost.h"


class Clyde : public Ghost
{
public:
    Clyde(int x, int y, char visual, Map * m, Game * g, int speed = 1) : Ghost(x, y, visual, m, g, speed) {};
    void Update() override;
    /*!
     * If clyde is closer than 8 fields he aims for his home corner, otherwise aiming for player
     * @return
     */
    MapPart * GetTarget() override;
    ~Clyde() override = default;
};


#endif //PACMAN_CLYDE_H
