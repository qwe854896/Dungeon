#ifndef Menu_H_INCLUDED
#define Menu_H_INCLUDED

#include<iostream>
#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>
#include"Button.h"

using namespace std;
using namespace sf;

class Menu {
    private:
        int isSelected;

        RenderWindow* window;

        Font *font;
        vector<Button*> buttons;
    public:
        Menu(RenderWindow* window, Font* font, vector<Button*> buttons);
        ~Menu();

        void update(const float& dt);
        void render(RenderTarget* target = nullptr);
};

#endif // Menu_H_INCLUDED