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
    Dungeon dungeon = Dungeon();

//    int cnt[4] = {0};
//    for (int i = 0; i < 10000; ++i) {
//        int x = dungeon.getRand( vector<long double>({10, 50, 70, 100}) );
////        cout << x  << endl;
//        ++cnt[x];
//    }
//
//    for (int i = 0; i < 4; ++i) cout << cnt[i] << endl;
    dungeon.runDungeon();
    return 0;
}
