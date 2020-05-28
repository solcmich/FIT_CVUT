//
// Created by solcmich on 5/24/19.
//

#include "Speedy.h"

void Speedy::Update()
{
    if (!m_released)
        return;

    SuperMegaAIMethodMakingThisGameSoExcelent(m_map->GetRightBottom(), GetTarget());
}

MapPart * Speedy::GetTarget()
{
    //4 Tiles in direction of player's travel
    int x = m_map->GetPlayer()->GetX();
    int y = m_map->GetPlayer()->GetY();
    Player * p = m_map->GetPlayer();

    if (p->GetFacingDir() == Player::Dir::Up)
    {
        for (int i = 3; i >= 0; i--)
            if (m_map->GetSizeX() > (x + i) && m_map->GetPart(x + i, y)->IsWalkable())
            {
                x += i;
                break;
            }
    }

    else if (p->GetFacingDir() == Player::Dir::Right)
    {
        for (int i = 3; i >= 0; i--)
            if (m_map->GetSizeY() > y + i && m_map->GetPart(x, y + i)->IsWalkable())
            {
                y += i;
                break;
            }

    }
    else if (p->GetFacingDir() == Player::Dir::Down)
    {
        for (int i = 3; i >= 0; i--)
            if (x - i >= 0 && m_map->GetPart(x - i, y)->IsWalkable())
            {
                x -= i;
                break;
            }
    }
    else /*if(p->GetFacingDir() == Player::Dir::Left)*/{
        for (int i = 3; i >= 0; i--)
            if (y - i >= 0 && m_map->GetPart(x, y - i)->IsWalkable())
            {
                y -= i;
                break;
            }
    }


    return m_map->GetPart(x, y);
}
