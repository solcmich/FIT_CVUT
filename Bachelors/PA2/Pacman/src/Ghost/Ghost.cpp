//
// Created by solcmich on 5/24/19.
//
#include "Ghost.h"
#include <queue>
#include <math.h>
#include <limits.h>

Ghost::Ghost(int x, int y, char visual, Map * map, Game * game, int speed) : Movable(map, game, x, y, visual, speed), m_map(map)
{
    m_released = true;
    m_cycleStep = 0;
    m_currentBehaviour = Settings::ghostBehaviour;
    m_currentMode = Settings::Mode::Chase;
    m_lastDir=Movable::Dir::None;
}

void Ghost::SetMode(Settings::Mode b)
{
    if(m_currentMode != b)
    {
        RevertDirection();
        m_currentMode = b;
    }

    if(b == Settings::Mode::Frightened)
    {
        m_visual = 'X';
        m_speed = Settings::frightenedSpeed;
        m_frigtenedTimer = Settings::frightenedDur;
    }
    else if(b == Settings::Mode::Home)
    {
        if(Settings::ghostSpeed > 1)
            m_speed = Settings::ghostSpeed / 2;
        else
            m_speed = 1;

        m_visual = 'D';
    }
    else
    {
        m_visual = m_originVisual;
        m_speed = Settings::ghostSpeed;
    }
}

void Ghost::RevertDirection()
{
    if(m_lastDir == Movable::Dir::Left)
        m_lastDir = Movable::Dir::Right;
    else if(m_lastDir == Movable::Dir::Up)
        m_lastDir = Movable::Dir::Down;
    else if(m_lastDir == Movable::Dir::Down)
        m_lastDir = Movable::Dir::Up;
    else if(m_lastDir == Movable::Dir::Right)
        m_lastDir = Movable::Dir::Left;
}

void Ghost::Reset()
{
    SetMode(Settings::Mode::Home);
    m_speed = 1;
}


void Ghost::SuperMegaAIMethodMakingThisGameSoExcelent(MapPart * homeCorner, MapPart * ghostRelatedTarget)
{
    if (!m_released)
        return;

    if (m_currentBehaviour == Settings::Behaviour::Normal)
    {
        if(m_currentMode == Settings::Mode::Chase)
            MoveToTarget(ghostRelatedTarget);
        else if(m_currentMode != Settings::Mode::Home) //Frightened / Scatter
            MoveToTarget(homeCorner);
        else
            m_currentPath = GetPath(m_map->GetPart(m_originX, m_originY)); //Always use smart algorithm on way home
    }

    CheckLifeCycle();

    //Perform movement
    if (!m_currentPath.empty())
    {
        MapPart * p = m_currentPath.front();
        SetPos(m_x, m_y, m_x - p->GetX(), m_y - p->GetY());
        m_currentPath.pop();
    }
}

void Ghost::MoveToTarget(MapPart * target)
{
    if (m_currentBehaviour == Settings::Behaviour::Normal)
        m_currentPath = DetermineNextStep(target);
    else if(m_currentPath.empty())
        m_currentPath = GetPath(target);
}

void Ghost::CheckLifeCycle()
{
    if(m_currentMode == Settings::Mode::Frightened)
    {
        if(m_frigtenedTimer <= Settings::frightenedDur / 3) //Notify player about soon frightened exit
        {
            if(m_frigtenedTimer % 2 == 0)
                m_visual = ' ';
            else
                m_visual = 'X';
        }
        if(--m_frigtenedTimer == 0)
            SetMode(Settings::Mode::Chase);
    }else if(m_currentMode == Settings::Mode::Home)
    {
        if (m_currentPath.empty()) //Reached our goal
            SetMode(Settings::Mode::Chase);
    }else if(m_currentMode == Settings::Mode::Scatter)
    {
        if(m_cycleStep % Settings::scatterModeDur == 0)
        {
            SetMode(Settings::Mode::Chase);
            m_cycleStep = 0;
        }
    }
    else //Chase
    {
        if(m_cycleStep % Settings::chaseModeDur == 0)
        {
            SetMode(Settings::Mode::Scatter);
            m_cycleStep = 0;
        }
    }

    m_cycleStep++;
}

std::queue<MapPart*> Ghost::GetPath(MapPart * destination)
{
    m_map->ResetForPathFinding();
    std::queue<std::queue<MapPart * >> q;
    std::queue<MapPart* > currPath;
    MapPart * source = m_map->GetPart(m_x, m_y);

    currPath.push(source);
    q.push(currPath);

    while (!q.empty())
    {
        currPath = q.front();
        q.pop();

        MapPart * last = currPath.back();

        if (last->GetX() == destination->GetX() && last->GetY() == destination->GetY())
        {
            if (!currPath.empty())
                currPath.pop();

            return currPath;
        }
        for (size_t i = 0; i < last->GetNeighbors().size(); i++)
        {
            if (!last->GetNeighbors()[i]->GetVisited())
            {
                std::queue <MapPart * > newPath(currPath);
                newPath.push(last->GetNeighbors()[i]);
                q.push(newPath);
                last->GetNeighbors()[i]->SetVisited(true);
            }
        }
    }
    return std::queue<MapPart*>();
}


std::vector<MapPart*> Ghost::GetPossibleNextSteps() const
{
    std::vector<MapPart*> ret;

    if (m_lastDir != Movable::Dir::Left && m_map->GetSizeY() > m_y + 1 && m_map->GetPart(m_x, m_y + 1)->IsWalkable())
        ret.push_back(m_map->GetPart(m_x, m_y + 1));

    if (m_lastDir != Movable::Dir::Right && m_y - 1 >= 0 && m_map->GetPart(m_x, m_y - 1)->IsWalkable())
        ret.push_back(m_map->GetPart(m_x, m_y - 1));

    if (m_lastDir != Movable::Dir::Up && m_map->GetSizeX() > m_x + 1 && m_map->GetPart(m_x + 1, m_y)->IsWalkable())
        ret.push_back(m_map->GetPart(m_x + 1, m_y));

    if (m_lastDir != Movable::Dir::Down && m_x - 1 >= 0 && m_map->GetPart(m_x - 1, m_y)->IsWalkable())
        ret.push_back(m_map->GetPart(m_x - 1, m_y));

    return ret;
}

std::queue<MapPart*> Ghost::DetermineNextStep(MapPart * target)
{
    std::vector<MapPart*> nextPossibleSteps = GetPossibleNextSteps();

    if (nextPossibleSteps.empty())
        return std::queue<MapPart*>();

    MapPart * nextStep = nextPossibleSteps[0];

    int distanceMin = 99999999;    //Never will be bigger

    for (size_t i = 0; i < nextPossibleSteps.size(); i++)
    {
        int distance = sqrt(pow(nextPossibleSteps[i]->GetX() - target->GetX(), 2) + pow(nextPossibleSteps[i]->GetY() - target->GetY(), 2));
        if (distanceMin >= distance)
        {
            distanceMin = distance;
            nextStep = nextPossibleSteps[i];
        }
    }
    std::queue<MapPart*> path;
    path.push(nextStep);
    return path;
}


