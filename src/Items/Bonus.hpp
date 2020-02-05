/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Bonus
*/

#ifndef BONUS_HPP_
#define BONUS_HPP_

#include "../Item.hpp"

class Bonus : public Item
{
public:
    enum Type {
        BOMB_UP,
        RADIUS_UP,
        SPEED_UP,
        BOMB_PASS,
        WALL_PASS,
        COUNT
    };

    Bonus(irr::video::IVideoDriver* driver, bool random = true, Bonus::Type type = RADIUS_UP);
    ~Bonus();

    using Item::getId;
    using Item::getNode;
    using Item::getOwnerId;
    using Item::getType;
    using Item::isBlocking;
    using Item::setBlocking;
    using Item::setId;
    using Item::setOwnerId;

    Bonus::Type getType();
    void createNode(irr::core::vector3df pos, irr::scene::ISceneManager *smgr);
    void destroy(irr::scene::ISceneManager *smgr);

protected:
private:
    irr::video::IVideoDriver *driver;
    Bonus::Type bonusType;
};

#endif /* !BONUS_HPP_ */
