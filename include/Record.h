#ifndef RECORD_H_INCLUDED
#define RECORD_H_INCLUDED

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
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
#include "Button.h"
#include "Menu.h"

using namespace std;
using namespace sf;
using std::cout;

using coord = pair<int, int>;

/* This is the record system. Get the information of the  */
/* player and rooms then save them to (a) file(s). Notice */
/* that using pass by reference can prevent sending the   */
/* whole vector to the function.                          */

class Record
{
private:
    bool gainedFocus, holdEnter;
    int CRIndex, PRIndex, option;
    vector<pair<pair<string, int>, string>> fileList;

    void savePlayer(Player *, ofstream &);
    void saveRooms(vector<Room> &, ofstream &);
    void saveVis(vector<bool> &, ofstream &);
    void saveCoord(map<coord, int> &, ofstream &);
    void saveRoomCnt(vector<int> &, ofstream &);

    void loadPlayer(Player *, ifstream &);
    void loadRooms(vector<Room> &, ifstream &);
    void loadVis(vector<bool> &, ifstream &);
    void loadCoord(map<coord, int> &, ifstream &);
    void loadRoomCnt(vector<int> &, ifstream &);

    void saveFileNames();
    void loadFileNames();
    void listFileNames();
    void initFileNames(int);

    RenderWindow *_window;
    Event sfEvent;
    Font *font;
    Button *button;
    Menu *menu;

public:
    Record();
    Record(int, RenderWindow *_window);
    void saveToFile(Player *, vector<Room> &, vector<bool> &, int &, map<coord, int> &, vector<int> &);
    void saveToFile(Player *, vector<Room> &, vector<bool> &, int &, map<coord, int> &, vector<int> &, ofstream &);
    bool loadFromFile(Player *, vector<Room> &, vector<bool> &, int &, map<coord, int> &, vector<int> &);
};

#endif // RECORD_H_INCLUDED
