#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#define OPS (unsigned int)(ops[0] - 'A')

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
    vector<Item> skills; // skills
    int gold;

    /* Supplement */
    static int getArmorIndex(string);
    static string getArmor(int);
public:
    Player();
    Player(string, string, string, int);
    Player(string, string, string, int, int, int, int, int, int, int);

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
    void addItem(Item);
    void popItem(int);
    void useItem(int);
    void sellItem(int);

    void changeRoom(Room*);

    void increaseGold(int);
    void decreaseGold(int);
    void increaseArmorDurability(int);
    void decreaseArmorDurability(int);
    void decreaseWeaponDurability(int);

    bool listInventory();
    void showStatusinFight();
    
    bool handleInventory(string);
    bool handleAct();
    Item handleAttack();
    Item handleSkills(int);
};

ifstream& operator>>(ifstream&, Player&);
ofstream& operator<<(ofstream&, const Player&);


#endif // PLAYER_H_INCLUDED
