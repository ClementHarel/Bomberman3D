/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Player
*/

#include "Player.hpp"

#define F_E_I(f, i, a) (i - a < f && f < i + a)

void Player::die()
{
    this->node = 0;
    this->bulb = 0;
    this->dead = true;
}

irr::scene::ISceneNode *Player::drawPlayer(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver *driver)
{
    irr::scene::IAnimatedMesh *mesh = smgr->getMesh(ASSETS_FOLDER "sydney.md2");

    if (this->skins[0] == 0) // Set to black
    {
        mesh = smgr->getMesh(ASSETS_FOLDER "sydney.md2");
        node = smgr->addAnimatedMeshSceneNode(mesh);
        node->setMaterialTexture(0, driver->getTexture(ASSETS_FOLDER "sydney.bmp"));
        node->setScale(irr::core::vector3df(0.025, 0.025, 0.025));
        std::cerr << "black" << std::endl;
    }
    if (this->skins[0] == 1) // Set to yellow
    {
        mesh = smgr->getMesh(ASSETS_FOLDER "faerie.md2");
        node = smgr->addAnimatedMeshSceneNode(mesh);
        node->setMaterialTexture(0, driver->getTexture(ASSETS_FOLDER "faerie2.bmp"));
        node->setScale(irr::core::vector3df(0.025, 0.025, 0.025));
        std::cerr << "yellow" << std::endl;
    }
    if (this->skins[0] == 2) // set to white
    {
        mesh = smgr->getMesh(ASSETS_FOLDER "sydney.md2");
        node = smgr->addAnimatedMeshSceneNode(mesh);
        node->setMaterialTexture(0, driver->getTexture(ASSETS_FOLDER "nskinbl.jpg"));
        node->setScale(irr::core::vector3df(0.025, 0.025, 0.025));
        std::cerr << "white" << std::endl;
    }
    node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
    node->setDebugDataVisible(irr::scene::EDS_OFF);
    node->setMD2Animation(irr::scene::EMAT_STAND);
    bulb = smgr->addLightSceneNode(node, irr::core::vector3df(0 * (1.0 / 0.025), 0 * (1.0 / 0.025), 0 * (1.0 / 0.025)),
                                   color, 1.5f);
    bulb->getLightData().Attenuation = irr::core::vector3df(1.0 / 1.0, 1.0 / 1.0, 1.0 / 1.0);
    return node;
};

void Player::setId(int id)
{
    this->id = id;
    bomb.setOwnerId(id);
}

void Player::setMap(BomberMap *map)
{
    this->map = map;
}

void Player::setColor(irr::video::SColorf color)
{
    this->color = color;
    if (bulb)
        bulb->getLightData().DiffuseColor = this->color;
}

void Player::setPos(irr::core::vector2df npos)
{
    pos = npos;
    if (node)
    {
        if (map && map->isFree(pos.X, pos.Y))
        {
            irr::core::vector3df tmp_pos = map->getRealPos(pos.X, pos.Y);
            tmp_pos.Y = 0.5;
            node->setPosition(tmp_pos);
            node->setPosition(map->getRealPos(pos.X, pos.Y));
        }
        else
            node->setPosition(irr::core::vector3df(pos.X, 0.5f, pos.Y));
    }
}

Bomb *Player::actions(const MyEventReceiver receiver, int mybombs)
{
    int up[] = {irr::KEY_KEY_Z, irr::KEY_UP};
    int dn[] = {irr::KEY_KEY_S, irr::KEY_DOWN};
    int lt[] = {irr::KEY_KEY_Q, irr::KEY_LEFT};
    int rt[] = {irr::KEY_KEY_D, irr::KEY_RIGHT};
    int act[] = {irr::KEY_SPACE, irr::KEY_RETURN};
    move();
    if (receiver.IsKeyDown((irr::EKEY_CODE)up[id]))
        checkDir(moveDir::UP);
    else if (receiver.IsKeyDown((irr::EKEY_CODE)dn[id]))
        checkDir(moveDir::DOWN);
    else if (receiver.IsKeyDown((irr::EKEY_CODE)lt[id]))
        checkDir(moveDir::LEFT);
    else if (receiver.IsKeyDown((irr::EKEY_CODE)rt[id]))
        checkDir(moveDir::RIGHT);
    else if (this->dir == moveDir::NONE && running)
    {
        node->setMD2Animation(irr::scene::EMAT_STAND);
        running = !running;
    }
    if (receiver.IsKeyDown((irr::EKEY_CODE)act[id]))
        if (mybombs < maxBomb && (F_E_I(pos.X + 0.5 - (int)(pos.X + 0.5), 0.5, 0.01) && F_E_I(pos.Y + 0.5 - (int)(pos.Y + 0.5), 0.5, 0.01)))
        {
            Bomb *tmp = new Bomb(bomb);
            if (map->addItemBlockAt(pos.X + 0.5, pos.Y + 0.5, tmp))
                return tmp;
        }
    if (F_E_I(pos.X + 0.5 - (int)(pos.X + 0.5), 0.5, 0.01) && F_E_I(pos.Y + 0.5 - (int)(pos.Y + 0.5), 0.5, 0.01))
    {
        std::vector<Item *> content = map->getContent(pos.X + 0.5, pos.Y + 0.5);
        for (auto i : content)
        {
            if (i->getType() == Item::Type::BONUS)
            {
                useBonus((Bonus *)i);
                map->removeItemBlockAt(pos.X + 0.5, pos.Y + 0.5, i);
            }
        }
    }

    return 0;
}

void Player::useBonus(Bonus *bonus)
{
    switch (bonus->getType())
    {
    case Bonus::Type::BOMB_UP:
        maxBomb += 1;
        break;
    case Bonus::Type::RADIUS_UP:
        bomb.setRadius(bomb.getRadius() + 1);
        break;
    case Bonus::Type::SPEED_UP:
        moveSpd = 0.055;
        break;
    case Bonus::Type::BOMB_PASS:
        bomb.setThrough(true);
        break;
    case Bonus::Type::WALL_PASS:
        wall_pass = true;
        break;
    default:
        break;
    }
}

void Player::checkDir(moveDir dir)
{
    float tmp_x = pos.X + 0.5;
    float tmp_y = pos.Y + 0.5;

    if (dir == moveDir::UP)
        tmp_y += 0.5 + moveSpd;
    else if (dir == moveDir::DOWN)
        tmp_y -= 0.5 + moveSpd;
    else if (dir == moveDir::LEFT)
        tmp_x -= 0.5 + moveSpd;
    else if (dir == moveDir::RIGHT)
        tmp_x += 0.5 + moveSpd;
    if (!map || (map && map->isFree(tmp_x, tmp_y, wall_pass)))
        if (this->dir == moveDir::NONE)
        {
            this->dir = dir;
            if (!running)
            {
                node->setMD2Animation(irr::scene::EMAT_RUN);
                running = !running;
            }
        }
}

void Player::move()
{
    if (this->dir == moveDir::UP)
        pos.Y += moveSpd;
    else if (this->dir == moveDir::DOWN)
        pos.Y -= moveSpd;
    else if (this->dir == moveDir::LEFT)
        pos.X -= moveSpd;
    else if (this->dir == moveDir::RIGHT)
        pos.X += moveSpd;
    if (node)
    {
        if (this->dir != moveDir::NONE)
            node->setRotation(irr::core::vector3df(0, 90.0 * dir + 90, 0));
        if (map)
        {
            irr::core::vector3df tmp_pos = map->getRealPos(pos.X, pos.Y);
            tmp_pos.Y = 0.5;
            node->setPosition(tmp_pos);
        }
        else
            node->setPosition(irr::core::vector3df(pos.X, 0.5f, pos.Y));
    }
    if ((F_E_I(pos.X + 0.5 - (int)(pos.X + 0.5), 0.5, 0.02) && F_E_I(pos.Y + 0.5 - (int)(pos.Y + 0.5), 0.5, 0.02)))
    {
        this->dir = moveDir::NONE;
        pos.X = (int)(pos.X + 0.5);
        pos.Y = (int)(pos.Y + 0.5);
        irr::core::vector3df tmp_pos = map->getRealPos(pos.X, pos.Y);
        tmp_pos.Y = 0.5;
        node->setPosition(tmp_pos);
    }
}

irr::core::vector2di Player::getPos()
{
    return irr::core::vector2di((int)(pos.X + 0.5), (int)(pos.Y + 0.5));
}

irr::scene::ISceneNode *Player::getNode()
{
    return node;
}

int Player::getId()
{
    return id;
}

Player::Player(irr::IrrlichtDevice *device) : bomb(device)
{
    color = irr::video::SColorf(1.f, 1.f, 1.f);
    node = 0;
    bulb = 0;
    map = 0;
    pos = irr::core::vector2df(0, 0);
    moveSpd = 0.04;
    dir = moveDir::NONE;
    maxBomb = 1;
    running = false;
    dead = false;
    wall_pass = false;
    id = -1;
}

Player::~Player()
{
}
