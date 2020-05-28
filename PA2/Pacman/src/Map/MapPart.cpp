//
// Created by solcmich on 5/24/19.
//

#include "MapPart.h"
#include "../Gameplay/Constants.h"

MapPart::MapPart(int initialX, int initialY, char visual, bool walkable) :
        m_X(initialX),
        m_Y(initialY),
        m_visual(visual),
        m_walkable(walkable),
        m_origin(visual)
{
    m_visuals.push(' ');
}

void MapPart::AddNeighbor(MapPart * p)
{
    if (!p->IsWalkable() || !m_walkable)
        return;

    m_neighbors.push_back(p);
    p->AddBackNeighbor(this);
}

void MapPart::AddBackNeighbor(MapPart * p)
{
    m_neighbors.push_back(p);
}

void MapPart::OnStep(char visual)
{
    if (visual == PLAYER_VISUAL) //Player picks everything - if it's a ghost, then ghost interaction logic will be used.
        m_origin = ' ';

    m_visual = visual;
}

void MapPart::OnLeft()
{
    if (m_origin == PLAYER_VISUAL ||
        m_origin == CLYDE_VISUAL  || //Covers the first step of movables
        m_origin == SPEEDY_VISUAL ||
        m_origin == BLINKY_VISUAL ||
        m_origin == INKY_VISUAL)
        m_origin = ' ';

    m_visual = m_origin;
}
