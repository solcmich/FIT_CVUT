//
// Created by solcmich on 5/24/19.
//

#ifndef PACMAN_BLINKY_H
#define PACMAN_BLINKY_H


#include "Ghost.h"

class Blinky : public Ghost
{
public:
    Blinky(int x, int y, char visual, Map * m, Game * g, int speed = 1) : Ghost(x, y, visual, m, g, speed) {};
    void Update() override;

    /*!
     * Blinky is simply aiming for player
     * @return player's position
     */
    MapPart * GetTarget() override;

    ~Blinky() override = default;
};



#endif //PACMAN_BLINKY_H
