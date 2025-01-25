#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <windows.h>

#include <utility>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>


#include "Interceptor.h"
#include "Game.h"


class Rock: public sf::ConvexShape{
    public:
        //statyczne tekstury
        static sf::Texture tx;
        static sf::Texture txg;

        //prędkosć standardowa
        float speed;

        //skosy w ruchu
        float direct = 0;

        //wierzchołki konteneru
        std::vector<sf::Vector2f> vertex;

        //trafienie
        bool hit = false;

        //uśmiercenie
        bool kill = false;

        //rotacja 
        int rot;

        //czy leczy
        bool heal = false;

        Rock();

        //nowa pozycja, zmiana animacji
        void Update();

        virtual ~Rock();
};
