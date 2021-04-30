#include "Dungeon.h"
Dungeon::Dungeon() : _player(150) {
    _window = new RenderWindow(VideoMode(1920, 1080), "Dungeon!");

    _player.setFillColor(Color::Blue);
    _player.setPosition(10, 20);
}
Dungeon::~Dungeon(){
    delete _window;
    for (auto& room : rooms) {
        for (auto &object : room.getObjects()) {
            delete object;
        }
    }
}

namespace {
    /* randomly pick a number by given probability */
    int getRand(vector<long double> p)
    {
        uniform_real_distribution<long double> distribution(0, p.back());
        mt19937 engine( rand() );

        long double value = distribution(engine);
        return (int)(upper_bound(p.begin(), p.end(), value) - p.begin());
    }
}

/* Create a new player, and give him/her basic status */
void Dungeon::createPlayer() {
    string name = "";
    cout << "Please enter player's name: ";
    while (name == "") getline(cin, name);
    player = Player(name, "^_^", "Player", 0);
    player.increaseLV();
    // player.setAttack(1000000);
}

/* Create a map, which include several different rooms */
void Dungeon::createMap() {
    cout << "Creating map...\n";

    rooms.resize(128);
    vis.resize(128);

    rooms[0] = Room(0, 0, 0, 0); // init
    vis[0] = 0;
    coordToIndex[ coord(0, 0) ] = 0;
    // rooms[0].setRightRoom(&rooms[1]);
    player.setCurrentRoom(&rooms[0]);
    player.setPreviousRoom(&rooms[0]);

    // rooms[1] = Room(0, 1, 1, 0); // monster
    // vis[1] = 1;
    // coordToIndex[ coord(1, 0) ] = 1;
    // rooms[1].setUpRoom(&rooms[2]);
    // rooms[1].setDownRoom(&rooms[3]);
    // rooms[1].setLeftRoom(&rooms[0]);
    // rooms[1].setRightRoom(&rooms[4]);

    // Monster *slime = new Monster("Slime", ">_<", "Slime", 0, 10, 10, 10, 10, 10, 10);
    // rooms[1].pushObject(slime);

    // rooms[2] = Room(0, 2, 1, 1); // NPC
    // vis[2] = 0;
    // coordToIndex[ coord(1, 1) ] = 2;
    // rooms[2].setDownRoom(&rooms[1]);

    // Item sword = Item("Rusty Sword", "Weapon", 0, 0, 0, 10, 0, 3, 50);
    // Item sticks = Item("Rusty Sticks", "Weapon", 0, 0, 0, 5, 0, 3, 25);
    // Item shoes = Item("Old Shoes", "Boots", 0, 0, 0, 0, 10, 3, 50);
    // vector <Item> goods;
    // goods.emplace_back(sword);
    // goods.emplace_back(sticks);
    // goods.emplace_back(shoes);
    // NPC *Chris = new NPC("Chris", ">_>", "Shop", "Hello, I'm Chris.\nI sell the following items: \n", goods, 10);
    // rooms[2].pushObject(Chris);

    // rooms[3] = Room(0, 3, 1, -1); // chest
    // vis[3] = 0;
    // coordToIndex[ coord(1, -1) ] = 3;
    // rooms[3].setUpRoom(&rooms[1]);

    // Item *chest = new Item("chest", "Chest", 0, 0, 0, 0, 0, 100, 0);
    // rooms[3].pushObject(chest);

    // rooms[4] = Room(1, 4, 2, 0); // Boss
    // vis[4] = 1;
    // coordToIndex[ coord(2, 0) ] = 4;
    // rooms[4].setLeftRoom(&rooms[1]);

    // Monster *boss = new Monster("Boss", "~_~", "Boss", 10, 20, 1000, 1000, 600, 100, 100);
    // rooms[4].pushObject(boss);

    usedIndex = 0;
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
    // system("cls");
    if (obj->triggerEvent(&player)) {
        player.getCurrentRoom()->popObject(obj);
        delete obj;
    }
}

/* Deal with player's interaction with inventory */
void Dungeon::handleInventory()
{
    player.handleInventory("use");
}

/* Deal with all game initial setting       */
/* Including create player, create map etc  */
void Dungeon::startGame() {
    Font font; font.loadFromFile("../fonts/Dosis-Light.ttf");

    Button probs(160, 100, 1600, 200, 60, &font, "Do you want to load previous data?", Color(100, 100, 150, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
    cout << "Do you want to load previous data? (Y/N)\n";
    Menu YN(380, 670, 500, 200, 60, false, &font, _window, Color(20, 20, 20, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
    YN.push_back("Yes");
    YN.push_back("NO");

    int ops;
    while (_window->isOpen()) {
        updateDt();
        while (_window->pollEvent(sfEvent)) {
            if ((sfEvent.type == Event::KeyPressed) && (sfEvent.key.code == Keyboard::Escape)) {
                _window->close();
            }
            switch (sfEvent.type) {
                case Event::Closed:
                    _window->close();
                    break;
                default:
                    break;
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Enter)) {
            ops = YN.getIsSelected();
            break;
        }
        if (Keyboard::isKeyPressed(Keyboard::K)) {
            cout << Mouse::getPosition().x << ' ' << Mouse::getPosition().y << endl;
        }
        YN.update();

        _window->clear();

        probs.render(_window);
        YN.render(_window);

        _window->display();
    }
    

    // string ops; getline(cin, ops);
    // while (ops[0] != 'Y' && ops[0] != 'N') {
    //     cout << "Error, please enter operation again.\n";
    //     getline(cin, ops);
    // }

    if (ops == 0) {
        system("cls");
        Record agent(6, _window);
        if (agent.loadFromFile(&player, rooms, vis, usedIndex, coordToIndex)) return;
    }

    system("cls");

    createPlayer();
    createMap();
}

void Dungeon::encounterDanny()
{
    cout << "Worst Nightmare! You encounter Danny,Food King in this Dungeon!\n";
    cout << "Danny : I am going to rob all of your food :D\n";
    vector<Item> inv = player.getInventory();
    for(int i=0; i<inv.size(); ++i)
    {
        if(inv[i].getKind() == "Food")
        {
            player.popItem(i);
            --i;
        }
    }
    cout << "All of your food has eaten by Danny!\n";
}
/* Deal with the player's action     */
/* including showing the action list */
/* that player can do at that room   */
/* and dealing with player's input   */
void Dungeon::chooseAction(const vector<Object*>& objects) {
    system("cls");
    generateDescription();

    cout << "What do you want to do?\n";
    cout << "A. MOVE\nB. STAT\nC. SAVE\nD. ITEM\n";

    char option = 'D';
    vector<Object*> options;

    if (checkMonsterRoom(objects))
    {
        for (auto obj : objects) {
            if (obj->getTag() == "Monster") {
                if(obj->getName() == "Danny")
                {
                    encounterDanny();
                }
                cout << (++option) << ". Fight the monster: " << obj->getName() << "\n";
                options.emplace_back(obj);
            }
        }

    }
    else
    {
        for (auto obj : objects) {
            if (obj->getTag() == "NPC") {
                cout << (++option) << ". Talk to Shopkeeper: " << obj->getName() << "\n";
                options.emplace_back(obj);
            }
            else if (obj->getTag() == "Item") {
                Item *item = dynamic_cast<Item*>(obj);
                cout << (++option);
                if (item->getKind() == "Chest") cout << ". Open the chest\n";
                else cout << ". Pick up Item: " << obj->getName() << "\n";
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
        return;
    }
    else if (ops[0] == 'B') {
        player.triggerEvent(&player);
    }
    else if (ops[0] == 'C') {
        Record agent(6, _window);
        agent.saveToFile(&player, rooms, vis, usedIndex, coordToIndex);
    }
    else if (ops[0] == 'D') {
        handleInventory();
    }
    else {
        handleEvent(options[OPS - 4]);
    }
    cout << endl;
    system("pause");
}

/* Check whether the game should end or not */
/* Including player victory, or he/she dead */
bool Dungeon::checkGameLogic() {
    system("cls");
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
void Dungeon::runDungeon(int minimum_frame_per_seconds) {
    bool isStarted = 0, isChecked = 1, isCreated = 1, isChose = 1;

    while (_window->isOpen()) {
        if (!isStarted) {
            isStarted = 1;
            startGame();
        }

        createRoom(player.getCurrentRoom()->getIndex());
        chooseAction(player.getCurrentRoom()->getObjects());    
        
        updateDt();
        update();
        render();
    }

    // startGame();

    // cout << "\n";
    // system("pause");

    // while (!checkGameLogic()) {
    //     createRoom(player.getCurrentRoom()->getIndex());
    //     chooseAction(player.getCurrentRoom()->getObjects());
    // }
    // cout << endl;
    // system("pause");

    // while (_window->isOpen()) {
    //     updateDt();
    //     update();
    //     render();
    // }
}

void Dungeon::updateDt() {
    /* updates the dt variable with the time it takes to update and render one frame. */
    this->dt = this->dtClock.restart().asSeconds();
    // system("cls");
    // cout << this->dt << endl;
}

void Dungeon::updateSFMLEvents() {
    while (_window->pollEvent(sfEvent)) {
        if ((sfEvent.type == Event::KeyPressed) && (sfEvent.key.code == Keyboard::Escape)) {
            _window->close();
        }
        switch (sfEvent.type) {
            case Event::Closed:
                _window->close();
                break;
            case Event::LostFocus:
                cout << "Player leaves the app\n";
                break;
            case Event::GainedFocus:
                cout << "Player is looking at the app\n";
                break;
            case Event::TextEntered:
                if (sfEvent.text.unicode < 128)
                    cout << char(sfEvent.text.unicode) << endl;
                break;
            case Event::Resized:
                cout << "new width: " << sfEvent.size.width << endl;
                cout << "new height: " << sfEvent.size.height << endl;
                break;
            case Event::KeyPressed:
                if (sfEvent.key.code == Keyboard::Escape) {
                    cout << "escape key pressed" << endl;
                    cout << "control: " << sfEvent.key.control << endl;
                    cout << "alt: " << sfEvent.key.alt << endl;
                    cout << "shift: " << sfEvent.key.shift << endl;
                    cout << "system: " << sfEvent.key.system << endl;
                }
                break;
            default:
                break;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            // left key is pressed: move our character
            cout << "UP" << endl;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            // left key is pressed: move our character
            cout << "LEFT" << endl;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            // left key is pressed: move our character
            cout << "DOWN" << endl;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            // left key is pressed: move our character
            cout << "RIGHT" << endl;
        }
    }
}

void Dungeon::update() {
    updateSFMLEvents();
}

void Dungeon::render() {
    _window->clear();

    // Render Items
    _window->draw(_player);

    _window->display();
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

    int x = rooms[index].getX(), y = rooms[index].getY();
    int X, Y;

    bool flag = 0;
    long double prob = 75;
    
    system("cls");
    for (int i = 0; i < 4; ++i) {
        switch ( getRand(vector<long double>( {25, 50, 75, 100} ) ) ) {
            case 0:
                X = x, Y = y + 1;
                if (rooms[index].getUpRoom() == nullptr)
                {
                    if ( coordToIndex.find( coord(X, Y) ) != coordToIndex.end() ) {
                        if (vis[ coordToIndex[ coord(X, Y) ] ]) cout << "A path to the past room appeared!!!(Up room)\n"; flag = 1;
                        rooms[index].setUpRoom(&rooms[ coordToIndex[ coord(X, Y) ] ]);
                        rooms[ coordToIndex[ coord(X, Y) ] ].setDownRoom(&rooms[index]);
                        continue;
                    }
                    int genOrNot = getRand(vector<long double>( {prob, 100} ));
                    if (!genOrNot)
                    {
                        prob -= 25;
                        ++usedIndex;
                        rooms[usedIndex] = generateRoom(0, usedIndex, X, Y, player.getLV(), "RANDOM");
                        rooms[index].setUpRoom(&rooms[usedIndex]);
                        rooms[usedIndex].setDownRoom(&rooms[index]);
                    }
                }
                break;
            case 1:
                X = x, Y = y - 1;
                if (rooms[index].getDownRoom() == nullptr)
                {
                    if ( coordToIndex.find( coord(X, Y) ) != coordToIndex.end() ) {
                        if (vis[ coordToIndex[ coord(X, Y) ] ]) cout << "A path to the past room appeared!!!(Down room)\n"; flag = 1;
                        rooms[index].setDownRoom(&rooms[ coordToIndex[ coord(X, Y) ] ]);
                        rooms[ coordToIndex[ coord(X, Y) ] ].setUpRoom(&rooms[index]);
                        continue;
                    }
                    int genOrNot = getRand(vector<long double>( {prob, 100} ));
                    if (!genOrNot)
                    {
                        prob -= 25;
                        ++usedIndex;
                        rooms[usedIndex] = generateRoom(0, usedIndex, X, Y, player.getLV(), "RANDOM");
                        rooms[index].setDownRoom(&rooms[usedIndex]);
                        rooms[usedIndex].setUpRoom(&rooms[index]);
                    }
                }
                break;
            case 2:
                X = x - 1, Y = y;
                if (rooms[index].getLeftRoom() == nullptr)
                {
                    if ( coordToIndex.find( coord(X, Y) ) != coordToIndex.end() ) {
                        if (vis[ coordToIndex[ coord(X, Y) ] ]) cout << "A path to the past room appeared!!!(Left room)\n"; flag = 1;
                        rooms[index].setLeftRoom(&rooms[ coordToIndex[ coord(X, Y) ] ]);
                        rooms[ coordToIndex[ coord(X, Y) ] ].setRightRoom(&rooms[index]);
                        continue;
                    }
                    int genOrNot = getRand(vector<long double>( {prob, 100} ));
                    if (!genOrNot)
                    {
                        prob -= 25;
                        ++usedIndex;
                        rooms[usedIndex] = generateRoom(0, usedIndex, X, Y, player.getLV(), "RANDOM");
                        rooms[index].setLeftRoom(&rooms[usedIndex]);
                        rooms[usedIndex].setRightRoom(&rooms[index]);
                    }
                }
                break;
            case 3:
                X = x + 1, Y = y;
                if (rooms[index].getRightRoom() == nullptr)
                {
                    if ( coordToIndex.find( coord(X, Y) ) != coordToIndex.end() ) {
                        if (vis[ coordToIndex[ coord(X, Y) ] ]) cout << "A path to the past room appeared!!!(Right room)\n"; flag = 1;
                        rooms[index].setRightRoom(&rooms[ coordToIndex[ coord(X, Y) ] ]);
                        rooms[ coordToIndex[ coord(X, Y) ] ].setLeftRoom(&rooms[index]);
                        continue;
                    }
                    int genOrNot = getRand(vector<long double>( {prob, 100} ));
                    if (!genOrNot)
                    {
                        prob -= 25;
                        ++usedIndex;
                        rooms[usedIndex] = generateRoom(0, usedIndex, X, Y, player.getLV(), "RANDOM");
                        rooms[index].setRightRoom(&rooms[usedIndex]);
                        rooms[usedIndex].setLeftRoom(&rooms[index]);
                    }
                }
                break;
        }
    }
    if (flag) {
        cout << "\n";
        system("pause");
    }
}

/* Random Generator */
Monster Dungeon::generateMonster(int LV)
{
    vector<Monster> monster_list 
    {
        Monster("Orc", "", "Monster", LV - 1), 
        Monster("Dwarf", "", "Monster", LV - 1), 
        Monster("Elf", "", "Monster", LV - 1), 
        Monster("Dragon", "", "Monster", LV - 1), 
        Monster("Wizard", "", "Monster", LV - 1), 
        Monster("Troll", "", "Monster", LV - 1), 
        Monster("Basilisk", "", "Monster", LV - 1), 
        Monster("CaveMan", "", "Monster", LV - 1), 
        Monster("Phoenix", "", "Monster", LV - 1), 
        Monster("Cerberus", "", "Monster", LV - 1), 
        Monster("Danny", "", "Monster", LV - 1)
        };

    Monster monster = Monster("Slime's friend", ">_<", "Monster", LV - 1);
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
Room Dungeon::generateRoom(bool isExit, int index, int X, int Y, int LV, string kind)
{
    Room room = Room(isExit, index, X, Y);
    coordToIndex[ coord(X, Y) ] = index;

    string kindList[4] = {
        "Monster",
        "Shop",
        "Chest",
        "Boss"
    };

    long double condition = player.getLV() >= 5 ? 200 : 90;

    if (kind == "RANDOM") kind = kindList[ getRand(vector<long double>( {30, 60, 90, condition} )) ];

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
        boss->setName("Boss");
        room.pushObject(boss);
        room.setIsExit(1);
    }

    return room;
}
void Dungeon::generateDescription() {
    cout << "It's beautiful outside.\n";
    cout << "You are at (" << player.getCurrentRoom()->getX() << ", " << player.getCurrentRoom()->getY() << ").\n";
    cout << "\n";
} 