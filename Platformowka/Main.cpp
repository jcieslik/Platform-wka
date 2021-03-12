#include <iostream>
#include "Menu.h"
#include "Game.h"
#include "Unit.h"
#include "Player.h"

using namespace std;

int main()
{
    sf::Texture textt;
    textt.loadFromFile("images/Bullet.png");
    try {
        Menu main_menu;
        main_menu.main_menu();
    }
    catch (exception e)
    {
        cout << e.what() << endl;
        return 1;
    }
    return 0;
}