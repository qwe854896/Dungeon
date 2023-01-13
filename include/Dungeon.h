#ifndef DUNGEON_H_INCLUDED
#define DUNGEON_H_INCLUDED

#define OPS (unsigned int)(ops[0] - 'A')

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#include <map>
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
#include "Button.h"
#include "Menu.h"
#include "Minimap.h"

using namespace std;
using namespace sf;
using std::cout;

using coord = pair<int, int>;

class Dungeon
{
private:
    Player player;

    int usedIndex;
    vector<Room> rooms;
    vector<bool> vis;

    map<coord, int> coordToIndex;

    vector<int> roomCnt{1, 1, 1, 1, 1, 1};

    /* SFML */
    map<string, Texture> textures;
    Texture *monsterTexture, *npcTexture, *chestTexture, *backgroundTexture;
    Sprite background;

    RenderWindow *_window;
    Event sfEvent;
    Clock dtClock;
    float dt;
    Music backgroundMusic;

    int ops;
    bool holdKey;
    bool holdEnter;
    bool holdUp;
    bool holdDown;
    bool gainedFocus;
    Font *font;

    void processEvents();
    void updateDt();
    void updateSFMLEvents();
    void update();
    void render();

public:
    Dungeon();
    ~Dungeon();

    void initTextures();

    /* Create a new player, and give him/her basic status */
    void createPlayer();

    /* Create a map, which include several different rooms */
    void createMap();

    /* Deal with player's moving action */
    void handleMovement();

    /* Deal with player's interaction with objects in that room */
    void handleEvent(Object *);

    /* Deal with player's interaction with inventory */
    void handleInventory();

    /* Deal with all game initial setting       */
    /* Including create player, create map etc  */
    void startGame();

    /* Deal with the player's action     */
    /* including showing the action list */
    /* that player can do at that room   */
    /* and dealing with player's input   */
    void chooseAction(const vector<Object *> &);

    /* Check whether the game should end or not */
    /* Including player victory, or he/she dead */
    bool checkGameLogic();

    /* Deal with the whole game process */
    void runDungeon(int);

    /* Check there is a monster or not in the current room */
    bool checkMonsterRoom(const vector<Object *> &);

    /* If current Room is not visited, try to generate its neighbor */
    void createRoom(int);

    /* Random Generator*/
    Monster generateMonster(int);
    NPC generateNPC(int, string);
    Room generateRoom(bool, int, int, int, int, string);
    string generateDescription();

    void encounterDanny();
};

#endif // DUNGEON_H_INCLUDED
