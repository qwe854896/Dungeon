#include "Object.h"
/* Constructor */
Object::Object() : name("empty"), tag("NULL"), image("")
{
}
Object::Object(string name, string tag) : name(name), tag(tag), image("")
{
}
Object::Object(string name, string tag, string image) : name(name), tag(tag), image(image)
{
}

/* Set & Get function*/
void Object::setName(string name)
{
    this->name = name;
}
void Object::setTag(string tag)
{
    this->tag = tag;
}
void Object::setImage(string image)
{
    this->image = image;
}
string Object::getName() const
{
    return name;
}
string Object::getTag() const
{
    return tag;
}
string Object::getImage() const
{
    return image;
}

/* Supplement */
void Object::inputObject(ifstream &in)
{
    name = "";
    while (name == "")
        getline(in, name);

    string tmp;
    while (1)
    {
        tmp = "";
        while (tmp == "")
            getline(in, tmp);
        if (tmp == "image end.")
            break;
        tmp += "\n";
        image += tmp;
    }
}
void Object::outputObject(ofstream &out) const
{
    out << tag << endl;

    out << name << endl;
    out << image << "\nimage end.\n";
}
ifstream &operator>>(ifstream &in, Object &object)
{
    object.input(in);
    return in;
}
ofstream &operator<<(ofstream &out, const Object &object)
{
    object.output(out);
    return out;
}
