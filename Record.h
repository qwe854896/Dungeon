#ifndef RECORD_H_INCLUDED
#define RECORD_H_INCLUDED

#define OPS (unsigned int)(ops[0] - 'A')
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <assert.h>
#include <dirent.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <filesystem>
#include "Item.h"
#include "Monster.h"
#include "NPC.h"
#include "Player.h"

using namespace std;

using coord = pair<int, int>;

/* This is the record system. Get the information of the  */
/* player and rooms then save them to (a) file(s). Notice */
/* that using pass by reference can prevent sending the   */
/* whole vector to the function.                          */

class Record
{
private:
    int CRIndex, PRIndex;
    vector < pair<pair<string, int>, string> > fileList;

    void savePlayer(Player*, ofstream&);
    void saveRooms(vector<Room>&, ofstream&);
    void saveVis(vector<bool>&, ofstream&);
    void saveCoord(map <coord, int>&, ofstream&);

    void loadPlayer(Player*, ifstream&);
    void loadRooms(vector<Room>&, ifstream&);
    void loadVis(vector<bool>&, ifstream&);
    void loadCoord(map <coord, int>&, ifstream&);

    void saveFileNames();
    void loadFileNames();
    void listFileNames();
    void initFileNames(int);

public:
    Record();
    Record(int);
    void saveToFile(Player*, vector<Room>&, vector<bool>&, int&, map <coord, int>&);
    bool loadFromFile(Player*, vector<Room>&, vector<bool>&, int&, map <coord, int>&);
};

#endif // RECORD_H_INCLUDED
