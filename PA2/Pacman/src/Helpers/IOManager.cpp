//
// Created by solcmich on 5/24/19.
//

#include "IOManager.h"
#include <string>
#include "../Gameplay/Level.h"
#include "../Player/Player.h"

IOManager::IOManager()
{

    if ((m_mainwin = initscr()) == NULL ) {
        fprintf(stderr, "Error initializing ncurses.\n");
        exit(EXIT_FAILURE);
    }
    nodelay(stdscr, TRUE);
    cbreak();
    noecho();
}



void IOManager::Print(Game * g, std::string toAdd) const
{
    std::string s = g->GetLevel()->GetMap()->ToString();
    int score = g->GetLevel()->GetScore();
    int finalScore = g->GetLevel()->GetMap()->CoinsCount();
    s+="Score: " + std::to_string(score) + " / " + std::to_string(finalScore);

    int lives = g->GetLevel()->GetMap()->GetPlayer()->GetLives();
    s+=" || Lives: " + std::to_string(lives);
    s+=toAdd;

    clear();
    printw(s.c_str());
    refresh();
}

void IOManager::Print(std::string s) const
{
    clear();
    printw(s.c_str());
    refresh();
}

void IOManager::WaitForCharacter(char c)
{
    Reset();
    while(LastPressedKey() != c) //Wait for input
            CaptureInput();
}

void IOManager::WaitForCharacter(char asciiMin, char asciiMax)
{
    Reset();
    while(LastPressedKey() < asciiMin || LastPressedKey() > asciiMax) //Wait for input
        CaptureInput();
}
void IOManager::CaptureInput()
{
    char ch;
    if ((ch = getch()) != ERR)
        m_lastPressedKey = ch;
}

IOManager::~IOManager()
{
    refresh();
    delwin(m_mainwin);
    endwin();
}