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

using namespace std;

class Monster: public GameCharacter
{
private:
    Item drop;
    static pair<string, int> damageCalculate(int, int);
public:
    Monster();
    Monster(string, string, string, int, Item);
    Monster(string, string, string, int, int, int, int, int, int, int);

    /* Virtual function that you need to complete   */
    /* In Monster, this function should deal with   */
    /* the combat system.                           */
    bool triggerEvent(Object*, RenderWindow*) override;
    void input(ifstream&) override;
    void output(ofstream&) const override;

    /* Supplement */
    string produceStatus();
};



#endif // ENEMY_H_INCLUDED
