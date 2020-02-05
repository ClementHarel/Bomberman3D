/*
** EPITECH PROJECT, 2019
** IndieStudio
** File description:
** IndieStudio
*/

#ifndef INDIE_HPP
#define INDIE_HPP

#include "Resource.h"
#include "MapEditor/MapEditor.hpp"
#include "MyEventReceiver.hpp"
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "./include/irrlicht.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

class IndieStudio
{
public:
    IndieStudio();
    ~IndieStudio();
    void gameLoop(MyEventReceiver *receiver);

private:
    int width;
    int height;
    IrrlichtDevice *device;
    IVideoDriver *driver;
    ISceneManager *smgr;
    IGUIEnvironment *guienv;

protected:
};

#endif