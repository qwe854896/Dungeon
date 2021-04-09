#include "Player.h"
Player::Player() : GameCharacter() {
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

void Player::addItem(Item item){
    inventory.emplace_back(item);
}

void Player::popItem(int index) /*pop out the specific object, used when the interaction is done*/
{
    swap(inventory.back(), inventory[index]);
    inventory.pop_back();
}

void Player::useItem(int index)
{
    if (triggerEvent( &inventory[index] ))
    {
        popItem(index);
    }
}

void Player::changeRoom(Room* room) {
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

/* Virtual function that you need to complete   */
/* In Player, this function should show the     */
/* status of player.                            */
bool Player::triggerEvent(Object* object)
{
    /* player */
    if (object->getTag() == "Player")
    {
        cout << "Status:\n";
        cout << getName() << "   Lv. " << setw(2) << getLV() << endl;
        cout << left << setw(12) << "> EXP: " << getCurrentEXP() << '/' << getMaxEXP() << endl;
        cout << left << setw(12) << "> Gold: " << getGold() << endl;
        cout << left << setw(12) << "> HP: " << getCurrentHP() << '/' << getMaxHP() << endl;
        cout << left << setw(12) << "> MP: " << getCurrentMP() << '/' << getMaxMP() << endl;
        cout << left << setw(12) << "> FP: " << getCurrentFP() << '/' << getMaxFP() << endl;
        cout << left << setw(12) << "> Attack: " << getAttack() << endl;
        cout << left << setw(12) << "> Defense: " << getDefense() << endl;

        cout << "\nArmors:\n";
        for (int i = 0; i < 5; ++i) {
            cout << "> " << left << setw(12) << getArmor(i) + ": ";
            cout << armors[i].getName() << endl;
        }
    }

    /* Item */
    if (object->getTag() == "Item")
    {
        Item *item = dynamic_cast<Item*>(object); // Helmet, Chestplate, Leggings, Boots, Weapon, Props.

        if (item->getKind() == "Props") {
            cout << "You use " << item->getName() << endl;

            item->setDurability(item->getDurability() - 1);
            increaseStates(item->getHP(), item->getMP(), item->getFP(), item->getAttack(), item->getDefense());

            if (item->getDurability() <= 0) return true;
            return false;
        }

        int armorIndex = getArmorIndex(item->getKind());

        if (armorIndex != -1)
        {
            if (armors[armorIndex].getKind() != "NULL") {
                cout << "You take off " << armors[armorIndex].getName() << endl;
                addItem(armors[armorIndex]);
                decreaseStates(armors[armorIndex].getHP(), armors[armorIndex].getMP(), armors[armorIndex].getFP(), armors[armorIndex].getAttack(), armors[armorIndex].getDefense());
            }

            armors[armorIndex] = *item;

            cout << "You equip " << armors[armorIndex].getName() << endl;
            increaseStates(armors[armorIndex].getHP(), armors[armorIndex].getMP(), armors[armorIndex].getFP(), armors[armorIndex].getAttack(), armors[armorIndex].getDefense());
            return true;
        }


    }

    return false;
}

void Player::input(ifstream& in)
{
    setTag("Player");
    inputGameCharacter(in);

    int n;
    in >> n;
    inventory.resize(n);
    for (int i = 0; i < n; ++i) {
        string tag = ""; while (tag == "") getline(in, tag);
        in >> inventory[i];
    }

    in >> n;
    armors.resize(n);
    for (int i = 0; i < n; ++i) {
        string tag = ""; while (tag == "") getline(in, tag);
        in >> armors[i];
    }

    in >> gold;

    // CRindex and PRindex were input in "Record.cpp"
}

void Player::output(ofstream& out) const
{
    outputGameCharacter(out);

    out << inventory.size() << endl;
    for (auto item : inventory) {
        out << item << endl;
    }

    out << armors.size() << endl;
    for (auto armor : armors) {
        out << armor << endl;
    }

    out << gold << endl;

    out << currentRoom->getIndex() << ' ' << previousRoom->getIndex() << endl;
}

/* Set & Get function*/
void Player::setCurrentRoom(Room* currentRoom) {
	this->currentRoom = currentRoom;
}
void Player::setPreviousRoom(Room* previousRoom) {
	this->previousRoom = previousRoom;
}
void Player::setInventory(vector<Item> inventory) {
	this->inventory = inventory;
}
void Player::setGold(int gold) {
	this->gold = gold;
}
Room* Player::getCurrentRoom() const {
    return currentRoom;
}
Room* Player::getPreviousRoom() const {
    return previousRoom;
}
vector<Item> Player::getInventory() const {
    return inventory;
}
int Player::getGold() const {
    return gold;
}

/* Supplement */
bool Player::listInventory()
{
    if (inventory.empty()) {
        cout << "It's empty inside.\n";
        return false;
    }
    cout << "Choose one item you want to use: \n";

    char option = 'A' - 1;
    for (auto item : inventory) {
        cout << (++option) << ". " << item << endl;
    }
    cout << (++option) << ". Cancel.\n";

    return true;
}

int Player::getArmorIndex(string kind)
{
    if (kind == "Helmet") return 0;
    if (kind == "Chestplate") return 1;
    if (kind == "Leggings") return 2;
    if (kind == "Boots") return 3;
    if (kind == "Weapon") return 4;
    return -1;
}

string Player::getArmor(int kind)
{
    if (kind == 0) return "Helmet";
    if (kind == 1) return "Chestplate";
    if (kind == 2) return "Leggings";
    if (kind == 3) return "Boots";
    if (kind == 4) return "Weapon";
    return "";
}

ifstream& operator>>(ifstream& in, Player& player)
{
    player.input(in);
    return in;
}
ofstream& operator<<(ofstream& out, const Player& player)
{
    player.output(out);
    return out;
}
