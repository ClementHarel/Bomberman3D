/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Game
*/

#ifndef GAME_HPP_
#define GAME_HPP_
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "Player.hpp"
#include "MyEventReceiver.hpp"
#include "FX/Explosion.hpp"

class Game
{
public:
    void gameLoop(irr::IrrlichtDevice *device);
    void mapPreview(irr::IrrlichtDevice *device);
    bool mapTimeUp(bool reset);
    void killPlayer(Player *);
    int countBombs(int player_id);
    int playerAlive();
    void gameOver(irr::IrrlichtDevice *device);
    void addBomberPoints(Player *player, int amount);
    Game(Player *player1, Player *player2, std::string map, irr::IrrlichtDevice *device);
    ~Game();

    MyEventReceiver *receiver;
    video::IVideoDriver *driver;
    scene::ISceneManager *smgr;
    irr::gui::IGUIEnvironment *guienv;

    BomberMap *map;

protected:
private:
    void setupGui();

    std::vector<Bomb *> activeBombs;
    std::vector<Explosion *> explosions;
    std::vector<Player *> players;
    uint rect[4]; // timeup rect
    uint fill[2]; // timeup ite
    irr::ITimer *timer;
    int maxTime;
    irr::gui::IGUIStaticText *clock;
};

#endif /* !GAME_HPP_ */
