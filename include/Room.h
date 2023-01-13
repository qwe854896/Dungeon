#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <assert.h>
#include "Object.h"
using namespace std;

class Room
{
private:
    string script;
    Room *upRoom;
    Room *downRoom;
    Room *leftRoom;
    Room *rightRoom;
    bool isExit;
    int index;
    int X, Y;
    vector<Object *> objects; /*contain 1 or multiple objects, including monster, npc, etc*/
public:
    Room();
    Room(bool, int, int, int);
    Room(bool, int, int, int, vector<Object *>, string);

    /* Set & Get function*/
    void setScript(string);
    void setUpRoom(Room *);
    void setDownRoom(Room *);
    void setLeftRoom(Room *);
    void setRightRoom(Room *);
    void setIsExit(bool);
    void setIndex(int);
    void setX(int);
    void setY(int);
    void setObjects(const vector<Object *> &);
    string getScript() const;
    bool getIsExit() const;
    int getIndex() const;
    int getX() const;
    int getY() const;
    vector<Object *> getObjects() const;
    Room *getUpRoom() const;
    Room *getDownRoom() const;
    Room *getLeftRoom() const;
    Room *getRightRoom() const;

    /* Supplement */
    bool popObject(Object *);  /* pop out the specific object, used when the interaction is done */
    bool pushObject(Object *); /* push in the specific object, used when new item is added */
};

ifstream &operator>>(ifstream &, Room &);
ofstream &operator<<(ofstream &, const Room &);

#endif // ROOM_H_INCLUDED
