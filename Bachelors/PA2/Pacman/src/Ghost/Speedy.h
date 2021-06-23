//
// Created by solcmich on 5/24/19.
//

#ifndef PACMAN_SPEEDY_H
#define PACMAN_SPEEDY_H

#include "Ghost.h"

class Speedy : public Ghost
{
public:
    Speedy(int x, int y, char visual, Map * m, Game * g, int speed = 1): Ghost(x, y, visual, m, g, speed) {};
    void Update() override;
    /*!
     * Speedy aims 4 tiles in front of pacman in his direction of travel
     * @return 0-4 tiles in front of pacman travel direction based on which of those is walkable, the closer to pacman, the lower priority is has.
     */
    MapPart * GetTarget() override;
    ~Speedy() override = default;
};

#endif //PACMAN_SPEEDY_H
