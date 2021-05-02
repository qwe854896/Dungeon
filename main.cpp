#include <iostream>
#include <string>
#include <vector>
#include "Dungeon.h"
#include "Item.h"
#include "Monster.h"
#include "NPC.h"
#include "Player.h"
#include "Button.h"

using namespace std;

int main(){
    srand(123);
    Dungeon dungeon;
    dungeon.runDungeon(1);
    return 0;
}
