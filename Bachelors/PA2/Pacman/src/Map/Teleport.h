//
// Created by solcmich on 5/25/19.
//

#ifndef PACMAN_TELEPORT_H
#define PACMAN_TELEPORT_H


#include "MapPart.h"

/*!
 * Handles teleports logic
 */
class Teleport : public MapPart{
public:
    Teleport(int x, int y, char visual, Map * map) : MapPart(x, y, visual),  m_teleport(nullptr) , m_map(map){}
    /*!
     * Overriden base method of on step, calls base method and set's players position based on teleports type.
     * @param visual what stepped on this tile
     */
    void OnStep(char visual) override;
    /*!
     * Adds the second tp to current tp
     * @param tp to add
     */
    void AddTeleport(MapPart * tp) {m_teleport = tp;};
    MapPart * GetTeleport(){return m_teleport; }
    ~Teleport() override = default;
private:
    ///To teleport to.
    MapPart * m_teleport;
    Map * m_map;
};

#endif //PACMAN_TELEPORT_H
