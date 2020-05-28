//
// Created by solcmich on 5/24/19.
//
#ifndef PACMAN_GAME_H
#define PACMAN_GAME_H


#include <vector>
#include "GameBehaviour.h"
#include <string>
#include "Settings.h"
class Level;
class IOManager;

/*!
 * Class controlling the lifecycle of the game.
 */
class Game

{
public:
    explicit Game(std::string levelsPath) : m_levelsPath(levelsPath), m_pause(0), m_won(false) {}

    void Init();
    void Play();
    /**
    * Registers any object that need to maintain it's state during game is playing (Player, map, ghosts..)
    * @param toReg
    */
    void Register(GameBehaviour * toReg);
    /*!
     * Choose leven from given source directory
     * @param levelsPath
     * @return level folder name
     */
    std::string ChooseLevel(const std::string & levelsPath);
    /*!
     * When player interacts with ghost
     * @param final is final death (player lives == 0)
     */
    void OnDeath(bool final);

    /*!
     * Sets current game / ghost mode.
     * @param mode to set on
     */
    void SetMode(const Settings::Mode & mode);

    IOManager * GetIOManager() const {return m_io;}
    Level * GetLevel() const {return m_currentLevel;}

    ~Game();
private:
    /*!
     * Checks if the game was not paused
     */
    void CheckPause();
    /*!
     * Checks if the game was not paused by player
     */
    void CheckPlayerPause();
    /*!
     * Checks if the player won
     * @return
     */
    bool CheckVictory();
    ///All members to maintain during game
    std::vector<GameBehaviour*> m_updatable;

    Level * m_currentLevel;
    IOManager * m_io;
    bool m_playing;
    std::string m_levelsPath;
    int m_pause;
    bool m_won;
};
#endif //PACMAN_GAME_H
