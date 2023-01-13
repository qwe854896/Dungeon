#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include<iostream>
#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>

using namespace std;
using namespace sf;

enum button_states{BTN_IDLE = 0, BTN_HOVER, BTN_ACTIVE};

class Button{
    private:
        unsigned char buttonState;

        RectangleShape shape;
        Font* font;
        Text text;

        Sprite sprite;
        bool isSprite;
        float spriteWidth, spriteHeight;

        Sound sound, sound1;
        SoundBuffer *click, *uClick;
        void initSoundBuffer();

        int align;

        Color idleColor;
        Color hoverColor;
        Color activeColor;

    public:
        Button(float x, float y, float width, float height, int align, int size, Font* font, string text, Color idleColor, Color hoverColor, Color activeColor);
        Button(float x, float y, float width, float height, int align, int size, Font* font, string text, Color idleColor, Color hoverColor, Color activeColor, Texture *texture);
        ~Button();

        void updateText(string text);
        void update(bool isSelected);
        void render(RenderTarget *target);

        void updateTexture(Texture *texture);
        void setIsSprite(bool);

        const bool isPressed() const;
};


#endif // BUTTON_H_INCLUDED