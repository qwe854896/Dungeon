#include <iostream>
#include <string>
#include <vector>
#include "Dungeon.h"
#include "Item.h"
#include "Monster.h"
#include "NPC.h"
#include "Player.h"


using namespace std;

int main(){
    srand(clock());
    
    Dungeon dungeon;
    dungeon.runDungeon(1);
    return 0;
}
