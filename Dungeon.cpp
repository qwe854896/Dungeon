#include "Dungeon.h"

Dungeon::Dungeon() /*: _player(150)*/ {
    _window = new RenderWindow(VideoMode(1920, 1080), "Dungeon!", Style::Default | Style::Fullscreen);
    font = new Font();
    font->loadFromFile("../fonts/Dosis-Light.ttf");
    // _player.setFillColor(Color::Blue);
    // _player.setPosition(10, 20);
}
Dungeon::~Dungeon(){
    Record agent(6, _window);
    ofstream fout("../error");
    agent.saveToFile(&player, rooms, vis, usedIndex, coordToIndex, fout);
    delete _window;
    delete font;
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
    Button *button = new Button(540, 340, 840, 100, 1, 60, font, "Please enter player's name", Color(0, 0, 0, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
    Button *inputName = new Button(540, 450, 840, 100, 1, 60, font, "", Color(0, 0, 0, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));

    gainedFocus = 1;
    holdKey = 0;
    holdEnter = 1;
    while (_window->isOpen()) {
        while (_window->pollEvent(sfEvent)) {
            if ((sfEvent.type == Event::KeyPressed) && (sfEvent.key.code == Keyboard::Escape)) {
                _window->close();
            }
            switch (sfEvent.type) {
                case Event::Closed:
                    _window->close();
                    break;
                case Event::GainedFocus:
                    gainedFocus = 1;
                    break;
                case Event::LostFocus:
                    gainedFocus = 0;
                    break;
                case Event::TextEntered:
                    if (sfEvent.text.unicode < 128) {
                        char tmp = static_cast<char>(sfEvent.text.unicode);
                        name += string(1, tmp);
                        inputName->updateText(name);
                    }
                    break;
                default:
                    break;
            }
        }
        if (!gainedFocus) {
            sleep(milliseconds(10));
            continue;
        }
        if (!holdEnter && Keyboard::isKeyPressed(Keyboard::Enter)) {
            if (name != "") break;
            holdEnter = 1;
        }
        if (holdEnter && !Keyboard::isKeyPressed(Keyboard::Enter)) {
            holdEnter = 0;
        }
        if (!holdKey && Keyboard::isKeyPressed(Keyboard::Backspace)) {
            holdKey = 1;
            if (!name.empty()) name.pop_back();
            if (!name.empty()) name.pop_back();
            inputName->updateText(name);
        }
        if (holdKey && !Keyboard::isKeyPressed(Keyboard::Backspace)) {
            holdKey = 0;
        }
        
        _window->clear();

        button->render(_window);
        inputName->render(_window);

        _window->display();
    }
    delete button;
    delete inputName;

    // while (name == "") getline(cin, name);
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
    vis[0] = 1;
    coordToIndex[ coord(0, 0) ] = 0;
    rooms[0].setRightRoom(&rooms[1]);

    player.setCurrentRoom(&rooms[0]);
    player.setPreviousRoom(&rooms[0]);

    rooms[1] = Room(0, 1, 1, 0); // monster
    vis[1] = 1;
    coordToIndex[ coord(1, 0) ] = 1;
    rooms[1].setUpRoom(&rooms[2]);
    rooms[1].setDownRoom(&rooms[3]);
    rooms[1].setLeftRoom(&rooms[0]);
    rooms[1].setRightRoom(&rooms[4]);

    Monster *slime = new Monster("Slime", ">_<", "Slime", 0, 10, 10, 10, 10, 10, 10);
    rooms[1].pushObject(slime);

    rooms[2] = Room(0, 2, 1, 1); // NPC
    vis[2] = 0;
    coordToIndex[ coord(1, 1) ] = 2;
    rooms[2].setDownRoom(&rooms[1]);

    Item sword = Item("Rusty Sword", "Weapon", 0, 0, 0, 10, 0, 3, 50);
    Item sticks = Item("Rusty Sticks", "Weapon", 0, 0, 0, 5, 0, 3, 25);
    Item shoes = Item("Old Shoes", "Boots", 0, 0, 0, 0, 10, 3, 50);
    vector <Item> goods;
    goods.emplace_back(sword);
    goods.emplace_back(sticks);
    goods.emplace_back(shoes);
    NPC *Chris = new NPC("Chris", ">_>", "Shop", "Hello, I'm Chris.\nI sell the following items: \n", goods, 10);
    rooms[2].pushObject(Chris);

    rooms[3] = Room(0, 3, 1, -1); // chest
    vis[3] = 0;
    coordToIndex[ coord(1, -1) ] = 3;
    rooms[3].setUpRoom(&rooms[1]);

    Item *chest = new Item("chest", "Chest", 0, 0, 0, 0, 0, 100, 0);
    Item *chest1 = new Item("chest", "Chest", 0, 0, 0, 0, 0, 100, 0);
    Item *chest2 = new Item("chest", "Chest", 0, 0, 0, 0, 0, 100, 0);
    rooms[3].pushObject(chest);
    rooms[3].pushObject(chest1);
    rooms[3].pushObject(chest2);

    rooms[4] = Room(1, 4, 2, 0); // Boss
    vis[4] = 1;
    coordToIndex[ coord(2, 0) ] = 4;
    rooms[4].setLeftRoom(&rooms[1]);

    Monster *boss = new Monster("Boss", "~_~", "Boss", 10, 20, 1000, 1000, 600, 100, 100);
    rooms[4].pushObject(boss);

    usedIndex = 4;
    cout << "Done.\n";
}

/* Deal with player's moveing action */
void Dungeon::handleMovement() {
    Room* room = player.getCurrentRoom();
    // vector <Room*> options;

    // menu = new Menu(380, 670, 500, 200, 1, 60, false, font, _window, Color(20, 20, 20, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
    
    

    bool isAvailable[5] = {0};
    Button buttons[5] = {
        Button(710, 75, 500, 180, 1, 60, font, "Go up", Color(20, 20, 20, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255)),
        Button(710, 825, 500, 180, 1, 60, font, "Go Down", Color(20, 20, 20, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255)),
        Button(710, 450, 500, 180, 1, 60, font, "Cancel", Color(20, 20, 20, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255)),
        Button(40, 450, 500, 180, 1, 60, font, "Go Left", Color(20, 20, 20, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255)),
        Button(1380, 450, 500, 180, 1, 60, font, "Go right", Color(20, 20, 20, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255)),
    };

    if (checkMonsterRoom(room->getObjects())) {
        if (player.getPreviousRoom() == room->getUpRoom()) isAvailable[0] = 1;
        if (player.getPreviousRoom() == room->getDownRoom()) isAvailable[1] = 1;
        if (player.getPreviousRoom() == room->getLeftRoom()) isAvailable[3] = 1;
        if (player.getPreviousRoom() == room->getRightRoom()) isAvailable[4] = 1;

        // cout << "A. Go previous room\nB. Cancel\n";
        // menu->push_back("Go previous room");

        // string ops; getline(cin, ops);
        // while (ops[0] < 'A' || ops[0] > 'B') {
        //     cout << "Error, please enter operation again.\n";
        //     getline(cin, ops);
        // }

        // options.emplace_back(player.getPreviousRoom());
    }
    else {
        char option = 'A' - 1;
        
        if (room->getUpRoom() != nullptr)
        {
            cout << (++option) << ". Go up\n";
            isAvailable[0] = 1;
            // menu->push_back("Go up");
            // options.emplace_back(room->getUpRoom());
        }
        if (room->getDownRoom() != nullptr)
        {
            cout << (++option) << ". Go down\n";
            isAvailable[1] = 1;
            // menu->push_back("Go down");
            // options.emplace_back(room->getDownRoom());
        }
        if (room->getLeftRoom() != nullptr)
        {
            cout << (++option) << ". Go left\n";
            isAvailable[3] = 1;
            // menu->push_back("Go left");
            // options.emplace_back(room->getLeftRoom());
        }
        if (room->getRightRoom() != nullptr)
        {
            cout << (++option) << ". Go right\n";
            isAvailable[4] = 1;
            // menu->push_back("Go right");
            // options.emplace_back(room->getRightRoom());
        }
        cout << (++option) << ". Cancel\n";
    }

    isAvailable[2] = 1;
    // menu->push_back("Cancel");

    
    bool pressUp = false, pressDown = false, pressLeft = false, pressRight = false;
    gainedFocus = 1;
    holdEnter = 1;
    
    int isSelected = 2;
    while (_window->isOpen()) {
        while (_window->pollEvent(sfEvent)) {
            if ((sfEvent.type == Event::KeyPressed) && (sfEvent.key.code == Keyboard::Escape)) {
                _window->close();
            }
            switch (sfEvent.type) {
                case Event::Closed:
                    _window->close();
                    break;
                case Event::GainedFocus:
                    gainedFocus = 1;
                    break;
                case Event::LostFocus:
                    gainedFocus = 0;
                    break;
                default:
                    break;
            }
        }
        if (!gainedFocus) {
            sleep(milliseconds(10));
            continue;
        }
        if (!holdEnter && Keyboard::isKeyPressed(Keyboard::Enter)) {
            break;
        }
        if (holdEnter && !Keyboard::isKeyPressed(Keyboard::Enter)) {
            holdEnter = 0;
        }
        

        if (!pressUp && Keyboard::isKeyPressed(Keyboard::Up) ) {
            pressUp = 1;
            buttons[isSelected].update(0);
            switch (isSelected) {
                case 3:
                case 4:
                    if (isAvailable[0]) isSelected = 0;
                    else isSelected = 2;
                    break;
                case 2:
                    if (isAvailable[0]) isSelected = 0;
                    else if (isAvailable[1]) isSelected = 1;
                    break;
                case 0:
                    if (isAvailable[1]) isSelected = 1;
                    else isSelected = 2;
                    break;
                case 1:
                    isSelected = 2;
                    break;
            }
        }
        if (pressUp && !Keyboard::isKeyPressed(Keyboard::Up) ) {
            pressUp = 0;
        }
        

        if (!pressDown && Keyboard::isKeyPressed(Keyboard::Down) ) {
            pressDown = 1;
            buttons[isSelected].update(0);
            switch (isSelected) {
                case 3:
                case 4:
                    if (isAvailable[1]) isSelected = 1;
                    else isSelected = 2;
                    break;
                case 2:
                    if (isAvailable[1]) isSelected = 1;
                    else if (isAvailable[0]) isSelected = 0;
                    break;
                case 1:
                    if (isAvailable[0]) isSelected = 0;
                    else isSelected = 2;
                    break;
                case 0:
                    isSelected = 2;
                    break;
            }
        }
        if (pressDown && !Keyboard::isKeyPressed(Keyboard::Down) ) {
            pressDown = 0;
        }
        

        if (!pressLeft && Keyboard::isKeyPressed(Keyboard::Left) ) {
            pressLeft = 1;
            buttons[isSelected].update(0);
            switch (isSelected) {
                case 0:
                case 1:
                    if (isAvailable[3]) isSelected = 3;
                    else isSelected = 2;
                    break;
                case 2:
                    if (isAvailable[3]) isSelected = 3;
                    else if (isAvailable[4]) isSelected = 4;
                    break;
                case 3:
                    if (isAvailable[4]) isSelected = 4;
                    else isSelected = 2;
                    break;
                case 4:
                    isSelected = 2;
                    break;
            }
        }
        if (pressLeft && !Keyboard::isKeyPressed(Keyboard::Left) ) {
            pressLeft = 0;
        }
        

        if (!pressRight && Keyboard::isKeyPressed(Keyboard::Right) ) {
            pressRight = 1;
            buttons[isSelected].update(0);
            switch (isSelected) {
                case 0:
                case 1:
                    if (isAvailable[4]) isSelected = 4;
                    else isSelected = 2;
                    break;
                case 2:
                    if (isAvailable[4]) isSelected = 4;
                    else if (isAvailable[3]) isSelected = 3;
                    break;
                case 4:
                    if (isAvailable[3]) isSelected = 3;
                    else isSelected = 2;
                    break;
                case 3:
                    isSelected = 2;
                    break;
            }
        }
        if (pressRight && !Keyboard::isKeyPressed(Keyboard::Right) ) {
            pressRight = 0;
        }

        buttons[isSelected].update(1);

        _window->clear();

        for (int i = 0; i < 5; ++i) {
            if (isAvailable[i]) {
                buttons[i].render(_window);
            }
        }

        _window->display();
    }


    // string ops; getline(cin, ops);
    // while (ops[0] < 'A' || OPS > options.size()) {
    //     cout << "Error, please enter operation again.\n";
    //     getline(cin, ops);
    // }
    // if (ops < options.size()) player.changeRoom(options[ops]);

    // cout << "before change room" ;
    if (isSelected == 0) player.changeRoom(room->getUpRoom());
    if (isSelected == 1) player.changeRoom(room->getDownRoom());
    if (isSelected == 3) player.changeRoom(room->getLeftRoom());
    if (isSelected == 4) player.changeRoom(room->getRightRoom());
    // cout << "after change room";
}

/* Deal with player's interaction with objects in that room */
/* If triggerEvent return true, pop this object */
void Dungeon::handleEvent(Object* obj) {
    // system("cls");
    if (obj->triggerEvent(&player, _window)) {
        player.getCurrentRoom()->popObject(obj);
        delete obj;
    }
}

/* Deal with player's interaction with inventory */
void Dungeon::handleInventory()
{
    player.handleInventory("use", _window);
}

/* Deal with all game initial setting       */
/* Including create player, create map etc  */
void Dungeon::startGame() {
    Button *button = new Button(160, 100, 1600, 200, 1, 60, font, "Do you want to load previous data?", Color(100, 100, 150, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
    cout << "Do you want to load previous data? (Y/N)\n";

    Menu *menu = new Menu(380, 670, 500, 200, 1, 60, false, font, _window, Color(20, 20, 20, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
    menu->push_back("Yes");
    menu->push_back("NO");

    gainedFocus = 1;
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
                case Event::GainedFocus:
                    gainedFocus = 1;
                    break;
                case Event::LostFocus:
                    gainedFocus = 0;
                    break;
                default:
                    break;
            }
        }
        if (!gainedFocus) {
            sleep(milliseconds(10));
            continue;
        }
        if (!holdEnter && Keyboard::isKeyPressed(Keyboard::Enter)) {
            ops = menu->getIsSelected();
            break;
        }
        if (holdEnter && !Keyboard::isKeyPressed(Keyboard::Enter)) {
            holdEnter = 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::K)) {
            cout << Mouse::getPosition().x << ' ' << Mouse::getPosition().y << endl;
        }
        menu->update();

        _window->clear();

        button->render(_window);
        menu->render(_window);

        _window->display();
    }
    delete button;
    delete menu;
    

    // string ops; getline(cin, ops);
    // while (ops[0] != 'Y' && ops[0] != 'N') {
    //     cout << "Error, please enter operation again.\n";
    //     getline(cin, ops);
    // }

    if (ops == 0) {
        // system("cls");
        Record agent(6, _window);
        if (agent.loadFromFile(&player, rooms, vis, usedIndex, coordToIndex)) return;
    }

    // system("cls");

    createPlayer();
    createMap();
}

void Dungeon::encounterDanny()
{
    Button *button = new Button(160, 100, 1600, 200, 1, 60, font, "Worst Nightmare! You encounter Danny, Food King in this Dungeon!\nDanny : I am going to rob all of your food :D\nAll of your food has eaten by Danny!\n\nPress enter to continue...", Color(100, 100, 150, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
    cout << "Worst Nightmare! You encounter Danny, Food King in this Dungeon!\n";
    cout << "Danny : I am going to rob all of your food :D\n";

    vector<Item> inv = player.getInventory();
    for(int i = inv.size() - 1; i >= 0; --i)
    {
        if(inv[i].getKind() == "Food")
        {
            player.popItem(i);
        }
    }
    cout << "All of your food has eaten by Danny!\n";

    holdEnter = 1;
    gainedFocus = 1;
    while (_window->isOpen()) {
        while (_window->pollEvent(sfEvent)) {
            if ((sfEvent.type == Event::KeyPressed) && (sfEvent.key.code == Keyboard::Escape)) {
                _window->close();
            }
            switch (sfEvent.type) {
                case Event::Closed:
                    _window->close();
                    break;
                case Event::GainedFocus:
                    gainedFocus = 1;
                    break;
                case Event::LostFocus:
                    gainedFocus = 0;
                    break;
                default:
                    break;
            }
        }
        if (!gainedFocus) {
            sleep(milliseconds(10));
            continue;
        }
        if (!holdEnter && Keyboard::isKeyPressed(Keyboard::Enter)) {
            break;
        }
        if (holdEnter && !Keyboard::isKeyPressed(Keyboard::Enter)) {
            holdEnter = 0;
        }

        _window->clear();
		
        button->render(_window);

        _window->display();
    }
    delete button;
}

/* Deal with the player's action     */
/* including showing the action list */
/* that player can do at that room   */
/* and dealing with player's input   */
void Dungeon::chooseAction(const vector<Object*>& objects) {
    // system("cls");
    Button *button = new Button(10, 20, 1900, 100, 1, 60, font, generateDescription() + "   What do you want to do?", Color(100, 100, 150, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));

    cout << "What do you want to do?\n";

    Menu *menu = new Menu(20, 150, 1880, 100, 0, 60, true, font, _window, Color(20, 20, 20, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
    
    cout << "A. MOVE\nB. STAT\nC. SAVE\nD. ITEM\n";
    menu->push_back("MOVE");
    menu->push_back("STAT");
    menu->push_back("SAVE");
    menu->push_back("ITEM");

    char option = 'D';
    vector<Object*> options;

    Texture texture;
    if (!texture.loadFromFile("../Images/monster.png")) cout << "Oops!";

    if (checkMonsterRoom(objects))
    {
        for (auto obj : objects) {
            if (obj->getTag() == "Monster") {
                if(obj->getName() == "Danny")
                {
                    encounterDanny();
                }
                cout << (++option) << ". Fight the monster: " << obj->getName() << "\n";
                menu->push_back("Fight the monster: " + obj->getName(), &texture);
                options.emplace_back(obj);
            }
        }

    }
    else
    {
        for (auto obj : objects) {
            if (obj->getTag() == "NPC") {
                cout << (++option) << ". Talk to Shopkeeper: " << obj->getName() << "\n";
                menu->push_back("Talk to Shopkeeper: " + obj->getName());
                options.emplace_back(obj);
            }
            else if (obj->getTag() == "Item") {
                Item *item = dynamic_cast<Item*>(obj);
                cout << (++option);
                if (item->getKind() == "Chest") {
                    cout << ". Open the chest\n";
                    menu->push_back("Open the chest");
                }
                else {
                    cout << ". Pick up Item: " << obj->getName() << "\n";
                    menu->push_back("Pick up Item: " + obj->getName());
                }
                options.emplace_back(obj);
            }
        }
    }

    // string ops; getline(cin, ops);
    // while (ops[0] < 'A' || OPS >= options.size() + 4) {
    //     cout << "Error, please enter operation again.\n";
    //     getline(cin, ops);
    // }

    // system("cls");

    holdEnter = 1;
    gainedFocus = 1;
    while (_window->isOpen()) {
        while (_window->pollEvent(sfEvent)) {
            if ((sfEvent.type == Event::KeyPressed) && (sfEvent.key.code == Keyboard::Escape)) {
                _window->close();
            }
            switch (sfEvent.type) {
                case Event::Closed:
                    _window->close();
                    break;
                case Event::GainedFocus:
                    gainedFocus = 1;
                    break;
                case Event::LostFocus:
                    gainedFocus = 0;
                    break;
                default:
                    break;
            }
        }
        if (!gainedFocus) {
            sleep(milliseconds(10));
            continue;
        }
        if (!holdEnter && Keyboard::isKeyPressed(Keyboard::Enter)) {
            ops = menu->getIsSelected();
            break;
        }
        if (holdEnter && !Keyboard::isKeyPressed(Keyboard::Enter)) {
            holdEnter = 0;
        }
        menu->update();

        _window->clear();
		
        button->render(_window);
        menu->render(_window);

        _window->display();
    }
    delete menu;
    delete button;

    if (ops == 0) {
        cout << "debug" << endl;
        handleMovement();
        return;
    }
    else if (ops == 1) {
        player.triggerEvent(&player, _window);
    }
    else if (ops == 2) {
        Record agent(6, _window);
        agent.saveToFile(&player, rooms, vis, usedIndex, coordToIndex);
    }
    else if (ops == 3) {
        handleInventory();
    }
    else {
        handleEvent(options[ops - 4]);
    }
    cout << endl;
    // system("pause");
}

/* Check whether the game should end or not */
/* Including player victory, or he/she dead */
bool Dungeon::checkGameLogic() {
    // system("cls");
    string gameIsEnd = "";
    if (player.checkIsDead()) {
        gameIsEnd = "Game Over!";
    }
    if (player.getCurrentRoom()->getIsExit() && !checkMonsterRoom(player.getCurrentRoom()->getObjects())) {
        gameIsEnd = "Victory!";
    }
    if (gameIsEnd == "") return false;

    Button *button = new Button(160, 100, 1600, 200, 1, 60, font, gameIsEnd, Color(0, 0, 0, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));

    gainedFocus = 1;
	holdEnter = 1;
    while (_window->isOpen()) {
        while (_window->pollEvent(sfEvent)) {
            if ((sfEvent.type == Event::KeyPressed) && (sfEvent.key.code == Keyboard::Escape)) {
                _window->close();
            }
            switch (sfEvent.type) {
                case Event::Closed:
                    _window->close();
                    break;
                case Event::GainedFocus:
                    gainedFocus = 1;
                    break;
                case Event::LostFocus:
                    gainedFocus = 0;
                    break;
                default:
                    break;
            }
        }
        if (!gainedFocus) {
            sleep(milliseconds(10));
            continue;
        }
        if (!holdEnter && Keyboard::isKeyPressed(Keyboard::Enter)) {
            break;
        }
        if (holdEnter && !Keyboard::isKeyPressed(Keyboard::Enter)) {
            holdEnter = 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::K)) {
            cout << Mouse::getPosition().x << ' ' << Mouse::getPosition().y << endl;
        }
        button->update(0);

        _window->clear();

        button->render(_window);

        _window->display();
    }
    return true;
}

/* Deal with the whole game process */
void Dungeon::runDungeon(int minimum_frame_per_seconds) {
    bool isStarted = 0;

    while (_window->isOpen()) {
        if (!isStarted) {
            isStarted = 1;
            startGame();
        }
        // cout << "startGame" << endl;
        if (checkGameLogic()) {
            _window->close();
            break;
        }
        createRoom(player.getCurrentRoom()->getIndex());
        // cout << "createRoom" << endl;
        chooseAction(player.getCurrentRoom()->getObjects());    
        // cout << "chooseAction" << endl;
        
        // updateDt();
        // update();
        // render();
    }
}

void Dungeon::updateDt() {
    /* updates the dt variable with the time it takes to update and render one frame. */
    this->dt = this->dtClock.restart().asSeconds();
    // system("cls");
    // cout << this->dt << endl;
}

void Dungeon::updateSFMLEvents() {
    
}

void Dungeon::update() {
    updateSFMLEvents();
}

void Dungeon::render() {

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

    long double prob = 75;
    
    // system("cls");
    string info = "";
    for (int i = 0; i < 4; ++i) {
        switch ( getRand(vector<long double>( {25, 50, 75, 100} ) ) ) {
            case 0:
                X = x, Y = y + 1;
                if (rooms[index].getUpRoom() == nullptr)
                {
                    if ( coordToIndex.find( coord(X, Y) ) != coordToIndex.end() ) {
                        if (vis[ coordToIndex[ coord(X, Y) ] ]) info += "A path to the past room appeared!!!(Up room)\n";
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
                        if (vis[ coordToIndex[ coord(X, Y) ] ]) info += "A path to the past room appeared!!!(Down room)\n";
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
                        if (vis[ coordToIndex[ coord(X, Y) ] ]) info += "A path to the past room appeared!!!(Left room)\n";
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
                        if (vis[ coordToIndex[ coord(X, Y) ] ]) info += "A path to the past room appeared!!!(Right room)\n";
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
    if (info == "") return;
    Button *button = new Button(160, 100, 1600, 200, 1, 60, font, info, Color(0, 0, 0, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));

    gainedFocus = 1;
	holdEnter = 1;
    while (_window->isOpen()) {
        while (_window->pollEvent(sfEvent)) {
            if ((sfEvent.type == Event::KeyPressed) && (sfEvent.key.code == Keyboard::Escape)) {
                _window->close();
            }
            switch (sfEvent.type) {
                case Event::Closed:
                    _window->close();
                    break;
                case Event::GainedFocus:
                    gainedFocus = 1;
                    break;
                case Event::LostFocus:
                    gainedFocus = 0;
                    break;
                default:
                    break;
            }
        }
        if (!gainedFocus) {
            sleep(milliseconds(10));
            continue;
        }
        if (!holdEnter && Keyboard::isKeyPressed(Keyboard::Enter)) {
            break;
        }
        if (holdEnter && !Keyboard::isKeyPressed(Keyboard::Enter)) {
            holdEnter = 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::K)) {
            cout << Mouse::getPosition().x << ' ' << Mouse::getPosition().y << endl;
        }
        button->update(0);

        _window->clear();

        button->render(_window);

        _window->display();
    }
}

/* Random Generator */
Monster Dungeon::generateMonster(int LV)
{
    int LevelArg = LV * (LV + 1);
    vector<Monster> monster_list 
    {
        Monster("Slime's friend", ">_<", "Monster", LV - 1, Item()),
        Monster("Orc", "", "Monster", LV - 1, Item("Orc's Chestplate", "Chestplate", 0, 0, 0, 0, 10 * LevelArg, 100 * LevelArg, 50 * LevelArg)), 
        Monster("Dwarf", "", "Monster", LV - 1, Item("Dwarf's diamond", "Treasure", 0, 0, 0, 0, 0, 0, 1000 * LevelArg)), 
        Monster("Elf", "", "Monster", LV - 1, Item("Elf's sword", "Weapon", 0, 0, 10*LevelArg, 20 * LevelArg, 0, 0, 0)), 
        Monster("Dragon", "", "Monster", LV - 1, Item("Dragon's cloak", "cloak", 0, 0, 10*LevelArg, 0, 20 * LevelArg, 0, 0)), 
        Monster("Wizard", "", "Monster", LV - 1, Item("Wizard's potion", "Potion", 10 * LevelArg, 10 * LevelArg, 10 * LevelArg, 1 * LevelArg, 1 * LevelArg, 0, 0)), 
        Monster("Troll", "", "Monster", LV - 1, Item("Troll's stick", "Weapon", 0, 0, 0, 3 * LevelArg, 1 * LevelArg, 10 * LevelArg, 0)), 
        Monster("Basilisk", "", "Monster", LV - 1, Item("Basilisk's fang", "Treasure", 0, 0, 0, 0, 0, 0, 800 * LevelArg)), 
        Monster("CaveMan", "", "Monster", LV - 1, Item("Caveman's bow", "Weapon", 0, 0, 5 * LevelArg, 0, 0, 0, 100 * LevelArg)), 
        Monster("Phoenix", "", "Monster", LV - 1, Item("Phoenix's Tear", "Treasure", 100 * LevelArg , 100 * LevelArg , 100 * LevelArg, 0, 0, 0, 0)), 
        Monster("Cerberus", "", "Monster", LV - 1, Item("Cerberus's fur", "Leggings", 0, 0, 0, 0, 20 * LevelArg, 100 * LevelArg, 100 * LevelArg)), 
        Monster("Danny", "", "Monster", LV - 1, Item("Danny's cheese", "Food", 1000 * LevelArg, 0, 0, 0, 0, 0, 109 * LevelArg))
    };
    vector<long double> prob;
    for (int i = 0; i < monster_list.size(); ++i) prob.emplace_back(i + 1);
    Monster monster = monster_list[ getRand( prob ) ];

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
        goods.emplace_back(Item::randomItemGenerator(LV, "RANDOM"));
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

    long double condition = player.getLV() >= 5 ? 11 : 11;
    int id = getRand(vector<long double>( {6, 9, 11, condition} ));
    if (kind == "RANDOM") kind = kindList[ id ];

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
string Dungeon::generateDescription() {
    stringstream ss; ss.clear();

    cout << "It's beautiful outside.\n";
    ss << "It's beautiful outside.   ";

    cout << "You are at (" << player.getCurrentRoom()->getX() << ", " << player.getCurrentRoom()->getY() << ").\n";
    ss << "You are at (" << player.getCurrentRoom()->getX() << ", " << player.getCurrentRoom()->getY() << ").\n";
    
    string description = "", tmp;

    getline(ss, tmp);
    description += tmp;

    getline(ss, tmp);
    description += tmp;

    return description;
} 