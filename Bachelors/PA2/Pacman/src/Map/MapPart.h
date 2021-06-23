//
// Created by solcmich on 5/24/19.
//

#ifndef PACMAN_MAPPART_H
#define PACMAN_MAPPART_H


#include <vector>
#include <queue>
class Map;
/*!
 * Represents one tile in map.
 */
class MapPart
{
public:
    MapPart(int x, int y, char visual, bool walkable = true);

    /*!
     * Adds neighbour for current part
     * @param p to add
     */
    void AddNeighbor(MapPart * p);
    /*!
     * Adds neighbour back to current map part
     * @param p to add
     */
    void AddBackNeighbor(MapPart * p);

    /*!
     * Mantain visual strcture of each tile, sets current visual on step
     * @param visual stepped in visual
     */
    virtual void OnStep(char visual);

    /*!
     * Mantain visual structure of each tile, sets current visual on left
     */
    virtual void OnLeft();


    char GetVisual() const { return m_visual; }
    bool IsWalkable() const { return m_walkable; }
    std::vector<MapPart*> GetNeighbors() const { return m_neighbors; }
    bool GetVisited() const { return m_visited; }
    int GetX() const { return m_X; }
    int GetY() const { return m_Y; }

    void SetVisited(bool val) { m_visited = val; }

    virtual ~MapPart() = default;
protected:
    int m_X;
    int m_Y;
    char m_visual;


private:
    bool m_walkable;
    char m_origin;
    bool m_visited;
    std::vector<MapPart*> m_neighbors;
    std::queue<char> m_visuals;

};

#endif //PACMAN_MAPPART_H
