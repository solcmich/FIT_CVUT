//
// Created by solcmich on 5/24/19.
//

#ifndef PACMAN_ITEM_H
#define PACMAN_ITEM_H


#include "MapPart.h"
#include "../Gameplay/Game.h"

/*!
 * All pickables in game.
 */
class Item : public MapPart
{
public:
    Item(int X, int Y, char visual, int score, Game * game, bool isBonus = false) : MapPart(X, Y, visual), m_score(score), m_game(game), m_isBonus(isBonus) {};
    void OnStep(char visual) override;
    /*!
     * Notifies game about picked item.
     */
    void Pick();
protected:
    int m_score;
    Game * m_game;
    bool m_isBonus;
};

#endif //PACMAN_ITEM_H
