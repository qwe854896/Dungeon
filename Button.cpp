#include"Button.h"

Button::Button(float x, float y, float width, float height, int align, int size, Font* font, string text, Color idleColor, Color hoverColor, Color activeColor)
: align(align), font(font), idleColor(idleColor), hoverColor(hoverColor), activeColor(activeColor)
{
    this->shape.setPosition(Vector2f(x, y));
    shape.setSize(Vector2f(width, height));

    this->text.setFont(*this->font);
    this->text.setString(text);
    this->text.setFillColor(Color::White);
    this->text.setCharacterSize(size);

    if (align == 0) 
        this->text.setPosition(
            this->shape.getPosition().x + 0.1 * this->shape.getGlobalBounds().width,
            this->shape.getPosition().y + (this->shape.getGlobalBounds().height - this->text.getGlobalBounds().height) / 2.f
        );
    else if (align == 1) 
        this->text.setPosition(
            this->shape.getPosition().x + (this->shape.getGlobalBounds().width - this->text.getGlobalBounds().width) / 2.f,
            this->shape.getPosition().y + (this->shape.getGlobalBounds().height - this->text.getGlobalBounds().height) / 2.f
        );
    else if (align == 2)
        this->text.setPosition(
            this->shape.getPosition().x + (this->shape.getGlobalBounds().width - this->text.getGlobalBounds().width) - 0.1 * this->shape.getGlobalBounds().width,
            this->shape.getPosition().y + (this->shape.getGlobalBounds().height - this->text.getGlobalBounds().height) / 2.f
        );

    this->shape.setFillColor(this->idleColor);
}

Button::Button(float x, float y, float width, float height, int align, int size, Font* font, string text, Color idleColor, Color hoverColor, Color activeColor, Texture *texture) 
: Button(x, y, width, height, align, size, font, text, idleColor, hoverColor, activeColor)
{
    updateTexture(texture);
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

void Button::updateText(string text) {
    this->text.setString(text);
    if (align == 0) 
        this->text.setPosition(
            this->shape.getPosition().x + 0.1 * this->shape.getGlobalBounds().width,
            this->shape.getPosition().y + (this->shape.getGlobalBounds().height - this->text.getGlobalBounds().height) / 2.f
        );
    else if (align == 1) 
        this->text.setPosition(
            this->shape.getPosition().x + (this->shape.getGlobalBounds().width - this->text.getGlobalBounds().width) / 2.f,
            this->shape.getPosition().y + (this->shape.getGlobalBounds().height - this->text.getGlobalBounds().height) / 2.f
        );
    else if (align == 2)
        this->text.setPosition(
            this->shape.getPosition().x + (this->shape.getGlobalBounds().width - this->text.getGlobalBounds().width) - 0.1 * this->shape.getGlobalBounds().width,
            this->shape.getPosition().y + (this->shape.getGlobalBounds().height - this->text.getGlobalBounds().height) / 2.f
        );
}

void Button::updateTexture(Texture *texture) {
    shape.setTexture(texture);
}
