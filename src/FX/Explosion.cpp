/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Explosion
*/

#include "Explosion.hpp"

void Explosion::setLine(BomberMap *map, irr::core::vector2df pos, irr::core::vector2di dir, int radius, bool through)
{
    for (int i = 1; i <= radius; i++)
    {
        if (pos.X + dir.X * i <= 0 || pos.Y + dir.Y * i <= 0 ||
            pos.X + dir.X * i >= map->getSize().X - 1 || pos.Y + dir.Y * i >= map->getSize().Y - 1 ||
            (!through && map->getBlocAt(pos.X + dir.X * i, pos.Y + dir.Y * i)->getType() == Bloc::Types::UNBREAKABLE))
            break;
        putExplosion(map, pos.X + dir.X * i, pos.Y + dir.Y * i);
    }
}

void Explosion::putExplosion(BomberMap *map, int x, int y)
{
    irr::core::vector3df pos = map->getRealPos(x, y);
    pos.Y = 0.5;
    irr::scene::ILightSceneNode *light = smgr->addLightSceneNode(0, pos,
                                                                 video::SColorf(1.0f, 0.6f, 0.7f), 1.5f);
    light->getLightData().Attenuation = irr::core::vector3df(1.0 / 1.5, 1.0 / 1.5, 1.0 / 1.5);

    scene::IParticleSystemSceneNode *ps =
        smgr->addParticleSystemSceneNode(false, light);

    scene::IParticleEmitter *em = ps->createSphereEmitter(
        core::vector3df(0, 0, 0),
        0.25,
        core::vector3df(0, 0.002, 0),
        100, 1000,
        video::SColor(255, 200, 0, 0), // darkest color
        video::SColor(255, 255, 170, 0),
        50, 100, 360,
        core::dimension2df(0.05f, 0.05f), // min size
        core::dimension2df(0.05f, 0.05f));

    ps->setEmitter(em); // this grabs the emitter
    em->drop();         // so we can drop it here without deleting it

    scene::IParticleAffector *paf = ps->createFadeOutParticleAffector(irr::video::SColor(255, 100, 20, 0));
    ps->addAffector(paf); // same goes for the affector
    paf->drop();
    paf = ps->createScaleParticleAffector(irr::core::vector2df(0.4, 0.4));
    ps->addAffector(paf); // same goes for the affector
    paf->drop();

    ps->setScale(core::vector3df(1, 1, 1));
    ps->setMaterialFlag(video::EMF_LIGHTING, true);
    ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
    ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    nodes.push_back(light);
}

bool Explosion::update()
{
    if (timer->getTime() - startTime >= 500)
    {
        sound.stop();
        for (auto i : nodes)
        {
            smgr->addToDeletionQueue(i);
        }
        nodes.clear();
        return true;
    }
    return false;
}

Explosion::Explosion(irr::IrrlichtDevice *device, BomberMap *map, irr::core::vector2df pos, int radius, bool through) : timer(device->getTimer())
{
    smgr = device->getSceneManager();
    startTime = timer->getTime();
    putExplosion(map, pos.X, pos.Y);
    setLine(map, pos, irr::core::vector2di(1, 0), radius, through);
    setLine(map, pos, irr::core::vector2di(-1, 0), radius, through);
    setLine(map, pos, irr::core::vector2di(0, 1), radius, through);
    setLine(map, pos, irr::core::vector2di(0, -1), radius, through);
    if (buffer.loadFromFile(ASSETS_SOUNDS "boom.ogg"))
    {
        sound.setBuffer(buffer);
        sound.play();
    }
}

Explosion::~Explosion()
{
}
