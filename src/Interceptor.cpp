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

    this->hpRect.setSize(sf::Vector2f(this->hpRectX * this-> hp /this->hpmax * 1.f, 3.f));
    this->hpRect.setOrigin(this->skin.getSize().x/2, 2.5f);
    this->hpRect.setFillColor(sf::Color(0xff0000ff));
    this->hpRect.setPosition(this->getPosition().x, this->getPosition().y - this->skin.getSize().y);
    this->hpRect.setOutlineColor(sf::Color(0x000000ff));
    this->hpRect.setOutlineThickness(1);

    this->ss << this->pt << "punktów";
    this->strPt.setFont(Game::Font);
    this->strPt.setCharacterSize(16);
    this->strPt.setPosition(SCREENX-200, SCREENY-20);
    this->strPt.setString(ss.str());
    this->strPt.setFillColor(sf::Color(0xffff00ff));
    ss.str("");

    this-> ss << this->hp << " hp";
    this->strHp.setFont(Game::Font);
    this->strHp.setCharacterSize(20);
    this->strHp.setPosition(SCREENX-200, SCREENY-42);
    this->strHp.setString(ss.str());
    this->strHp.setFillColor(sf::Color(0xff0000ff));
    this->strHp.setStyle(sf::Text::Bold);
    ss.str("");
}

//rysowanie

void Interceptor::CatchEvents(sf::Event &e){
    if(this->hp <=0) return;
    bool moves = false;

    if(e.type == sf::Event::KeyPressed && (e.key.code == sf::Keyboard::Up || e.key.code == sf::Keyboard::W)){
        this->speed = -this->boostSpeed;
        moves = true;
    }
    if(e.type == sf::Event::KeyPressed && (e.key.code == sf::Keyboard::Down || e.key.code == sf::Keyboard::S)){
        this->speed = this->boostSpeed;
        moves = true;
    }
    if(moves) this->sounds->play("move", 50, false, 1, true);
    if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Space){
        this->Shoot();
    }
}

//update

void Interceptor::Update(Game &theGame){
    //aktualizacja napisów
    this->ss << this->pt << " punktów";
    this->strPt.setString(ss.str());
    this->ss.str("");
    this->ss << this->hp << " hp";
    this->strHp.setString(this->ss.str());
    this->ss.str("");

    if(this->hp <=0 && this->destroy){// i umarł przed czasem
        this->move(rand()%3-1, rand()%3-1); // więc się telepie
        this->rotate(rand()%7-2);
    } 
    else{ // alive
        //po  ruchu test kolizji ze ściankami na górze
        if(this->getGlobalBounds().top + this->getGlobalBounds().height >= SCREENY) this->speed *=-1.5;
        if(this->getGlobalBounds().top <= 0) this->speed *= -1.5;
    }

    //smoke generate
    if(this->speed > 0.1f || this->speed < -0.1f){
        for(int i=0; i<100; i++){
            this->smoke[i].position = sf::Vector2f(this->getPosition().x -60 - (rand()%21-10) -i, (rand()%(i+1)- i/2) + this->getPosition().y);
            this->smoke[i].color = sf::Color(100 + rand()%155, 100 + rand()%155, 100 + rand()%155, 255);
        }
    }

    //zmiana prędkości (spadek)
    if(this->speed > 0){
        this->speed = std::max(0.f, this->speed - (this->boostSpeed/this->speedCooldown));
    }
    else if(speed <0){
        this->speed = std::min(0.f, this->speed - (this->boostSpeed/this->speedCooldown));
    }

    this->setPosition(this->getPosition().x , this->getPosition().y + this->speed); // preędkość
    this->hpRect.setPosition(this->getPosition().x, this->getPosition().y + this->skin.getSize().y/2);
    this->hpRect.setSize(sf::Vector2f(this->hpRectX * this->hp/this->hpmax * 1.f, 3.f));

    //update kulek
    if(this->bullets,size()){
        for(auto i= this->bullets.begin(); i != bullets.end(); i++){
            (*i).Update(theGame); // upgrade kulek
        }
    }

    //usuwanie kulek za ekranem
    Bullet *p;
    if(this->bullets.size()){
        for(auto i = this->bullets.begin(); i != bullets.end(); i++){
            p = &(*i);
            if(p->kill || p->getPosition().x > SCREENX + Bullet::xy*2){
                this->bullets.erase(i);break;
            }
        }
    }

    //testy kolizji dla każdego punktu
    if(this->hp > 0){
        Rock *r; 
        for(auto i = Game::Rocks.begin(); i != Game::Rocks.end(); i++){
            r = &(*i);
            if(r-> getGlobalBounds().left <= this->getGlobalBounds().left + this->getGlobalBounds().width && 
                this->getGlobalBounds().intersects(r->getGlobalBounds())){
                    bool coll = false;
                    for(auto v = this->localColisionPoints.begin(); v != this->localColisionPoints.end(); v++){
                        sf::Vector2f p(*v);
                        float x = this->skin.getSize().x;
                        float y = this->skin.getSize().y;
                        if(Game::IsPointInsidePolygon(r->getTransform(), r->vertex, sf::Vector2f(this->getPosition().x -x + p.x, this->getPosition().y -y + p.y))){
                            if(r->heal){ //leczę i zwiekszam ilość pocisków
                                r->kill = true;
                                this->hp += r->GetPointCount(); //heal
                                coll = true;
                                this->sounds->play("skok3", 75, false, 1.f, false);
                                this->maxBullets += 4;
                                break;
                            }
                            else{
                                r->kill = true;
                                this->hp -= r->GetPointCount(); //damage
                                coll = true;
                                if(this-> hp <=0 ){
                                    this->sounds->play("fly", 50, true, 1.f, false);
                                    this->destroy = true;
                                }
                                else this->sounds->play("crash", 75, false, 1.f, false);
                                break;
                            }
                        }
                    }
                    if(coll) break;
                }
        }
    }
}

void Interceptor::Shoot(){
    if(this->maxBullets <= this->bullets.size()){
        this->sounds.play("block", 100, false, 0.95f + (rand()%11)/100.f, false);
        return;
    }
    this->bullets.push_back(Bullet(sf::Vector2(this->getPosition().x + this->getTexture()->getSize().x/2, this->getPosition().y + 10.f), this));
    this->sounds->play("shoot2", 100, false, 0.85f + rand()%31/100.f, false);
}

void Interceptor::Paint(Game &game){
    this->Update(game);
    game.draw(*this);
    if(this->hp >0) game.draw(this->hpRect);
    for(auto i = this->bullets.begin(); i != this->bullets.end(); i++){
        game.draw(*i);
    }
    game.draw(this->strPt);
    game.draw(this->strHp);
    if(this->speed > 0.1f || this->speed < -0.1f){
        game.draw(this->smoke, 100, sf::PrimitiveType::Points);
    }
    
}