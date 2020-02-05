/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Bonus
*/

#include "Bonus.hpp"

Bonus::Bonus(irr::video::IVideoDriver *driver, bool random, Bonus::Type type)
{
    this->blocking = false;
    this->type = Item::Type::BONUS;
    bonusType = (random) ? Bonus::Type(rand() % COUNT) : type;
    this->driver = driver;
}

Bonus::~Bonus()
{
}

Bonus::Type Bonus::getType()
{
    return bonusType;
}

void Bonus::createNode(irr::core::vector3df pos, irr::scene::ISceneManager *smgr)
{
    if (!node)
    {
        node = smgr->addCubeSceneNode(0.5, 0, -1, pos);
        node->setMaterialFlag(video::EMF_LIGHTING, false);
        node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
        node->setDebugDataVisible(scene::EDS_OFF);
        node->addAnimator(smgr->createFlyStraightAnimator(pos, irr::core::vector3df(pos.X, pos.Y + 0.2, pos.Z), 1000, true, true));
        node->addAnimator(smgr->createRotationAnimator(irr::core::vector3df(0, 1, 0)));

        irr::scene::ILightSceneNode *light = smgr->addLightSceneNode(node, core::vector3df(0, 0, 0),
                                                                     video::SColorf(1.f, 1.f, 1.f), 1.5f);
        light->getLightData().Attenuation = irr::core::vector3df(1.0 / 1.5, 1.0 / 1.5, 1.0 / 1.5);
    }
    if (node)
    {
        switch (bonusType)
        {
        case Bonus::Type::BOMB_UP:
            node->setMaterialTexture(0, driver->getTexture(ASSETS_BONUS "bomb_up.png"));
            break;
        case Bonus::Type::RADIUS_UP:
            node->setMaterialTexture(0, driver->getTexture(ASSETS_BONUS "radius_up.png"));
            break;
        case Bonus::Type::SPEED_UP:
            node->setMaterialTexture(0, driver->getTexture(ASSETS_BONUS "speed_up.png"));
            break;
        case Bonus::Type::BOMB_PASS:
            node->setMaterialTexture(0, driver->getTexture(ASSETS_BONUS "bomb_pass.png"));
            break;
        case Bonus::Type::WALL_PASS:
            node->setMaterialTexture(0, driver->getTexture(ASSETS_BONUS "wall_pass.png"));
            break;
        default:
            break;
        }
    }
}

void Bonus::destroy(irr::scene::ISceneManager *smgr)
{
    if (node)
        smgr->addToDeletionQueue(node);
}