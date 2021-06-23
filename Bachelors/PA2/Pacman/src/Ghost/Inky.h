//
// Created by solcmich on 5/24/19.
//

#ifndef PACMAN_INKY_H
#define PACMAN_INKY_H

#include "Ghost.h"


class Inky : public Ghost
{
public:
    Inky(int x, int y, char visual, Map * m, Game * g, int speed = 1) : Ghost(x, y, visual, m, g, speed) {};
    void Update() override;
    /*!
     * Inky's aim position is calculated based on vector from Blinky to Player, the vector is
     * doubled and that is inky's target
     * @return
     */
    MapPart * GetTarget() override;
    ~Inky() override = default;
};


#endif //PACMAN_INKY_H
