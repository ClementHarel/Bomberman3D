/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Item
*/

#include "Item.hpp"

Item::Item()
{
    this->id = -1;
    this->owner_id = -1;
    this->blocking = false;
    this->type = Item::Type::ITEM;
    this->meshPath = "";
    this->mesh = 0;
    this->node = 0;
}

Item::~Item()
{
}

void Item::setId(const int id)
{
    this->id = id;
}

void Item::setOwnerId(const int ownerId)
{
    this->owner_id = ownerId;
}

void Item::setBlocking(const bool blocking)
{
    this->blocking = blocking;
}

const int Item::getId() const
{
    return (this->id);
}

const int Item::getOwnerId() const
{
    return this->owner_id;
}

const bool Item::isBlocking() const
{
    return this->blocking;
}

const Item::Type Item::getType() const
{
    return this->type;
}

const scene::ISceneNode *Item::getNode() const
{
    return this->node;
}

