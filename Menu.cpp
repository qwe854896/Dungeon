#include"Menu.h"

Menu::Menu(float x, float y, float width, float height, int size, bool isUpDown, Font* font, RenderWindow* window, Color idleColor, Color hoverColor, Color activeColor) 
: isSelected(0), window(window), font(font), isUpDown(isUpDown), x(x), y(y), width(width), height(height), size(size), idleColor(idleColor), hoverColor(hoverColor), activeColor(activeColor)
{
    
}

Menu::Menu(float x, float y, float width, float height, int size, bool isUpDown, Font* font, vector<string> texts, RenderWindow* window, Color idleColor, Color hoverColor, Color activeColor)
: isSelected(0), window(window), font(font), isUpDown(isUpDown), x(x), y(y), width(width), height(height), size(size), idleColor(idleColor), hoverColor(hoverColor), activeColor(activeColor)
{
    this->isUpDown = isUpDown;
    for (auto text : texts) {
        buttons.emplace_back(x, y, width, height, size, font, text, idleColor, hoverColor, activeColor);

        if (isUpDown) this->y += 1.1 * height;
        else this->x += 1.1 * width;
    }
}
Menu::~Menu() {

}

void Menu::update() {
    static bool pressLeft = false, pressRight = false;
    if (!pressLeft && (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::Up))) {
        pressLeft = 1;
        buttons[isSelected].update(0);
        if (isSelected) --isSelected;
        else isSelected = buttons.size() - 1;
    }

    if (!pressRight && (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::Down))) {
        pressRight = 1;
        buttons[isSelected].update(0);
        if (isSelected != buttons.size() - 1) ++isSelected;
        else isSelected = 0;
    }

    if (pressLeft && (!Keyboard::isKeyPressed(Keyboard::Left) && !Keyboard::isKeyPressed(Keyboard::Up))) {
        pressLeft = 0;
    }

    if (pressRight && (!Keyboard::isKeyPressed(Keyboard::Right) && !Keyboard::isKeyPressed(Keyboard::Down))) {
        pressRight = 0;
    }

    buttons[isSelected].update(1);
}
void Menu::render(RenderTarget* target) {
    for (auto &button : buttons) button.render(window);
}

void Menu::push_back(string text) {
    buttons.emplace_back(x, y, width, height, size, font, text, idleColor, hoverColor, activeColor);

    if (isUpDown) y += 1.1 * height;
    else x += 1.1 * width;
}

void Menu::pop_back() {
    buttons.pop_back();

    if (isUpDown) y -= 1.1 * height;
    else x -= 1.1 * width;
}

int Menu::getIsSelected() const {
    return isSelected;
}
