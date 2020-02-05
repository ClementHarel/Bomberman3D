/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** MyEventREceiver
*/

#ifndef MYEVENTRECEIVER_HPP_
#define MYEVENTRECEIVER_HPP_

#include "./include/irrlicht.h"

using namespace irr;

class MyEventReceiver : public IEventReceiver
{
public:
    MyEventReceiver()
    {
        for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
            KeyIsDown[i] = false;
    }
    virtual bool OnEvent(const SEvent &event)
    {
        this->event = event;
        if (event.EventType == irr::EET_KEY_INPUT_EVENT)
        {
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
        }
        return false;
    }
    virtual bool IsKeyDown(EKEY_CODE keyCode) const
    {
        return KeyIsDown[keyCode];
    }
    virtual bool resetKeys()
    {
        event.EventType = irr::EEVENT_TYPE::EET_USER_EVENT;
        for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
            KeyIsDown[i] = false;
        return false;
    }
    SEvent event;

private:
    bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

#endif