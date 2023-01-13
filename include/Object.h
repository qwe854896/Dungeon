#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
using namespace sf;

class Object
{
private:
    string name;
    string tag;
    string image;

public:
    Object();
    Object(string, string);
    Object(string, string, string);
    virtual ~Object() = default;

    /* pure virtual function */
    virtual bool triggerEvent(Object *, RenderWindow *) = 0;
    virtual void input(ifstream &) = 0;
    virtual void output(ofstream &) const = 0;

    /* Set & Get function*/
    void setName(string);
    void setTag(string);
    void setImage(string);
    string getName() const;
    string getTag() const;
    string getImage() const;

    /* Supplement */
    void inputObject(ifstream &);
    void outputObject(ofstream &) const;
};

ifstream &operator>>(ifstream &, Object &);
ofstream &operator<<(ofstream &, const Object &);

#endif // OBJECT_H_INCLUDED
