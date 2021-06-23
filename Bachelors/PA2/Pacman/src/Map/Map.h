//
// Created by solcmich on 5/24/19.
//

#ifndef PACMAN_MAP_H
#define PACMAN_MAP_H


#include <string>
#include "Item.h"
#include "../Gameplay/GameBehaviour.h"
#include  <list>
#include  <vector>
#include "../Gameplay/Game.h"
#include <map>
#include "Teleport.h"
class Player;
class Ghost;

/*!
 * Controlls the map object, loads from file and maintain its visual apperance.
 */
class Map : public GameBehaviour
{
public:
    Map(Game * game, std::string path) : m_game(game), m_path(path) {};

    /*!
     * Initializes new map, loads from file etc
     * @return is initialization was succesfull
     */
    bool Init();
    /*!
     *
     * @return If the rules for map satisfied
     */
    bool IsValidMap();
    void LateUpdate() override;

    /*!
     * Sets all map parts as not visited, used for BFS here
     */
    void ResetForPathFinding();

    /*!
     * Called from movable when is changes it's position
     * @param x pos
     * @param y pos
     * @param visual what stepped in
     */
    void TileStep(int x, int y, char visual) const { m_parts[x][y]->OnStep(visual);};

    /*!
     * Called from movable when is changes it's position
     * @param x
     * @param y
     */
    void TileLeft(int x, int y) const { m_parts[x][y]->OnLeft(); };

    /*!
     *
     * @return visual reprezentation of the map
     */
    std::string ToString() const {  return m_visual; };

    MapPart * GetPart(int x, int y) const {return m_parts[x][y];};
    Player * GetPlayer() const { return m_player; }
    int GetSizeX() const { return m_SizeX; };
    int GetSizeY() const { return m_SizeY; };
    std::vector<Ghost*> GetGhosts() const { return m_ghosts; };
    Ghost * GetBlinky() const { return m_blinky; };
    MapPart * GetLeftTop() const { return m_leftTop; }
    MapPart * GetRightTop() const { return m_rightTop; }
    MapPart * GetLeftBottom() const { return m_leftBottom; }
    MapPart * GetRightBottom() const { return m_rightBottom; }
    int CoinsCount() const {return m_totalCoins;}

    ~Map();
private:
    /*!
     * Fits the char into your map matrix
     * @param c character to parse
     * @param x position X
     * @param y position Y
     * @return parsed char
     */
    bool ParseChar(char c, int x, int y, MapPart ** part);
    /*!
     * Loads map from file and parses it into the final matrix
     * @param path source file
     */
    bool ParseMapFromFile(const std::string & path);

    //Holds lot's of useful information
    std::string m_visual;
    std::vector<std::vector<MapPart*>> m_parts;
    Player * m_player;

    std::vector<Ghost*> m_ghosts;
    Ghost * m_blinky;
    Game * m_game;

    MapPart * m_leftTop;
    MapPart * m_rightTop;
    MapPart * m_leftBottom;
    MapPart * m_rightBottom;

    std::map<char, Teleport*> m_teleportsMap;
    std::vector<Teleport*> m_teleports;

    int m_totalCoins;

    int m_SizeX;
    int m_SizeY;
    std::string m_path;
};


#endif //PACMAN_MAP_H
