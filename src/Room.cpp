#include "Room.h"
Room::Room()
{
    script = "";
    upRoom = downRoom = leftRoom = rightRoom = nullptr;
    isExit = 0;
    index = 0;
    X = Y = 0;
    objects.clear();
}
Room::Room(bool isExit, int index, int X, int Y)
    : isExit(isExit), index(index), X(X), Y(Y)
{
    upRoom = downRoom = leftRoom = rightRoom = nullptr;
    script = "";
    objects.clear();
}
Room::Room(bool isExit, int index, int X, int Y, vector<Object *> objects, string script)
    : Room(isExit, index, X, Y)
{
    this->script = script;
    this->objects = objects;
}

/* Set & Get function*/
void Room::setScript(string script)
{
    this->script = script;
}
void Room::setUpRoom(Room *upRoom)
{
    this->upRoom = upRoom;
}
void Room::setDownRoom(Room *downRoom)
{
    this->downRoom = downRoom;
}
void Room::setLeftRoom(Room *leftRoom)
{
    this->leftRoom = leftRoom;
}
void Room::setRightRoom(Room *rightRoom)
{
    this->rightRoom = rightRoom;
}
void Room::setIsExit(bool isExit)
{
    this->isExit = isExit;
}
void Room::setIndex(int index)
{
    this->index = index;
}
void Room::setX(int X)
{
    this->X = X;
}
void Room::setY(int Y)
{
    this->Y = Y;
}
void Room::setObjects(const vector<Object *> &objects)
{
    this->objects = objects;
}
string Room::getScript() const
{
    return script;
}
bool Room::getIsExit() const
{
    return isExit;
}
int Room::getIndex() const
{
    return index;
}
int Room::getX() const
{
    return X;
}
int Room::getY() const
{
    return Y;
}
vector<Object *> Room::getObjects() const
{
    return objects;
}
Room *Room::getUpRoom() const
{
    return upRoom;
}
Room *Room::getDownRoom() const
{
    return downRoom;
}
Room *Room::getLeftRoom() const
{
    return leftRoom;
}
Room *Room::getRightRoom() const
{
    return rightRoom;
}

/* Supplement */

/* pop out the specific object, used when the interaction is done */
bool Room::popObject(Object *object)
{
    for (int i = int(objects.size()) - 1; i >= 0; --i)
    {
        if (objects[i] == object)
        {
            swap(objects.back(), objects[i]);
            objects.pop_back();
            return true;
        }
    }
    return false;
}

/* push in the specific object, used when new item is added */
bool Room::pushObject(Object *object)
{
    objects.emplace_back(object);
    return true;
}

ifstream &operator>>(ifstream &in, Room &room)
{
    int index, X, Y;
    bool isExit;
    string script = "";

    in >> index;
    in >> isExit;
    in >> X >> Y;

    string tmp;
    while (1)
    {
        tmp = "";
        while (tmp == "")
            getline(in, tmp);
        if (tmp == "script end.")
            break;
        tmp += "\n";
        script += tmp;
    }

    room.setIndex(index);
    room.setX(X);
    room.setY(Y);
    room.setIsExit(isExit);
    room.setScript(script);

    // objects and UDLR index were input in "Record.cpp"

    return in;
}
ofstream &operator<<(ofstream &out, const Room &room)
{
    out << room.getIndex() << endl;
    out << room.getIsExit() << endl;
    out << room.getX() << ' ' << room.getY() << endl;
    out << room.getScript() << "\nscript end.\n"
        << endl;

    out << room.getObjects().size() << endl;
    for (auto obj : room.getObjects())
    {
        out << (*obj) << endl;
    }

    if (room.getUpRoom() == nullptr)
        out << -1;
    else
        out << room.getUpRoom()->getIndex();
    out << ' ';
    if (room.getDownRoom() == nullptr)
        out << -1;
    else
        out << room.getDownRoom()->getIndex();
    out << ' ';
    if (room.getLeftRoom() == nullptr)
        out << -1;
    else
        out << room.getLeftRoom()->getIndex();
    out << ' ';
    if (room.getRightRoom() == nullptr)
        out << -1;
    else
        out << room.getRightRoom()->getIndex();
    out << endl;

    return out;
}
