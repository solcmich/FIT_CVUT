//
// Created by solcmich on 5/24/19.
//

#ifndef PACMAN_GHOST_H
#define PACMAN_GHOST_H


#include "../Map/MapPart.h"
#include "../Gameplay/GameBehaviour.h"
#include <queue>
#include "../Map/Movable.h"
#include <vector>
#include "../Gameplay/Game.h"
#include "../Gameplay/Settings.h"
#include "../Gameplay/Constants.h"
#include "../Player/Player.h"

/*!
 * Base class for all ghosts, handles movement when given target. Also controls the lifecycle of each ghost.
 */
class Ghost : public Movable
{
public:
    Ghost(int x, int y, char visual, Map * map, Game * game, int speed = 1);
    /*!
     * BFS search to reach the destination.
     * @param destination to reach
     * @return path in queue.
     */
    std::queue<MapPart*> GetPath(MapPart * destination);
    Settings::Mode GetMode() { return m_currentMode; }
    /*!
     * Sets current behaviour and changes appropriate attributes
     * @param b to change to
     */
    void SetMode(Settings::Mode b);
    /*!
     * Sends the ghost home.
     */
    void Reset();
    /*!
     * Determines next ghost steps based on current mode. My fantasy about naming is gone, sorry.
     * @param home Ghost's home corner
     * @param ghostRelatedTarget is the current ghost target based on his behaviour.
     */
    void SuperMegaAIMethodMakingThisGameSoExcelent(MapPart * home, MapPart * ghostRelatedTarget);
    /*!
     * Makes the ghost move
     */
     void Release() {m_released = true;};
    /*!
     * Every ghost has to override this method
     * @return target of derived ghost based on his character
     */
    virtual MapPart * GetTarget() = 0;
    /*!
     * Search for possible next steps in the Pac-Man like manner - can be described as short sighted algorithm
     * @return next possible steps while keeping in mind that ghosts can't change direction
     */
    virtual std::vector<MapPart*> GetPossibleNextSteps() const;
    /*!
     * Determines next step with use of short sighted algorithm.
     * @param target Final target to reach
     * @return next steps in queue format.
     */
    std::queue<MapPart*>  DetermineNextStep(MapPart * target);
    /*!
     * Makes ghost revert his current movement direction.
     */
    void RevertDirection();

    void CheckLifeCycle();

    void MoveToTarget(MapPart * target);

    ~Ghost() override = default;
protected:
    Map * m_map;
    std::queue<MapPart*> m_currentPath;
    Settings::Mode m_currentMode;
    Settings::Behaviour m_currentBehaviour;
    bool m_released = false;
    int m_frigtenedTimer;
    int m_cycleStep;
};

#endif //PACMAN_GHOST_H
