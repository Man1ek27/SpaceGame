#include "../include/MySounds.h"

using namespace std;
sf::Sound MySounds::sounds[MAX_SOUNDS];

MySounds::MySounds() { sf::Listener::setGlobalVolume(100); };
MySounds::MySounds (const string nfolder) { this->folder = nfolder; sf::Listener::setGlobalVolume(100); }
MySounds::MySounds(const string nfolder, std::initializer_list <string> fileStringList){
   this->folder = nfolder;
   pair<map<string,sf::SoundBuffer>::iterator,bool> ret;
   for (auto i=fileStringList.begin(); i != fileStringList.end(); i++) {
          // zwrot z emplance
         ret = this->bufor.emplace(make_pair(*i,sf::SoundBuffer()));
         if (ret.second==false) { cout << "Problem z dodaniem pliku " << *i << " do bufora d�wi�ku.\n"; return; }
         if (! this->bufor[*i].loadFromFile(this->folder + "/" + *i + "." + this->type) ) cout << "Problem z �adowaniem pliku " << *i << endl;
   }
   sf::Listener::setGlobalVolume(100);
}
//! dodaje do bufora kolejny plik
void MySounds::AddFile(const string &file) {
      pair<map<string,sf::SoundBuffer>::iterator,bool> ret; // zwrot z emplance
      ret = this->bufor.emplace(make_pair(file,sf::SoundBuffer()));
      if (ret.second==false) { cout << "Problem z dodaniem pliku " << file << " do bufora d�wi�ku.\n"; return; }
      if (! this->bufor[file].loadFromFile(this->folder + "/" + file + "." + this->type)) cout << "Problem z �adowaniem pliku " << file << endl;
}

/**
   \param fileName - plik z d�wi�kiem bez rozszerzenia
   \param volume
   \param loop
   \param pitch
   \param relatywny ? (true - odleg�o�� si� nie liczy, domy�lnie false)
   \param attenuation
   \param punkt zaczepienia
   \param distance float
*/
int MySounds::Play(string fileName, int volume=100, bool loop=false, float pitch=1.f, bool rel=false) {
      return this->Play(fileName,volume,loop,pitch,rel, 10.0f, sf::Vector2f(0.f,0.f),25.f);
}
int MySounds::Play(string fileName, int volume=100, bool loop=false, float pitch=1.0, bool rel=false,
float att=10.f, const sf::Vector2f &vec = sf::Vector2f(0.f,0.f), float dist=25.f) {
      bool isPlay=0;
      sf::Sound *s;
      for (int i=0;i<MAX_SOUNDS;i++) {
         if (MySounds::sounds[i].getStatus() == sf::Sound::Playing) { continue; } // ten sound gra, id� do nast�pnego
         else {
            s = &(MySounds::sounds[i]);
            s->resetBuffer();
            s->setBuffer(this->bufor[fileName]);
            s->setVolume(volume);
            s->setLoop(loop);
            s->setPitch(pitch);
            s->setAttenuation(att);
            s->setRelativeToListener(rel);
            s->setMinDistance(dist);
            s->setPosition(vec.x,vec.y,0.f);
            s->play();
            isPlay = i+1;
            break;
         }
      }
      if (!isPlay) cout << "Wszystkie sloty na d�wi�ki s� w u�yciu. Nie odtworzono " << fileName << "." << this->type << endl;
      return isPlay;
}
void MySounds::PlayOnce(sf::Sound *s, int volume=100,
                    bool loop=false, float pitch=1.0, bool rel=false, float att=10.f,
                    const sf::Vector2f &vec = sf::Vector2f(SCREENX/2,SCREENY/2),
                    float dist=25.f) {
         if (s->getStatus() == sf::Sound::Playing) return;
            s->setVolume(volume);
            s->setLoop(loop);
            s->setPitch(pitch);
            s->setAttenuation(att);
            s->setRelativeToListener(rel);
            s->setMinDistance(dist);
            s->setPosition(vec.x,vec.y,0.f);
            s->play();
}

void MySounds::StopAll() { for (int i=0; i<MAX_SOUNDS; i++) MySounds::sounds[i].stop(); }
void MySounds::PauseAll() { for (int i=0; i<MAX_SOUNDS; i++) MySounds::sounds[i].pause(); }
void MySounds::UnpauseAll() { for (int i=0; i<MAX_SOUNDS; i++) MySounds::sounds[i].play(); }

MySounds::~MySounds(){}