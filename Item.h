#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include "Object.h"
#include "Player.h"
#include "Button.h"
#include "Menu.h"
using namespace std;

class Player;

class Item: public Object
{
private:
    string kind;
    int HP, MP, FP, attack, defense, durability, price;
public:
    Item();
    Item(string, int, int, int, int, int);
    Item(string, string, int, int, int, int, int, int, int);
    Item(string, string, string, int, int, int, int, int, int, int);

    /* Virtual function that you need to complete    */
    /* In Item, this function should deal with the   */
    /* pick up action. You should add status to the  */
    /* player.                                       */
    bool triggerEvent(Object*, RenderWindow*) override;
    void input(ifstream& in) override;
    void output(ofstream& out) const override;

    /* Set & Get function*/
    int getHP() const;
    int getMP() const;
    int getFP() const;
    int getAttack() const;
    int getDefense() const;
    int getDurability() const;
    int getPrice() const;
    string getKind() const;
    void setHP(int);
    void setMP(int);
    void setFP(int);
    void setAttack(int);
    void setDefense(int);
    void setDurability(int);
    void setPrice(int);
    void setKind(string);

    /* Supplement */
    void increaseDurability(int);
    bool decreaseDurability(int);
    string genInfo();
    static Item randomItemGenerator(int, string);
    void renderOnWindow(float x, float y, float width, float height, RenderWindow* window, Color color);
};

ifstream& operator>>(ifstream&, Item&);
ofstream& operator<<(ofstream&, const Item&);
ostream& operator<<(ostream&, const Item&);

#endif // ITEM_H_INCLUDED
