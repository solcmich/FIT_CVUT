//
// Created by solcmich on 5/24/19.
//

#include "Blinky.h"

void Blinky::Update()
{
    if (!m_released)
        return;

    SuperMegaAIMethodMakingThisGameSoExcelent(m_map->GetLeftTop(), GetTarget());
}

MapPart * Blinky::GetTarget()
{
    return m_map->GetPart(m_map->GetPlayer()->GetX(), m_map->GetPlayer()->GetY());
}

