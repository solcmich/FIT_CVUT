//
// Created by solcmich on 5/24/19.
//

#include "Map.h"
#include <string>
#include <fstream>
#include "../Gameplay/Settings.h"
#include <vector>
#include "../Gameplay/Constants.h"
#include "MapPart.h"
#include "Item.h"
#include "../Player/Player.h"
#include "../Ghost/Clyde.h"
#include "../Ghost/Blinky.h"
#include "../Ghost/Speedy.h"
#include "../Ghost/Inky.h"



bool Map::Init()
{
    m_game->Register(this);
    m_totalCoins = 0;
    m_leftBottom = nullptr;
    m_rightBottom = nullptr;
    m_rightTop = nullptr;
    m_leftTop = nullptr;
    m_player = nullptr;
    m_blinky = nullptr;
    if(ParseMapFromFile(m_path))
    {
        return IsValidMap();
    }
    return false;
}
bool Map::ParseMapFromFile(const std::string & path)
{
    std::string line;
    std::ifstream file(path);

    size_t width = 999999999999;
    if (file.is_open())
    {
        int lineCounter = 0;

        while (std::getline(file, line))
        {
            std::vector<MapPart*> matrixLine;

            if(width != 999999999999 && width != line.length()) //Rectangle shape
                return false;

            width = line.length();

            for (size_t i = 0; i < width; i++)
            {
                MapPart * m = nullptr;
                if(!ParseChar(line[i], lineCounter, i, &m))
                {
                    m_parts.emplace_back((matrixLine));
                    return false;
                }

                //Prepare for pathfinding
                if (i > 0)
                    m->AddNeighbor(matrixLine[i - 1]);
                if (lineCounter > 0)
                    m->AddNeighbor(m_parts[lineCounter - 1][i]);

                matrixLine.emplace_back(m);
            }
            m_parts.emplace_back(matrixLine);

            lineCounter++;
        }
        m_SizeX = m_parts.size();
        if (m_SizeX > 0)
            m_SizeY = m_parts[0].size();

        file.close();
    }
    return true;
}

bool Map::IsValidMap()
{
    if(m_SizeX == 0 || m_SizeY == 0)
    {
        return false;
    }

    if(m_leftTop == nullptr || m_leftBottom == nullptr || m_rightTop == nullptr || m_rightBottom == nullptr || m_player == nullptr)
    {
        return false;
    }
    for (size_t i = 0; i < m_teleports.size(); ++i) //Teleports in tuples
    {
        if(m_teleports[i]->GetTeleport() == nullptr)
            return false;
    }
    //Edges covered by valid characters
    for (size_t y = 0; y < m_parts.size(); y++)
    {
        for (size_t x = 0; x < m_parts[y].size(); x++)
        {
            if(y == 0 || x == 0 || y == m_parts.size() - 1 || x ==  m_parts[y].size() - 1)
            {
                if(m_parts[y][x]->GetVisual() != WALL2_VISUAL && m_parts[y][x]->GetVisual() != WALL_VISUAL && (m_parts[y][x]->GetVisual() <= 64 && m_parts[y][x]->GetVisual() >= 91))
                {
                    return false;
                }
            }
        }
    }
    return true;
}

bool Map::ParseChar(char c, int x, int y, MapPart ** part)
{
    MapPart * m;
    switch(c) {
        case PLAYER_VISUAL:
        {
            if(m_player != nullptr)
                return false;

            m = new MapPart(x, y, PLAYER_VISUAL);
            Player *p = new Player(m_game, this, x, y, m_game->GetIOManager(), PLAYER_VISUAL, Settings::playerSpeed);
            m_player = p;
            break;
        }
        case INKY_VISUAL:
        {
            m = new MapPart(x, y, INKY_VISUAL);
            Ghost * g = new Inky(x, y, INKY_VISUAL, this, m_game, Settings::ghostSpeed);
            m_ghosts.push_back(g);
            break;
        }case CLYDE_VISUAL:
        {
            m = new MapPart(x, y, CLYDE_VISUAL);
            Ghost * g = new Clyde(x, y, CLYDE_VISUAL, this, m_game, Settings::ghostSpeed);
            m_ghosts.push_back(g);
            break;
        }case BLINKY_VISUAL:
        {
            m = new MapPart(x, y, BLINKY_VISUAL);
            Ghost * g = new Blinky(x, y, BLINKY_VISUAL, this, m_game, Settings::ghostSpeed);
            m_ghosts.push_back(g);
            m_blinky = g;
            break;
        }case SPEEDY_VISUAL:
        {
            m = new MapPart(x, y, SPEEDY_VISUAL);
            Ghost * g = new Speedy(x, y, SPEEDY_VISUAL, this, m_game, Settings::ghostSpeed);
            m_ghosts.push_back(g);
            break;
        }case COIN_VISUAL:
        {
            m = new Item(x, y, COIN_VISUAL, 1, m_game);
            m_totalCoins++;
            break;
        }case TRESEN_VISUAL:
        {
            m = new Item(x, y, TRESEN_VISUAL, 10, m_game);
            m_totalCoins++;
            break;
        }case WALL_VISUAL:
        {
            m = new MapPart(x, y, WALL_VISUAL, false);
            break;
        }case WALL2_VISUAL:
        {
            m = new MapPart(x, y, '|', false);
            break;
        }
        case LEFT_BOTTOM_VISUAL:
        {
            m = new Item(x, y, COIN_VISUAL, 1, m_game);
            m_totalCoins++;
            m_leftBottom = m;
            break;
        }
        case RIGHT_BOTTOM_VISUAL:
        {
            m = new Item(x, y, COIN_VISUAL, 1, m_game);
            m_totalCoins++;
            m_rightBottom = m;
            break;
        }case LEFT_TOP_VISUAL:
        {
            m = new Item(x, y, COIN_VISUAL, 1, m_game);
            m_totalCoins++;
            m_leftTop = m;
            break;
        }case RIGHT_TOP_VISUAL:
        {
            m = new Item(x, y, COIN_VISUAL, 1, m_game);
            m_totalCoins++;
            m_rightTop = m;
            break;
        }case BONUS_VISUAL:
        {
            m = new Item(x, y, BONUS_VISUAL, 0, m_game, true);
            break;
        }
        default:
        {
            if(c > 64 && c< 91) //Teleport can be from A-Z
            {
                if(m_teleportsMap.find(c) != m_teleportsMap.end())
                {
                    Teleport * t = new Teleport(x, y, c, this);
                    m = t;

                    m_teleportsMap[c]->AddTeleport(t);
                    t->AddTeleport(m_teleportsMap[c]);
                    m_teleports.push_back(t);

                }else
                {
                    Teleport * t = new Teleport(x, y, c, this);
                    m = t;
                    m_teleportsMap.insert(std::pair<char, Teleport*>(c, t));
                    m_teleports.push_back(t);
                }
            }else if(c == ' ')
            {
                m = new MapPart(x, y,' ');
            }else
            {
                return false; //Invalid character
            }
        }
    }
    *part = m;
    return true;
}

void Map::LateUpdate()
{
    m_visual = "";
    for (size_t y = 0; y < m_parts.size(); y++)
    {
        for (size_t x = 0; x < m_parts[y].size(); x++)
            m_visual += m_parts[y][x]->GetVisual();
        m_visual += '\n';
    }

    //Ghost interaction
    for (size_t i = 0; i < m_ghosts.size(); i++)
        if (m_player->GetX() == m_ghosts[i]->GetX() && m_player->GetY() == m_ghosts[i]->GetY())
            m_player->OnGhostStep(m_ghosts[i]);
}

void Map::ResetForPathFinding()
{
    for (auto i : m_parts)
        for (auto x : i)
            x->SetVisited(false);
}

Map::~Map()
{
    for (size_t y = 0; y < m_parts.size(); y++)
        for (size_t x = 0; x < m_parts[y].size(); x++)
            delete m_parts[y][x];

    for (size_t i = 0; i < m_ghosts.size(); ++i)
        delete(m_ghosts[i]);

    if(m_player != nullptr)
        delete m_player;
}
