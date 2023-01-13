#include "GameCharacter.h"
GameCharacter::GameCharacter()
    : Object(), type("NULL")
{
    LV = maxEXP = currentEXP = maxHP = currentHP = maxMP = currentMP = maxFP = currentFP = attack = defense = 0;
}
GameCharacter::GameCharacter(string name, string tag, int HP, int LV) : Object(name, tag, ""), type(tag), LV(LV), maxEXP(10), currentEXP(0), maxHP(HP), currentHP(HP), maxMP(10), currentMP(10), maxFP(10), currentFP(0), attack(10), defense(10)
{
}
GameCharacter::GameCharacter(string name, string tag, string image, string type, int LV) : Object(name, tag, image), type(type), LV(LV)
{
    maxEXP = currentEXP = maxHP = currentHP = maxMP = currentMP = maxFP = currentFP = attack = defense = 0;
}
GameCharacter::GameCharacter(string name, string tag, string image, string type, int LV, int EXP, int HP, int MP, int FP, int attack, int defense) : Object(name, tag, image), type(type), LV(LV), maxEXP(EXP), currentEXP(0), maxHP(HP), currentHP(HP), maxMP(MP), currentMP(MP), maxFP(FP), currentFP(0), attack(attack), defense(defense)
{
}

/* Set & Get function*/
void GameCharacter::setLV(int LV)
{
    this->LV = LV;
}
void GameCharacter::setMaxEXP(int maxEXP)
{
    this->maxEXP = maxEXP;
}
void GameCharacter::setCurrentEXP(int currentEXP)
{
    this->currentEXP = currentEXP;
}
void GameCharacter::setMaxHP(int maxHP)
{
    this->maxHP = maxHP;
}
void GameCharacter::setCurrentHP(int currentHP)
{
    this->currentHP = currentHP;
}
void GameCharacter::setMaxMP(int maxMP)
{
    this->maxMP = maxMP;
}
void GameCharacter::setCurrentMP(int currentMP)
{
    this->currentMP = currentMP;
}
void GameCharacter::setMaxFP(int maxFP)
{
    this->maxFP = maxFP;
}
void GameCharacter::setCurrentFP(int currentFP)
{
    this->currentFP = currentFP;
}
void GameCharacter::setAttack(int attack)
{
    this->attack = attack;
}
void GameCharacter::setDefense(int defense)
{
    this->defense = defense;
}
int GameCharacter::getLV() const
{
    return LV;
}
int GameCharacter::getMaxEXP() const
{
    return maxEXP;
}
int GameCharacter::getCurrentEXP() const
{
    return currentEXP;
}
int GameCharacter::getMaxHP() const
{
    return maxHP;
}
int GameCharacter::getCurrentHP() const
{
    return currentHP;
}
int GameCharacter::getMaxMP() const
{
    return maxMP;
}
int GameCharacter::getCurrentMP() const
{
    return currentMP;
}
int GameCharacter::getMaxFP() const
{
    return maxFP;
}
int GameCharacter::getCurrentFP() const
{
    return currentFP;
}
int GameCharacter::getAttack() const
{
    return attack;
}
int GameCharacter::getDefense() const
{
    return defense;
}

/* Supplement */

/* Need to discuss about the balance of game */
int GameCharacter::expNeedToNextLV()
{
    int exp;
    if (LV <= 5)
        exp = 2 * LV + 7;
    else if (LV <= 10)
        exp = 5 * LV - 12;
    else
        exp = 9 * LV - 52;

    if (getTag() == "Monster")
        exp /= 2;

    return exp;
}
int GameCharacter::HPNextLV()
{
    int HP = 200 * LV * (LV + 1);
    if (getTag() == "Monster")
        HP *= 0.5;
    return HP;
}
int GameCharacter::MPNextLV()
{
    int MP = 200 * LV * (LV + 1);
    if (getTag() == "Monster")
        MP *= 0.8;
    return MP;
}
int GameCharacter::FPNextLV()
{
    int FP = 100 * LV * (LV + 1);
    if (getTag() == "Monster")
        FP *= 1;
    return FP;
}
int GameCharacter::attackNextLV()
{
    int attack = 20 * LV * (LV + 1);
    if (getTag() == "Monster")
        attack *= 0.8;
    return attack;
}
int GameCharacter::defenseNextLV()
{
    int defense = 20 * LV * (LV + 1);
    if (getTag() == "Monster")
        defense *= 0.8;
    return defense;
}

void GameCharacter::increaseStates(int HP, int MP, int FP, int attack, int defense)
{
    this->currentHP += HP;
    this->currentMP += MP;
    this->currentFP += FP;
    this->attack += attack;
    this->defense += defense;
}
void GameCharacter::decreaseStates(int HP, int MP, int FP, int attack, int defense)
{
    increaseStates(-HP, -MP, -FP, -attack, -defense);
}

void GameCharacter::increaseFP(int FP)
{
    this->currentFP += FP;
}

void GameCharacter::increaseEXP(int exp)
{
    currentEXP += exp;
    while (currentEXP >= maxEXP)
        increaseLV();
}

void GameCharacter::increaseLV()
{
    ++LV;
    if (getTag() == "Player")
        cout << "Level Up! You are Lv. " << LV << " now!\n\n";

    currentEXP -= maxEXP;
    maxEXP = expNeedToNextLV();
    maxHP = currentHP = HPNextLV();
    maxMP = currentMP = MPNextLV();
    maxFP = FPNextLV();
    attack = attackNextLV();
    defense = defenseNextLV();
}

bool GameCharacter::checkIsDead()
{
    return currentHP <= 0;
}

int GameCharacter::takeDamage(int damage)
{
    return currentHP = currentHP - damage;
}

bool GameCharacter::ultimateSkillAvailable()
{
    return currentFP >= maxFP;
}

void GameCharacter::inputGameCharacter(ifstream &in)
{
    inputObject(in);

    type = "";
    while (type == "")
        getline(in, type);
    in >> LV >> maxEXP >> currentEXP >> maxHP >> currentHP >> maxMP >> currentMP >> maxFP >> currentFP >> attack >> defense;
}

void GameCharacter::outputGameCharacter(ofstream &out) const
{
    outputObject(out);

    out << type << endl;
    out << LV << endl;
    out << maxEXP << ' ' << currentEXP << endl;
    out << maxHP << ' ' << currentHP << endl;
    out << maxMP << ' ' << currentMP << endl;
    out << maxFP << ' ' << currentFP << endl;
    out << attack << ' ' << defense << endl;
}
