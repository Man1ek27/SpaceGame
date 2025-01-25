#include "../include/Game.h"

std::vector <Rock> Game::Rocks;
sf::Vertex Game::va[1000];
sf::Font Game::Font;
sf::Clock Game::clock;

void Game::StartConsole() {
   std::stringstream ss;
   ss << "konsola" << time(0);
   std::string s = ss.str();
   std::wstring stemp = std::wstring(s.begin(), s.end());
   LPCWSTR sw = stemp.c_str();
   HWND cw = GetConsoleWindow(); // okno konsoli przypisuj� do cw dzi�ki bibliotece windows.h
   SetWindowText(cw, sw); // zmieniam tytu� okna
   SetWindowPos(cw,HWND_TOPMOST,0,0,940,1080,SWP_NOMOVE); // always on top
}

void Game::SetSFMLWindow(HWND gw) {
   RECT rect;
   GetWindowRect(gw, &rect);
   SetWindowPos(gw,HWND_TOPMOST,rect.left,rect.top,rect.right,rect.bottom,SWP_NOMOVE);
   SetWindowLong(gw, GWL_EXSTYLE, GetWindowLong(gw, GWL_EXSTYLE) | WS_EX_LAYERED );
   SetLayeredWindowAttributes(gw,0x000000, 0xff, LWA_ALPHA);
}

Game::Game(sf::String title) : sf::RenderWindow(sf::VideoMode(SCREENX, SCREENY, CBITS), title){
    this->setActive(true);

    this->setFramerateLimit(FRAME_LIMIT);
    this->setVerticalSyncEnabled(true); 

    this->setPosition(sf::Vector2i(POSX, POSY));
    this->setKeyRepeatEnabled(false);

    this->gg.setFont(Game::Font);
    this->gg.setPosition(SCREENX/6, SCREENY/3);
    this->gg.setCharacterSize(30);
    this->gg.setFillColor(sf::Color(0xffffffff));
    this->gg.setStyle(sf::Text::Bold);

    this->strT.setFont(Font);
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
        this->bgtx.loadFromFile("../images/kosmo2.png");
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
    Bullet::tx.loadFromFile("../images/bullet2.png"); // animacja pocisków
    Rock::tx.loadFromFile("../images/rock.png"); 
    Rock::txg.loadFromFile("../images/rockg.png");
    Game::Font.loadFromFile("../fonts/OpenSans-Light.ttf");
    
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
            itor.pt = itor.pt + itor.hp *3; // END OF TIME
            itor.hp = 0;
            itor.destroy = false;
        }
        while(this->pollEvent(e)){
            if(e.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) this->close();
            if(e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard ::P){
                this->pause = (this->pause)? false: true;
            }
            if(!this->pause) itor.CatchEvents(e);
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
        
        //Losujemy kamień 
        if(itor.hp > 0 && !(rand()%20)) Game::Rocks.push_back(Rock());

        this->clear(sf::Color(0xffffffff));
        this->draw(this->bg);

        //pył gwiezdny
        this->draw(Game::va, 1000, sf::PrimitiveType::Points);

        for(int i=0; i<1000; i++){
            Game::va[i].position.x -= (i%3 +1);
            if(Game::va[i].position.x <= 0){
                Game::va[i].position = sf::Vector2f(SCREENX, Game::va[i].position.y);
            }
        }

        //lecą głazy
        if(itor.hp >0) Game::UpdateRocks(*this);
        else{ this->Gogg(&itor);}

        //statek
        itor.Paint(*this);

        //czas
        this->ss.str("");
        this->ss << "[w][s]-move, [a][d]-firemode, [space]-shoot    ";

        if(itor.hp > 0){
            this->ss << (int) this->t.asSeconds() << "sek ";
        }
        this->ss <<itor.bullets.size() << "/" << itor.maxBullets << " bulets";
        this->strT.setString(this->ss.str());
        this->ss.str("");
        this->draw(this->strT);

        sf::sleep(sf::milliseconds(10));
        this->display();

        if(!this->pause) this->t = tpause - Game::clock.getElapsedTime();

        tick = Game::clock.getElapsedTime() - start;
        if(tick.asMilliseconds() > 1000){
            std::cout << "średnio: " << steps << "k/s\n";
            start = Game::clock.getElapsedTime();
            steps = 0;
        }
        steps++;
    }
}

void Game::UpdateRocks(Game &g){
    Rock *r;
    for(auto i = Game::Rocks.begin(); i != Game::Rocks.end(); i++){
        r = &*i;
        if(r->kill || r->getPosition().x <0 || r->getPosition().y < -50 || r->getPosition().y > SCREENY + 50){
            Game::Rocks.erase(i); i--; continue;
        }
        r->Update();
        g.draw(*r);
    }
}

void Game::Gogg(Interceptor *i){
    this->ss << "Wynik = " << i->pt << " pkt";
    this->gg.setString(this->ss.str());
    this->ss.str("");
    this->draw(this->gg);
}

//czy punkt zawiera sięwewnątrz bloku wypuchłych punktów ?
bool Game::IsPointInsidePolygon(std::vector <sf::Vector2f> &points, const sf::Vector2f &vec){
    int is = -1;
    int n = points.size();
    float xi, yi,xj, yj, d;
    for(int i=0; i<n; i++){
        xi = points[i].x;
        yi = points[i].y;

        xj = (i+1==n)? points[0].x: points[i+1].x;
        yj = (i+1==n)? points[0].y: points[i+1].y;
        d = (vec.x -xi)*(yj - yi) - (vec.y - yi)*(xj - xi);
        if(d == 0) continue;
        else{
            if(is == -1){
                is = d > 0.0;
            }
            else if((d > 0.0) != is) return false;
        }
    }
    return true;
}

bool Game::IsPointInsidePolygon(const sf::Transform &t, std::vector<sf::Vector2f> &points, const sf::Vector2f &vec){
    int is = -1;
    int n = points.size();
    float xi, yi,xj, yj, d;
    for(int i=0; i<n; i++){
        sf:: Vector2f point(t.transformPoint(points[i]));
        xi = point.x;
        yi = point.y;
        if(i+1 ==n){
            sf::Vector2f point2(t.transformPoint(points[0]));
            xj = point2.x;
            yj = point2.y;
        }
        else{
            sf::Vector2f point2(t.transformPoint(points[i+1]));
            xj = point2.x;
            yj = point2.y;
        }
        
        d = (vec.x -xi)*(yj - yi) - (vec.y - yi)*(xj - xi);
        if(d == 0) continue;
        else{
            if(is == -1){
                is = d > 0.0;
            }
            else if((d > 0.0) != is) return false;
        }
    }
    return true;
}

Game::~Game(){}