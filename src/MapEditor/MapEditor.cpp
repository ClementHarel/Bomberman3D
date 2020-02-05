/*
** EPITECH PROJECT, 2019
** indie_map_edt
** File description:
** MapEditor
*/

#include "MapEditor.hpp"

void MapEditor::setupMapEditorGui()
{
    guienv->clear();

    irr::gui::IGUISkin *skin = guienv->getSkin();
    irr::gui::IGUIFont *font = guienv->getFont(ASSETS_FONT "fontcourier.bmp");
    if (font)
        skin->setFont(font);

    core::rect<s32> bound = core::rect<s32>(0, 0, 300, guienv->getVideoDriver()->getScreenSize().Height / 2);
    IGUIElement *root = guienv->getRootGUIElement();
    IGUITabControl *tab = guienv->addTabControl(core::rect<s32>(2, 20, bound.LowerRightCorner.X - 2, bound.LowerRightCorner.Y - 2), 0, true, true, GUI_ID_TAB_CONTROLER);
    core::vector2di map_size = core::vector2di();

    if (map)
    {
        map_size.X = map->getSize().X;
        map_size.Y = map->getSize().Y;
    }

    // Map tab
    IGUITab *tabMap = tab->addTab(L"Map");
    guienv->addStaticText(L"name :", core::rect<s32>(10, 10, 80, 30), false, false, tabMap);
    guienv->addEditBox(L"unnamed_map", core::rect<s32>(80, 10, 270, 10 + 20), true, tabMap, GUI_ID_MAP_EDT_NAME);
    guienv->addStaticText(L"width :", core::rect<s32>(10, 40, 80, 60), false, false, tabMap);
    guienv->addEditBox(L"", core::rect<s32>(80, 40, 40 + 100, 60), true, tabMap, GUI_ID_MAP_EDT_WIDTH);
    guienv->addStaticText(core::stringw(L"current: ").append(core::stringw(map_size.X)).c_str(), core::rect<s32>(150, 40, 300, 60), false, false, tabMap, GUI_ID_MAP_TXT_WIDTH);
    guienv->addStaticText(L"height:", core::rect<s32>(10, 70, 80, 90), false, false, tabMap);
    guienv->addEditBox(L"", core::rect<s32>(80, 70, 40 + 100, 90), true, tabMap, GUI_ID_MAP_EDT_HEIGHT);
    guienv->addStaticText(core::stringw(L"current: ").append(core::stringw(map_size.X)).c_str(), core::rect<s32>(150, 70, 300, 90), false, false, tabMap, GUI_ID_MAP_TXT_HEIGHT);
    guienv->addButton(core::rect<s32>(10, 100, 140, 120), tabMap, GUI_ID_MAP_BTN_RESIZE,
                      L"Resize", L"Resize the map");
    guienv->addButton(core::rect<s32>(10, 130, 140, 150), tabMap, GUI_ID_MAP_BTN_SAVE,
                      L"Save", L"Saves the map");
    guienv->addButton(core::rect<s32>(10, 160, 140, 180), tabMap, GUI_ID_MAP_BTN_LOAD,
                      L"Load", L"load map");

    // Bloc tab
    IGUITab *tabBloc = tab->addTab(L"Bloc");
    guienv->addStaticText(L"Type :", core::rect<s32>(10, 10, 40, 30), false, false, tabBloc);
    IGUIComboBox *edtSpin_blocType = guienv->addComboBox(core::rect<s32>(10, 30, bound.LowerRightCorner.X / 2 - 10, 80), tabBloc, GUI_ID_BLOC_SELECT);
    edtSpin_blocType->addItem(L"Empty");
    edtSpin_blocType->addItem(L"Breakable");
    edtSpin_blocType->addItem(L"Unbreakable");
    guienv->addStaticText(L"X:", core::rect<s32>(10, 90, 40, 110), false, false, tabBloc);
    guienv->addEditBox(L"", core::rect<s32>(40, 90, 140, 110), true, tabBloc, GUI_ID_BLOC_EDT_X);
    guienv->addStaticText(L"Y:", core::rect<s32>(10, 120, 40, 140), false, false, tabBloc);
    guienv->addEditBox(L"", core::rect<s32>(40, 120, 140, 140), true, tabBloc, GUI_ID_BLOC_EDT_Y);
    guienv->addButton(core::rect<s32>(10, 150, 148 - 5, 170), tabBloc, GUI_ID_BLOC_BTN_PREVIEW, L"Preview", L"preview the action");
    guienv->addButton(core::rect<s32>(148 + 5, 150, 286, 170), tabBloc, GUI_ID_BLOC_BTN_EXEC, L"Execute", L"execute");

    // Texture tab
    IGUITab *tabTexture = tab->addTab(L"Texture");
    guienv->addStaticText(L"Type :", core::rect<s32>(10, 10, 40, 30), false, false, tabTexture);
    IGUIComboBox *edtSpin_texture = guienv->addComboBox(core::rect<s32>(10, 30, bound.LowerRightCorner.X / 2 - 10, 80), tabTexture, GUI_ID_TEXTURE_SELECT);
    edtSpin_texture->addItem(L"Floor");
    edtSpin_texture->addItem(L"Breakable");
    edtSpin_texture->addItem(L"Unbreakable");
    guienv->addButton(core::rect<s32>(10, 90, 140, 110), tabTexture, GUI_ID_TEXTURE_LOAD, L"Load", L"fetch a texture");
    guienv->addStaticText(L"", core::rect<s32>(10, 120, bound.LowerRightCorner.X - 10, 130), false, false, tabTexture, GUI_ID_TEXTURE_TEXT);
    guienv->addImage(core::rect<s32>(10, 140, 10 + bound.LowerRightCorner.X / 2, 140 + bound.LowerRightCorner.X / 2), tabTexture, GUI_ID_TEXTURE_TEX);
    guienv->addButton(core::rect<s32>(10, 150 + bound.LowerRightCorner.X / 2, 140, 170 + bound.LowerRightCorner.X / 2), tabTexture, GUI_ID_TEXTURE_UPDATE, L"Update", L"set the texture");

    // Lighting tab
    IGUITab *tabLighting = tab->addTab(L"Lighting");
    guienv->addStaticText(L"Time Night :", core::rect<s32>(10, 10, 40, 30), false, false, tabLighting);
    guienv->addCheckBox(false, core::rect<s32>(50, 10, 70, 30), tabLighting, GUI_ID_LIGHTING_TIME);
}

void MapEditor::eventHandler(SEvent event)
{
    static int filefetch = 0;
    if (!evt_ready)
        return;
    if (event.EventType == EET_GUI_EVENT)
    {
        IGUIElement *root = guienv->getRootGUIElement();
        s32 id = event.GUIEvent.Caller->getID();

        switch (event.GUIEvent.EventType)
        {
        case EGET_BUTTON_CLICKED:
        {
            switch (id)
            {
            case GUI_ID_MAP_BTN_RESIZE:
            {
                if (map)
                {
                    evt_ready = false;
                    core::stringc s;
                    s = root->getElementFromId(GUI_ID_MAP_EDT_WIDTH, true)->getText();
                    int w = atoi(s.c_str());
                    s = root->getElementFromId(GUI_ID_MAP_EDT_HEIGHT, true)->getText();
                    int h = atoi(s.c_str());
                    if (h >= MAP_MIN_H && w >= MAP_MIN_W && h <= MAP_MAX_H && w <= MAP_MAX_W)
                    {
                        map->resizeMap(w, h);
                        root->getElementFromId(GUI_ID_MAP_TXT_WIDTH, true)->setText((L"current: " + std::to_wstring(map->getSize().X)).c_str());
                        root->getElementFromId(GUI_ID_MAP_TXT_HEIGHT, true)->setText((L"current: " + std::to_wstring(map->getSize().Y)).c_str());
                    }
                }
                break;
            }
            case GUI_ID_MAP_BTN_SAVE:
            {
                if (map)
                {
                    evt_ready = false;
                    core::stringc s;
                    s = root->getElementFromId(GUI_ID_MAP_EDT_NAME, true)->getText();
                    map->saveMap(s.c_str());
                }
                break;
            }
            case GUI_ID_MAP_BTN_LOAD:
            {
                filefetch = 1;
                guienv->addFileOpenDialog(L"Please choose a map.", true, 0, -1, true, (irr::c8 *)SAVES_MAP);
                break;
            }
            case GUI_ID_BLOC_BTN_EXEC:
            {
                if (map)
                {
                    evt_ready = false;
                    core::stringc s;
                    s = root->getElementFromId(GUI_ID_BLOC_EDT_X, true)->getText();
                    int x = atoi(s.c_str());
                    s = root->getElementFromId(GUI_ID_BLOC_EDT_Y, true)->getText();
                    int y = atoi(s.c_str());
                    int type = ((IGUIComboBox *)root->getElementFromId(GUI_ID_BLOC_SELECT, true))->getSelected();
                    if (x >= 0 && y >= 0 && x < map->getSize().X && y < map->getSize().Y)
                    {
                        map->setBlocAt(x, y, (Bloc::Types)type);
                    }
                }
                break;
            }
            case GUI_ID_TEXTURE_LOAD:
            {
                filefetch = 2;
                guienv->addFileOpenDialog(L"Please choose a texture.", true, 0, -1, true);
                break;
            }
            case GUI_ID_TEXTURE_UPDATE:
            {
                int type = ((IGUIComboBox *)root->getElementFromId(GUI_ID_TEXTURE_SELECT, true))->getSelected();
                core::stringc s;
                s = root->getElementFromId(GUI_ID_TEXTURE_TEXT, true)->getText();
                if (type == 0)
                    map->setFloorTexture(s.c_str());
                else
                {
                    map->setTexture((Bloc::Types)type, s.c_str());
                }
            }
            default:
                break;
            }
            break;
        }
        case EGET_CHECKBOX_CHANGED:
        {
            switch (id)
            {
            case GUI_ID_LIGHTING_TIME:
            {
                gui::IGUICheckBox *box = (irr::gui::IGUICheckBox *)root->getElementFromId(GUI_ID_LIGHTING_TIME, true);
                map->setNight(box->isChecked());
                break;
            }
            default:
                break;
            }
            break;
        }
        }
        if (event.GUIEvent.EventType == EGET_FILE_SELECTED)
        {
            fileSys->changeWorkingDirectoryTo(cwd);
            evt_ready = false;
            IGUIFileOpenDialog *dialog = (IGUIFileOpenDialog *)event.GUIEvent.Caller;
            core::stringw str = core::stringw(dialog->getFileName());
            if (filefetch == 1)
            {
                if (map->loadMap(wcharToString(str.c_str())))
                {
                    root->getElementFromId(GUI_ID_MAP_EDT_NAME, true)->setText(str.subString(str.findLastChar(L"/") + 1, str.findLastChar(L".") - str.findLastChar(L"/") - 1).c_str());
                    root->getElementFromId(GUI_ID_MAP_TXT_WIDTH, true)->setText((L"current: " + std::to_wstring(map->getSize().X)).c_str());
                    root->getElementFromId(GUI_ID_MAP_TXT_HEIGHT, true)->setText((L"current: " + std::to_wstring(map->getSize().Y)).c_str());
                    ((irr::gui::IGUICheckBox *)root->getElementFromId(GUI_ID_LIGHTING_TIME, true))->setChecked(map->isNight());
                }
            }
            else if (filefetch == 2)
            {
                root->getElementFromId(GUI_ID_TEXTURE_TEXT, true)->setText(str.c_str());
                video::ITexture *tex = driver->getTexture(str);
                ((IGUIImage *)root->getElementFromId(GUI_ID_TEXTURE_TEX, true))->setImage(tex);
                ((IGUIImage *)root->getElementFromId(GUI_ID_TEXTURE_TEX, true))->setScaleImage(true);
            }
        }
    }
    evt_ready = true;
}

void MapEditor::editorLoop(IrrlichtDevice *device)
{
    core::vector3df cam1_pos(M_PI_2 / 1.5, -M_PI_2, 0);
    float cam_dist = 10;

    smgr->getActiveCamera()->setFarValue(10000.0f);

    while (device->run() && driver)
    {
        cam_dist = (float)std::max(map->getSize().X, map->getSize().Y) * 1;
        eventHandler(receiver->event);
        if (receiver->IsKeyDown(irr::KEY_LEFT))
        {
            cam1_pos.Y -= CAM_MOV_SPEED;
        }
        else if (receiver->IsKeyDown(irr::KEY_RIGHT))
            cam1_pos.Y += CAM_MOV_SPEED;
        if (receiver->IsKeyDown(irr::KEY_ESCAPE))
        {
            break;
        }
        driver->beginScene(true, true, video::SColor(255, 0, 0, 255));
        smgr->getActiveCamera()->setPosition(core::vector3df(
            cos(cam1_pos.X) * cos(cam1_pos.Y) * cam_dist,
            sin(cam1_pos.X) * cam_dist,
            cos(cam1_pos.X) * sin(cam1_pos.Y) * cam_dist));
        smgr->drawAll();
        guienv->drawAll();
        driver->endScene();
    }
}

MapEditor::MapEditor(IrrlichtDevice *device)
{
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();
    fileSys = device->getFileSystem();
    evt_ready = true;

    if (!(receiver = (MyEventReceiver *)device->getEventReceiver()))
    {
        receiver = new MyEventReceiver();
        device->setEventReceiver(receiver);
    }

    smgr->clear();

    map = new BomberMap(smgr, driver, 5, 5, true);

    setupMapEditorGui();

    smgr->addCameraSceneNode(0, core::vector3df(0, 0, 0), core::vector3df(0, 0, 0), 1, true);
    smgr->getActiveCamera()->setFOV(M_PI_2);

    cwd = fileSys->getWorkingDirectory();

    editorLoop(device);

    smgr->clear();
    guienv->clear();
    receiver->resetKeys();
}

MapEditor::~MapEditor()
{
}