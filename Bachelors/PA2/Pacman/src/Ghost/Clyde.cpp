//
// Created by solcmich on 5/24/19.
//

#include "Clyde.h"
#include <math.h>


void Clyde::Update()
{
    if (!m_released)
        return;

    SuperMegaAIMethodMakingThisGameSoExcelent(m_map->GetLeftBottom(), GetTarget());
}

MapPart * Clyde::GetTarget()
{
    int distance = sqrt(pow(m_map->GetPlayer()->GetX() - m_x, 2) + pow(m_map->GetPlayer()->GetY() - m_y, 2));
    if (distance > 8)
    {
        return m_map->GetPart(m_map->GetPlayer()->GetX(), m_map->GetPlayer()->GetY());
    }
    return m_map->GetLeftBottom();
}
