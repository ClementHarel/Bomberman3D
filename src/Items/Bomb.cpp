/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Bomb
*/

#include "Bomb.hpp"

Bomb::Bomb(irr::IrrlichtDevice *device) : Item(), time(device->getTimer())
{
    this->blocking = true;
    this->type = Item::Type::BOMB;
    this->radius = 1;
    this->through = false;
    this->meshPath = ASSETS_FOLDER "bomb.obj";
    this->startTime = 0;
    this->fuseTime = 3000;
}

Bomb::~Bomb()
{
    fuze.stop();
}

void Bomb::setRadius(uint radius)
{
    this->radius = radius;
}

void Bomb::setThrough(bool through)
{
    this->through = through;
}

const uint Bomb::getRadius() const
{
    return this->radius;
}

const bool Bomb::passthrough() const
{
    return this->through;
}

void Bomb::createNode(irr::core::vector3df pos, irr::scene::ISceneManager *smgr)
{
    startTime = time->getTime();
    mesh = 0;
    mesh = smgr->getMesh(meshPath.c_str());
    node = smgr->addMeshSceneNode(mesh, 0, -1, pos);
    node->setMaterialFlag(video::EMF_LIGHTING, true);
    node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    node->setDebugDataVisible(scene::EDS_OFF);
    node->addShadowVolumeSceneNode();

    irr::scene::ILightSceneNode *light = smgr->addLightSceneNode(node, core::vector3df(-0.01, 0.1, -0.01),
                                                                 video::SColorf(1.0f, 0.6f, 0.7f), 1.5f);
    light->getLightData().Attenuation = irr::core::vector3df(1.0 / 1.5, 1.0 / 1.5, 1.0 / 1.5);

    light->addAnimator(smgr->createFlyStraightAnimator(core::vector3df(-0.01, 0.95, -0.01), core::vector3df(-0.01, 0.9, -0.01), fuseTime));

    scene::IParticleSystemSceneNode *ps =
        smgr->addParticleSystemSceneNode(false, light);

    scene::IParticleEmitter *em = ps->createSphereEmitter(
        core::vector3df(0, 0, 0),
        0,
        core::vector3df(0, 0.002, 0),
        200, 1000,
        video::SColor(255, 255, 170, 0), // darkest color
        video::SColor(255, 255, 170, 0),
        100, 200, 360,
        core::dimension2df(0.1f, 0.1f), // min size
        core::dimension2df(0.1f, 0.1f));

    ps->setEmitter(em); // this grabs the emitter
    em->drop();         // so we can drop it here without deleting it

    scene::IParticleAffector *paf = ps->createFadeOutParticleAffector();

    ps->addAffector(paf); // same goes for the affector
    paf->drop();

    ps->setScale(core::vector3df(1, 1, 1));
    ps->setMaterialFlag(video::EMF_LIGHTING, true);
    ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
    ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    if (fuzeBuffer.loadFromFile(ASSETS_SOUNDS "fuze.ogg"))
    {
        fuze.setBuffer(fuzeBuffer);
        fuze.setLoop(true);
        fuze.play();
    }
}

bool posOnPath(BomberMap *map, irr::core::vector2di pos, irr::core::vector2di origin, irr::core::vector2di dir, uint radius, bool pass)
{
    for (int i = 0; i <= radius; i++)
    {
        if (origin.X + dir.X * i <= 0 || origin.Y + dir.Y * i <= 0 ||
            origin.X + dir.X * i >= map->getSize().X - 1 || origin.Y + dir.Y * i >= map->getSize().Y - 1 ||
            (!pass && map->getBlocAt(origin.X + dir.X * i, origin.Y + dir.Y * i)->getType() == Bloc::Types::UNBREAKABLE))
            break;
        if (pos.X == origin.X + dir.X * i && pos.Y == origin.Y + dir.Y * i)
            return true;
    }
    return false;
}

bool Bomb::withinExplosion(BomberMap *map, int x, int y)
{
    return posOnPath(map, irr::core::vector2di(x, y), pos, irr::core::vector2di(1, 0), radius, through) ||
           posOnPath(map, irr::core::vector2di(x, y), pos, irr::core::vector2di(-1, 0), radius, through) ||
           posOnPath(map, irr::core::vector2di(x, y), pos, irr::core::vector2di(0, 1), radius, through) ||
           posOnPath(map, irr::core::vector2di(x, y), pos, irr::core::vector2di(0, -1), radius, through);
}

bool Bomb::destroyBlocs(BomberMap *map, irr::core::vector2di dir)
{
    for (int i = 0; i <= radius; i++)
    {
        if (pos.X + dir.X * i <= 0 || pos.Y + dir.Y * i <= 0 ||
            pos.X + dir.X * i >= map->getSize().X - 1 || pos.Y + dir.Y * i >= map->getSize().Y - 1 ||
            (!through && map->getBlocAt(pos.X + dir.X * i, pos.Y + dir.Y * i)->getType() == Bloc::Types::UNBREAKABLE))
            break;
        if (map->getBlocAt(pos.X + dir.X * i, pos.Y + dir.Y * i)->getType() != Bloc::Types::UNBREAKABLE)
            map->setBlocAt(pos.X + dir.X * i, pos.Y + dir.Y * i, Bloc::Types::EMPTY);
    }
}

bool Bomb::isExploding()
{
    return (time->getTime() - startTime >= fuseTime);
}

bool Bomb::explode(BomberMap *map)
{
    if (time->getTime() - startTime >= fuseTime)
    {
        map->removeItemBlockAt(pos.X, pos.Y, this);

        fuze.stop();
        destroyBlocs(map, irr::core::vector2di(1, 0));
        destroyBlocs(map, irr::core::vector2di(-1, 0));
        destroyBlocs(map, irr::core::vector2di(0, 1));
        destroyBlocs(map, irr::core::vector2di(0, -1));
        return true;
    }
    return false;
}

void Bomb::destroy(irr::scene::ISceneManager *smgr)
{
    fuze.stop();

    if (node)
        smgr->addToDeletionQueue(node);
    node = 0;
}