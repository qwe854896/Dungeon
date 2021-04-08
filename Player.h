#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include "GameCharacter.h"
#include "Room.h"
#include "Item.h"

using namespace std;

class Item;

class Player: public GameCharacter
{
private:
    Room* currentRoom;
    Room* previousRoom;

    vector<Item> inventory;
    vector<Item> armors; // Helmet, Chestplate, Leggings, Boots, Weapon;
    int gold;

    /* Supplement */
    int getArmorIndex(string);
    string getArmor(int);
public:
    Player();
    Player(string, string, string, int);
    Player(string, string, int, int, int, int, int, int, int);
    void addItem(Item);
    void popItem(int);
    void useItem(int);
    void changeRoom(Room*);
    void increaseGold(int);

    /* Virtual function that you need to complete   */
    /* In Player, this function should show the     */
    /* status of player.                            */
    bool triggerEvent(Object*) override;
    void input(ifstream&) override;
    void output(ofstream&) const override;

    /* Set & Get function*/
    void setCurrentRoom(Room*);
    void setPreviousRoom(Room*);
    void setInventory(vector<Item>);
    void setGold(int);
    Room* getCurrentRoom() const;
    Room* getPreviousRoom() const;
    vector<Item> getInventory() const;
    int getGold() const;

    /* Supplement */
    bool listInventory();
};

ifstream& operator>>(ifstream&, Player&);
ofstream& operator<<(ofstream&, const Player&);


#endif // PLAYER_H_INCLUDED
