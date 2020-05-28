//
// Created by solcmich on 5/24/19.
//

#ifndef PACMAN_IOMANAGER_H
#define PACMAN_IOMANAGER_H


#include <ctime>    // For time()
#include <cstdlib>
#include <queue>
#include "../Map/Movable.h"
#include <ncurses.h>

/*!
 * Handles input and output operations with help of ncurses
 */
class IOManager
{
public:
    IOManager();
    char LastPressedKey() const { return  m_lastPressedKey; }
    /*!
     * Clears last pressed key
     */
    void Reset() {m_lastPressedKey = ' ';}

  /*!
   * Prints current game state with some aditional info
   * @param g game to print
   * @param s aditional info
   */
    void Print(Game * g, std::string s = "") const;
    /*!
     * Prints text on a console
     * @param s to print
     */
    void Print(std::string s) const;

    /*!
     * Cepture currently pressed key if there is any.
     */
    void CaptureInput();
    /*!
     * Loops until character is hit
     * @param c character to hit
     */
    void WaitForCharacter(char c);
    /*!
     * Loops until character in range is hit
     * @param asciiMin
     * @param asciiMax
     */
    void WaitForCharacter(char asciiMin, char asciiMax);

    ~IOManager();
private:
    char m_lastPressedKey;
    WINDOW * m_mainwin;
    Map * m_map;
};
#endif //PACMAN_IOMANAGER_H
