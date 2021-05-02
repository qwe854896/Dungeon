#ifndef MINIMAP_H_INCLUDED
#define MINIMAP_H_INCLUDED

#include<iostream>
#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>
#include"Button.h"
#include"Room.h"

using namespace std;
using namespace sf;

class Minimap {
    private:
        RenderWindow* window;
        float x, y, width, height;
        bool roomSquare[3][3];
        bool pathRectangle[18];

        vector <RectangleShape*> squares;
    public:
        Minimap(float x, float y, float width, float height, RenderWindow* window, Room* room);
        ~Minimap();

        void DFS(Room* x, int i, int j);
        void update();
        void render(RenderTarget* target = nullptr);
};


#endif // MINIMAP_H_INCLUDED
