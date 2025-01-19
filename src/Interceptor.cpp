#include "../include/Interceptor.h"

Interceptor::Interceptor() : sf::Sprite(){
    this->skin.loadFromFile("../images/interceptor.png");
    for(unsigned int x = 0; x < this->skin.getSize().x; x++){
        for(unsigned int y = 0; y < this->skin.getSize().y; y++){
            if(this->skin.getPixel(x, y) == sf::Color(255, 0, 0, 255)){
                this->localColisionPoints.push_back(sf::Vector2i(x,y));
                this->skin.setPixel(x, y, sf::Color(0xffffff00)); // czerwony kasacja
            }
        }
    }

    this->skinTx.loadFromImage(this->skin, sf::IntRect(0,0, this->skin.getSize().x, this->skin.getSize().y));
    this->setTexture(this->skinTx);
    this->setOrigin(this->skin.getSize().x/2, this->skin.getSize().y/2);
    this->setPosition(this->skin.getSize().x*2, SCREENY/2);
    this->setTextureRect(sf::IntRect(0,0, this->skin.getSize().x, this->skin.getSize().y));
    this->sounds = new MySounds;

    this->sounds->addFile("shoot2");
    this->sounds->addFile("stop2");
    this->sounds->addFile("stop");
    this->sounds->addFile("block");
    this->sounds->addFile("fly");
    this->sounds->addFile("skok3");
    this->sounds->addFile("crash");
    this->sounds->addFile("crash2");
    this->sounds->addFile("move");

    //6:49
}