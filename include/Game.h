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
#include <windows.h>

#include "Interceptor.h"
#include "Bullet.h"
#include "Interceptor.h"
#include "Rock.h"

#define FRAME_LIMIT 75

#define POSX 10
#define POSY 10

#define SCREENX 800
#define SCREENY 600

#define CBITS 32
#define MAX_SOUNDS 40

class Bullet;
class Interceptor;
class MySounds;
class Game;
class Rock;




/// GAME 

class Game: public sf::RenderWindow{
    public:
        //zegar gry
        static sf::Clock clock;

        //Rock kontener na przeszkody 
        static std::vector<Rock> Rocks;

        //Stars
        static sf::Vertex va[1000];

        //Font
        static sf::Font Font;

        //napis konczowy
        sf::Text gg;

        //upływ czasu
        sf::Text strT;

        //Tło 
        sf::Texture bgtx;
        sf::RectangleShape bg;

        //pauza
        bool pause = true;

        //bufor
        std::stringstream ss;

        //maksymalny czas gry
        sf::Time t;


        ///Konstruktor głowny
        Game() = default;
        Game(sf::String);

        //uruchamia gre
        void Run();

        //Wlacza tryb Game Over
        void Gogg(Interceptor *);

        //obsługuje konsole - statyczna
        static void StartConsole();

        //pobiera uchwyt do okna i pozwala obsłużyć okno z wykorzytsaniem biblioteki window.h
        static void SetSFMLWindow(HWND gw);
        static bool IsPointInsidePolygon(std::vector <sf::Vector2f> &, const sf::Vector2f &);
        static bool IsPointInsidePolygon(const sf::Transform &, std::vector<sf::Vector2f> &, const sf::Vector2f &);
        static void UpdateRocks(Game &);
        virtual ~Game();
};

