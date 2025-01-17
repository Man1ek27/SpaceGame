#include "../include/Game.h"

std::vector <Rock> Game::Rocks;
sf::Vertex Game::va[1000];
sf::Font Game::font;

Game::Game(sf::String title) : sf::RenderWindow(sf::VideoMode(SCREENX, SCREENY, CBITS), title){
    this->setActive(true);

    this->setFramerateLimit(FRAME_LIMIT);
    this->setVerticalSyncEnabled(true); 

    this->setPosition(sf::Vector2i(POSX, POSY));
    this->setKeyRepeatEnabled(false);

    this->gg.setFont(Game::font);
    this->gg.setPosition(SCREENX/6, SCREENY/3);
    this->gg.setCharacterSize(30);
    this->gg.setFillColor(sf::Color(0xffffffff));
    this->gg.setStyle(sf::Text::Bold);

    this->strT.setFont(font);
    this->strT.setPosition(SCREENX*0.2, 22);
    this->strT.setCharacterSize(16);
    this->strT.setFillColor(sf::Color(0xff6464ff));
    this->strT.setStyle(sf::Text::Bold);

    this->t = sf::seconds(90);

    int ra;
    srand(time(NULL));
    if((ra = rand()%2) == 0){
        this->bgtx.loadFromFile("../images/kosmo3.png");
    }
    else{
        this->bgtx.loadFromFile("..images/kosmo2.png");
    }

    this->bg.setTexture(&(this->bgtx));
    this->bg.setPosition(0,0);
    this->bg.setSize(sf::Vector2f(SCREENX, SCREENY));

    for(int i=0; i<1000; i++){
        Game::va[i].position = sf::Vector2f(rand()%SCREENX, rand()%SCREENY);
        Game::va[i].color = sf::Color(100+rand()%155, 100+rand()%155, 100+rand()%155, 255);
    }
}

void Game::Run(){
    Game::font.loadFromFile("..fonts/OpenSans-Light.ttf");
    SetSFMLWindow(this->getSystemHandle());
    sf::Time start = Game::clock.getElapsedTime();
    sf::Time tick = sf::microseconds(0);

    sf::Event e;
    //inicjacja interceptora
    Interceptor itor;
    sf::Transform vaMove; // ruszanie gwiazdkami
    sf::Time tpause; // zmienna pomocnicza do pauzy

    unsigned steps = 0;

    while(this->isOpen()){
        if(itor.hp > 0 && this->t.asSeconds() <= 0){
            itor.pt = itor.pt + itor.hp *3 // END OF TIME
            itor.hp = 0;
            itor.destroy = false;
        }
        while(this->pollEvent(e)){
            if(e.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) this->close();
            if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard ::P){
                this->pause = (this->pause)? false: true;
            }
            if(!this->pasue) itor.catchEvents(e);
        }
        if(this->pause){
            this->clear(sf::Color(0xffffffff));
            this->draw(this->bg); //tło
            this->ss.str("");
            this->strT.setString("Pasue - klick [p] to unpause");
            this->draw(this->strT);
            this->display();
            Game::clock.restart();
            tpause = this->t;
            continue;
        }
    }
}