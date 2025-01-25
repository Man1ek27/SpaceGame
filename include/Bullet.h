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

#include "Game.h"
#include "Interceptor.h"

class Bullet;
class Interceptor;
class MySounds;
class Game;
class Rock;

class Bullet: public sf::CircleShape{
    public:
        //statyczna tekstura
        static sf::Texture tx;

        //rozmiary klatki animacji
        static unsigned int xy;
        //klatek animacji
        unsigned int frames = 6*3;

        //predkosc standardowa
        float speed = 3;

        //statek wlascicel
        Interceptor *interceptor;

        //czy usuwa pocisk
        bool kill = false;

        Bullet(const sf::Vector2f &, Interceptor*);

        //nowa pozycja, zmiana animacji
        void Update(Game &);

        //test kolizji
        bool collTest();

        virtual ~Bullet();
};