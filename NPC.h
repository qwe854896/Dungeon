#ifndef NPC_H_INCLUDED
#define NPC_H_INCLUDED

#define OPS (unsigned int)(ops[0] - 'A')
#include <iostream>
#include <string>
#include <vector>
#include "GameCharacter.h"
#include "Player.h"
#include "Item.h"

using namespace std;
using namespace sf;

class NPC: public GameCharacter
{
private:
    string script;
    vector<Item> commodity;
public:
    NPC();
    NPC(string, string, string, string, int);
    NPC(string, string, string, string, vector<Item>, int);

    /* Virtual function that you need to complete   */
    /* In NPC, this function should deal with the   */
    /* transaction in easy implementation           */
    bool triggerEvent(Object*, RenderWindow*) override;
    void input(ifstream&) override;
    void output(ofstream&) const override;

    /* Set & Get function*/
    void setScript(string);
    void setCommodity(vector<Item>);
    string getScript() const;
    vector<Item> getCommodity() const;

    /* Supplement */
    int listCommodity(RenderWindow*, int); /* print all the Item in this NPC */
    void pushCommodity(Item); /* push an item into commodity */

    bool handleBuy(Player*, RenderWindow*);
    bool handleSell(Player*, RenderWindow*);
    bool handleTalk(RenderWindow*);
};


#endif // NPC_H_INCLUDED
