#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <windows.h>
#include <map>

#include <utility>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "Game.h"
#define MAX_SOUNDS 40

class MySounds {
public:
   std::string folder="../sounds";
   std::string type="wav";
   std::map <std::string,sf::SoundBuffer> bufor;
   static sf::Sound sounds[MAX_SOUNDS];

   MySounds();
   virtual ~MySounds();
   MySounds(const std::string);
   MySounds(const std::string, std::initializer_list<std::string> );
   void AddFile(const std::string &);
   int Play(std::string, int , bool , float , bool);
   int Play(std::string, int , bool , float , bool, float , const sf::Vector2f &, float );
   static void StopAll();
   static void PauseAll();
   static void UnpauseAll();
   static void PlayOnce(sf::Sound *, int , bool , float , bool, float , const sf::Vector2f &, float );
};