#include "Monster.h"

Monster::Monster() : GameCharacter() {}
Monster::Monster(string name, string image, string type, int LV, Item drop)
: GameCharacter(name, "Monster", image, type, LV), drop(drop)
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
bool Monster::triggerEvent(Object* object, RenderWindow* window) {
    stringstream ss;
    Font font; font.loadFromFile("../Fonts/Dosis-Light.ttf");
    if (object->getTag() == "Player") {
        Player *player = dynamic_cast<Player*>(object);
        
        string statusText = produceStatus();
        string playerStatusText = player->showStatusinFight();

        Button *status = new Button(10, 20, 1900, 100, 1, 60, &font, "", Color(100, 100, 150, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));;
        Button *playerStatus = new Button(10, 20, 1900, 100, 1, 60, &font, "", Color(100, 100, 150, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
        Button *description = new Button(10, 20, 1900, 100, 1, 60, &font, "", Color(100, 100, 150, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));;

        Menu *menu = new Menu(20, 150, 1880, 100, 0, 60, true, &font, window, Color(20, 20, 20, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));;

        
        menu->push_back("ATTACK");
        menu->push_back("ACT");
        menu->push_back("ITEM");
        menu->push_back("FLEE");

        int state = 4;
        bool playerTakeAction = false;
        bool gainedFocus = 1;
	    bool holdEnter = 1;
        bool showMenu = 1;

        string info, tmp;

        Item effect;
        pair<string, int> damageInfo;

        Event sfEvent;
        while (window->isOpen()) {
            // detect and update
            while (window->pollEvent(sfEvent)) {
                if ((sfEvent.type == Event::KeyPressed) && (sfEvent.key.code == Keyboard::Escape)) {
                    window->close();
                }
                switch (sfEvent.type) {
                    case Event::Closed:
                        window->close();
                        break;
                    case Event::GainedFocus:
                        gainedFocus = 1;
                        break;
                    case Event::LostFocus:
                        gainedFocus = 0;
                        break;
                    default:
                        break;
                }
            }
            if (!gainedFocus) {
                sleep(milliseconds(10));
                continue;
            }
            if (!holdEnter && Keyboard::isKeyPressed(Keyboard::Enter)) {
                if (state == 4) {
                    if (playerTakeAction) state = 5;
                    else state = menu->getIsSelected();
                }
                else if (state == 9) {
                    break;
                }
                else if (state == 7) {
                    if (checkIsDead()) state = 6;
                    else state = 5;
                }
                else if (state == 8) {
                    if (player->checkIsDead()) break;
                    else state = 4;
                }
                holdEnter = 1;
            }
            if (holdEnter && !Keyboard::isKeyPressed(Keyboard::Enter)) {
                holdEnter = 0;
            }
            switch (state) {
                case 0: // ATTACK
                    effect = player->handleAttack(window);
                    if (effect.getKind() == "NULL") {
                        state = 4;
                        continue;
                    }

                    playerTakeAction = true;
                    
                    damageInfo = damageCalculate( effect.getHP(), getDefense() );
                    if (damageInfo.second < 0) damageInfo.second = 0;

                    ss.clear();
                    ss << "You caused " << damageInfo.second << " damages!\n\n";
                    info = "";
                    while (getline(ss, tmp)) info += tmp + "\n";

                    description->updateText( "Your attack!\n" + damageInfo.first + info + "\n" + player->decreaseWeaponDurability(1) );

                    takeDamage(damageInfo.second);
                    state = 7;
                    break;

                case 1: // ACT
                    playerTakeAction = player->handleAct(window);
                    state = 4;
                    continue;
                    break;

                case 2: // ITEM
                    playerTakeAction = player->handleInventory("use", window);
                    state = 4;
                    continue;
                    break;

                case 3: // FLEE
                    description->updateText("You flee!");
                    player->changeRoom(player->getPreviousRoom());

                    state = 9;
                    break;

                case 4: // Menu
                    description->updateText(getName() + " blocked the way!\n");
                    menu->update();
                    break;

                case 5:
                    /* Monster attack! */
                    damageInfo = damageCalculate( getAttack(), player->getDefense() );
                    if (damageInfo.second < 0) damageInfo.second = 0;

                    ss.clear();
                    ss << getName() << " caused " << damageInfo.second << " damages!\n\n";
                    info = "";
                    while (getline(ss, tmp)) info += tmp + "\n";
                    info.pop_back();
                    description->updateText(getName() + " attack!\n" + damageInfo.first + info + player->decreaseArmorDurability(1));

                    player->takeDamage(damageInfo.second);
                    player->increaseFP(1);
                    playerTakeAction = 0;
                    state = 8;
                    break;

                case 6:
                    /* Trophy */
                    player->increaseEXP( getMaxEXP() );

                    tmp = "";
                    if (getRand( vector<long double>{80, 100} )) {
                        if (drop.getKind() == "NULL") drop = Item::randomItemGenerator(player->getLV(), "RANDOM");
                        tmp = "\n" + getName() + " drop " + drop.getName();
                        player->addItem(drop);
                    }

                    description->updateText(getName() + " is dead!\nYou win!" + tmp);
                    state = 9;
                    break;
            }  
            menu->update();

            window->clear();

            switch (state) {
                case 3:
                case 7:
                case 8:
                case 9:
                    description->render(window);
                    break;
                case 4:
                    description->render(window);
                    menu->render();
                    
                    break;
                
                default:
                    break;
            }

            window->display();
        }
        delete status;
        delete playerStatus;
        delete description;
        delete menu;

        return checkIsDead();
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

pair<string, int> Monster::damageCalculate(int attack, int defense) {
    int damage = attack * 4 - defense * 2;
    string info;

    int event = getRand(vector<long double>( {1, 2, 12, 92} )); // 1 1 10 80
    switch (event) {
        case 0:
            info = "MISS!\n";
            damage = 0;
            break;
        case 1:
            info = "Critical Hit!\n";
            damage *= 2;
            break;
        case 2:
            info = "Defensed!\n";
            damage *= 0.9;
            break;
        case 3:
            info = "";
            damage *= 1;
            break;
    }

    return pair<string, int>(info, damage);
}


string Monster::produceStatus() {
    cout << getImage() << endl;
    cout << getName() << "   Lv. " << setw(2) << getLV() << endl;
    cout << left << setw(12) << "> HP: "; showBar(getCurrentHP(), getMaxHP()); cout << getCurrentHP() << '/' << getMaxHP() << endl;

    stringstream ss;  ss.clear();
    ss << getName() << "   Lv. " << setw(2) << getLV() << endl;
    ss << left << setw(12) << "> HP: " << getCurrentHP() << '/' << getMaxHP() << endl;

    string info = "", tmp;
    while (getline(ss, tmp)) info += tmp + "\n";
    info.pop_back();

    return info;
}