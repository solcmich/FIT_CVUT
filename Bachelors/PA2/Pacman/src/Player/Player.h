//
// Created by solcmich on 5/24/19.
//

#ifndef PACMAN_PLAYER_H
#define PACMAN_PLAYER_H


#include "../Helpers/IOManager.h"
#include "../Gameplay/GameBehaviour.h"
#include "../Map/MapPart.h"
#include "../Map/Movable.h"
#include "../Gameplay/Game.h"

/*!
 * Controls player, handles IO operations for player and ghost interaction logic
 */
class Player : public Movable
{
public:
    Player(Game * game, Map * map, int x, int y, IOManager * IOMan, char visual, int speed = 1);

    void LateUpdate() override;

    /*!
     * Not you, but the PacMan.
     * @return true if he died definitely
     */
    bool Die();

    /*!
     * Handles interaction with ghost
     * @param g ghost to interact with
     */
    void OnGhostStep(Ghost * g);

    Dir GetFacingDir() const { return m_lastDir; };
    int GetLives() const {return m_lives;}

    ~Player() override = default;
private:
    /*!
    * Decides next direction based on position and last pressed key
    * @param c wanted dir.
    * @return true if we can go "c" direction
    */
    bool DecideDir(Dir c);

    Map * m_map;
    IOManager * m_IO;
    int m_lives;
};
#endif //PACMAN_PLAYER_H