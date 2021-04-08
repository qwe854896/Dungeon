#include "Item.h"
Item::Item() : Object(){
}
Item::Item(string name, string kind, int HP, int MP, int FP, int attack, int defense, int durability, int price)
: Object(name, "Item"), kind(kind), HP(HP), MP(MP), FP(FP), attack(attack), defense(defense), durability(durability), price(price)
{}
Item::Item(string name, string kind, string image, int HP, int MP, int FP, int attack, int defense, int durability, int price)
: Object(name, "Item", image), kind(kind), HP(HP), MP(MP), FP(FP), attack(attack), defense(defense), durability(durability), price(price)
{}

/* Virtual function that you need to complete    */
/* In Item, this function should deal with the   */
/* pick up action. You should add status to the  */
/* player.                                       */
bool Item::triggerEvent(Object* object) {
    Player *player = dynamic_cast<Player*>(object);
    if (player == nullptr) return false;

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

Item Item::randomItemGenerator(int LV, string kind)
{
    Item item = Item();
    if (kind == "RANDOM")
    {
        item = Item("Rusty Sword", "Weapon", 0, 0, 0, 10, 0, 100, 50);
    }
    else if (kind == "Props")
    {
        item = Item("Strange Potion", kind, "O", -10, 0, 10, 0, 0, 0, 0);
    }

    return item;
}
