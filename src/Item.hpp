/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Item
*/

#ifndef ITEM_HPP_
#define ITEM_HPP_

#include "./include/irrlicht.h"
#include "./Textures.hpp"

class Item
{
public:
    enum Type
    {
        ITEM,
        BOMB,
        BONUS
    };
    Item();
    ~Item();

    void setId(const int id);
    void setOwnerId(const int ownerId);
    void setBlocking(const bool blocking);

    const int getId() const;
    const int getOwnerId() const;
    const bool isBlocking() const;
    const Item::Type getType() const;
    const scene::ISceneNode *getNode() const;

    virtual void createNode(irr::core::vector3df pos, irr::scene::ISceneManager *smgr) = 0;
    virtual void destroy(irr::scene::ISceneManager *smgr) = 0;

    std::string meshPath;
    scene::IMesh *mesh;
    scene::IMeshSceneNode *node;

    irr::core::vector2d<int> pos;
protected:
    int id;
    int owner_id;
    bool blocking;
    Item::Type type;
};

#endif /* !ITEM_HPP_ */
