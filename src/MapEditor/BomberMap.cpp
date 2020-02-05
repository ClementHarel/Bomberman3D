/*
** EPITECH PROJECT, 2019
** src
** File description:
** BomberMap
*/

#include "BomberMap.hpp"
#include <iostream>
#include <sys/file.h>

#include "../Items/Bomb.hpp"

/*
 *  Bloc
 */

Bloc::Bloc(float _x, float _y, Bloc::Types _type)
{
    x = _x;
    y = _y;
    type = _type;
}

Bloc::~Bloc()
{
}

void Bloc::setType(Bloc::Types _type)
{
    type = _type;
}

const Bloc::Types Bloc::getType()
{
    return (type);
}

const core::vector2df Bloc::getPos()
{
    return (core::vector2df(x, y));
}

int Bloc::getId()
{
    return (cube->getID());
}

/*
 *  BomberMap
 */

void BomberMap::drawMap()
{
    int i = 0;

    lighting = smgr->addLightSceneNode(0, core::vector3df(0, (w + h), 0),
                                       video::SColorf(1.0f, 1.0f, 1.0f), (w + h));
    skybox = smgr->addSkyBoxSceneNode(driver->getTexture(ASSETS_SKYBOX "day/irrlicht2_up.jpg"),
                                      driver->getTexture(ASSETS_SKYBOX "day/irrlicht2_dn.jpg"),
                                      driver->getTexture(ASSETS_SKYBOX "day/irrlicht2_lf.jpg"),
                                      driver->getTexture(ASSETS_SKYBOX "day/irrlicht2_rt.jpg"),
                                      driver->getTexture(ASSETS_SKYBOX "day/irrlicht2_ft.jpg"),
                                      driver->getTexture(ASSETS_SKYBOX "day/irrlicht2_bk.jpg"));
    setNight(night);
    if (!floor)
    {
        floor = smgr->addMeshSceneNode(floorMesh);
        floor->setMaterialTexture(0, floorTexture);
        floor->setMaterialFlag(video::EMF_LIGHTING, true);
        floor->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    }
    for (auto bloc : blocs)
    {
        int id = i;
        while (smgr->getSceneNodeFromId(id) != nullptr)
            id += 1;
        bloc->cube = smgr->addCubeSceneNode(1, 0, id, core::vector3df(bloc->getPos().X - (((float)w - 1) / 2.0), 0.5, bloc->getPos().Y - (((float)h - 1) / 2.0)));
        bloc->cube->setMaterialFlag(video::EMF_LIGHTING, true);
        bloc->cube->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
        updateBlocDisplay(bloc);
        i += 1;
    }
}

void BomberMap::unloadMap()
{
    if (floor)
        smgr->addToDeletionQueue(floor);
    floor = 0;
    for (auto node : blocs)
    {
        if (node->cube)
            smgr->addToDeletionQueue(node->cube);
        node->cube = 0;
        for (auto content : node->content)
            content->destroy(smgr);
        node->content.clear();
    }
    if (lighting)
        smgr->addToDeletionQueue(lighting);
    lighting = 0;
    if (skybox)
        smgr->addToDeletionQueue(skybox);
    skybox = 0;
}

void BomberMap::updateTexture()
{
    floorTexture = driver->getTexture(irr::io::path(floorPath.c_str()));
    if (floor && floorTexture)
        floor->setMaterialTexture(0, floorTexture);
    textures.updateTextures(driver);
}

void BomberMap::saveMap(std::string map_name)
{
    std::string filename = SAVES_MAP + map_name + ".map";
    std::ofstream file_ostream;

    mkdir(SAVES_MAP, 0777);
    file_ostream.open(filename);
    if (file_ostream.is_open())
    {
        file_ostream << map_name << std::endl;
        file_ostream << w << ":" << h << std::endl;
        file_ostream << floorPath << std::endl;
        std::string s = "";
        int i = 0;
        file_ostream << textures.toString();
        for (auto tmp : blocs)
        {
            if (i != tmp->getPos().Y)
            {
                s += "\n";
                i = tmp->getPos().Y;
            }
            s.append(std::to_string(tmp->getType()));
        }
        file_ostream << s << std::endl;
        file_ostream << night << std::endl;
        file_ostream.close();
    }
}

bool BomberMap::loadMap(std::string map_name)
{
    std::string filename = map_name;
    std::ifstream file_istream;

    file_istream.open(filename);
    if (file_istream.is_open())
    {
        std::string s;
        getline(file_istream, s);
        getline(file_istream, s);
        int width = atoi(s.substr(0, s.find(':')).c_str());
        int height = atoi(s.substr(s.find(':') + 1).c_str());
        getline(file_istream, s);
        setFloorTexture(s);
        getline(file_istream, s);
        int t = atoi(s.substr(s.find(":") + 1).c_str());
        for (int i = 0; i < t; i++)
        {
            getline(file_istream, s);
            int type = atoi(s.substr(0, s.find(",")).c_str());
            std::string path = s.substr(s.find(',') + 1);
            Textures<Bloc::Types>::Texture *tex = textures.getTextureFromType((Bloc::Types)type);
            if (tex)
                tex->path = path;
            else
                addTexture((Bloc::Types)type, path);
        }
        updateTexture();
        resizeMap(width, height);
        for (int y = 0; y < height; y++)
        {
            getline(file_istream, s);
            for (int x = 0; x < width; x++)
            {
                std::string tmp = s.substr(x, 1);
                setBlocAt(x, y, (Bloc::Types)atoi(tmp.c_str()));
            }
        }
        getline(file_istream, s);
        setNight((bool)atoi(s.c_str()));
        unloadMap();
        drawMap();
        file_istream.close();
        return (true);
    }
    else
    {
        std::cerr << "fail loading map" << std::endl;
        return (false);
    }
}

void BomberMap::addTexture(Bloc::Types type, std::string path)
{
    Textures<Bloc::Types>::Texture *tmp = new Textures<Bloc::Types>::Texture(type, path);
    textures.list.push_back(tmp);
}

void BomberMap::setBlocAt(const uint _x, const uint _y, const Bloc::Types _type)
{
    if (_x > 0 && _x < w - 1 && _y > 0 && _y < h - 1 &&
        !onCorner(_x, _y, BomberMap::Corner::BOT_LEFT) &&
        !onCorner(_x, _y, BomberMap::Corner::BOT_RIGHT) &&
        !onCorner(_x, _y, BomberMap::Corner::TOP_LEFT) &&
        !onCorner(_x, _y, BomberMap::Corner::TOP_RIGHT))
    {
        if (_y * w + _x < blocs.size())
        {
            Bloc::Types prevtype = blocs[_y * w + _x]->getType();
            blocs[_y * w + _x]->setType(_type);
            if (!editMode && prevtype == Bloc::Types::BREAKABLE)
                addItemBlockAt(_x, _y, new Bonus(driver, true));
        }
        if (blocs[_y * w + _x]->cube)
        {
            scene::ISceneNode *bloc = blocs[_y * w + _x]->cube;
            updateBlocDisplay(blocs[_y * w + _x]);
        }
    }
}

void BomberMap::opSetBlocAt(const uint _x, const uint _y, const Bloc::Types _type)
{
    if (_y * w + _x < blocs.size())
    {
        Bloc::Types prevtype = blocs[_y * w + _x]->getType();
        blocs[_y * w + _x]->setType(_type);
        if (!editMode && prevtype == Bloc::Types::BREAKABLE)
            addItemBlockAt(_x, _y, new Bonus(driver, true));
    }
    if (blocs[_y * w + _x]->cube)
    {
        scene::ISceneNode *bloc = blocs[_y * w + _x]->cube;
        updateBlocDisplay(blocs[_y * w + _x]);
    }
}

void BomberMap::setFloorTexture(const std::string _texture)
{
    floorPath = _texture;
    floorTexture = driver->getTexture(irr::io::path(floorPath.c_str()));
    if (floor && floorTexture)
        floor->setMaterialTexture(0, floorTexture);
}

void BomberMap::setTexture(Bloc::Types type, const std::string _texture)
{
    video::ITexture *tx = driver->getTexture(irr::io::path(_texture.c_str()));
    if (tx)
    {
        Textures<Bloc::Types>::Texture *tmp = textures.getTextureFromType(type);
        if (tmp)
            tmp->path = _texture;
        else
            addTexture(type, _texture);
        updateTexture();
    }
    else
    {
        std::cout << "fail" << std::endl;
    }
    for (uint i = 0; i < blocs.size(); i++)
    {
        updateBlocDisplay(blocs[i]);
    }
}

void BomberMap::setNight(bool night)
{
    this->night = night;

    if (night)
    {
        smgr->addToDeletionQueue(skybox);
        skybox = smgr->addSkyBoxSceneNode(driver->getTexture(ASSETS_SKYBOX "night/irrlicht2_up.jpg"),
                                          driver->getTexture(ASSETS_SKYBOX "night/irrlicht2_dn.jpg"),
                                          driver->getTexture(ASSETS_SKYBOX "night/irrlicht2_lf.jpg"),
                                          driver->getTexture(ASSETS_SKYBOX "night/irrlicht2_rt.jpg"),
                                          driver->getTexture(ASSETS_SKYBOX "night/irrlicht2_ft.jpg"),
                                          driver->getTexture(ASSETS_SKYBOX "night/irrlicht2_bk.jpg"));
        lighting->getLightData().DiffuseColor = irr::video::SColorf(1.0, 1.0, 0.95);
        lighting->getLightData().Attenuation = irr::core::vector3df(0, NIGHT_LIGHT, 0);
    }
    else
    {
        smgr->addToDeletionQueue(skybox);
        skybox = smgr->addSkyBoxSceneNode(driver->getTexture(ASSETS_SKYBOX "day/irrlicht2_up.jpg"),
                                          driver->getTexture(ASSETS_SKYBOX "day/irrlicht2_dn.jpg"),
                                          driver->getTexture(ASSETS_SKYBOX "day/irrlicht2_lf.jpg"),
                                          driver->getTexture(ASSETS_SKYBOX "day/irrlicht2_rt.jpg"),
                                          driver->getTexture(ASSETS_SKYBOX "day/irrlicht2_ft.jpg"),
                                          driver->getTexture(ASSETS_SKYBOX "day/irrlicht2_bk.jpg"));
        lighting->getLightData().DiffuseColor = irr::video::SColorf(1.0, 1.0, 1.0);
        lighting->getLightData().Attenuation = irr::core::vector3df(0, 1.0 / (float)(w + h), 0);
    }
}

void BomberMap::resizeMap(uint width, uint height)
{
    if (width >= MAP_MIN_W && height >= MAP_MIN_H && width <= MAP_MAX_W && height <= MAP_MAX_H)
    {
        unloadMap();
        floorMesh = smgr->getGeometryCreator()->createPlaneMesh(core::dimension2df(1, 1), core::dimension2du(width, height), 0, core::dimension2df(width, height));
        std::vector<Bloc *> tmpBlocList;
        int old_w = w, old_h = h;
        w = width;
        h = height;
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                Bloc *tmpbloc = new Bloc((float)x,
                                         (float)y,
                                         (x == 0 || y == 0 || x == width - 1 || y == height - 1) ? Bloc::Types::UNBREAKABLE : Bloc::Types::EMPTY);
                if (x > 0 && x < old_w - 1 && y > 0 && y < old_h - 1 &&
                    !onCorner(x, y, BomberMap::Corner::BOT_LEFT) &&
                    !onCorner(x, y, BomberMap::Corner::BOT_RIGHT) &&
                    !onCorner(x, y, BomberMap::Corner::TOP_LEFT) &&
                    !onCorner(x, y, BomberMap::Corner::TOP_RIGHT))
                {
                    for (auto tmp : blocs)
                    {
                        if (tmp->getPos().X == x && tmp->getPos().Y == y)
                        {
                            tmpbloc = tmp;
                            break;
                        }
                    }
                }
                tmpBlocList.push_back(tmpbloc);
            }
        }
        blocs = tmpBlocList;
    }
    drawMap();
}

bool BomberMap::addItemBlockAt(const uint _x, const uint _y, Item *item)
{
    if (_y * w + _x < blocs.size() && (!item->isBlocking() || (item->isBlocking() && isFree(_x, _y))))
    {
        item->pos.X = _x;
        item->pos.Y = _y;
        item->createNode(irr::core::vector3df(blocs[_y * w + _x]->getPos().X - (((float)w - 1) / 2.0), 0.0, blocs[_y * w + _x]->getPos().Y - (((float)h - 1) / 2.0)), smgr);
        blocs[_y * w + _x]->content.push_back(item);
        return true;
    }
    return false;
}

void BomberMap::removeItemBlockAt(const uint _x, const uint _y, Item *item)
{
    if (_y * w + _x < blocs.size())
    {
        std::vector<Item *>::iterator it = std::find(blocs[_y * w + _x]->content.begin(), blocs[_y * w + _x]->content.end(), item);
        if (it != blocs[_y * w + _x]->content.end())
        {
            if (item->node)
                smgr->addToDeletionQueue(item->node);
            blocs[_y * w + _x]->content.erase(it);
        }
    }
}

bool BomberMap::onCorner(int x, int y, BomberMap::Corner corner)
{
    if (corner == BomberMap::Corner::BOT_LEFT &&
        ((x == 1 && y == 1) ||
         (x == 1 && y == 2) ||
         (x == 2 && y == 1)))
    {
        return true;
    }
    else if (corner == BomberMap::Corner::BOT_RIGHT &&
             ((x == w - 2 && y == 1) ||
              (x == w - 2 && y == 2) ||
              (x == w - 3 && y == 1)))
    {
        return true;
    }
    else if (corner == BomberMap::Corner::TOP_LEFT &&
             ((x == 1 && y == h - 2) ||
              (x == 1 && y == h - 3) ||
              (x == 2 && y == h - 2)))
    {
        return true;
    }
    else if (corner == BomberMap::Corner::TOP_RIGHT &&
             ((x == w - 2 && y == h - 2) ||
              (x == w - 2 && y == h - 3) ||
              (x == w - 3 && y == h - 2)))
    {
        return true;
    }
    return false;
}

Bloc *BomberMap::getBlocAt(const uint _x, const uint _y) const
{
    return (blocs[_y * w + _x]);
}

video::ITexture *BomberMap::getFloorTexture() const
{
    return (floorTexture);
}

core::vector2di BomberMap::getSize() const
{
    return (core::vector2di(w, h));
}

void BomberMap::updateBlocDisplay(Bloc *bloc)
{
    Bloc::Types _type = bloc->getType();
    if (_type == Bloc::EMPTY)
    {
        bloc->cube->setVisible(false);
    }
    else
    {
        bloc->cube->setVisible(true);
        video::ITexture *tex = textures.getITextureFromType(_type);
        if (tex)
        {
            bloc->cube->setMaterialTexture(0, tex);
        }
    }
}

bool BomberMap::isFree(uint x, uint y, bool pass)
{
    if (y * w + x < blocs.size())
    {
        Bloc::Types type = blocs[y * w + x]->getType();
        if (type == Bloc::Types::UNBREAKABLE || (!pass && type == Bloc::Types::BREAKABLE))
            return false;
        if (!pass)
        {
            for (auto i : blocs[y * w + x]->content)
                if (i->isBlocking())
                    return false;
        }
        return true;
    }
    return false;
}

std::vector<Item *> &BomberMap::getContent(uint x, uint y)
{
    return (blocs[y * w + x]->content);
}

irr::core::vector3df BomberMap::getRealPos(float x, float y)
{
    core::vector3df pos(x - (((float)w - 1.0) / 2.0), 0.0, y - (((float)h - 1.0) / 2.0));
    return (pos);
}

bool BomberMap::isNight() const
{
    return night;
}

BomberMap::BomberMap(scene::ISceneManager *_smgr, video::IVideoDriver *_driver, int width, int height, bool editMode)
{
    this->editMode = editMode;
    night = false;
    smgr = _smgr;
    driver = _driver;
    w = (width >= MAP_MIN_W && width <= MAP_MAX_W) ? width : MAP_MIN_W;
    h = (height >= MAP_MIN_H && height <= MAP_MAX_H) ? height : MAP_MIN_H;
    floorPath = ASSETS_FOLDER "floor.jpg";
    floorTexture = driver->getTexture(irr::io::path(floorPath.c_str()));
    floorMesh = smgr->getGeometryCreator()->createPlaneMesh(core::dimension2df(1, 1), core::dimension2du(w, h), 0, core::dimension2df(w, h));
    floor = 0;
    lighting = 0;
    skybox = 0;
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            blocs.push_back(new Bloc((float)x,
                                     (float)y,
                                     (x == 0 || y == 0 || x == w - 1 || y == h - 1) ? Bloc::Types::UNBREAKABLE : Bloc::Types::EMPTY));
        }
    }
    smgr->setShadowColor(video::SColor(150, 0, 0, 0));
    addTexture(Bloc::UNBREAKABLE, ASSETS_FOLDER "brick.jpg");
    addTexture(Bloc::BREAKABLE, ASSETS_FOLDER "wooden_crate.jpg");
    updateTexture();
    drawMap();
}

BomberMap::~BomberMap()
{
}
