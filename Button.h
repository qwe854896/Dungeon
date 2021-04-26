#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include<iostream>
#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>

using namespace std;
using namespace sf;

enum button_states{BTN_IDLE = 0, BTN_HOVER, BTN_ACTIVE};

class Button{
    private:
        unsigned char buttonState;

        RectangleShape shape;
        Font* font;
        Text text;

        Color idleColor;
        Color hoverColor;
        Color activeColor;

    public:
        Button(float x, float y, float width, float height, Font* font, string text, Color idleColor, Color hoverColor, Color activeColor);
        ~Button();
        
        void update(bool isSelected);
        void render(RenderTarget *target);

        const bool isPressed() const;
};

#endif // BUTTON_H_INCLUDED