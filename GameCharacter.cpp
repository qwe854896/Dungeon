#include "GameCharacter.h"
GameCharacter::GameCharacter() {
}
GameCharacter::GameCharacter(string name, string tag, int HP, int LV) :
Object(name, tag), type(tag), LV(LV), maxEXP(10), currentEXP(0), maxHP(HP), currentHP(HP), maxMP(10), currentMP(10), maxFP(10), currentFP(0), attack(10), defense(10)
{}
GameCharacter::GameCharacter(string name, string tag, string image, string type, int LV) :
Object(name, tag, image), type(type), LV(LV) {
}
GameCharacter::GameCharacter(string name, string tag, string image, string type, int LV, int EXP, int HP, int MP, int FP, int attack, int defense) :
Object(name, tag, image), type(type), LV(LV), maxEXP(EXP), currentEXP(0), maxHP(HP), currentHP(HP), maxMP(MP), currentMP(MP), maxFP(FP), currentFP(0), attack(attack), defense(defense) {
}

bool GameCharacter::checkIsDead() {
	return currentHP <= 0;
}
int GameCharacter::takeDamage(int damage) {
	return currentHP = currentHP - damage;
}

/* Supplement */
int GameCharacter::expNeedToNextLV()
{
    return 500 * LV * (LV + 1);
}
int GameCharacter::HPNextLV()
{
    return 200 * LV * (LV + 1);
}
int GameCharacter::MPNextLV()
{
    return 200 * LV * (LV + 1);
}
int GameCharacter::FPNextLV()
{
    return 100 * LV * (LV + 1);
}
int GameCharacter::attackNextLV()
{
    return 50 * LV * (LV + 1);
}
int GameCharacter::defenseNextLV()
{
    return 50 * LV * (LV + 1);
}

void GameCharacter::increaseStates(int HP, int MP, int FP, int attack, int defense) {
    this->currentHP += HP;
    this->currentMP += MP;
    this->currentFP += FP;
    this->attack += attack;
    this->defense += defense;
}

void GameCharacter::decreaseStates(int HP, int MP, int FP, int attack, int defense) {
    increaseStates(-HP, -MP, -FP, -attack, -defense);
}

void GameCharacter::increaseEXP(int exp)
{
    currentEXP += exp;
    while (currentEXP >= maxEXP) increaseLV();
}

void GameCharacter::increaseLV()
{
    ++LV;
    currentEXP -= maxEXP;
    maxEXP = expNeedToNextLV();
    maxHP = currentHP = HPNextLV();
    maxMP = currentMP = MPNextLV();
    maxFP = currentFP = FPNextLV();
    attack = attackNextLV();
    defense = defenseNextLV();
}

/* Set & Get function*/
void GameCharacter::setLV(int LV) {
	this->LV = LV;
}
void GameCharacter::setMaxEXP(int maxEXP) {
	this->maxEXP = maxEXP;
}
void GameCharacter::setCurrentEXP(int currentEXP) {
	this->currentEXP = currentEXP;
}
void GameCharacter::setMaxHP(int maxHP) {
	this->maxHP = maxHP;
}
void GameCharacter::setCurrentHP(int currentHP) {
	this->currentHP = currentHP;
}
void GameCharacter::setMaxMP(int maxMP) {
	this->maxMP = maxMP;
}
void GameCharacter::setCurrentMP(int currentMP) {
	this->currentMP = currentMP;
}
void GameCharacter::setMaxFP(int maxFP) {
	this->maxFP = maxFP;
}
void GameCharacter::setCurrentFP(int currentFP) {
	this->currentFP = currentFP;
}
void GameCharacter::setAttack(int attack) {
	this->attack = attack;
}
void GameCharacter::setDefense(int defense) {
	this->defense = defense;
}
int GameCharacter::getLV() const {
	return LV;
}
int GameCharacter::getMaxEXP() const {
	return maxEXP;
}
int GameCharacter::getCurrentEXP() const {
	return currentEXP;
}
int GameCharacter::getMaxHP() const {
	return maxHP;
}
int GameCharacter::getCurrentHP() const {
	return currentHP;
}
int GameCharacter::getMaxMP() const {
	return maxMP;
}
int GameCharacter::getCurrentMP() const {
	return currentMP;
}
int GameCharacter::getMaxFP() const {
	return maxFP;
}
int GameCharacter::getCurrentFP() const {
	return currentFP;
}
int GameCharacter::getAttack() const {
	return attack;
}
int GameCharacter::getDefense() const {
	return defense;
}

/* Supplement */

void GameCharacter::inputGameCharacter(ifstream& in)
{
    inputObject(in);

    type = ""; while (type == "") getline(in, type);
    in >> LV >> maxEXP >> currentEXP >> maxHP >> currentHP >> maxMP >> currentMP >> maxFP >> currentFP >> attack >> defense;
}
void GameCharacter::outputGameCharacter(ofstream& out) const
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
