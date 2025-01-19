#include <locale>
#include <windows.h>
#include "../include/Game.h"

Game theGame("Game !");
int main(){
    srand(time(NULL));
    setlocale(LC_ALL, "pl.PL_UTF-8");
    Game::StartConsole();
    theGame.Run();

    return 0;
}