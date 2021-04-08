#ifndef GAMECHARACTER_H_INCLUDED
#define GAMECHARACTER_H_INCLUDED

#include <iostream>
#include <string>
#include <cmath>
#include "Object.h"
using namespace std;

class GameCharacter: public Object
{
private:
    string type;
    int LV;
    int maxEXP, currentEXP;
    int maxHP, currentHP;
    int maxMP, currentMP;
    int maxFP, currentFP;
    int attack;
    int defense;

    /* Supplement */
    int expNeedToNextLV();
    int HPNextLV();
    int MPNextLV();
    int FPNextLV();
    int attackNextLV();
    int defenseNextLV();
public:
    GameCharacter();
    GameCharacter(string,string,int,int); // name, tag, HP, LV
    GameCharacter(string,string,string,string,int); // name, tag, image, type, LV
    GameCharacter(string,string,string,string,int,int,int,int,int,int,int); // name, tag, image, type, LV, EXP, HP, MP, FP, attack, defense
    bool checkIsDead();
    int takeDamage(int);
    void increaseStates(int,int,int,int,int);
    void decreaseStates(int,int,int,int,int);
    void increaseEXP(int);
    void increaseLV();

    /* Set & Get function*/
    void setLV(int);
    void setMaxEXP(int);
    void setCurrentEXP(int);
    void setMaxHP(int);
    void setCurrentHP(int);
    void setMaxMP(int);
    void setCurrentMP(int);
    void setMaxFP(int);
    void setCurrentFP(int);
    void setAttack(int);
    void setDefense(int);
    int getLV() const;
    int getMaxEXP() const;
    int getCurrentEXP() const;
    int getMaxHP() const;
    int getCurrentHP() const;
    int getMaxMP() const;
    int getCurrentMP() const;
    int getMaxFP() const;
    int getCurrentFP() const;
    int getAttack() const;
    int getDefense() const;

    /* Supplement */
    void inputGameCharacter(ifstream&);
    void outputGameCharacter(ofstream&) const;
};
#endif // GAMECHARACTER_H_INCLUDED
