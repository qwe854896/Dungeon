#include "NPC.h"

NPC::NPC() : GameCharacter(), script("") {}
NPC::NPC(string name, string image, string type, string script, int LV)
: GameCharacter(name, "NPC", image, type, LV), script(script)
{
}
NPC::NPC(string name, string image, string type, string script, vector<Item> commodity, int LV)
: NPC(name, image, type, script, LV)
{
    this->commodity = commodity;
}

/* Virtual function that you need to complete   */
/* In NPC, this function should deal with the   */
/* transaction in easy implementation           */
bool NPC::triggerEvent(Object* object) {
    if (object->getTag() == "Player") {
        Player *player = dynamic_cast<Player*>(object);
        
        while (1) {
            system("cls");

            cout << script << endl;

            cout << "A. BUY\nB. SELL\nC. TALK\nD. EXIT\n";
            string ops; getline(cin, ops);
            while (ops[0] < 'A' || ops[0] > 'D') {
                cout << "Error, please enter operation again.\n";
                getline(cin, ops);
            }

            switch (ops[0]) {
                case 'A':
                    while ( handleBuy(player) );
                    break;
                case 'B':
                    while ( handleSell(player) );
                    cout << "Your bag is empty now!\n\n";
                    break;
                case 'C':
                    while ( handleTalk() );
                    break;
                case 'D':
                    return false;
            }
        }
    }

    return false;
}

void NPC::input(ifstream& in)
{
    setTag("NPC");
    inputGameCharacter(in);

    script = "";
    string tmp;
    while (1) {
        tmp = ""; while (tmp == "") getline(in, tmp);
        if (tmp == "script end.") break;
        tmp += "\n";
        script += tmp;
    }

    int n;
    in >> n;
    commodity.resize(n);
    for (int i = 0; i < n; ++i) {
        string tag = ""; while (tag == "") getline(in, tag);
        in >> commodity[i];
    }
}

void NPC::output(ofstream& out) const
{
    outputGameCharacter(out);

    out << script << "\nscript end.\n" << endl;

    out << commodity.size() << endl;
    for (auto good : commodity) {
        out << good << endl;
    }
}

/* Set & Get function*/
void NPC::setScript(string script) {
    this->script = script;
}
void NPC::setCommodity(vector<Item> commodity) {
    this->commodity = commodity;
}
string NPC::getScript() const {
    return script;
}
vector<Item> NPC::getCommodity() const {
    return commodity;
}

/* Supplement */

void NPC::listCommodity() /* print all the Item in this NPC*/
{
    char option = 'A' - 1;
    for (auto good : commodity) {
        cout << (++option) << ". " << good << endl;
    }
    cout << (++option) << ". Cancel\n";
}

void NPC::pushCommodity(Item item) /* push an item into commodity */
{
    commodity.emplace_back(item);
}

bool NPC::handleBuy(Player *player) {
    system("cls");
    cout << "What do you want to buy?\n";

    listCommodity();

    string ops; getline(cin, ops);
    while (1) {
        while (ops[0] < 'A' || OPS > commodity.size()) {
            cout << "Error, please enter operation again.\n";
            getline(cin, ops);
        }
        if (OPS == commodity.size() || commodity[OPS].getPrice() <= player->getGold()) break;

        cout << "Sorry, you don't have enough money!\nPlease enter operation again.\n";
        getline(cin, ops);
    }

    if (OPS == commodity.size()) return false;

    cout << "You buy " << commodity[OPS].getName() << endl;
    player->addItem(commodity[OPS]);
    player->decreaseGold(commodity[OPS].getPrice());

    cout << endl;
    system("pause");

    return true;
}

bool NPC::handleSell(Player *player) {
    system("cls");

    cout << "What do you want to sell?\n";

    return player->handleInventory("sell");
}

bool NPC::handleTalk() {
    system("cls");
    cout << "I am shy!\n\n";
    system("pause");
    return false;
}

