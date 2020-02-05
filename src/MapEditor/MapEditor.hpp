/*
** EPITECH PROJECT, 2019
** indie_map_edt
** File description:
** MapEditor
*/

#ifndef MAPEDITOR_HPP_
#define MAPEDITOR_HPP_

#include "../include/irrlicht.h"
#include "BomberMap.hpp"
#include "../MyEventReceiver.hpp"
#include <string>
#include <stdio.h>
#include <vector>
#include <iostream>
using namespace irr;
using namespace gui;

#define SRC_PATH "../src/"
#define CAM_MOV_SPEED (M_PI_2 / 128);

enum
{
    GUI_ID_DIALOG_ROOT_WINDOW = 0x10000,

    GUI_ID_TAB_CONTROLER,

    GUI_ID_MAP_EDT_NAME,
    GUI_ID_MAP_EDT_WIDTH,
    GUI_ID_MAP_EDT_HEIGHT,
    GUI_ID_MAP_TXT_WIDTH,
    GUI_ID_MAP_TXT_HEIGHT,
    GUI_ID_MAP_BTN_RESIZE,
    GUI_ID_MAP_BTN_SAVE,
    GUI_ID_MAP_BTN_LOAD,

    GUI_ID_BLOC_SELECT,
    GUI_ID_BLOC_EDT_X,
    GUI_ID_BLOC_EDT_Y,
    GUI_ID_BLOC_BTN_PREVIEW,
    GUI_ID_BLOC_BTN_EXEC,

    GUI_ID_TEXTURE_SELECT,
    GUI_ID_TEXTURE_LOAD,
    GUI_ID_TEXTURE_TEX,
    GUI_ID_TEXTURE_TEXT,
    GUI_ID_TEXTURE_UPDATE,

    GUI_ID_LIGHTING_TIME
};

class MapEditor
{
public:
    std::string wcharToString(const wchar_t *txt)
    {
        std::wstring ws(txt);
        std::string str(ws.begin(), ws.end());
        return (str);
    }
    MapEditor(IrrlichtDevice *device);
    ~MapEditor();
    void eventHandler(SEvent event);
    void editorLoop(IrrlichtDevice *device);
    void setupMapEditorGui();

protected:
private:
    MyEventReceiver *receiver;
    video::IVideoDriver *driver;
    scene::ISceneManager *smgr;
    IGUIEnvironment *guienv;
    irr::io::IFileSystem *fileSys;
    BomberMap *map;
    irr::io::path cwd;
    bool evt_ready;
};

#endif /* !MAPEDITOR_HPP_ */
