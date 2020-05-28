//
// Created by solcmich on 5/25/19.
//

#include "Teleport.h"
#include "../Player/Player.h"
#include "../Gameplay/Constants.h"

void Teleport::OnStep(char visual)
{
    if(visual == PLAYER_VISUAL)
    {
        if(m_teleport->GetX() == 0 && m_teleport->GetY() > 0)//Down->Up
            m_map->GetPlayer()->SetPos(m_X, m_Y, m_X - m_teleport->GetX() - 1, m_Y - m_teleport->GetY(), Player::Dir::Down);
        else if(m_X == 0 && m_teleport->GetY() > 0) //Up->Down
            m_map->GetPlayer()->SetPos(m_X, m_Y, m_X - m_teleport->GetX() + 1, m_Y - m_teleport->GetY(), Player::Dir::Up);
        else if(m_Y == 0 && m_teleport->GetX() > 0) //Left->Right
            m_map->GetPlayer()->SetPos(m_X, m_Y, m_X - m_teleport->GetX(), m_Y - m_teleport->GetY() + 1, Player::Dir::Left);
        else if(m_teleport->GetY() == 0 && m_teleport->GetX() > 0) //Right->Left
            m_map->GetPlayer()->SetPos(m_X, m_Y, m_X - m_teleport->GetX(), m_Y - m_teleport->GetY() - 1, Player::Dir::Right);
    }
    else
        MapPart::OnStep(visual);
}