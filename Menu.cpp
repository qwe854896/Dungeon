#include"Menu.h"

Menu::Menu(float x, float y, float width, float height, int align, int size, bool isUpDown, Font* font, RenderWindow* window, Color idleColor, Color hoverColor, Color activeColor) 
: align(align), isSelected(0), window(window), font(font), isUpDown(isUpDown), x(x), y(y), width(width), height(height), size(size), idleColor(idleColor), hoverColor(hoverColor), activeColor(activeColor), showColor(1), notSingleMenu(false)
{
}

Menu::Menu(float x, float y, float width, float height, int align, int size, bool isUpDown, Font* font, vector<string> texts, RenderWindow* window, Color idleColor, Color hoverColor, Color activeColor)
: align(align), isSelected(0), window(window), font(font), isUpDown(isUpDown), x(x), y(y), width(width), height(height), size(size), idleColor(idleColor), hoverColor(hoverColor), activeColor(activeColor), showColor(1), notSingleMenu(false)
{
    for (auto text : texts) {
        buttons.emplace_back(x, y, width, height, align, size, font, text, idleColor, hoverColor, activeColor);

        if (isUpDown) this->y += 1.1 * height;
        else this->x += 1.1 * width;
    }
}
Menu::~Menu() {

}

void Menu::update() {
    if (buttons.empty()) return;
    if (showColor == 0) {
        for (auto& button : buttons) button.update(0);
        return;
    }
    static bool pressLeft = false, pressRight = false;
    if (!pressLeft && (Keyboard::isKeyPressed(Keyboard::Left) || (!notSingleMenu && Keyboard::isKeyPressed(Keyboard::Up)))) {
        pressLeft = 1;
        buttons[isSelected].update(0);
        if (isSelected) --isSelected;
        else isSelected = buttons.size() - 1;
    }

    if (!pressRight && (Keyboard::isKeyPressed(Keyboard::Right) || (!notSingleMenu && Keyboard::isKeyPressed(Keyboard::Down)))) {
        pressRight = 1;
        buttons[isSelected].update(0);
        if (isSelected != buttons.size() - 1) ++isSelected;
        else isSelected = 0;
    }

    if (pressLeft && (!Keyboard::isKeyPressed(Keyboard::Left) && !(!notSingleMenu && Keyboard::isKeyPressed(Keyboard::Up)))) {
        pressLeft = 0;
    }

    if (pressRight && (!Keyboard::isKeyPressed(Keyboard::Right) && !(!notSingleMenu && Keyboard::isKeyPressed(Keyboard::Down)))) {
        pressRight = 0;
    }

    buttons[isSelected].update(1);
}
void Menu::render(RenderTarget* target) {
    for (auto &button : buttons) button.render(window);
}

void Menu::push_back(string text) {
    buttons.emplace_back(x, y, width, height, align, size, font, text, idleColor, hoverColor, activeColor);

    if (isUpDown) y += 1.1 * height;
    else x += 1.1 * width;
}

void Menu::push_back(string text, Texture *texture) {
    buttons.emplace_back(x, y, width, height, align, size, font, text, idleColor, hoverColor, activeColor, texture);
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

void Menu::setShowColor(bool showColor) {
    this->showColor = showColor;
}

bool Menu::empty() {
    return buttons.empty();
}

void Menu::shiftIsSelected(int delta) {
    isSelected = (isSelected + delta + buttons.size()) % buttons.size();
}

void Menu::setNotSingleMenu(bool notSingleMenu) {
    this->notSingleMenu = notSingleMenu;
}
