/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Game
*/

#include "Game.hpp"

void Game::addBomberPoints(Player *player, int amount)
{
    std::string str;

    str = "Players/" + player->name;
    remove(str.c_str());

    std::fstream f;
    f.open("Players/" + player->name, std::fstream::in | std::fstream::out | std::fstream::app);
    f << player->name << "\n"
      << player->bomberPoints + amount << "\n"
      << player->skins[0] << "\n"
      << player->skins[1] << "\n"
      << player->skins[2];
}

void Game::gameOver(irr::IrrlichtDevice *device)
{

    map->unloadMap();
    while (activeBombs.size() > 0)
    {
        activeBombs.erase(activeBombs.begin());
    }
    while (explosions.size() > 0 && explosions[0]->update())
    {
        for (irr::scene::ISceneNode *node : explosions[0]->nodes)
            smgr->addToDeletionQueue(node);
        explosions[0]->nodes.clear();
        explosions.erase(explosions.begin());
    }

    srand(time(0));
    int bomberPoints = rand() % (200 - 100) + 100;

    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();
    smgr->clear();
    guienv->clear();
    irr::gui::IGUIStaticText *winText = guienv->addStaticText(L"", irr::core::recti(572, 32, 572 + 940, 32 + 194), true, false, 0, -1, true);
    winText->setTextAlignment(irr::gui::EGUI_ALIGNMENT::EGUIA_CENTER, irr::gui::EGUI_ALIGNMENT::EGUIA_CENTER);
    irr::gui::IGUIStaticText *looseText = guienv->addStaticText(L"", irr::core::recti(317, 15, 317 + 99, 15 + 92), true, false, 0, -1, true);
    looseText->setTextAlignment(irr::gui::EGUI_ALIGNMENT::EGUIA_CENTER, irr::gui::EGUI_ALIGNMENT::EGUIA_CENTER);
    if (players.at(0)->dead == false)
    {
        std::wstring wsTmp(players.at(0)->name.begin(), players.at(0)->name.end());
        std::wstring wsTmp2(players.at(1)->name.begin(), players.at(1)->name.end());
        wsTmp = L"\nWINNER IS  : " + wsTmp + L"\n You won : " + std::to_wstring(bomberPoints) + L" BomberPoints !!!";
        winText->setText(wsTmp.c_str());
        looseText->setText(wsTmp2.c_str());
        addBomberPoints(players.at(0), bomberPoints);
    }
    else
    {
        std::wstring wsTmp(players.at(1)->name.begin(), players.at(1)->name.end());
        std::wstring wsTmp2(players.at(0)->name.begin(), players.at(0)->name.end());
        wsTmp = L"\nWINNER IS  : " + wsTmp + L"\n You won : " + std::to_wstring(bomberPoints) + L" BomberPoints !!!";
        winText->setText(wsTmp.c_str());
        looseText->setText(wsTmp2.c_str());
        addBomberPoints(players.at(1), bomberPoints);
    }

    video::ITexture *gameOver = this->driver->getTexture(ASSETS_FOLDER "win.png");
    while (device->run())
    {
        if (receiver->IsKeyDown(irr::KEY_ESCAPE))
            break;
        driver->beginScene(true, true, video::SColor(255, 0, 0, 0));
        driver->draw2DImage(gameOver, core::position2d<s32>(0, 0),
                            core::rect<s32>(0, 0, gameOver->getOriginalSize().Width, gameOver->getOriginalSize().Height),
                            0,
                            video::SColor(255, 255, 255, 255), true);
        smgr->drawAll();
        guienv->drawAll();
        driver->endScene();
    }
}

std::string cleanName(std::string str)
{
    int i = str.size();

    while (str[i] != '/' || i == 0)
        i--;
    return str.substr(i + 1, str.size() - i + 1);
}

void Game::setupGui()
{
    irr::gui::IGUISkin *skin = guienv->getSkin();
    irr::gui::IGUIFont *font = guienv->getFont(ASSETS_FONT "fontcourier.bmp");
    if (font)
        skin->setFont(font);

    clock = guienv->addStaticText(L"", irr::core::recti(960 - 15, 0, 960 + 15, 30), true, false, 0, -1, true);
    clock->setTextAlignment(irr::gui::EGUI_ALIGNMENT::EGUIA_CENTER, irr::gui::EGUI_ALIGNMENT::EGUIA_CENTER);
    clock->setText(std::to_wstring(maxTime - timer->getTime() / 1000).c_str());

    irr::gui::IGUIStaticText *playerName1 = guienv->addStaticText(L"", irr::core::recti(0, 0, 180, 40), true, false, 0, -1, true);
    playerName1->setTextAlignment(irr::gui::EGUI_ALIGNMENT::EGUIA_UPPERLEFT, irr::gui::EGUI_ALIGNMENT::EGUIA_UPPERLEFT);
    std::wstring wsTmp(players.at(0)->name.begin(), players.at(0)->name.end());
    wsTmp = wsTmp + L"\nBomberPoints : " + std::to_wstring(players.at(0)->bomberPoints);
    playerName1->setText(wsTmp.c_str());

    irr::gui::IGUIStaticText *playerName2 = guienv->addStaticText(L"", irr::core::recti(1920 - 180, 0, 1920, 40), true, false, 0, -1, true);
    std::wstring wsTmp2(players.at(1)->name.begin(), players.at(1)->name.end());
    wsTmp2 = wsTmp2 + L"\nBomberPoints : " + std::to_wstring(players.at(1)->bomberPoints);
    playerName2->setText(wsTmp2.c_str());
}

bool Game::mapTimeUp(bool reset)
{
    static int step = 0;

    if (reset)
        step = 0;
    if (rect[3] > rect[1])
    {
        map->setBlocAt(fill[0], fill[1], Bloc::Types::UNBREAKABLE);
        if (step == 0)
            fill[1] += 1;
        else if (step == 1)
            fill[0] += 1;
        else if (step == 2)
            fill[1] -= 1;
        else if (step == 3)
            fill[0] -= 1;
        if (fill[0] == rect[0] && fill[1] == rect[1])
        {
            step = 0;
            rect[2] -= 1;
        }
        else if (fill[0] == rect[0] && fill[1] == rect[3])
        {
            step = 1;
            rect[1] += 1;
        }
        else if (fill[0] == rect[2] && fill[1] == rect[3])
        {
            step = 2;
            rect[0] += 1;
        }
        else if (fill[0] == rect[2] && fill[1] == rect[1])
        {
            step = 3;
            rect[3] -= 1;
        }
        return false;
    }
    else
        step = 0;
    return true;
}

void Game::killPlayer(Player *player)
{
    if (map->getBlocAt(player->getPos().X, player->getPos().Y)->getType() == Bloc::Types::UNBREAKABLE)
    {
        if (player->getNode())
            smgr->addToDeletionQueue(player->getNode());
        player->die();
    }
}

int Game::playerAlive()
{
    int count = 0;

    for (auto i : players)
    {
        if (!i->dead)
            count += 1;
    }
    return count;
}

int Game::countBombs(int id)
{
    int count = 0;

    for (auto b : activeBombs)
    {
        if (b->getOwnerId() == id)
            count += 1;
    }
    return count;
}

void Game::mapPreview(irr::IrrlichtDevice *device)
{
    core::vector3df cam1_pos(M_PI_2 / 2.0, -M_PI_2, 0);
    float cam_dist = ((float)std::max(this->map->getSize().X, this->map->getSize().Y)) * 2;

    while (device->run() && driver && cam1_pos.Y < M_PI * 1.5)
    {
        smgr->getActiveCamera()->setPosition(core::vector3df(
            cos(cam1_pos.X) * cos(cam1_pos.Y) * cam_dist,
            sin(cam1_pos.X) * cam_dist,
            cos(cam1_pos.X) * sin(cam1_pos.Y) * cam_dist));
        driver->beginScene(true, true, video::SColor(255, 0, 0, 0));
        smgr->drawAll();
        guienv->drawAll();
        driver->endScene();
        cam1_pos.Y += M_1_PI / 16.0;
    }

    cam_dist = ((float)std::max(this->map->getSize().X, this->map->getSize().Y));
    core::vector3df cam_pos = smgr->getActiveCamera()->getPosition();
    core::vector3df end_pos = core::vector3df(0, cam_dist, -1);
    core::vector3df move = core::vector3df(((end_pos.X - cam_pos.X) / 32.0), ((end_pos.Y - cam_pos.Y) / 32.0), ((end_pos.Z - cam_pos.Z) / 32.0));

    float dist = sqrt(pow(end_pos.X - cam_pos.X, 2.0) + pow(end_pos.Y - cam_pos.Y, 2.0) + pow(end_pos.Z - cam_pos.Z, 2.0));

    while (device->run() && driver && dist > 0.1)
    {
        smgr->getActiveCamera()->setPosition(core::vector3df(cam_pos.X + move.X, cam_pos.Y + move.Y, cam_pos.Z + move.Z));
        driver->beginScene(true, true, video::SColor(255, 0, 0, 0));
        smgr->drawAll();
        guienv->drawAll();
        driver->endScene();
        cam_pos = smgr->getActiveCamera()->getPosition();
        dist = sqrt(pow(cam_pos.X - end_pos.X, 2.0) + pow(cam_pos.Y - end_pos.Y, 2.0) + pow(cam_pos.Z - end_pos.Z, 2.0));
    }
    smgr->getActiveCamera()->setPosition(end_pos);
}

void Game::gameLoop(irr::IrrlichtDevice *device)
{
    float cam_dist = ((float)std::max(this->map->getSize().X, this->map->getSize().Y));
    bool timeUp = false;
    smgr->addCameraSceneNode(0, core::vector3df(0, cam_dist, -1), core::vector3df(0, 0, 0), 1, true);
    smgr->getActiveCamera()->setFOV(M_PI_2);

    timer->stop();
    timer->setTime(0);

    setupGui();

    mapPreview(device);

    timer->start();
    uint time = 0;

    while (device->run() && driver && playerAlive() > 1)
    {
        if (receiver->IsKeyDown(irr::KEY_ESCAPE))
            break;
        if (!timeUp)
        {
            clock->setText(std::to_wstring(maxTime - time / 1000).c_str());
        }
        if (!timeUp && (timer->getTime() - time) / 1000 >= maxTime)
        {
            timeUp = true;
            time = 0;
        }
        if (timeUp && timer->getTime() - time >= 1000)
        {
            mapTimeUp(false);
            time = 0;
        }
        for (auto p : players)
        {
            if (!p->dead)
            {
                Bomb *tmp = p->actions(*receiver, countBombs(p->getId()));
                if (tmp)
                    activeBombs.push_back(tmp);
            }
            killPlayer(p);
        }
        while (activeBombs.size() > 0 && activeBombs[0]->explode(map))
        {
            for (auto p : players)
            {
                if (!p->dead)
                {
                    if (activeBombs[0]->withinExplosion(map, p->getPos().X, p->getPos().Y))
                    {
                        if (p->getNode())
                            smgr->addToDeletionQueue(p->getNode());
                        p->die();
                    }
                }
            }
            explosions.push_back(new Explosion(device, map, irr::core::vector2df(activeBombs[0]->pos.X, activeBombs[0]->pos.Y), activeBombs[0]->getRadius(), activeBombs[0]->passthrough()));

            activeBombs.erase(activeBombs.begin());
        }
        while (explosions.size() > 0 && explosions[0]->update())
        {
            explosions.erase(explosions.begin());
        }
        driver->beginScene(true, true, video::SColor(255, 0, 0, 0));
        smgr->drawAll();
        guienv->drawAll();
        driver->endScene();
        time = timer->getTime();
    }
    gameOver(device);
}

Game::Game(Player *player1, Player *player2, std::string map, irr::IrrlichtDevice *device) : timer(device->getTimer())
{
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();
    maxTime = 180;

    if (!(receiver = (MyEventReceiver *)device->getEventReceiver()))
    {
        receiver = new MyEventReceiver();
        device->setEventReceiver(receiver);
    }

    std::cout << "Player 1 name = " << player1->name << "\nPlayer 2 name = " << player2->name << " \nMap = " << map << std::endl;
    std::cout << player1->bomberPoints << "\n"
              << player2->bomberPoints << std::endl;

    smgr->clear();
    guienv->clear();

    this->map = new BomberMap(smgr, driver, 6, 6);
    this->map->loadMap(map);

    rect[0] = 1;
    rect[1] = 0;
    rect[2] = this->map->getSize().X - 2;
    rect[3] = this->map->getSize().Y - 2;
    fill[0] = 1;
    fill[1] = 1;

    players.push_back(player1);
    players.push_back(player2);

    int ids = 0;
    for (auto i : players)
    {
        i->setId(ids++);
        i->setMap(this->map);
        i->drawPlayer(smgr, driver);
    }
    players[0]->setPos(irr::core::vector2df(1, 1));
    players[0]->setColor(irr::video::SColorf(1.0, 1.0, 1.0));
    players[1]->setPos(irr::core::vector2df(this->map->getSize().X - 2, this->map->getSize().Y - 2));
    players[1]->setColor(irr::video::SColorf(1.0, 1.0, 1.0));

    this->gameLoop(device);

    guienv->clear();
    smgr->clear();
}

Game::~Game()
{
    mapTimeUp(true);
}
