#include "Button.h"

Button::Button(float x, float y, float width, float height, int align, int size, Font *font, string text, Color idleColor, Color hoverColor, Color activeColor)
    : align(align), font(font), idleColor(idleColor), hoverColor(hoverColor), activeColor(activeColor), isSprite(false)
{
    // cout << x << ", " << y << ", " << width << ", " << height << ", " << align << ", " << size << ", " << endl;
    this->shape.setPosition(Vector2f(x, y));
    shape.setSize(Vector2f(width, height));

    this->text.setFont(*this->font);
    this->text.setString(text);
    this->text.setFillColor(Color::White);
    this->text.setCharacterSize(size);

    if (align == 0)
        this->text.setPosition(
            this->shape.getPosition().x + 0.1 * this->shape.getLocalBounds().width,
            this->shape.getPosition().y + (this->shape.getLocalBounds().height - this->text.getLocalBounds().height) / 2.f);
    else if (align == 1)
        this->text.setPosition(
            this->shape.getPosition().x + (this->shape.getLocalBounds().width - this->text.getLocalBounds().width) / 2.f,
            this->shape.getPosition().y + (this->shape.getLocalBounds().height - this->text.getLocalBounds().height) / 2.f);
    else if (align == 2)
        this->text.setPosition(
            this->shape.getPosition().x + (this->shape.getLocalBounds().width - this->text.getLocalBounds().width) - 0.1 * this->shape.getGlobalBounds().width,
            this->shape.getPosition().y + (this->shape.getLocalBounds().height - this->text.getLocalBounds().height) / 2.f);

    this->shape.setFillColor(this->idleColor);

    initSoundBuffer();
}

Button::Button(float x, float y, float width, float height, int align, int size, Font *font, string text, Color idleColor, Color hoverColor, Color activeColor, Texture *texture)
    : Button(x, y, width, height, align, size, font, text, idleColor, hoverColor, activeColor)
{
    updateTexture(texture);
}

Button::~Button()
{
}

void Button::initSoundBuffer()
{
    click = new SoundBuffer();
    uClick = new SoundBuffer();
    click->loadFromFile("assets/sound/click.ogg");
    uClick->loadFromFile("assets/sound/u_click.ogg");
}

void Button::update(bool isSelected)
{
    /* Update the booleans for hover and pressed */

    // IDLE
    this->buttonState = BTN_IDLE;

    // Hover
    if (isSelected == 1)
    {
        this->buttonState = BTN_HOVER;

        // Pressed
        if (Keyboard::isKeyPressed(Keyboard::Enter))
        {
            this->buttonState = BTN_ACTIVE;
        }
    }

    switch (this->buttonState)
    {
    case BTN_IDLE:
        this->shape.setFillColor(this->idleColor);
        this->sprite.setScale(sf::Vector2f(spriteWidth, spriteHeight));
        break;

    case BTN_HOVER:
        // sound.stop();
        // sound.setBuffer(*click);
        // sound.play();
        this->shape.setFillColor(this->hoverColor);
        this->sprite.setScale(sf::Vector2f(spriteWidth * 1.31, spriteHeight * 1.31));
        break;

    case BTN_ACTIVE:
        // sound1.stop();
        // sound1.setBuffer(*uClick);
        // sound1.play();
        // sleep(milliseconds(100));
        this->shape.setFillColor(this->activeColor);
        this->sprite.setScale(sf::Vector2f(spriteWidth * 1.31, spriteHeight * 1.31));
        break;

    default:
        this->shape.setFillColor(Color::Red);
        break;
    }
}

void Button::render(RenderTarget *target)
{
    if (isSprite)
        target->draw(sprite);
    else
    {
        target->draw(this->shape);
        target->draw(this->text);
    }
}

const bool Button::isPressed() const
{
    return this->buttonState == BTN_ACTIVE;
}

void Button::updateText(string text)
{
    this->text.setString(text);
    if (align == 0)
        this->text.setPosition(
            this->shape.getPosition().x + 0.1 * this->shape.getGlobalBounds().width,
            this->shape.getPosition().y + (this->shape.getGlobalBounds().height - this->text.getGlobalBounds().height) / 2.f);
    else if (align == 1)
        this->text.setPosition(
            this->shape.getPosition().x + (this->shape.getGlobalBounds().width - this->text.getGlobalBounds().width) / 2.f,
            this->shape.getPosition().y + (this->shape.getGlobalBounds().height - this->text.getGlobalBounds().height) / 2.f);
    else if (align == 2)
        this->text.setPosition(
            this->shape.getPosition().x + (this->shape.getGlobalBounds().width - this->text.getGlobalBounds().width) - 0.1 * this->shape.getGlobalBounds().width,
            this->shape.getPosition().y + (this->shape.getGlobalBounds().height - this->text.getGlobalBounds().height) / 2.f);
}

void Button::updateTexture(Texture *texture)
{
    isSprite = 1;
    sprite.setTexture(*texture);

    spriteWidth = shape.getGlobalBounds().width / texture->getSize().x;
    spriteHeight = shape.getGlobalBounds().height / texture->getSize().y;

    sprite.scale(Vector2f(spriteWidth, spriteWidth));

    FloatRect shapeRect = sprite.getLocalBounds();
    sprite.setOrigin(
        shapeRect.left + shapeRect.width / 2.0f,
        shapeRect.top + shapeRect.height / 2.0f);

    sprite.setPosition(
        shape.getPosition().x + shape.getGlobalBounds().width / 2.0,
        shape.getPosition().y + shape.getGlobalBounds().height / 2.0);
}

void Button::setIsSprite(bool isSprite)
{
    this->isSprite = isSprite;
}