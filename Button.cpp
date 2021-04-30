#include"Button.h"

Button::Button(float x, float y, float width, float height, int size, Font* font, string text, Color idleColor, Color hoverColor, Color activeColor)
: font(font), idleColor(idleColor), hoverColor(hoverColor), activeColor(activeColor)
{
    this->shape.setPosition(Vector2f(x, y));
    shape.setSize(Vector2f(width, height));

    this->text.setFont(*this->font);
    this->text.setString(text);
    this->text.setFillColor(Color::White);
    this->text.setCharacterSize(size);
    this->text.setPosition(
        this->shape.getPosition().x + (this->shape.getGlobalBounds().width - this->text.getGlobalBounds().width) / 2.f,
        this->shape.getPosition().y + (this->shape.getGlobalBounds().height - this->text.getGlobalBounds().height) / 2.f
    );

    this->shape.setFillColor(this->idleColor);
}

Button::~Button() {

}


void Button::update(bool isSelected) {
    /* Update the booleans for hover and pressed */

    // IDLE
    this->buttonState = BTN_IDLE;

    // Hover
    if (isSelected == 1) {
        this->buttonState = BTN_HOVER;

         // Pressed
        if (Keyboard::isKeyPressed(Keyboard::Enter)) {
            this->buttonState = BTN_ACTIVE;
        }
    }

    switch (this->buttonState) {
        case BTN_IDLE:
            this->shape.setFillColor(this->idleColor);
            break;

        case BTN_HOVER:
            this->shape.setFillColor(this->hoverColor);
            break;

        case BTN_ACTIVE:
            this->shape.setFillColor(this->activeColor);
            break;

        default:
            this->shape.setFillColor(Color::Red);
            break;
    }
}

void Button::render(RenderTarget *target) {
    target->draw(this->shape);
    target->draw(this->text);
}

const bool Button::isPressed() const {
    return this->buttonState == BTN_ACTIVE;
}