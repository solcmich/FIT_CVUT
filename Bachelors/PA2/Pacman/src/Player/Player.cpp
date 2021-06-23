//
// Created by solcmich on 5/24/19.
//

#include "Player.h"
#include <iostream>
#include <cstdlib>
#include "../Ghost/Ghost.h"
#include "../Gameplay/Level.h"

Player::Player(Game * game, Map * map, int x, int y, IOManager * IOMan, char visual, int speed) : Movable(map, game, x, y, visual, speed), m_map(map)
{
    m_IO = IOMan;
    m_lastDir = Dir::Up;
    m_lives = Settings::playerLives;
}

bool Player::DecideDir(Dir c)
{
    if (c == Dir::Left && m_map->GetPart(m_x, m_y - 1)->IsWalkable())
    {
        SetPos(m_x, m_y, 0, 1);
        return true;
    }
    else if (c == Dir::Up && m_map->GetPart(m_x - 1, m_y)->IsWalkable())
    {
        SetPos(m_x, m_y, 1, 0);
        return true;
    }
    else if (c == Dir::Right && m_map->GetPart(m_x, m_y + 1)->IsWalkable())
    {
        SetPos(m_x, m_y, 0, -1);
        return true;
    }
    else if (c == Dir::Down && m_map->GetPart(m_x + 1, m_y)->IsWalkable())
    {
        SetPos(m_x, m_y, -1, 0);
        return true;
    }
    return false;
}

void  Player::OnGhostStep(Ghost * g)
{
    if(g->GetMode() == Settings::Mode::Frightened)
    {
        g->SetMode(Settings::Mode::Home);
         m_game->GetLevel()->IncreaseScore(200);
    }
    else if(g->GetMode() != Settings::Mode::Home)
    {
       Die();
    }


    //Otherwise met them on their way home, do not disturb them!
}

void Player::LateUpdate()
{
    Movable::Dir dir = Movable::Dir::None;
    char c = m_IO->LastPressedKey();
    if(c == 'w')
        dir = Movable::Dir::Up;
    else if(c == 'a')
        dir = Movable::Dir::Left;
    else if(c == 'd')
        dir = Movable::Dir::Right;
    else if(c == 's')
        dir = Movable::Dir::Down;

    if (!DecideDir(dir))
    	DecideDir(m_lastDir);
}

bool Player::Die()
{
    m_game->OnDeath(--m_lives == 0);
    return m_lives == 0;
}

