/*
** EPITECH PROJECT, 2019
** src
** File description:
** BomberMap
*/

#ifndef BOMBERMAP_HPP_
#define BOMBERMAP_HPP_

#include "../Items/Bonus.hpp"
#include <string>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <algorithm>

#define MAP_MIN_W 6
#define MAP_MIN_H 6
#define MAP_MAX_W 42
#define MAP_MAX_H 42
#define NIGHT_LIGHT 1.0

class Bloc
{
public:
    enum Types
    {
        EMPTY,
        BREAKABLE,
        UNBREAKABLE
    };
    Bloc(float _x = 0, float _y = 0, Bloc::Types _type = EMPTY);
    ~Bloc();

    // Setter
    void setType(Bloc::Types _type = EMPTY);
    // Getter
    const Bloc::Types getType();
    const core::vector2df getPos();
    int getId();
    std::vector<Item *> content;

    scene::ISceneNode *cube;

private:
    float x, y;
    Types type;
};

class BomberMap
{
public:
    enum Corner {
        TOP_LEFT,
        TOP_RIGHT,
        BOT_LEFT,
        BOT_RIGHT
    };
    BomberMap(scene::ISceneManager *smgr, video::IVideoDriver *driver, int width = MAP_MIN_W, int height = MAP_MIN_H, bool editMode = false);
    ~BomberMap();
    // draw the map on the scene
    void drawMap();
    void updateTexture();
    void saveMap(std::string map_name);
    bool loadMap(std::string map_name);
    void unloadMap();

    // Getter
    bool onCorner(int x, int y, BomberMap::Corner corner);
    Bloc *getBlocAt(const uint _x, const uint _y) const;
    video::ITexture *getFloorTexture() const;
    Textures<Bloc::Types>::Texture &getTextures() const;
    core::vector2di getSize() const;
    bool isFree(uint x, uint y, bool pass = false);
    std::vector<Item *> &getContent(uint x, uint y);
    irr::core::vector3df getRealPos(float x, float y);
    bool isNight() const;
    // Setter
    void addTexture(Bloc::Types type, std::string path);
    void setBlocAt(const uint _x, const uint _y, const Bloc::Types _type);
    void setFloorTexture(const std::string _texture);
    void setTexture(Bloc::Types type, const std::string _texture);
    void setNight(bool night);
    void resizeMap(uint width = 0, uint height = 0);
    bool addItemBlockAt(const uint _x, const uint _y, Item *item);
    void removeItemBlockAt(const uint _x, const uint _y, Item *item);

protected:
private:
    void opSetBlocAt(const uint _x, const uint _y, const Bloc::Types _type);
    void updateBlocDisplay(Bloc *bloc);

    Textures<Bloc::Types> textures;
    video::IVideoDriver *driver;
    scene::ISceneManager *smgr;
    uint w, h;
    irr::scene::ISceneNode *skybox;
    irr::scene::ILightSceneNode *lighting;
    std::vector<Bloc *> blocs;
    std::string floorPath;
    video::ITexture *floorTexture;
    scene::IMesh *floorMesh;
    scene::ISceneNode *floor;
    bool night;
    bool editMode;
};

#endif /* !BOMBERMAP_HPP_ */