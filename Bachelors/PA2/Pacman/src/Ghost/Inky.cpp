//
// Created by solcmich on 5/24/19.
//

#include "Inky.h"
#include "Blinky.h"
#include <iostream>
class Blinky;

void Inky::Update()
{
    if (!m_released)
        return;

    SuperMegaAIMethodMakingThisGameSoExcelent(m_map->GetRightTop(), GetTarget());
}


MapPart * Inky::GetTarget()
{
    Ghost * b = m_map->GetBlinky();

    if(b == nullptr) //Without blinky, aim for player
        return m_map->GetPart(m_map->GetPlayer()->GetX(), m_map->GetPlayer()->GetY());

    //Get doubled vector from blinky to player
    int blinkyTargetX = b->GetTarget()->GetX();
    int blinkyTargetY = b->GetTarget()->GetY();
    int blinkyX = b->GetX();
    int blinkyY = b->GetY();
    int targetX = 2 * blinkyTargetX - blinkyX;
    int targetY = 2 * blinkyTargetY - blinkyY;

    //Hold direction of the vector for future use
    bool xFlag = (blinkyX - blinkyTargetX) > 0; //Left dir
    bool yFlag = (blinkyY - blinkyTargetY) > 0; //Up dir

    //Fit the extended vector into map size
    while (targetX >= m_map->GetSizeX())
        targetX--;
    while (targetY >= m_map->GetSizeY())
        targetY--;
    while (targetX < 0)
        targetX++;
    while (targetY < 0)
        targetY++;

    //Find first walkable on the vector.
    while (!m_map->GetPart(targetX, targetY)->IsWalkable())
    {
        if((blinkyX - blinkyTargetX) != 0)
            targetX += xFlag ?  1 : -1;
        if((blinkyY - blinkyTargetY) != 0)
            targetY += yFlag ?  1 : -1;

        if(targetX >= m_map->GetSizeX() || targetX < 0 || targetY >= m_map->GetSizeY() || targetY < 0) //No walkable found
        {
            targetX = m_map->GetPlayer()->GetX();
            targetY = m_map->GetPlayer()->GetY();
        }
    }

    return m_map->GetPart(targetX, targetY);
}