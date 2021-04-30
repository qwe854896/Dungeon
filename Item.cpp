#include "Item.h"
Item::Item()
: Object("empty", "Item", "")
{
    kind = "NULL";
    HP = MP = FP = attack = defense = durability = price = 0;
}
Item::Item(string kind, int HP, int MP, int FP, int attack, int defense)
: Object("", "Item", ""), kind(kind), HP(HP), MP(MP), FP(FP), attack(attack), defense(defense), durability(0), price(0)
{}
Item::Item(string name, string kind, int HP, int MP, int FP, int attack, int defense, int durability, int price)
: Object(name, "Item", ""), kind(kind), HP(HP), MP(MP), FP(FP), attack(attack), defense(defense), durability(durability), price(price)
{}
Item::Item(string name, string kind, string image, int HP, int MP, int FP, int attack, int defense, int durability, int price)
: Object(name, "Item", image), kind(kind), HP(HP), MP(MP), FP(FP), attack(attack), defense(defense), durability(durability), price(price)
{}
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

/* Virtual function that you need to complete    */
/* In Item, this function should deal with the   */
/* pick up action. You should add status to the  */
/* player.                                       */
bool Item::triggerEvent(Object* object) {
    if (object->getTag() == "Player") {
        Player *player = dynamic_cast<Player*>(object);
        if (kind == "Chest")
        {
            cout << "You open the chest\n";
            Item treasure = randomItemGenerator(player->getLV(), "RANDOM");
            cout << "You obtain " << treasure.getName() << endl;
            player->addItem(treasure);

            return true;
        }

        cout << "You pick up " << getName() << endl;
        player->addItem(*this);

        return true;
    }

	return false;
}

void Item::input(ifstream& in)
{
    setTag("Item");
    inputObject(in);

    kind = ""; while (kind == "") getline(in, kind);

    in >> HP >> MP >> FP >> attack >> defense >> durability >> price;
}

void Item::output(ofstream& out) const
{
    outputObject(out);

    out << kind << endl;

    out << HP << ' ' << MP << ' ' << FP << ' ' << attack << ' ' << defense << ' ' << durability << ' ' << price << endl;
}

/* Set & Get function*/
int Item::getHP() const {
	return HP;
}
int Item::getMP() const {
	return MP;
}
int Item::getFP() const {
	return FP;
}
int Item::getAttack() const {
	return attack;
}
int Item::getDefense() const {
	return defense;
}
int Item::getDurability() const {
    return durability;
}
int Item::getPrice() const {
    return price;
}
string Item::getKind() const {
    return kind;
}
void Item::setHP(int HP) {
	this->HP = HP;
}
void Item::setMP(int MP) {
	this->MP = MP;
}
void Item::setFP(int FP) {
	this->FP = FP;
}
void Item::setAttack(int attack) {
	this->attack = attack;
}
void Item::setDefense(int defense) {
	this->defense = defense;
}
void Item::setDurability(int durability) {
    this->durability = durability;
}
void Item::setPrice(int price) {
    this->price = price;
}
void Item::setKind(string kind) {
    this->kind = kind;
}

/* Supplement */

void Item::increaseDurability(int delta)
{
    durability += delta;
}

bool Item::decreaseDurability(int delta)
{
    durability -= delta;
    return durability <= 0;
}

ostream& operator<<(ostream& out, const Item& item) {
    out << item.getName() << endl;
    out << "> type: " << item.getKind() << endl;
    if (item.getHP())         out << "> HP: " << item.getHP() << endl;
    if (item.getMP())         out << "> MP: " << item.getMP() << endl;
    if (item.getFP())         out << "> FP: " << item.getFP() << endl;
    if (item.getAttack())     out << "> Attack: " << item.getAttack() << endl;
    if (item.getDefense())    out << "> Defense: " << item.getDefense() << endl;
    if (item.getDurability()) out << "> Durability: " << item.getDurability() << endl;
    if (item.getPrice())      out << "> Price: " << item.getPrice() << endl;
    return out;
}

ifstream& operator>>(ifstream& in, Item& item)
{
    item.input(in);
    return in;
}
ofstream& operator<<(ofstream& out, const Item& item)
{
    item.output(out);
    return out;
}

Item Item::randomItemGenerator(int LV, string kind = "RANDOM")
{
    int LevelArg= LV * (LV + 1);
    vector<Item> Booty //This is for items that monster drops.
    {
        Item("Orc's armor", "Armor", 0, 0, 0, 0, 10 * LevelArg, 100 * LevelArg, 50 * LevelArg), 
        Item("Dwarf's diamond", "Treasure", 0, 0, 0, 0, 0, 0, 0, 1000 * LevelArg),
        Item("Dragon's cloak", "Armor", 0, 0, 10*LevelArg, 0, 20 * LevelArg, 0, 0, 0),
        Item("Elf's sword", "Weapon", 0, 0, 10*LevelArg, 20 * LevelArg, 0, 0, 0, 0),
        Item("Wizard's potion", "Potion", 10 * LevelArg, 10 * LevelArg, 10 * LevelArg, 1 * LevelArg, 1 * LevelArg, 0, 0),
        Item("Troll's stick", "Weapon", 0, 0, 0, 3 * LevelArg, 1 * LevelArg, 10 * LevelArg, 0),
        Item("Basilisk's fang", "Treasure", 0, 0, 0, 0, 0, 0, 0, 800 * LevelArg),
        Item("Caveman's bow", "Weapon", 0, 0, 0, 5 * LevelArg, 0, 0, 0, 100 * LevelArg),
        Item("Phoenix's Tear", "Treasure", 100 * LevelArg , 100 * LevelArg , 100 * LevelArg, 0, 0, 0, 0),
        Item("Cerberus's fur", "Armor", 0, 0, 0, 0, 20 * LevelArg, 100 * LevelArg, 100 * LevelArg),
        Item("Danny's cheese", "Food", 1000 * LevelArg, 0, 0, 0, 0, 0, 109 * LevelArg)
    };

    vector<Item> ItemsforSale //This is for npc to sell them.
    {
        Item("Sword", "Weapon", 0, 0, 0, 5 * LevelArg, 0, 10 * LevelArg, 10 * LevelArg),
        Item("Shield", "Weapon", 0, 0, 0, 0, 5 * LevelArg, 10 * LevelArg, 10 * LevelArg),
        Item("Bow and Arrow", "Weapon", 0, 0, 0, 5 * LevelArg, 2 * LevelArg, 10 * LevelArg, 15 * LevelArg),
        Item("Wand", "Weapon", 0, 0, 3 * LevelArg, 7 * LevelArg, 3 * LevelArg, 15 * LevelArg, 25 * LevelArg),
        Item("HP potion", "Potion", 10 * LevelArg, 0, 0, 0, 0, 0, 5 * LevelArg),
        Item("MP potion", "Potion", 0, 10 * LevelArg, 0, 0, 0, 0, 5 * LevelArg),
        Item("FP potion", "Potion", 0, 0, 10 * LevelArg, 0, 0, 0, 5 * LevelArg),
        Item("Attack potion", "Potion", 0, 0, 0, 3 * LevelArg, 0, 0, 10 * LevelArg),
        Item("Defense potion", "Potion", 0, 0, 0, 0, 3 * LevelArg, 0, 10 * LevelArg),
        Item("Helmet", "Armor", 0, 0, 0, 0, 3 * LevelArg, 10 * LevelArg, 12 * LevelArg),
        Item("Chest Armor", "Armor", 0, 0, 0, 0, 5 * LevelArg, 10 * LevelArg, 20 * LevelArg),
        Item("Armor Pants", "Armor", 0, 0, 0, 0, 4 * LevelArg, 10 * LevelArg, 16 * LevelArg),
        Item("Boots", "Armor", 0, 0, 0, 0, 2 * LevelArg, 10 * LevelArg, 8 * LevelArg),
        Item("Bread", "Food", 3 * LevelArg, 3 * LevelArg, 1 * LevelArg, 0, 0, 0, 2 * LevelArg)
    };

    Item item = Item();
    string kindList[2] = {
        "Props",
        "Weapon"
    };
    if (kind == "RANDOM")
    {
        kind = kindList[ getRand(vector<long double>({50, 100}) ) ];
    }
    
    if (kind == "Weapon")
    {
        //have to be done
    }

    if (kind == "Armor")
    {
        //have to be done
    }

    if (kind == "Food")
    {
        //have to be done
    }

    if (kind == "Treasure")
    {
        //have to be done
    }

    return item;
}
