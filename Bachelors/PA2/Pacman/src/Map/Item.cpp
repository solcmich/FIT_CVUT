//
// Created by solcmich on 5/24/19.
//

#include "Item.h"
#include "../Gameplay/Level.h"
#include "../Gameplay/Constants.h"

void Item::OnStep(char visual)
{
    MapPart::OnStep(visual);

    if(visual == PLAYER_VISUAL)
        Pick();
}

void Item::Pick()
{
    if(m_isBonus)
    {
         m_game->SetMode(Settings::Mode::Frightened);
         m_isBonus = false;
    }else if(m_score > 0)
         m_game->GetLevel()->PickCoin();

    m_game->GetLevel()->IncreaseScore(m_score);
    m_score = 0;
}
