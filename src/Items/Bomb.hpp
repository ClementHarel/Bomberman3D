/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Bomb
*/

#ifndef BOMB_HPP_
#define BOMB_HPP_

#include "../MapEditor/BomberMap.hpp"
#include <SFML/Audio.hpp>

class Bomb : virtual public Item
{
public:
    Bomb(irr::IrrlichtDevice *device);
    ~Bomb();

    using Item::getId;
    using Item::getNode;
    using Item::getOwnerId;
    using Item::getType;
    using Item::isBlocking;
    using Item::setBlocking;
    using Item::setId;
    using Item::setOwnerId;

    void setRadius(uint radius);
    void setThrough(bool through);

    const uint getRadius() const;
    const bool passthrough() const;
    void createNode(irr::core::vector3df pos, irr::scene::ISceneManager *smgr);

    bool withinExplosion(BomberMap *map, int x, int y);
    bool isExploding();
    bool explode(BomberMap *map);
    void destroy(irr::scene::ISceneManager *smgr);

protected:
private:
    bool destroyBlocs(BomberMap *map, irr::core::vector2di dir);
    uint radius;
    bool through;
    irr::ITimer *time;
    int startTime;
    int fuseTime;
    sf::SoundBuffer fuzeBuffer;
    sf::Sound fuze;
};

#endif /* !BOMB_HPP_ */
