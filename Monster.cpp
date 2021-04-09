#include "Monster.h"

Monster::Monster() : GameCharacter() {}
Monster::Monster(string name, string image, string type, int LV)
: GameCharacter(name, "Monster", image, type, LV)
{}
Monster::Monster(string name, string image, string type, int LV, int EXP, int HP, int MP, int FP, int attack, int defense)
: GameCharacter(name, "Monster", image, type, LV, EXP, HP, MP, FP, attack, defense)
{}

/* Virtual function that you need to complete   */
/* In Monster, this function should deal with   */
/* the combat system.                           */
/* return true if the monster is dead. */
bool Monster::triggerEvent(Object* object) {
    if (object->getTag() == "Player") {
        Player *player = dynamic_cast<Player*>(object);
        
        while (1) {
            cout << "A. attack\n";
            cout << "B. retreat\n";

            string ops; getline(cin, ops);
            while (ops[0] < 'A' || ops[0] > 'B') {
                cout << "Error, please enter operation again.\n";
                getline(cin, ops);
            }

            system("cls");

            if (ops[0] == 'A') {
                int damage;
                cout << "Your attack!\n";

                damage = player->getAttack() - getDefense(); // calculateDamage
                if (damage < 0) damage = 0;
                cout << "You caused " << damage << " damages!\n\n";

                takeDamage(damage);
                if (checkIsDead()) {
                    cout << getName() << " is dead!\nYou win!\n";
                    return true;
                }

                cout << getName() << " attack!\n";

                damage = getAttack() - player->getDefense();
                if (damage < 0) damage = 0;
                cout << getName() << " caused " << damage << " damages!\n\n";

                player->takeDamage(damage);
                if (player->checkIsDead()) {
                    return false;
                }
            }
            else if (ops[0] == 'B') {
                cout << "You flee!\n";
                player->changeRoom(player->getPreviousRoom());
                return false;
            }

            cout << endl;
            system("pause"); system("cls");
        }
    }
    
    return false;
}

void Monster::input(ifstream& in)
{
    setTag("Monster");
    inputGameCharacter(in);
}

void Monster::output(ofstream& out) const
{
    outputGameCharacter(out);
}

/* Supplement */
