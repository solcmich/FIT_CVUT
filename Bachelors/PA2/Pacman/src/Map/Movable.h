//
// Created by solcmich on 5/24/19.
//

#ifndef PACMAN_MOVABLE_H
#define PACMAN_MOVABLE_H

#include "../Gameplay/GameBehaviour.h"
#include "Map.h"


/*!
 * Base class for every movement capable object
 */
class Movable : public GameBehaviour
{
public:
    enum Dir { Up, Right, Down, Left, None };
    Movable(Map * map, Game * game, uint x, uint y, char visual, int speed = 1);

    int GetX() const { return m_x; }
    int GetY() const { return m_y; }
    /*!
     * Calculates direction of travel from delta of coordinates
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     * @return Direction of travel
     */
     Dir GetDirByDelta(int x1, int y1, int x2, int y2) const;

     /*!
      * Sets postion of movable object
      * @param x
      * @param y
      * @param offsetX
      * @param offsetY
      * @param overrideDir don't use calculation by delta to set direction, used for example with teleports.
      */
    void SetPos(int x, int y, int offsetX, int offsetY, Dir overrideDir = Dir::None);

    ~Movable() = default;
protected:
    int m_x;
    int m_y;
    int m_originX;
    int m_originY;

    char m_visual;
    char m_originVisual;

    Map * m_map;
    Game * m_game;
    Dir m_lastDir;
};


#endif //PACMAN_MOVABLE_H
