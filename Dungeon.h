#ifndef DUNGEON_H_INCLUDED
#define DUNGEON_H_INCLUDED

#define OPS (unsigned int)(ops[0] - 'A')

#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <cmath>
#include <random>
#include <algorithm>
#include "Player.h"
#include "Monster.h"
#include "NPC.h"
#include "Room.h"
#include "Record.h"
#include "Object.h"

using namespace std;

class Dungeon{
private:
    Player player;

    int usedIndex;
    vector<Room> rooms;
    vector<bool> vis;
public:
    Dungeon();

    /* randomly pick a number by given probability */
    int getRand(vector<long double>);

    /* Create a new player, and give him/her basic status */
    void createPlayer();

    /* Create a map, which include several different rooms */
    void createMap();

    /* Deal with player's moving action */
    void handleMovement();

    /* Deal with player's interaction with objects in that room */
    void handleEvent(Object*);

    /* Deal with player's interaction with inventory */
    void handleInventory();

    /* */
//    bool handleRetreat();

    /* Deal with all game initial setting       */
    /* Including create player, create map etc  */
    void startGame();

    /* Deal with the player's action     */
    /* including showing the action list */
    /* that player can do at that room   */
    /* and dealing with player's input   */
    void chooseAction(const vector<Object*>&);

    /* Check whether the game should end or not */
    /* Including player victory, or he/she dead */
    bool checkGameLogic();

    /* Deal with the whole game process */
    void runDungeon();

    /* According to player and room to generate actions*/
//    vector<Object> generateActions(const vector<Object*>&);

    /* Check there is a monster or not in the current room */
    bool checkMonsterRoom(const vector<Object*>&);

    /* If current Room is not visited, try to generate its neighbor */
    void createRoom(int);

    /* Random Generator*/
    Monster generateMonster(int);
//    Item generateItem(int);
    NPC generateNPC(int, string);
    Room generateRoom(bool, int, int, string);
};


#endif // DUNGEON_H_INCLUDED
