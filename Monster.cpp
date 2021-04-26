#include "Monster.h"

Monster::Monster() : GameCharacter() {}
Monster::Monster(string name, string image, string type, int LV)
: GameCharacter(name, "Monster", image, type, LV)
{}
Monster::Monster(string name, string image, string type, int LV, int EXP, int HP, int MP, int FP, int attack, int defense)
: GameCharacter(name, "Monster", image, type, LV, EXP, HP, MP, FP, attack, defense)
{}
namespace {
    /* randomly pick a number by given probability */
    int getRand(vector<long double> p)
    {
        uniform_real_distribution<long double> distribution(0, p.back());
        mt19937 engine( rand() );

        long double value = distribution(engine);
        return (int)(upper_bound(p.begin(), p.end(), value) - p.begin());
    }
    void showBar(int current, int max) {
        const int nstars = 50;
        const int per = current * nstars / max;
        cout << "| " << string(per, '=') << string(nstars - per, ' ') << " | ";
    }
}

/* Virtual function that you need to complete   */
/* In Monster, this function should deal with   */
/* the combat system.                           */
/* return true if the monster is dead. */
bool Monster::triggerEvent(Object* object) {
    if (object->getTag() == "Player") {
        Player *player = dynamic_cast<Player*>(object);
        
        while (1) {
            system("cls");
            showStatus(); cout << endl;
            
            player->showStatusinFight(); cout << endl;

            cout << getName() << " blocked the way!\n";
            cout << "A. ATTACK\n";
            cout << "B. ACT\n";
            cout << "C. ITEM\n";
            cout << "D. FLEE\n";

            string ops; getline(cin, ops);
            while (ops[0] < 'A' || ops[0] > 'D') {
                cout << "Error, please enter operation again.\n";
                getline(cin, ops);
            }

            system("cls");

            bool playerTakeAction = false;
            if (ops[0] == 'A') {
                Item effect = player->handleAttack();
                if (effect.getKind() == "NULL") continue;

                playerTakeAction = true;
                cout << "Your attack!\n";
                
                int damage = damageCalculate( effect.getHP(), getDefense() );
                if (damage < 0) damage = 0;
                cout << "You caused " << damage << " damages!\n\n";

                takeDamage(damage);
                player->decreaseWeaponDurability(1);
                
                if (checkIsDead()) {
                    cout << getName() << " is dead!\nYou win!\n";

                    /* Trophy */
                    player->increaseEXP( getMaxEXP() );

                    return true;
                }
            }
            else if (ops[0] == 'B') {
                playerTakeAction = player->handleAct();
            }
            else if (ops[0] == 'C') {
                playerTakeAction = player->handleInventory("use");
            }
            else if (ops[0] == 'D') { 
                cout << "You flee!\n";
                player->changeRoom(player->getPreviousRoom());
                return false;
            }

            if (!playerTakeAction) {
                cout << endl;
                system("pause");
                continue;
            } 

            /* Monster attack! */
            cout << getName() << " attack!\n";

            int damage = damageCalculate( getAttack(), player->getDefense() );
            if (damage < 0) damage = 0;
            cout << getName() << " caused " << damage << " damages!\n\n";

            player->takeDamage(damage);
            player->increaseFP(1);
            player->decreaseArmorDurability(1);

            if (player->checkIsDead()) {
                return false;
            }

            cout << endl;
            system("pause");
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

int Monster::damageCalculate(int attack, int defense) {
    int damage = attack * 4 - defense * 2;

    int event = getRand(vector<long double>( {1, 2, 12, 92} )); // 1 1 10 80
    switch (event) {
        case 0:
            cout << "MISS!\n";
            damage = 0;
            break;
        case 1:
            cout << "Critical Hit!\n";
            damage *= 2;
            break;
        case 2:
            cout << "Defensed!\n";
            damage *= 0.9;
            break;
        case 3:
            cout << "Attack!\n";
            damage *= 1;
            break;
    }

    return damage;
}


void Monster::showStatus() {
    cout << getImage() << endl;
    cout << getName() << "   Lv. " << setw(2) << getLV() << endl;

    cout << left << setw(12) << "> HP: "; showBar(getCurrentHP(), getMaxHP()); cout << getCurrentHP() << '/' << getMaxHP() << endl;
}