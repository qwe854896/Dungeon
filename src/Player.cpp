#include "Player.h"
Player::Player() : GameCharacter()
{
    armors.resize(5);
    gold = 0;
}
Player::Player(string name, string image, string type, int LV)
    : GameCharacter(name, "Player", image, type, LV)
{
    armors.resize(5);
    gold = 100;
}
Player::Player(string name, string image, string type, int LV, int EXP, int HP, int MP, int FP, int attack, int defense)
    : GameCharacter(name, "Player", image, type, LV, EXP, HP, MP, FP, attack, defense)
{
    armors.resize(5);
    gold = 0;
}

namespace
{
    stringstream ss;
    void showBar(int current, int max)
    {
        const int nstars = 50;
        const int per = current * nstars / max;
        cout << "| " << string(per, '=') << string(nstars - per, ' ') << " | ";
    }
}

/* Virtual function that you need to complete   */
/* In Player, this function should show the     */
/* status of player.                            */
bool Player::triggerEvent(Object *object, RenderWindow *window)
{
    bool flag = false;
    vector<Button *> buttons;
    string info, tmp;
    Font font;
    font.loadFromFile("assets/font/Dosis-Light.ttf");

    /* player */
    if (object->getTag() == "Player")
    {
        ss.clear();
        ss << "Status:\n";
        ss << getName() << "   Lv. " << setw(2) << getLV() << endl;
        ss << left << setw(12) << "> EXP: " << getCurrentEXP() << '/' << getMaxEXP() << endl;
        ss << left << setw(12) << "> Gold: " << getGold() << endl;
        ss << left << setw(12) << "> HP: " << getCurrentHP() << '/' << getMaxHP() << endl;
        ss << left << setw(12) << "> MP: " << getCurrentMP() << '/' << getMaxMP() << endl;
        ss << left << setw(12) << "> FP: " << getCurrentFP() << '/' << getMaxFP() << endl;
        ss << left << setw(12) << "> Attack: " << getAttack() << endl;
        ss << left << setw(12) << "> Defense: " << getDefense() << endl;

        info = "";
        while (getline(ss, tmp))
            info += tmp + "\n";
        info.pop_back();
        font.loadFromFile("assets/font/coolvetica condensed rg.ttf");

        Button *playerStatus = new Button(
            250, 150, 500, 800, 1, 60, &font,
            info,
            Color(0, 0, 0, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
        buttons.emplace_back(playerStatus);

        ss.clear();
        ss << "Armors:\n";
        for (int i = 0; i < 5; ++i)
        {
            ss << "> " << left << setw(12) << getArmor(i) + ": ";
            ss << armors[i].getName() << endl;
        }

        info = "";
        while (getline(ss, tmp))
            info += tmp + "\n";
        info.pop_back();

        playerStatus = new Button(
            1070, 150, 500, 800, 1, 60, &font,
            info,
            Color(0, 0, 0, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
        buttons.emplace_back(playerStatus);
    }

    /* Item */
    if (object->getTag() == "Item")
    {
        Item *item = dynamic_cast<Item *>(object); // Helmet, Chestplate, Leggings, Boots, Weapon, Props

        if (item->getKind() == "Potion" || item->getKind() == "Food")
        {
            Button *status = new Button(
                520, 0, 880, 880, 1, 60, &font,
                "You use " + item->getName(),
                Color(0, 0, 0, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
            buttons.emplace_back(status);

            item->decreaseDurability(1);
            increaseStates(item->getHP(), item->getMP(), item->getFP(), item->getAttack(), item->getDefense());

            info = item->genInfo();

            Button *itemInfo = new Button(
                520, 250, 880, 880, 1, 60, &font,
                info,
                Color(0, 0, 0, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
            buttons.emplace_back(itemInfo);

            if (item->getDurability() <= 0)
            {
                flag = true;
            }
            else
                flag = false;
        }

        int armorIndex = getArmorIndex(item->getKind());
        if (armorIndex != -1)
        {
            ss.clear();
            if (armors[armorIndex].getKind() != "NULL")
            {
                ss << "You take off " << armors[armorIndex].getName() << endl;
                addItem(armors[armorIndex]);
                decreaseStates(armors[armorIndex].getHP(), armors[armorIndex].getMP(), armors[armorIndex].getFP(), armors[armorIndex].getAttack(), armors[armorIndex].getDefense());
            }

            armors[armorIndex] = *item;

            Button *status = new Button(
                520, 0, 880, 880, 1, 60, &font,
                "You equip " + armors[armorIndex].getName(),
                Color(0, 0, 0, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
            buttons.emplace_back(status);

            increaseStates(armors[armorIndex].getHP(), armors[armorIndex].getMP(), armors[armorIndex].getFP(), armors[armorIndex].getAttack(), armors[armorIndex].getDefense());
            flag = true;

            info = item->genInfo();

            Button *itemInfo = new Button(
                520, 250, 880, 880, 1, 60, &font,
                info,
                Color(0, 0, 0, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
            buttons.emplace_back(itemInfo);
        }
    }

    if (buttons.empty())
        return flag;

    bool gainedFocus = 1;
    bool holdEnter = 1;
    Event sfEvent;
    while (window->isOpen())
    {
        while (window->pollEvent(sfEvent))
        {
            if ((sfEvent.type == Event::KeyPressed) && (sfEvent.key.code == Keyboard::Escape))
            {
                window->close();
            }
            switch (sfEvent.type)
            {
            case Event::Closed:
                window->close();
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
        if (!gainedFocus)
        {
            sleep(milliseconds(10));
            continue;
        }
        if (!holdEnter && Keyboard::isKeyPressed(Keyboard::Enter))
        {
            break;
        }
        if (holdEnter && !Keyboard::isKeyPressed(Keyboard::Enter))
        {
            holdEnter = 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::K))
        {
            cout << Mouse::getPosition().x << ' ' << Mouse::getPosition().y << endl;
        }

        window->clear();

        for (auto &button : buttons)
            button->render(window);

        window->display();
    }
    for (auto &button : buttons)
        delete button;

    return flag;
}

void Player::input(ifstream &in)
{
    setTag("Player");
    inputGameCharacter(in);

    int n;
    in >> n;
    inventory.resize(n);
    for (int i = 0; i < n; ++i)
    {
        string tag = "";
        while (tag == "")
            getline(in, tag);
        in >> inventory[i];
    }

    in >> n;
    armors.resize(n);
    for (int i = 0; i < n; ++i)
    {
        string tag = "";
        while (tag == "")
            getline(in, tag);
        in >> armors[i];
    }

    in >> gold;

    // CRindex and PRindex were input in "Record.cpp"
}

void Player::output(ofstream &out) const
{
    outputGameCharacter(out);

    out << inventory.size() << endl;
    for (auto item : inventory)
    {
        out << item << endl;
    }

    out << armors.size() << endl;
    for (auto armor : armors)
    {
        out << armor << endl;
    }

    out << gold << endl;

    out << currentRoom->getIndex() << ' ' << previousRoom->getIndex() << endl;
}

/* Set & Get function*/
void Player::setCurrentRoom(Room *currentRoom)
{
    this->currentRoom = currentRoom;
}
void Player::setPreviousRoom(Room *previousRoom)
{
    this->previousRoom = previousRoom;
}
void Player::setInventory(vector<Item> inventory)
{
    this->inventory = inventory;
}
void Player::setGold(int gold)
{
    this->gold = gold;
}
Room *Player::getCurrentRoom() const
{
    return currentRoom;
}
Room *Player::getPreviousRoom() const
{
    return previousRoom;
}
vector<Item> Player::getInventory() const
{
    return inventory;
}
int Player::getGold() const
{
    return gold;
}

/* Supplement */

void Player::addItem(Item item)
{
    inventory.emplace_back(item);
}

void Player::popItem(int index) /*pop out the specific object, used when the interaction is done*/
{
    inventory.erase(inventory.begin() + index);
}

void Player::useItem(int index, RenderWindow *window)
{
    if (triggerEvent(&inventory[index], window))
    {
        popItem(index);
    }
}

void Player::sellItem(int index)
{
    gold += inventory[index].getPrice();
    popItem(index);
}

void Player::changeRoom(Room *room)
{
    previousRoom = currentRoom;
    currentRoom = room;
}

void Player::increaseGold(int gold)
{
    this->gold += gold;
}

void Player::decreaseGold(int gold)
{
    this->gold -= gold;
}

void Player::increaseArmorDurability(int delta)
{
    for (int i = 0; i < 5; ++i)
    {
        if (armors[i].getKind() != "NULL")
        {
            armors[i].increaseDurability(delta);
        }
    }
}

string Player::decreaseArmorDurability(int delta)
{
    ss.clear();
    for (int i = 0; i < 4; ++i)
    { // doesn't contain "Weapon"
        if (armors[i].getKind() != "NULL")
        {
            if (armors[i].decreaseDurability(delta))
            {
                ss << "Your " << armors[i].getName() << " is broken.\n"
                   << endl;
                decreaseStates(armors[i].getHP(), armors[i].getMP(), armors[i].getFP(), armors[i].getAttack(), armors[i].getDefense());
                armors[i] = Item();
            }
        }
    }

    string info = "", tmp;
    while (getline(ss, tmp))
        info += tmp + "   ";
    if (!info.empty())
        info.pop_back();
    return info;
}

string Player::decreaseWeaponDurability(int delta)
{
    ss.clear();
    if (armors[4].getKind() != "NULL")
    {
        if (armors[4].decreaseDurability(delta))
        {
            ss << "Your " << armors[4].getName() << " is broken.\n"
               << endl;
            decreaseStates(armors[4].getHP(), armors[4].getMP(), armors[4].getFP(), armors[4].getAttack(), armors[4].getDefense());

            armors[4] = Item();
        }
    }

    string info = "", tmp;
    while (getline(ss, tmp))
        info += tmp + "   ";
    if (!info.empty())
        info.pop_back();
    return info;
}

int Player::listInventory(string operation, RenderWindow *window)
{
    if (operation == "sell" && inventory.empty())
        return -1;

    int ops;
    string info, tmp;
    Font font;
    font.loadFromFile("assets/font/Dosis-Light.ttf");

    bool isEmpty = inventory.empty();

    Button *button;
    if (isEmpty)
        button = new Button(
            520, 370, 880, 440, 1, 60, &font,
            "It's empty inside.",
            Color(0, 0, 0, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
    else
        button = new Button(
            10, 20, 1900, 100, 1, 60, &font,
            "Choose one item you want to " + operation + ": ",
            Color(0, 0, 0, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));

    Menu *menu = new Menu(
        60, 200, 450, 500,
        0, 60, false, &font, window,
        Color(20, 20, 20, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));

    for (auto &item : inventory)
    {
        stringstream().swap(ss);
        ss << item.getName() << endl;
        ss << "> type: " << item.getKind() << endl;
        if (item.getHP())
            ss << "> HP: " << item.getHP() << endl;
        if (item.getMP())
            ss << "> MP: " << item.getMP() << endl;
        if (item.getFP())
            ss << "> FP: " << item.getFP() << endl;
        if (item.getAttack())
            ss << "> Attack: " << item.getAttack() << endl;
        if (item.getDefense())
            ss << "> Defense: " << item.getDefense() << endl;
        if (item.getDurability())
            ss << "> Durability: " << item.getDurability() << endl;
        if (item.getPrice())
            ss << "> Price: " << item.getPrice() << endl;

        info = "";
        while (getline(ss, tmp))
            info += tmp + "\n";
        info.pop_back();

        menu->push_back(info);
    }
    menu->push_back("Cancel");

    bool gainedFocus = 1;
    bool holdEnter = 1;

    Event sfEvent;
    while (window->isOpen())
    {
        while (window->pollEvent(sfEvent))
        {
            if ((sfEvent.type == Event::KeyPressed) && (sfEvent.key.code == Keyboard::Escape))
            {
                window->close();
            }
            switch (sfEvent.type)
            {
            case Event::Closed:
                window->close();
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
        if (!gainedFocus)
        {
            sleep(milliseconds(10));
            continue;
        }
        if (!holdEnter && Keyboard::isKeyPressed(Keyboard::Enter))
        {
            if (!isEmpty)
                ops = menu->getIsSelected();
            break;
            holdEnter = 1;
        }
        if (holdEnter && !Keyboard::isKeyPressed(Keyboard::Enter))
        {
            holdEnter = 0;
        }
        if (!isEmpty)
            menu->update();

        window->clear();

        button->render(window);
        if (!isEmpty)
            menu->render(window);

        window->display();
    }
    delete menu;
    delete button;

    if (isEmpty)
        return -1;
    return ops;
}

string Player::showStatusinFight()
{
    ss.clear();
    ss << "Status: \n";
    ss << left << setw(12) << "> HP " << getCurrentHP() << '/' << getMaxHP() << endl;
    ss << left << setw(12) << "> MP " << getCurrentMP() << '/' << getMaxMP() << endl;
    ss << left << setw(12) << "> FP " << getCurrentFP() << '/' << getMaxFP() << endl;
    ss << left << setw(12) << "> Attack: " << getAttack() << endl;
    ss << left << setw(12) << "> Defense: " << getDefense() << endl;

    string info = "", tmp;
    while (getline(ss, tmp))
        info += tmp + "\n";
    info.pop_back();

    return info;
}

/* operation: use, sell */
bool Player::handleInventory(string operation, RenderWindow *window)
{
    int ops = listInventory(operation, window);
    if (ops != -1)
    {
        if (ops < inventory.size())
        {
            if (operation == "use")
                useItem(ops, window);
            else if (operation == "sell")
                sellItem(ops);
            return true;
        }
    }
    return false;
}

bool Player::handleAct(RenderWindow *window)
{
    return handleInventory("use", window);
}

Item Player::handleAttack(RenderWindow *window)
{
    int option = 2;
    int ops;

    Font font;
    font.loadFromFile("assets/font/Dosis-Light.ttf");
    Button *button = new Button(
        10, 20, 1900, 100, 1, 60, &font,
        "Choose the method to attack:",
        Color(0, 0, 0, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
    Menu *menu = new Menu(
        20, 150, 1880, 100,
        0, 60, true, &font, window,
        Color(20, 20, 20, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
    menu->push_back("Default Attack");
    menu->push_back("Skills");
    if (ultimateSkillAvailable())
        ++option, menu->push_back("Ultimate Skills");
    menu->push_back("Cancel");

    bool gainedFocus = 1;
    bool holdEnter = 1;
    Event sfEvent;
    while (window->isOpen())
    {
        while (window->pollEvent(sfEvent))
        {
            if ((sfEvent.type == Event::KeyPressed) && (sfEvent.key.code == Keyboard::Escape))
            {
                window->close();
            }
            switch (sfEvent.type)
            {
            case Event::Closed:
                window->close();
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
        if (!gainedFocus)
        {
            sleep(milliseconds(10));
            continue;
        }
        if (!holdEnter && Keyboard::isKeyPressed(Keyboard::Enter))
        {
            ops = menu->getIsSelected();
            break;
            holdEnter = 1;
        }
        if (holdEnter && !Keyboard::isKeyPressed(Keyboard::Enter))
        {
            holdEnter = 0;
        }

        menu->update();

        window->clear();

        button->render(window);
        menu->render(window);

        window->display();
    }
    delete menu;
    delete button;

    if (ops < option)
    {
        if (ops == 0)
            return Item("Skill", getAttack(), 0, 0, 0, 0);
        else
            return handleSkills(ops - 1, window);
    }

    return Item("NULL", 0, 0, 0, 0, 0);
}

Item Player::handleSkills(int ultimate, RenderWindow *window)
{
    return Item("NULL", 0, 0, 0, 0, 0);
}

int Player::getArmorIndex(string kind)
{
    if (kind == "Helmet")
        return 0;
    if (kind == "Chestplate")
        return 1;
    if (kind == "Leggings")
        return 2;
    if (kind == "Boots")
        return 3;
    if (kind == "Weapon")
        return 4;
    return -1;
}

string Player::getArmor(int kind)
{
    if (kind == 0)
        return "Helmet";
    if (kind == 1)
        return "Chestplate";
    if (kind == 2)
        return "Leggings";
    if (kind == 3)
        return "Boots";
    if (kind == 4)
        return "Weapon";
    return "";
}

ifstream &operator>>(ifstream &in, Player &player)
{
    player.input(in);
    return in;
}
ofstream &operator<<(ofstream &out, const Player &player)
{
    player.output(out);
    return out;
}
