/*
** EPITECH PROJECT, 2019
** IndieStudio
** File description:
** IndieStudio
*/

#include "IndieStudio.h"
#include "Title.h"
#include <exception>

void IndieStudio::gameLoop(MyEventReceiver *receiver)
{
    Title *title = new Title(this->driver, this->guienv, this->device);

    while (this->device->run())
    {
        this->driver->beginScene(true, true, SColor(0, 0, 0, 0));
        if (title)
        {
            title->getInput(receiver, this->device);
            title->drawAll(this->driver);
        }
        this->smgr->drawAll();
        this->guienv->drawAll();
        this->driver->endScene();
        this->device->yield();
    }
}
IndieStudio::IndieStudio()
{
    MyEventReceiver receiver;
    this->width = 1920;
    this->height = 1080;
    this->device = createDevice(video::EDT_OPENGL, dimension2d<u32>(this->width, this->height), 16, false, false, false, &receiver);
    this->device->maximizeWindow();
    this->device->setWindowCaption(L"Indie Studio");
    this->driver = this->device->getVideoDriver();
    this->smgr = this->device->getSceneManager();
    this->guienv = this->device->getGUIEnvironment();
    if (!this->device)
        exit(1);
    this->gameLoop(&receiver);
}

IndieStudio::~IndieStudio()
{
    this->device->drop();
}
