//
// Created by solcmich on 5/24/19.
//
#include "Game.h"
#include <chrono>
#include <thread>
#include "Level.h"
#include "Constants.h"
#include "../Helpers/IOManager.h"
#include "../Ghost/Ghost.h"
#include "../Helpers/FileManager.h"

int frame = 0; //Counting frames..


void Game::Init()
{
    m_io = new IOManager();
    m_io->Print("WELCOME SUMMONER!\n"
                "Are you ready to play the game?\n"
                "Press \"x\" to start\n");

    m_io->WaitForCharacter('x');

    m_playing = true;

    std::string levelName = ChooseLevel(m_levelsPath);

    if(!m_playing)
    {
        m_io->Print("No level to choose");
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        return;
    }
    m_currentLevel = new Level(this, m_levelsPath +  "/" + levelName);
    if(!m_currentLevel->Init())
    {
        m_io->Print("Game loading failed");
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        m_playing = false;
    }

}
void Game::OnDeath(bool final)
{
    if(final)
    {
        m_playing = false;
        return;
    }
    for (size_t i = 0; i <m_currentLevel->GetMap()->GetGhosts().size(); ++i)
        m_currentLevel->GetMap()->GetGhosts()[i]->Reset();

    m_pause = 30;
}

std::string Game::ChooseLevel(const std::string & levelsPath)
{
    std::vector<std::string> files;
    std::string print = "You can choose from following levels:\n";

    if(FileManager::ListFiles(levelsPath, files))
    {
        for (size_t i = 2; i < files.size(); ++i) //Ignore "." and ".." folder
            print += std::to_string(i-1) + ". " + files[i] + "\n";
    }else
    {
        m_io->Print("Invalid directory");
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        m_playing = false;
        return "";
    }

    m_io->Print(print + "Enter number of desired level\n");

    m_io->WaitForCharacter(49, files.size() + 46);

    int i = m_io->LastPressedKey() - 47; //From ASCII to int
    return files[i];
}

void Game::SetMode(const Settings::Mode & mode)
{
   std::vector<Ghost*> ghosts = m_currentLevel->GetMap()->GetGhosts();
   for (size_t i = 0; i < ghosts.size(); ++i)
       ghosts[i]->SetMode(mode);
}

void Game::Play()
{
    while (m_playing)
    {
        auto start = std::chrono::high_resolution_clock::now();

        CheckPause();

        if(CheckVictory())
        {
            m_playing = false;
        }

        m_io->CaptureInput();

        CheckPlayerPause();

        if (m_io->LastPressedKey() == 'q')
            break;

        for (size_t i = 0; i < m_updatable.size(); i++)
            if (frame % m_updatable[i]->GetSpeed() == 0)
                m_updatable[i]->Update();


        for (size_t i = 0; i < m_updatable.size(); i++)
            if (frame % m_updatable[i]->GetSpeed() == 0)
                m_updatable[i]->LateUpdate();


        m_io->Print(this, " || Press \"p\" to PAUSE or \"q\" to QUIT");

        auto end = std::chrono::high_resolution_clock::now();
        auto dur = end - start;

        //Smooth it
        int elapsedFrame = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
        if (elapsedFrame > REFRESH_RATE)
            elapsedFrame = REFRESH_RATE;

        std::this_thread::sleep_for(std::chrono::milliseconds((REFRESH_RATE - elapsedFrame)));

        frame++;
    }
}

void Game::CheckPause()
{
    while(m_pause > 0)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds((REFRESH_RATE)));
        m_pause--;
    }
}
void Game::CheckPlayerPause()
{
    if(m_io->LastPressedKey() == 'p')
    {
        m_io->Print("PAUSED, press \"r\" to refresh ");
        m_io->WaitForCharacter('r');
    }
}
bool Game::CheckVictory()
{
    int score = m_currentLevel->GetPickedCoins();
    int finalScore = m_currentLevel->GetMap()->CoinsCount();

    if(score == finalScore)
    {
        m_won = true;
        return true;
    }
    return false;
}

void Game::Register(GameBehaviour * toReg)
{
    m_updatable.push_back(toReg);
}

Game::~Game()
{
    if(m_won)
        m_io->Print("YOU WON WITHOUT CAUSING A SEGFAULT, WOW");
    else
        m_io->Print("YOU LOST TRY AGAIN LATER");

    std::this_thread::sleep_for(std::chrono::milliseconds(4000));

    m_io->Print("Bye!");
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    if(m_currentLevel != nullptr)
        delete m_currentLevel;
    if(m_io != nullptr)
        delete m_io;
}