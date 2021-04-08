#include "NPC.h"

NPC::NPC(){}
NPC::NPC(string name, string image, string type, string script, int LV)
: GameCharacter(name, "NPC", image, type, LV), script(script)
{
}
NPC::NPC(string name, string script, vector<Item> commodity, int LV)
: GameCharacter(name, "NPC", 100, LV), script(script), commodity(commodity)
{
}

void NPC::listCommodity() /*print all the Item in this NPC*/
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

/* Virtual function that you need to complete   */
/* In NPC, this function should deal with the   */
/* transaction in easy implementation           */
bool NPC::triggerEvent(Object* object) {
    Player *player = dynamic_cast<Player*>(object);
    if (player == nullptr) return false;

    cout << script << endl;
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
    player->increaseGold(-commodity[OPS].getPrice());

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

