#include "../include/Bullet.h"

unsigned int Bullet::xy = 10;
sf::Texture Bullet::tx;

Bullet::Bullet(const sf::Vector2f &vec, Interceptor *inter): sf::CircleShape(){
    this->setRadius(Bullet::xy);
    this->setOrigin(Bullet::xy, Bullet::xy);
    this->setTexture(&Bullet::tx);
    this->setTextureRect(sf::IntRect(0,0,Bullet::xy,Bullet::xy));
    this->frames = Bullet::tx.getSize().y / Bullet::xy;
    this->setPosition(vec.x, vec.y);
    this->interceptor = inter;
    this->setScale(0.5f, 0.5f);
}

void Bullet::Update(Game &theGame){
    int tempSpeed = this->speed;//przesuwam pocisk ale nie od razu o cały SPEED ale po troche
    if(theGame.hasFocus() && sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        for(int i=1; i<=tempSpeed +2; i++){
            this->move(1,0);
            if(this->collTest()) break;
        }
    }
    else if(theGame.hasFocus() && sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        this->move(this->speed -2, rand()%5 -2);
        this->collTest();
    }
    else{
        this->move(this->speed, 0.f);
        this->collTest();
    }
    
    
    //animacja
    int top = (this->getTextureRect().top + Bullet::xy)% (this->frames * Bullet::xy);
    this->setTextureRect(sf::IntRect(0, top, Bullet::xy, Bullet::xy));
}

//test kolizji pocisku z wszelkimi głazami

bool Bullet::collTest(){
    for(auto i = Game::Rocks.begin(); i!= Game::Rocks.end(); i++ ){
        Rock *r = &(*i);
        //kolizja o ile w ogóle zazębiają się prostokąty
        if(this->getGlobalBounds().intersects(r->getGlobalBounds()) && 
            Game::IsPointInsidePolygon(r->getTransform(), r->vertex, this->getPosition())){
                r->hit = true;
                this->kill = true;
                if(!r->heal) this->interceptor->sounds->Play("crash2", 20, false, 0.95f +(rand()%11)/100, false);
                if(this->interceptor->hp > 0) this->interceptor->pt += (r->speed >3)?2:1;
                return true;
            }
    }
    return false;
}

Bullet::~Bullet(){}