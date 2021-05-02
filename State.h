#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <assert.h>

using namespace std;
using namespace sf;

class State
{
private:
    vector<Texture> textures;
    
public:
    State(vector<Texture>);
    virtual ~State() = default;

    virtual void update() = 0;
    virtual void render() = 0;
};


#endif // STATE_H_INCLUDED
