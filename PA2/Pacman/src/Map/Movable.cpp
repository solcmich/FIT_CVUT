//
// Created by solcmich on 5/24/19.
//

#include "Movable.h"

class InvalidMoveType {};

Movable::Movable(Map * map, Game * game, uint x, uint y, char visual, int speed) : GameBehaviour(speed), m_x(x), m_y(y), m_originX(x), m_originY(y) , m_visual(visual),m_originVisual(visual),
m_map(map), m_game(game)

{
    m_game->Register(this);
}

void Movable::SetPos(int x, int y, int offsetX, int offsetY, Dir overrideDir )
{
    m_x = x - offsetX;
    m_y = y - offsetY;

    m_map->TileStep(m_x, m_y, m_visual);
    m_map->TileLeft(x, y);

    if (overrideDir != Dir::None)
        m_lastDir = overrideDir;
    else
        m_lastDir = GetDirByDelta(x, y, m_x, m_y);
}

Movable::Dir Movable::GetDirByDelta(int x1, int y1, int x2, int y2) const
{
    if (x1 - x2 > 0)
        return Movable::Dir::Up;
    else if (x1 - x2 < 0)
        return Movable::Dir::Down;
    else if (y1 - y2 < 0)
        return Movable::Dir::Right;
    else if (y1 - y2 > 0)
        return Movable::Dir::Left;
    else if (y1 - y2 == 0 && x1 - x2 == 0)
        //Nothing to do here
        return m_lastDir;
    else //Can't happen?
        throw new InvalidMoveType;
}


