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


class Interceptor: public sf::Sprite{
    public:
        //Dzwiek statku
        MySounds *sounds;
        
        //prędkość statku (po osi Y)
        float speed = 0;

        //o ile podnoszę preędkosc podczas skoku
        float boostSpeed = 4;

        //stopień zwalaniania przyspieszenia 
        float speedCooldown = 50;

        //obrazek do tekstury
        sf::Image skin;
        //tekstura 
        sf::Texture skinTx;

        //punkty które wchodzą w kolizje
        std::vector <sf::Vector2i> localColisionPoints;

        //HP
        int hp = 30;
        int hpmax = 30;
        sf::RectangleShape hpRect;
        //dlugość paska zdrowia
        float hpRectX = 50;
        
        //kontener z pociskami
        std::vector <Bullet> bullets;
        //magazynek
        unsigned int maxBullets = 12;

        //czcionka z punktami
        sf::Text strPt;
        //czcionka z HP
        sf::Text strHp;
        // punkty
        int pt = 0;

        //srting stream do przeróbek 
        std::stringstream ss;
        //zniszczony przed końcem gry
        bool destry = true;

        //piksele do animacji dymu silnika
        sf::Vertex smoke[100];

        //Konstruktor dziedziczy po sf::Sprite
        Interceptor();
        //rysujemy Interceptor i jego podelementy Game &
        void Paint(Game &);

        //przechwyć i obsłuż zdarzenia sf::Event &
        void CatchEvents(sf::Event &e);

        //update pozycji 
        void Update(Game &);

        //shoot
        void Shoot();

        virtual ~Interceptor();

    
};