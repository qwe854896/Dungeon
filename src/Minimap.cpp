#include "Minimap.h"

Minimap::Minimap(float x, float y, float width, float height, RenderWindow *window, Room *room)
    : x(x), y(y), width(width), height(height), window(window)
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            roomSquare[i][j] = 0;
    for (int i = 0; i < 18; ++i)
        pathRectangle[i] = 0;

    DFS(room, 1, 1);

    float dw = width / 34.0, dh = height / 34.0;

    RectangleShape *shape = new RectangleShape();
    shape->setPosition(x, y);
    shape->setSize(Vector2f(width, height));
    shape->setFillColor(Color(77, 77, 77, 255));
    squares.emplace_back(shape);

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (roomSquare[i][j])
            {
                shape = new RectangleShape();
                shape->setPosition(x + dw * (1 + j * 11), y + dh * (1 + i * 11));
                shape->setSize(Vector2f(10 * dw, 10 * dh));
                shape->setFillColor(Color(179, 179, 179, 255));
                squares.emplace_back(shape);
            }

            if (j % 3 != 2 && pathRectangle[(3 * i + j) << 1])
            {
                shape = new RectangleShape();
                shape->setPosition(x + dw * (8.5 + j * 11), y + dh * (3.5 + i * 11));
                shape->setSize(Vector2f(5 * dw, 5 * dh));
                shape->setFillColor(Color(179, 179, 179, 255));
                squares.emplace_back(shape);
            }

            if (i % 3 != 2 && pathRectangle[((3 * i + j) << 1) | 1])
            {
                shape = new RectangleShape();
                shape->setPosition(x + dw * (3.5 + j * 11), y + dh * (8.5 + i * 11));
                shape->setSize(Vector2f(5 * dw, 5 * dh));
                shape->setFillColor(Color(179, 179, 179, 255));
                squares.emplace_back(shape);
            }
        }
    }

    shape = new RectangleShape();
    shape->setPosition(x + dw * 12, y + dh * 12);
    shape->setSize(Vector2f(10 * dw, 10 * dh));
    shape->setFillColor(Color(255, 255, 255, 255));
    squares.emplace_back(shape);
}

Minimap::~Minimap()
{
    for (auto &shape : squares)
        delete shape;
}

void Minimap::DFS(Room *x, int i, int j)
{
    roomSquare[i][j] = 1;

    int I, J;

    I = i - 1, J = j;
    if (x->getUpRoom() != nullptr)
    {
        if (0 <= I && !roomSquare[I][J])
            DFS(x->getUpRoom(), I, J);
    }
    I = i + 1;
    if (x->getDownRoom() != nullptr)
    {
        pathRectangle[((3 * i + j) << 1) | 1] = 1;
        if (I < 3 && !roomSquare[I][J])
            DFS(x->getDownRoom(), I, J);
    }
    I = i, J = j - 1;
    if (x->getLeftRoom() != nullptr)
    {
        if (0 <= J && !roomSquare[I][J])
            DFS(x->getLeftRoom(), I, J);
    }
    J = j + 1;
    if (x->getRightRoom() != nullptr)
    {
        pathRectangle[(3 * i + j) << 1] = 1;
        if (J < 3 && !roomSquare[I][J])
            DFS(x->getRightRoom(), I, J);
    }
}

void Minimap::render(RenderTarget *target)
{
    for (auto &shape : squares)
        target->draw(*shape);
}
