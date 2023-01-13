#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include "GameCharacter.h"
#include "Player.h"
#include "Item.h"
#include "Button.h"

using namespace std;

class Monster: public GameCharacter
{
private:
    Item drop;

    Sprite background;
    Button* monster;
    Texture *monsterTexture;
    Texture *backgroundTexture;
    Sound sound;
    SoundBuffer hit;

    static pair<string, int> damageCalculate(int, int);
public:
    Monster();
    Monster(string, string, string, int, Item);
    Monster(string, string, string, int, Item, Texture*, Texture*);
    Monster(string, string, string, int, int, int, int, int, int, int);

    /* Virtual function that you need to complete   */
    /* In Monster, this function should deal with   */
    /* the combat system.                           */
    bool triggerEvent(Object*, RenderWindow*) override;
    void input(ifstream&) override;
    void output(ofstream&) const override;

    /* Setter & Getter */
    void setMonsterTexture(Texture*);
    void setBackgroundTexture(Texture*);

    /* Supplement */
    string produceStatus();
};



#endif // ENEMY_H_INCLUDED
