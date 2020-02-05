/*
** EPITECH PROJECT, 2019
** IndieStudio
** File description:
** Title
*/

#ifndef TITLE_H_
#define TITLE_H_
#include "IndieStudio.h"
#include "Game.hpp"
#include <string>

enum
{
    GUI_ID_EDITBOX,
    GUI_ID_OK,

    GUI_ID_FILE_P1,
    GUI_ID_FILE_P2,
    GUI_ID_FILE_MAP,
	GUI_ID_FILE_OPTIONS
};

class Title
{
public :
	Title(IVideoDriver *driver, IGUIEnvironment *guienv, irr::IrrlichtDevice *device);
	~Title();
	void submenu(IrrlichtDevice *device);
	void drawTitle(IVideoDriver *driver);
	void drawCursor(IVideoDriver *driver);
	void drawAll(IVideoDriver *driver);
	void getInput(MyEventReceiver *receiver, IrrlichtDevice *device);
	void howToPlay(IVideoDriver *driver);
	void drawHtp(IVideoDriver *driver);
	void changeSkin(int skin);
	void drawProfile(IVideoDriver *driver);
	void drawOptions(IVideoDriver *driver);
	void eventHandler(SEvent event);
	void fillPlayer(std::string str, std::string str2, std::string map);
	int getCursorPos();
private :
    irr::IrrlichtDevice *device;
	IVideoDriver *driver;
	IGUIEnvironment *guienv;
	video::ITexture* titleScreen;
	video::ITexture* htp1;
	video::ITexture* htp2;
	video::ITexture* htp3;
	video::ITexture* profilePng;
	video::ITexture* cursor;
	video::ITexture* cursorBis;
	video::ITexture* optionsText;
	irr::gui::IGUIStaticText *winText;
	std::string options_player;
	IGUIElement *root;
    irr::io::IFileSystem *fileSys;
    irr::io::path cwd;
	int nbOfSkins;
	bool htp;
	bool play;
	bool profile;
	bool option;
	int cursorPos;
	int *cursorTabWidth;
	int *cursorTabHeight;
	int *cursorTabWidthProfile;
	int *cursorTabHeightProfile;
	int *cursorTabWidthOptions;
	int *cursorTabHeightOptions;
	int sleep;
	int bomberPoints;
    sf::Music *music;

protected:
};

#endif /* !TITLE_H_ */
