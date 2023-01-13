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
        bool isUpDown;
        bool showColor;
        bool notSingleMenu;
        int isSelected;

        RenderWindow* window;

        Font *font;
        vector<Button> buttons;

        float x, y, width, height;
        int align, size;

        Color idleColor;
        Color hoverColor;
        Color activeColor;
    public:
        Menu(float x, float y, float width, float height, int align, int size, bool isUpDown, Font* font, RenderWindow* window, Color idleColor, Color hoverColor, Color activeColor);
        Menu(float x, float y, float width, float height, int align, int size, bool isUpDown, Font* font, vector<string> texts, RenderWindow* window, Color idleColor, Color hoverColor, Color activeColor);
        ~Menu();

        void push_back(string text);
        void push_back(string text, Texture *texture);
        void pop_back();

        void setShowColor(bool);
        bool empty();
        void shiftIsSelected(int);
        void setNotSingleMenu(bool);

        void update();
        void render(RenderTarget* target = nullptr);
        int getIsSelected() const;
};

#endif // Menu_H_INCLUDED