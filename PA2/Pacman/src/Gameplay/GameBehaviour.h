//
// Created by solcmich on 5/24/19.
//

#ifndef PACMAN_GAMEBEHAVIOUR_H
#define PACMAN_GAMEBEHAVIOUR_H
/*!
 * Base class for everything which needs to move in this game.
 */
class GameBehaviour
{
public:
    explicit GameBehaviour(int speed = 1) : m_speed(speed) {};
    /*!
     * Let's use Update design pattern
     */
    virtual void Update() { };
    /*!
     * Late update is called after all update functions were called, used for example for rendering
     */
    virtual void LateUpdate() { };

    int GetSpeed() const { return m_speed; }
    virtual ~GameBehaviour() = default;
protected:
    int m_speed;
};
#endif //PACMAN_GAMEBEHAVIOUR_H
