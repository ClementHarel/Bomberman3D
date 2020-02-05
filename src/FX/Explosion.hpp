/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Explosion
*/

#ifndef EXPLOSION_HPP_
#define EXPLOSION_HPP_

#include "../MapEditor/BomberMap.hpp"
#include <SFML/Audio.hpp>
#include <vector>

class Explosion {
	public:
		Explosion(irr::IrrlichtDevice *device, BomberMap *map, irr::core::vector2df pos, int radius, bool through);
		~Explosion();

        bool update();

        irr::scene::ISceneManager *smgr;
        irr::ITimer *timer;
        std::vector<irr::scene::ILightSceneNode *> nodes;
	protected:
	private:
    int startTime;
    void putExplosion(BomberMap *map, int x, int y);
    void setLine(BomberMap *map, irr::core::vector2df pos, irr::core::vector2di dir, int radius, bool pass);
    sf::SoundBuffer buffer;
    sf::Sound sound;
};

#endif /* !EXPLOSION_HPP_ */
