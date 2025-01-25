#include "../include/Rock.h"

Rock::Rock(): sf::ConvexShape(){

    //punktow będzie points Y
    int points = rand()%5 +3;

    this->heal = (! (rand()%20))? true: false;
    this->setPointCount(points);
    this->speed = rand()%7 +1;
    float angle = 360.f/points;

    //tekstura statyczna
    if(!this-> heal)
        this->setTexture(&Rock::tx);
    else
        this->setTexture(&Rock::tx);

    //generuje pozycje
    this->setPosition(sf::Vector2f(SCREENX + 150, rand()%(SCREENY -50) +25));

    //generuje punkty figury (zblizone do koła, ale nie koło)
    sf::Transform temp = sf::Transform::Identity; // transformacja do obrotu
    float cx = this->getPosition().x; // punkt wg. którego obracamy
    float cy = this->getPosition().y;

    temp.rotate(angle, cx, cy);
    this->setPoint(0, sf::Vector2f(cx, cy+100/(1+this->speed)));
    this->vertex.push_back(this->getPoint(0));

    for(int nr =1; nr < points; nr++){
        this->setPoint(nr, temp, transformPoint(this->getPoint(nr-1)));
        this->vertex.push_back(this->getPoint(nr));
    }

    this->setOrigin(cx, cy);
    this->rotate(rand()%180);
    this->rot = rand()%5 -2;
}

