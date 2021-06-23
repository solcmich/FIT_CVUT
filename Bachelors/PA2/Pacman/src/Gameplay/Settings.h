//
// Created by solcmich on 5/24/19.
//

#ifndef PACMAN_SETTINGS_H
#define PACMAN_SETTINGS_H

#include <string>
/*!
 * Holds settings for the game, intialized in level class.
 */
class Settings
{
public:
    enum Mode {Frightened, Home, Chase, Scatter};

    enum Behaviour{Smart, Normal};

    static int ghostSpeed;
    static int playerSpeed;
    static int playerLives;
    static Behaviour ghostBehaviour;
    static int scatterModeDur;
    static int chaseModeDur;
    static int frightenedDur;
    static int frightenedSpeed;

};
#endif //PACMAN_SETTINGS_H
