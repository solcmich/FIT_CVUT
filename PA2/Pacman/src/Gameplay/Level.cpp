//
// Created by solcmich on 5/24/19.
//

#include "Level.h"
#include <string>
#include <fstream>

//Init settings here
int Settings::playerSpeed = 5;
Settings::Behaviour Settings::ghostBehaviour = Settings::Behaviour::Normal;
int Settings::ghostSpeed = 20;
int Settings::playerLives = 3;
int Settings::scatterModeDur = 5;
int Settings::chaseModeDur = 10;
int Settings::frightenedDur = 30;
int Settings::frightenedSpeed = 10;


bool IsNum(const std::string & s)
{
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
        return false;

    char * c;
    strtol(s.c_str(), &c, 10);

    return (*c == 0);
}

bool Level::Init()
{
    std::string line;
    std::ifstream file(m_path + "/Config.txt");

    if (file.is_open()) {
        while (std::getline(file, line))
        {
            //Config is in format "KEY" "VALUE"
            if(line.find(' ') != std::string::npos)
            {
                std::string key = line.substr(0, line.find(' '));
                std::string val = line.substr(line.find(' ') + 1, line.size());
                int numVal;
                if(key == "PLAYER_SPEED")
                {
                    if(IsNum(val))
                    {
                        numVal = std::atoi(val.c_str());
                            if(numVal > 0)
                                Settings::playerSpeed = numVal;
                    }
                }
                else if(key == "GHOST_SPEED")
                {
                    if(IsNum(val))
                    {
                        numVal = std::atoi(val.c_str());
                        if(numVal > 0)
                            Settings::ghostSpeed = numVal;
                    }
                }
                else if(key == "PLAYER_LIVES")
                {
                    if(IsNum(val))
                    {
                        numVal = std::atoi(val.c_str());

                        if(numVal > 0)
                            Settings::playerLives = numVal;
                    }
                }
                else if(key == "GHOST_MODE")
                {
                    if(val == "Normal")
                        Settings::ghostBehaviour = Settings::Behaviour::Normal;
                    else if(val == "Smart")
                        Settings::ghostBehaviour = Settings::Behaviour::Smart;
                }else if(key == "SCATTER_MODE_DUR")
                {
                    if(IsNum(val))
                    {
                        numVal = std::atoi(val.c_str());
                        if(numVal > 0)
                            Settings::scatterModeDur = numVal;
                    }
                }
                else if(key == "CHASE_MODE_DUR")
                {
                    if(IsNum(val))
                    {
                        numVal = std::atoi(val.c_str());
                        if(numVal > 0)
                            Settings::chaseModeDur = numVal;
                    }
                }
                else if (key == "FRIGHTENED_MODE_DUR")
                {
                    if(IsNum(val))
                    {
                        numVal = std::atoi(val.c_str());
                        if(numVal > 0)
                            Settings::frightenedDur = numVal;
                    }
                }
                else if (key == "FRIGHTENED_SPEED")
                {
                    if(IsNum(val))
                    {
                        numVal = std::atoi(val.c_str());
                        if(numVal > 0)
                            Settings::frightenedSpeed = numVal;
                    }
                }

            }
        }
    }
    m_score = 0;
    m_coinsPicked = 0;
    //Init level map.
    m_map = new Map(m_game, m_path + "/Map.txt");
    return m_map->Init();
}



Level::~Level()
{
    if(m_map != nullptr)
        delete m_map;
}

