/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Player
*/

#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "./include/irrlicht.h"
#include "./Items/Bomb.hpp"
#include "./MyEventReceiver.hpp"
#include <iostream>

enum moveDir
{
    DOWN,
    LEFT,
    UP,
    RIGHT,
    NONE
};

class Player
{
public:
    Player(irr::IrrlichtDevice *device);
    ~Player();
    irr::scene::ISceneNode *drawPlayer(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver *driver);
    void die();
    void setMap(BomberMap *map);
    void setColor(irr::video::SColorf color);
    void setPos(irr::core::vector2df npos);
    void setId(int id);
    Bomb *actions(const MyEventReceiver receiver, int bombs);
    void checkDir(moveDir direction);
    void move();
    int getId();
    irr::core::vector2di getPos();
    irr::scene::ISceneNode *getNode();

    std::string name;
    int bomberPoints;
    int *skins;
    bool dead;
protected:
private:
    void useBonus(Bonus *bonus);
    int id;
    float moveSpd;
    irr::core::vector2df pos;
    irr::video::SColorf color;
    irr::scene::IAnimatedMeshSceneNode* node;
    irr::scene::ILightSceneNode *bulb;
    BomberMap *map;
    bool running;

    uint maxBomb;
    Bomb bomb;
    moveDir dir;
    bool wall_pass;
};

#endif /* !PLAYER_HPP_ */
