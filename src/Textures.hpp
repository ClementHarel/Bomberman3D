/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Textures
*/

#ifndef TEXTURES_HPP_
#define TEXTURES_HPP_

#include "Path.h"
#include "include/irrlicht.h"
#include <string>
#include <stdio.h>
#include <vector>

using namespace irr;

template <class T>
class Textures
{
public:
    class Texture
    {
    public:
        Texture(T _type, std::string _path)
        {
            type = _type;
            path = _path;
            texture = 0;
        }
        ~Texture(){};

        void updateTexture(video::IVideoDriver *driver)
        {
            texture = driver->getTexture(path.c_str());
        };
        std::string toString()
        {
            std::string str = "";
            str.append(std::to_string(type));
            str.append(","+path);
            return (str);
        }
        T type;
        std::string path;
        video::ITexture *texture;
    };

    std::vector<Texture*> list;

    Textures(){};
    ~Textures(){};

    void updateTextures(video::IVideoDriver *driver)
    {
        for (Texture *i : list)
        {
            i->updateTexture(driver);
        }
    };

    Texture *getTextureFromType(T type)
    {
        for (Texture *i : list)
        {
            if (i->type == type)
                return (i);
        }
        return (nullptr);
    }

    video::ITexture *getITextureFromType(T type)
    {
        for (Texture *i : list)
        {
            if (i->type == type)
                return (i->texture);
        }
        return (nullptr);
    }

    std::string toString()
    {
        std::string str = "textures:"+std::to_string(list.size())+"\n";
        for (Texture *i : list)
        {
            str.append(i->toString()+"\n");
        }
        return (str);
    }
};

#endif /* !TEXTURE_HPP_ */
