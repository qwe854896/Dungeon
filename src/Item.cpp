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
    stringstream ss;
    mt19937 engine( rand() );
    int getRand(vector<long double> p)
    {
        uniform_real_distribution<long double> distribution(0, p.back());
        long double value = distribution(engine);
        int id = (int)(upper_bound(p.begin(), p.end(), value) - p.begin());
        assert(id != p.size());
        return id;
    }
}

/* Virtual function that you need to complete    */
/* In Item, this function should deal with the   */
/* pick up action. You should add status to the  */
/* player.                                       */
bool Item::triggerEvent(Object* object, RenderWindow* window) {
    ss.clear();
    bool flag;
    string info = "", tmp;
    if (object->getTag() == "Player") {
        Player *player = dynamic_cast<Player*>(object);
        if (kind == "Chest")
        {
            ss << "You open the chest\n";
            Item treasure = randomItemGenerator(player->getLV(), "RANDOM");
            ss << "You obtain " << treasure.getName() << endl;
            player->addItem(treasure);
        }
        else {
            ss << "You pick up " << getName() << endl;
            player->addItem(*this);
        }
        flag = true;
    }

    while (getline(ss, tmp)) info += tmp + "\n";
    if (info != "") info.pop_back();

    Font *font = new Font(); font->loadFromFile("../Fonts/Dosis-Light.ttf");
    Button* button = new Button(350, 280, 1220, 520, 1, 60, font, info, Color(20, 20, 20, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));

    bool gainedFocus = 1;
	bool holdEnter = 1;
    Event sfEvent;
    while (window->isOpen()) {
        while (window->pollEvent(sfEvent)) {
            if ((sfEvent.type == Event::KeyPressed) && (sfEvent.key.code == Keyboard::Escape)) {
                window->close();
            }
            switch (sfEvent.type) {
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

        window->clear();
		
        button->render(window);

        window->display();
    }
    delete button;

	return flag;
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

string Item::genInfo() {
    ss.clear();
    if (HP)      ss << "HP " << (HP < 0 ? "" : "+") << HP << endl;
    if (MP)      ss << "MP " << (MP < 0 ? "" : "+") << MP << endl;
    if (FP)      ss << "FP " << (FP < 0 ? "" : "+") << FP << endl;
    if (attack)  ss << "Atk " << (attack < 0 ? "" : "+") << attack << endl;
    if (defense) ss << "Def " << (defense < 0 ? "" : "+") << defense << endl;

    string info = "", tmp;
    while (getline(ss, tmp)) info += tmp + "\n";
    return info;
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
    int LevelArg = LV * (LV + 1);
    // vector<Item> Booty //This is for items that monster drops.
    // {
    //     Item("Orc's Chestplate", "Chestplate", 0, 0, 0, 0, 10 * LevelArg, 100 * LevelArg, 50 * LevelArg), 
    //     Item("Dwarf's diamond", "Treasure", 0, 0, 0, 0, 0, 0, 1000 * LevelArg),
    //     Item("Dragon's cloak", "cloak", 0, 0, 10*LevelArg, 0, 20 * LevelArg, 0, 0),
    //     Item("Elf's sword", "Weapon", 0, 0, 10*LevelArg, 20 * LevelArg, 0, 0, 0),
    //     Item("Wizard's potion", "Potion", 10 * LevelArg, 10 * LevelArg, 10 * LevelArg, 1 * LevelArg, 1 * LevelArg, 0, 0),
    //     Item("Troll's stick", "Weapon", 0, 0, 0, 3 * LevelArg, 1 * LevelArg, 10 * LevelArg, 0),
    //     Item("Basilisk's fang", "Treasure", 0, 0, 0, 0, 0, 0, 800 * LevelArg),
    //     Item("Caveman's bow", "Weapon", 0, 0, 5 * LevelArg, 0, 0, 0, 100 * LevelArg),
    //     Item("Phoenix's Tear", "Treasure", 100 * LevelArg , 100 * LevelArg , 100 * LevelArg, 0, 0, 0, 0),
    //     Item("Cerberus's fur", "Leggings", 0, 0, 0, 0, 20 * LevelArg, 100 * LevelArg, 100 * LevelArg),
    //     Item("Danny's cheese", "Food", 1000 * LevelArg, 0, 0, 0, 0, 0, 109 * LevelArg)
    // };

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
        Item("Helmet", "Helmet", 0, 0, 0, 0, 3 * LevelArg, 10 * LevelArg, 12 * LevelArg),
        Item("Chest Armor", "Chestplate", 0, 0, 0, 0, 5 * LevelArg, 10 * LevelArg, 20 * LevelArg),
        Item("Armor Pants", "Leggings", 0, 0, 0, 0, 4 * LevelArg, 10 * LevelArg, 16 * LevelArg),
        Item("Boots", "Boots", 0, 0, 0, 0, 2 * LevelArg, 10 * LevelArg, 8 * LevelArg),
        Item("Bread", "Food", 3 * LevelArg, 3 * LevelArg, 1 * LevelArg, 0, 0, 0, 2 * LevelArg)
    };

    Item item = Item();
    string kindList[] = {
        "Helmet",
        "Chestplate",
        "Leggings",
        "Boots",
        "Weapon",
        "Food",
        "Potion",
        // "Treasure"
    };
    if (kind == "RANDOM")
    {
        kind = kindList[ getRand( vector<long double>({5, 10, 15, 20, 25, 35, 40, /*41*/}) ) ];
    }

    vector <Item> pool;
    vector <long double> prob;
    int i = 0;
    for (auto& item : ItemsforSale) {
        if (item.getKind() == kind) {
            pool.emplace_back(item);
            prob.emplace_back(++i);
        }
    }
    if (prob.empty()) return Item("Bread", "Food", 3 * LevelArg, 3 * LevelArg, 1 * LevelArg, 0, 0, 0, 2 * LevelArg);
    return pool[ getRand( prob ) ];
}

void Item::renderOnWindow(float x, float y, float width, float height, RenderWindow* window, Color color) {
    stringstream ss; ss.clear();
    ss << getName() << endl;
    ss << "> type: " << getKind() << endl;
    if (getHP())         ss << "> HP: " << getHP() << endl;
    if (getMP())         ss << "> MP: " << getMP() << endl;
    if (getFP())         ss << "> FP: " << getFP() << endl;
    if (getAttack())     ss << "> Attack: " << getAttack() << endl;
    if (getDefense())    ss << "> Defense: " << getDefense() << endl;
    if (getDurability()) ss << "> Durability: " << getDurability() << endl;
    if (getPrice())      ss << "> Price: " << getPrice() << endl;

    string info = "", tmp;
    while (getline(ss, tmp)) {
        info += tmp;
        info.push_back('\n');
    }
    
    Font *font = new Font(); font->loadFromFile("../Fonts/Dosis-Light.ttf");
    Button* button = new Button(x, y, width, height, 1, 60, font, info, color, color, color);

    bool gainedFocus = 1;
	bool holdEnter = 1;
    Event sfEvent;
    while (window->isOpen()) {
        while (window->pollEvent(sfEvent)) {
            if ((sfEvent.type == Event::KeyPressed) && (sfEvent.key.code == Keyboard::Escape)) {
                window->close();
            }
            switch (sfEvent.type) {
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

        window->clear();
		
        button->render(window);

        window->display();
    }
    delete button;
}
