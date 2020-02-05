/*
** EPITECH PROJECT, 2019
** IndieStudio
** File description:
** Title
*/

#include "Title.h"
#include <fstream>

void Title::changeSkin(int skin)
{
    std::string str;
    std::fstream f;
    std::string name;
    std::string bomberPoints;

    f.open(this->options_player, std::fstream::in | std::fstream::out);
    std::cout << this->options_player << std::endl;
    getline(f, name);
    getline(f, bomberPoints);
    std::cout << name << std::endl;

    f.close();

    std::fstream f2;
    f2.open(this->options_player, std::fstream::out);
    f2 << name << "\n"
       << bomberPoints << "\n"
       << skin << "\n"
       << skin << "\n"
       << skin << std::endl;
    f2.close();
}

std::string cleanNameBis(std::string str)
{
    int i = str.size();

    while (str[i] != '/' || i == 0)
        i--;
    return str.substr(i + 1, str.size() - i + 1);
}

void Title::fillPlayer(std::string str, std::string str2, std::string map)
{
    fileSys->changeWorkingDirectoryTo(cwd);

    Player *player1 = new Player(device);
    Player *player2 = new Player(device);

    std::fstream f;
    std::fstream grimaldi;
    player1->skins = new int[this->nbOfSkins]{0, 0, 0};
    player2->skins = new int[this->nbOfSkins]{0, 0, 0};

    f.open(str, std::fstream::in | std::fstream::out | std::fstream::app);
    getline(f, str);
    player1->name = str;
    getline(f, str);
    player1->bomberPoints = std::atoi(str.c_str());
    getline(f, str);
    for (int i = 0; i < this->nbOfSkins; i++)
    {
        player1->skins[i] = std::atoi(str.c_str());
        getline(f, str);
    }
    grimaldi.open(str2, std::fstream::in | std::fstream::out | std::fstream::app);
    getline(grimaldi, str2);
    player2->name = str2;
    getline(grimaldi, str2);
    player2->bomberPoints = std::atoi(str2.c_str());
    getline(grimaldi, str2);
    for (int i = 0; i < this->nbOfSkins; i++)
    {
        player2->skins[i] = std::atoi(str2.c_str());
        getline(f, str2);
    }
    MyEventReceiver *evt_receiver = (MyEventReceiver *)device->getEventReceiver();
//    std::cout << device->getFileSystem()->getWorkingDirectory();
    Game *game = new Game(player1, player2, map, this->device);
    device->setEventReceiver(evt_receiver);
    std::cerr << "game end" << std::endl;
}

void Title::eventHandler(SEvent event)
{
    static std::string str;
    static std::string str2;
    fileSys->changeWorkingDirectoryTo(cwd);

    if (event.EventType == EET_GUI_EVENT)
    {
        s32 id = event.GUIEvent.Caller->getID();
        switch (event.GUIEvent.EventType)
        {
        case EGET_FILE_SELECTED:
        {
            IGUIFileOpenDialog *dialog = (IGUIFileOpenDialog *)event.GUIEvent.Caller;
            core::stringw txt = core::stringw(dialog->getFileName());
            std::wstring ws(txt.c_str());
            std::string temp_str(ws.begin(), ws.end());

            switch (id)
            {
            case GUI_ID_FILE_OPTIONS:
            {
                IGUIFileOpenDialog *dialog3 = (IGUIFileOpenDialog *)event.GUIEvent.Caller;
                core::stringw txt3 = core::stringw(dialog3->getFileName());
                std::wstring ws3(txt3.c_str());
                std::string temp_str3(ws3.begin(), ws3.end());
                this->options_player = temp_str3;
                std::fstream f;
                f.open(temp_str3, std::fstream::in | std::fstream::out | std::fstream::app);
                getline(f, temp_str3);
                getline(f, temp_str3);
                this->bomberPoints = std::atoi(temp_str3.c_str());
                std::wstring wsTmp4(temp_str3.begin(), temp_str3.end());
                this->winText = guienv->addStaticText(wsTmp4.c_str(), irr::core::recti(900, 452, 900 + 96, 452 + 87), true, false, 0, -1, true);
                this->option = true;
                break;
            }
            case GUI_ID_FILE_P1:
            {
                str = temp_str;
                guienv->addFileOpenDialog(L"Please choose the second player profile.", true, 0, GUI_ID_FILE_P2, true, (irr::c8 *)SAVES_PROFILE);
                break;
            }
            case GUI_ID_FILE_P2:
            {
                str2 = temp_str;
                guienv->addFileOpenDialog(L"Please choose the map", true, 0, GUI_ID_FILE_MAP, true, (irr::c8 *)SAVES_MAP);
                break;
            }
            case GUI_ID_FILE_MAP:
            {
                std::string str3 = temp_str;
                fillPlayer(str, str2, str3);
                this->htp = false;
                this->play = false;
                this->profile = false;
                return;
            }
            default:
                break;
            }
        }
        case EGET_BUTTON_CLICKED:
        {
            switch (id)
            {
            case GUI_ID_OK:
            {
                core::stringc s;
                if (this->root->getElementFromId(GUI_ID_EDITBOX, true))
                {
                    s = this->root->getElementFromId(GUI_ID_EDITBOX, true)->getText();
                }
                std::fstream f;
                std::string str = SAVES_PROFILE;
                str = str + s.c_str();
                f.open(str, std::fstream::in | std::fstream::out | std::fstream::app);
                f << s.c_str() << "\n"
                  << "0\n0\n0\n0\n";
                f.close();
            }
            default:
                break;
            }
        }
        default:
            break;
        }
    }
}

void Title::submenu(IrrlichtDevice *device)
{
    if (this->htp == true || this->option == true)
        return;
    if (this->profile == true)
    {
        if (this->cursorPos == 0)
        {
            // Ask Name
            if (this->play == false)
            {
                if (!this->root->getElementFromId(GUI_ID_EDITBOX, true))
                    this->guienv->addEditBox(L"Player", rect<s32>(600, 388, 1585, 561), true, 0, GUI_ID_EDITBOX);
                if (!this->root->getElementFromId(GUI_ID_OK, true))
                    this->guienv->addButton(rect<s32>(687, 781, 1468, 895), 0, GUI_ID_OK, L"OK");
                this->play = true;
            }
        }
        else
        {
            // Load Profile
            if (!this->root->getElementFromId(GUI_ID_FILE_P1, true))
            {
                guienv->addFileOpenDialog(L"Please choose the first player profile.", true, 0, GUI_ID_FILE_P1, true, (irr::c8 *)SAVES_PROFILE);
            }
            // LAUNCH GAME
        }
        return;
    }
    switch (this->cursorPos)
    {
    case (0):
        if (profile == true)
            profile = false;
        else
            profile = true;
        break;
    case (1):
    {
        MyEventReceiver *evt_receiver = (MyEventReceiver *)device->getEventReceiver();
        MapEditor editor(device);
        evt_receiver->resetKeys();
        device->setEventReceiver(evt_receiver);
        break;
    }
    case (2):
    {
        this->cursorPos = 0;
        if (htp == true)
            htp = false;
        else
            htp = true;
        break;
    }
    case (3):
        this->cursorPos = 0;
        if (option == true)
            option = false;
        else
        {
            guienv->addFileOpenDialog(L"Please choose your profile", true, 0, GUI_ID_FILE_OPTIONS, true, (irr::c8 *)SAVES_PROFILE);
        }
        break;
        break;
    case (4):
        exit(0);
        break;
    }
}

void Title::getInput(MyEventReceiver *receiver, IrrlichtDevice *device)
{
    static int time = this->sleep;

    if (time <= 0)
    {
        if (receiver->IsKeyDown(irr::KEY_UP) && this->htp == false && this->profile == false && this->option == false)
        {
            this->cursorPos--;
            time = this->sleep;
            if (this->cursorPos < 0)
                this->cursorPos = 4;
        }
        if (receiver->IsKeyDown(irr::KEY_DOWN) && this->htp == false && this->profile == false && this->option == false)
        {
            this->cursorPos++;
            time = this->sleep;
            if (this->cursorPos > 4)
                this->cursorPos = 0;
        }
        if (receiver->IsKeyDown(irr::KEY_SPACE) || receiver->IsKeyDown(irr::KEY_RETURN))
        {

            this->submenu(device);
            time = this->sleep;
        }
        if (receiver->IsKeyDown(irr::KEY_DOWN) && this->cursorPos != 2 && this->htp == true)
        {
            this->cursorPos++;
            time = this->sleep;
        }
        if (receiver->IsKeyDown(irr::KEY_UP) && this->cursorPos != 0 && this->htp == true)
        {
            this->cursorPos--;
            time = this->sleep;
        }
        if (receiver->IsKeyDown(irr::KEY_ESCAPE) && this->htp == true)
        {
            this->cursorPos = 0;
            this->htp = false;
            time = this->sleep;
            if (this->root->getElementFromId(GUI_ID_FILE_P1, true))
                this->root->removeChild(this->root->getElementFromId(GUI_ID_FILE_P1, true));
        }
        if (receiver->IsKeyDown(irr::KEY_ESCAPE))
        {
            if (this->root->getElementFromId(GUI_ID_EDITBOX, true))
                this->root->removeChild(this->root->getElementFromId(GUI_ID_EDITBOX, true));
            if (this->root->getElementFromId(GUI_ID_OK, true))
                this->root->removeChild(this->root->getElementFromId(GUI_ID_OK, true));
            if (this->root->getElementFromId(GUI_ID_FILE_P1, true))
                this->root->removeChild(this->root->getElementFromId(GUI_ID_FILE_P1, true));
        }
        if (receiver->IsKeyDown(irr::KEY_ESCAPE) && this->profile == true)
        {
            this->cursorPos = 0;
            if (this->play == true)
            {
                this->play = false;
                time = this->sleep;
            }
            else
            {
                this->profile = false;
            }
        }
        if (receiver->IsKeyDown(irr::KEY_ESCAPE) && this->option == true)
        {
            this->cursorPos = 0;
            this->option = false;
            winText->setText(L"");
            winText->remove();
            time = this->sleep;
        }
        if (receiver->IsKeyDown(irr::KEY_RIGHT) && this->option == true && this->cursorPos != 2)
        {
            this->cursorPos++;
            time = this->sleep;
        }
        if (receiver->IsKeyDown(irr::KEY_LEFT) && this->option == true && this->cursorPos != 0)
        {
            this->cursorPos--;
            time = this->sleep;
        }
        if ((receiver->IsKeyDown(irr::KEY_UP) || receiver->IsKeyDown(irr::KEY_DOWN)) && this->profile == true)
        {
            time = this->sleep;
            if (this->cursorPos == 0)
                this->cursorPos = 1;
            else
                this->cursorPos = 0;
        }
        if ((receiver->IsKeyDown(irr::KEY_RETURN) || receiver->IsKeyDown(irr::KEY_SPACE)) && this->option == true)
        {
            if (this->cursorPos == 0)
                changeSkin(0);
            if (this->cursorPos == 1 && this->bomberPoints >= 250)
                changeSkin(1);
            if (this->cursorPos == 2 && this->bomberPoints >= 999)
                changeSkin(2);
            else
            {
                // Error sound maybe
            }

            time = this->sleep;
        }

        eventHandler(receiver->event);
        receiver->event.EventType = irr::EEVENT_TYPE::EET_USER_EVENT;
    }
    else
        time--;
}

void Title::drawAll(IVideoDriver *driver)
{
    if (htp == true && option == false)
        drawHtp(driver);
    else if (option == true)
        drawOptions(driver);
    else if (profile == true)
        drawProfile(driver);
    else
    {
        drawTitle(driver);
        drawCursor(driver);
    }
}

void Title::drawProfile(IVideoDriver *driver)
{
    driver->draw2DImage(profilePng, core::position2d<s32>(0, 0),
                        core::rect<s32>(0, 0, titleScreen->getOriginalSize().Width,
                                        titleScreen->getOriginalSize().Height),
                        0,
                        video::SColor(255, 255, 255, 255), true);
    driver->draw2DImage(cursor, core::position2d<s32>(this->cursorTabWidthProfile[this->cursorPos], this->cursorTabHeightProfile[this->cursorPos]),
                        core::rect<s32>(0, 0, cursor->getOriginalSize().Width,
                                        cursor->getOriginalSize().Height),
                        0,
                        video::SColor(255, 255, 255, 255), true);
}

void Title::drawCursor(IVideoDriver *driver)
{
    driver->draw2DImage(cursor, core::position2d<s32>(this->cursorTabWidth[this->cursorPos], this->cursorTabHeight[this->cursorPos]),
                        core::rect<s32>(0, 0, cursor->getOriginalSize().Width,
                                        cursor->getOriginalSize().Height),
                        0,
                        video::SColor(255, 255, 255, 255), true);
}

void Title::drawOptions(IVideoDriver *driver)
{

    driver->draw2DImage(optionsText, core::position2d<s32>(0, 0),
                        core::rect<s32>(0, 0, optionsText->getOriginalSize().Width,
                                        optionsText->getOriginalSize().Height),
                        0,
                        video::SColor(255, 255, 255, 255), true);
    driver->draw2DImage(cursorBis, core::position2d<s32>(this->cursorTabWidthOptions[this->cursorPos], this->cursorTabHeightOptions[this->cursorPos]),
                        core::rect<s32>(0, 0, cursorBis->getOriginalSize().Width,
                                        cursorBis->getOriginalSize().Height),
                        0,
                        video::SColor(255, 255, 255, 255), true);
}

void Title::drawHtp(IVideoDriver *driver)
{
    if (this->cursorPos == 0)
        driver->draw2DImage(htp1, core::position2d<s32>(0, 0),
                            core::rect<s32>(0, 0, titleScreen->getOriginalSize().Width,
                                            titleScreen->getOriginalSize().Height),
                            0,
                            video::SColor(255, 255, 255, 255), true);
    if (this->cursorPos == 1)
        driver->draw2DImage(htp2, core::position2d<s32>(0, 0),
                            core::rect<s32>(0, 0, titleScreen->getOriginalSize().Width,
                                            titleScreen->getOriginalSize().Height),
                            0,
                            video::SColor(255, 255, 255, 255), true);
    if (this->cursorPos == 2)
        driver->draw2DImage(htp3, core::position2d<s32>(0, 0),
                            core::rect<s32>(0, 0, titleScreen->getOriginalSize().Width,
                                            titleScreen->getOriginalSize().Height),
                            0,
                            video::SColor(255, 255, 255, 255), true);
}

void Title::drawTitle(IVideoDriver *driver)
{
    driver->draw2DImage(titleScreen, core::position2d<s32>(0, 0),
                        core::rect<s32>(0, 0, titleScreen->getOriginalSize().Width,
                                        titleScreen->getOriginalSize().Height),
                        0,
                        video::SColor(255, 255, 255, 255), true);
}

Title::Title(IVideoDriver *driver, IGUIEnvironment *guienv, irr::IrrlichtDevice *device)
{
    this->nbOfSkins = 3;
    this->htp = false;
    this->device = device;
    this->guienv = guienv;
    this->option = false;
    this->profile = false;
    this->driver = driver;
    this->sleep = 20;
    this->cursorPos = 0;
    this->cursorTabWidth = new int[5]{669, 633, 500, 630, 679};
    this->cursorTabHeight = new int[5]{375, 569, 666, 770, 990};
    this->cursorTabWidthProfile = new int[2]{550, 600};
    this->cursorTabHeightProfile = new int[2]{469, 800};
    this->cursorTabWidthOptions = new int[3]{590, 815, 1032};
    this->cursorTabHeightOptions = new int[3]{775, 775, 775};
    this->titleScreen = this->driver->getTexture(ASSETS_FOLDER "title.png");
    this->cursor = this->driver->getTexture(ASSETS_FOLDER "arrow_right.png");
    this->cursorBis = this->driver->getTexture(ASSETS_FOLDER "arrow_down.png");
    this->htp1 = this->driver->getTexture(ASSETS_FOLDER "htp1.png");
    this->htp2 = this->driver->getTexture(ASSETS_FOLDER "htp2.png");
    this->htp3 = this->driver->getTexture(ASSETS_FOLDER "htp3.png");
    this->optionsText = this->driver->getTexture(ASSETS_FOLDER "options.png");
    this->profilePng = this->driver->getTexture(ASSETS_FOLDER "profile.png");
    this->driver->makeColorKeyTexture(titleScreen, core::position2d<s32>(0, 0));
    this->driver->makeColorKeyTexture(cursor, core::position2d<s32>(0, 0));
    this->root = this->guienv->getRootGUIElement();
    fileSys = device->getFileSystem();
    cwd = fileSys->getWorkingDirectory();

    irr::gui::IGUISkin *skin = guienv->getSkin();
    irr::gui::IGUIFont *font = guienv->getFont(ASSETS_FONT "fontcourier.bmp");
    if (font)
        skin->setFont(font);
    this->music = new sf::Music();
    music->openFromFile(ASSETS_SOUNDS "title.ogg");
    music->setLoop(true);
    music->setVolume(50);
    music->play();
}

Title::~Title()
{
}

int Title::getCursorPos()
{
    return this->cursorPos;
}
