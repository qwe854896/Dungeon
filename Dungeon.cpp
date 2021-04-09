#include "Dungeon.h"
Dungeon::Dungeon(){
}

/* randomly pick a number by given probability */
int Dungeon::getRand(vector<long double> p)
{
    uniform_real_distribution<long double> distribution(0, p.back());
    mt19937 engine( rand() );

    long double value = distribution(engine);
    return (int)(upper_bound(p.begin(), p.end(), value) - p.begin());
}

/* Create a new player, and give him/her basic status */
void Dungeon::createPlayer() {
    string name = "";
    cout << "Please enter player's name: ";
    while (name == "") getline(cin, name);
    player = Player(name, "^_^", "Player", 0);
    player.increaseLV();
}

/* Create a map, which include several different rooms */
void Dungeon::createMap() {
    cout << "Creating map...\n";

    rooms.resize(128);
    vis.resize(128);

    rooms[0] = Room(0, 0); // init
    vis[0] = 1;
    rooms[0].setRightRoom(&rooms[1]);
    player.setCurrentRoom(&rooms[0]);
    player.setPreviousRoom(&rooms[0]);

    rooms[1] = Room(0, 1); // monster
    vis[1] = 1;
    rooms[1].setUpRoom(&rooms[2]);
    rooms[1].setDownRoom(&rooms[3]);
    rooms[1].setLeftRoom(&rooms[0]);
    rooms[1].setRightRoom(&rooms[4]);

    Monster *slime = new Monster("Slime", "", "Slime", 1, 10, 10, 10, 10, 10, 10);
    rooms[1].pushObject(slime);

    rooms[2] = Room(0, 2); // NPC
    vis[2] = 0;
    rooms[2].setDownRoom(&rooms[1]);

    Item sword = Item("Rusty Sword", "Weapon", 0, 0, 0, 10, 0, 100, 50);
    Item sticks = Item("Rusty Sticks", "Weapon", 0, 0, 0, 5, 0, 100, 25);
    Item shoes = Item("Old Shoes", "Boots", 0, 0, 0, 0, 10, 100, 50);
    vector <Item> goods;
    goods.emplace_back(sword);
    goods.emplace_back(sticks);
    goods.emplace_back(shoes);
    NPC *Chris = new NPC("Chris", ">_>", "Shop", "Hello, I'm Chris.\nI sell the following items: \n", goods, 10);
    rooms[2].pushObject(Chris);

    rooms[3] = Room(0, 3); // chest
    vis[3] = 0;
    rooms[3].setUpRoom(&rooms[1]);

    Item *chest = new Item("chest", "Chest", 0, 0, 0, 0, 0, 100, 0);
    rooms[3].pushObject(chest);

    rooms[4] = Room(1, 4); // Boss
    vis[4] = 1;
    rooms[4].setLeftRoom(&rooms[1]);

    Monster *boss = new Monster("Boss", "", "Boss", 10, 200, 200, 100, 100, 20, 20);
    rooms[4].pushObject(boss);

    usedIndex = 4;
    cout << "Done.\n";
}

/* Deal with player's moveing action */
void Dungeon::handleMovement() {
    Room* room = player.getCurrentRoom();

    if (checkMonsterRoom(room->getObjects())) {
        cout << "A. Go previous room\nB. Cancel\n";

        string ops; getline(cin, ops);
        while (ops[0] < 'A' || ops[0] > 'B') {
            cout << "Error, please enter operation again.\n";
            getline(cin, ops);
        }

        if (ops[0] == 'A') player.changeRoom(player.getPreviousRoom());
        return;
    }

    char option = 'A' - 1;
    vector <Room*> options;
    if (room->getUpRoom() != nullptr)
    {
        cout << (++option) << ". Go up\n";
        options.emplace_back(room->getUpRoom());
    }
    if (room->getDownRoom() != nullptr)
    {
        cout << (++option) << ". Go down\n";
        options.emplace_back(room->getDownRoom());
    }
    if (room->getLeftRoom() != nullptr)
    {
        cout << (++option) << ". Go left\n";
        options.emplace_back(room->getLeftRoom());
    }
    if (room->getRightRoom() != nullptr)
    {
        cout << (++option) << ". Go right\n";
        options.emplace_back(room->getRightRoom());
    }
    cout << (++option) << ". Cancel\n";

    string ops; getline(cin, ops);
    while (ops[0] < 'A' || OPS > options.size()) {
        cout << "Error, please enter operation again.\n";
        getline(cin, ops);
    }
    if (OPS < options.size()) player.changeRoom(options[OPS]);
}

/* Deal with player's interaction with objects in that room */
/* If triggerEvent return true, pop this object */
void Dungeon::handleEvent(Object* obj) {
    if (obj->triggerEvent(&player)) {
        player.getCurrentRoom()->popObject(obj);
        delete obj;
    }
}

/* Deal with player's interaction with inventory */
void Dungeon::handleInventory()
{
    if (player.listInventory())
    {
        string ops; getline(cin, ops);
        while (ops[0] < 'A' || OPS > player.getInventory().size()) {
            cout << "Error, please enter operation again.\n";
            getline(cin, ops);
        }
        if (OPS < player.getInventory().size())
        {
            player.useItem(OPS);
        }
    }
}

/* Deal with all game initial setting       */
/* Including create player, create map etc  */
void Dungeon::startGame() {
    cout << "Do you want to load previous data? (Y/N)\n";

    string ops; getline(cin, ops);
    while (ops[0] != 'Y' && ops[0] != 'N') {
        cout << "Error, please enter operation again.\n";
        getline(cin, ops);
    }

    if (ops[0] == 'Y') {
        system("cls");
        Record agent(6);
        if (agent.loadFromFile(&player, rooms, vis, usedIndex)) return;
    }

    system("cls");

    createPlayer();
    createMap();
}

/* Deal with the player's action     */
/* including showing the action list */
/* that player can do at that room   */
/* and dealing with player's input   */
void Dungeon::chooseAction(const vector<Object*>& objects) {
    cout << "What do you want to do?\n";
    cout << "A. MOVE\nB. STAT\nC. SAVE\nD. ITEM\n";

    char option = 'D';
    vector<Object*> options;

    if (checkMonsterRoom(objects))
    {
        for (auto obj : objects) {
            if (obj->getTag() == "Monster") {
                cout << (++option) << ". Fight the monster\n";
                options.emplace_back(obj);
            }
        }

    }
    else
    {
        for (auto obj : objects) {
            if (obj->getTag() == "NPC") {
                cout << (++option) << ". Talk to Shop\n";
                options.emplace_back(obj);
            }
            else if (obj->getTag() == "Item") {
                Item *item = dynamic_cast<Item*>(obj);
                cout << (++option);
                if (item->getKind() == "Chest") cout << ". Open the chest\n";
                else cout << ". Pick up Item\n";
                options.emplace_back(obj);
            }
        }
    }

    string ops; getline(cin, ops);
    while (ops[0] < 'A' || OPS >= options.size() + 4) {
        cout << "Error, please enter operation again.\n";
        getline(cin, ops);
    }

    system("cls");

    if (ops[0] == 'A') {
        handleMovement();
        system("cls");
        return;
    }
    else if (ops[0] == 'B') {
        player.triggerEvent(&player);
    }
    else if (ops[0] == 'C') {
        Record agent(6);
        agent.saveToFile(&player, rooms, vis, usedIndex);
    }
    else if (ops[0] == 'D') {
        handleInventory();
    }
    else {
        handleEvent(options[OPS - 4]);
    }
    cout << endl;
    system("pause"); system("cls");
}

/* Check whether the game should end or not */
/* Including player victory, or he/she dead */
bool Dungeon::checkGameLogic() {
    if (player.checkIsDead()) {
        cout << "Game Over!\n";
        return true;
    }
    if (player.getCurrentRoom()->getIsExit() && !checkMonsterRoom(player.getCurrentRoom()->getObjects())) {
        cout << "Victory!\n";
        return true;
    }
    return false;
}

/* Deal with the whole game process */
void Dungeon::runDungeon() {
    startGame();

    cout << endl;
    system("pause"); system("cls");

    while (!checkGameLogic()) {
        createRoom(player.getCurrentRoom()->getIndex());
        system("cls");
        chooseAction(player.getCurrentRoom()->getObjects());
    }
}


/* Check there is a monster or not in the current room */
bool Dungeon::checkMonsterRoom(const vector<Object*>& objects)
{
    for (auto obj : objects) {
        if (obj->getTag() == "Monster") {
            return true;
        }
    }
    return false;
}

/* If current Room is not visited, try to generate its neighbor */
void Dungeon::createRoom(int index)
{
    if (vis[index]) return;
    vis[index] = 1;

    long double prob = 75;
    
    for (int i = 0; i < 4; ++i) {
        switch ( getRand(vector<long double>( {25, 50, 75, 100} ) ) ) {
            case 0:
                if (rooms[index].getUpRoom() == nullptr)
                {
                    int genOrNot = getRand(vector<long double>( {prob, 100} ));
                    if (!genOrNot)
                    {
                        prob -= 25;
                        ++usedIndex;
                        rooms[usedIndex] = generateRoom(0, usedIndex, player.getLV(), "RANDOM");
                        rooms[index].setUpRoom(&rooms[usedIndex]);
                        rooms[usedIndex].setDownRoom(&rooms[index]);
                    }
                }
                break;
            case 1:
                if (rooms[index].getDownRoom() == nullptr)
                {
                    int genOrNot = getRand(vector<long double>( {prob, 100} ));
                    if (!genOrNot)
                    {
                        prob -= 25;
                        ++usedIndex;
                        rooms[usedIndex] = generateRoom(0, usedIndex, player.getLV(), "RANDOM");
                        rooms[index].setDownRoom(&rooms[usedIndex]);
                        rooms[usedIndex].setUpRoom(&rooms[index]);
                    }
                }
                break;
            case 2:
                if (rooms[index].getLeftRoom() == nullptr)
                {
                    int genOrNot = getRand(vector<long double>( {prob, 100} ));
                    if (!genOrNot)
                    {
                        prob -= 25;
                        ++usedIndex;
                        rooms[usedIndex] = generateRoom(0, usedIndex, player.getLV(), "RANDOM");
                        rooms[index].setLeftRoom(&rooms[usedIndex]);
                        rooms[usedIndex].setRightRoom(&rooms[index]);
                    }
                }
                break;
            case 3:
                if (rooms[index].getRightRoom() == nullptr)
                {
                    int genOrNot = getRand(vector<long double>( {prob, 100} ));
                    if (!genOrNot)
                    {
                        prob -= 25;
                        ++usedIndex;
                        rooms[usedIndex] = generateRoom(0, usedIndex, player.getLV(), "RANDOM");
                        rooms[index].setRightRoom(&rooms[usedIndex]);
                        rooms[usedIndex].setLeftRoom(&rooms[index]);
                    }
                }
                break;
        }
    }
}

/* Random Generator */
Monster Dungeon::generateMonster(int LV)
{
    Monster monster = Monster("Slime's friend", ">_<", "Monster", LV - 2);
    monster.increaseLV();

    return monster;
}
NPC Dungeon::generateNPC(int LV, string kind)
{
    NPC npc = NPC("Chris's mother", "~_~", kind, "Hello, I'm Chris's mother.\nI sell the following items: \n", LV - 1);
    npc.increaseLV();

    if (kind == "Shop") {
        vector <Item> goods;
        goods.emplace_back(Item::randomItemGenerator(LV, "RANDOM"));
        npc.setCommodity(goods);
    }

    return npc;
}
Room Dungeon::generateRoom(bool isExit, int index, int LV, string kind)
{
    Room room = Room(isExit, index);

    string kindList[4] = {
        "Monster",
        "Shop",
        "Chest",
        "Boss"
    };
    if (kind == "RANDOM") kind = kindList[ getRand(vector<long double>( {30, 60, 90, 100} )) ];

    if (kind == "Monster")
    {
        Monster *monster = new Monster();
        *monster = generateMonster(LV);
        room.pushObject(monster);
    }
    else if (kind == "Shop")
    {
        NPC *npc = new NPC();
        *npc = generateNPC(LV, kind);
        room.pushObject(npc);
    }
    else if (kind == "Chest")
    {
        Item *chest = new Item("chest", "Chest", 0, 0, 0, 0, 0, 100, 0);
        room.pushObject(chest);
    }
    else if (kind == "Boss")
    {
        Monster *boss = new Monster();
        *boss = generateMonster(LV);
        room.pushObject(boss);
        room.setIsExit(1);
    }

    return room;
}
