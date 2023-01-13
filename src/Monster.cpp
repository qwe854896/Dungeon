#include "Monster.h"

Monster::Monster() : GameCharacter() {}
Monster::Monster(string name, string image, string type, int LV, Item drop)
    : GameCharacter(name, "Monster", image, type, LV), drop(drop)
{
    hit.loadFromFile("../Sounds/hit.ogg");
    sound.setBuffer(hit);
}
Monster::Monster(string name, string image, string type, int LV, Item drop, Texture *monsterTexture, Texture *backgroundTexture)
    : GameCharacter(name, "Monster", image, type, LV), drop(drop), monsterTexture(monsterTexture), backgroundTexture(backgroundTexture)
{
}
Monster::Monster(string name, string image, string type, int LV, int EXP, int HP, int MP, int FP, int attack, int defense)
    : GameCharacter(name, "Monster", image, type, LV, EXP, HP, MP, FP, attack, defense)
{
    delete monster;
}
namespace
{
    /* randomly pick a number by given probability */
    stringstream ss;
    mt19937 engine(rand());
    int getRand(vector<long double> p)
    {
        uniform_real_distribution<long double> distribution(0, p.back());

        long double value = distribution(engine);
        int id = (int)(upper_bound(p.begin(), p.end(), value) - p.begin());
        assert(id != p.size());
        return id;
    }
    void showBar(int current, int max)
    {
        const int nstars = 50;
        const int per = current * nstars / max;
        cout << "| " << string(per, '=') << string(nstars - per, ' ') << " | ";
    }
}

/* Setter & Getter */
void Monster::setMonsterTexture(Texture *monsterTexture)
{
    this->monsterTexture = monsterTexture;
}
void Monster::setBackgroundTexture(Texture *backgroundTexture)
{
    this->backgroundTexture = backgroundTexture;
}

/* Virtual function that you need to complete   */
/* In Monster, this function should deal with   */
/* the combat system.                           */
/* return true if the monster is dead. */
bool Monster::triggerEvent(Object *object, RenderWindow *window)
{
    Font font;
    font.loadFromFile("../Fonts/Dosis-Light.ttf");

    background.setTexture(*backgroundTexture);
    background.scale(Vector2f(window->getSize().x / backgroundTexture->getSize().x, window->getSize().y / backgroundTexture->getSize().y));
    monster = new Button(
        730, 300, 430, 480, 1, 60, &font,
        "",
        Color(128, 113, 130, 100), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
    monster->updateTexture(monsterTexture);

    if (object->getTag() == "Player")
    {
        Player *player = dynamic_cast<Player *>(object);

        string statusText = produceStatus();
        string playerStatusText = player->showStatusinFight();

        Button *status = new Button(
            720, 75, 500, 200, 1,
            60, &font, statusText, Color(100, 100, 150, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
        Button *playerStatus = new Button(
            100, 200, 500, 700, 1,
            60, &font, playerStatusText,
            Color(100, 100, 150, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
        Button *description = new Button(
            10, 820, 1900, 100, 1,
            60, &font, statusText,
            Color(0, 0, 0, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));

        Menu *menu = new Menu(
            44.1, 930, 426, 100, 1,
            60, false, &font, window,
            Color(20, 20, 20, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
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
        while (window->isOpen())
        {
            // detect and update
            while (window->pollEvent(sfEvent))
            {
                if ((sfEvent.type == Event::KeyPressed) && (sfEvent.key.code == Keyboard::Escape))
                {
                    window->close();
                }
                switch (sfEvent.type)
                {
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
            if (!gainedFocus)
            {
                sleep(milliseconds(10));
                continue;
            }
            if (!holdEnter && Keyboard::isKeyPressed(Keyboard::Enter))
            {
                if (state == 4)
                {
                    if (playerTakeAction)
                        state = 5;
                    else
                        state = menu->getIsSelected();
                }
                else if (state == 9)
                {
                    break;
                }
                else if (state == 7)
                {
                    if (checkIsDead())
                        state = 6;
                    else
                        state = 5;
                }
                else if (state == 8)
                {
                    if (player->checkIsDead())
                        break;
                    else
                        state = 4;
                }
                holdEnter = 1;
            }
            if (holdEnter && !Keyboard::isKeyPressed(Keyboard::Enter))
            {
                holdEnter = 0;
            }
            switch (state)
            {
            case 0: // ATTACK
                effect = player->handleAttack(window);
                if (effect.getKind() == "NULL")
                {
                    state = 4;
                    continue;
                }

                playerTakeAction = true;

                damageInfo = damageCalculate(effect.getHP(), getDefense());
                if (damageInfo.second < 0)
                    damageInfo.second = 0;

                ss.clear();
                ss << "You caused " << damageInfo.second << " damages!\n";
                info = "";
                while (getline(ss, tmp))
                    info += tmp + "\n";
                info.pop_back();

                description->updateText("Your attack!   " + damageInfo.first + "   " + info + "   " + player->decreaseWeaponDurability(1));

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
                damageInfo = damageCalculate(getAttack(), player->getDefense());
                if (damageInfo.second < 0)
                    damageInfo.second = 0;

                ss.clear();
                ss << getName() << " caused " << damageInfo.second << " damages!\n\n";
                info = "";
                while (getline(ss, tmp))
                    info += tmp + "   ";
                info.pop_back();

                description->updateText(getName() + " attack!   " + damageInfo.first + "   " + info + "   " + player->decreaseArmorDurability(1));

                player->takeDamage(damageInfo.second);
                player->increaseFP(1);
                playerTakeAction = 0;
                state = 8;
                break;

            case 6:
                /* Trophy */
                player->increaseEXP(getMaxEXP());

                tmp = "";
                if (getRand(vector<long double>{40, 100}))
                {
                    if (drop.getKind() == "NULL")
                        drop = Item::randomItemGenerator(player->getLV(), "RANDOM");
                    tmp = "   " + getName() + " drop " + drop.getName();
                    player->addItem(drop);
                }

                description->updateText(getName() + " is dead!   You win!" + tmp);
                state = 9;
                break;
            }
            menu->update();
            status->updateText(produceStatus());
            playerStatus->updateText(player->showStatusinFight());

            window->clear();

            window->draw(background);
            status->render(window);
            playerStatus->render(window);
            monster->render(window);

            if (state == 7 || state == 8)
            {
                if (sound.getStatus() != Sound::Playing)
                    sound.play();
            }

            switch (state)
            {
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

void Monster::input(ifstream &in)
{
    setTag("Monster");
    inputGameCharacter(in);
}

void Monster::output(ofstream &out) const
{
    outputGameCharacter(out);
}

/* Supplement */

pair<string, int> Monster::damageCalculate(int attack, int defense)
{
    int damage = attack * 4 - defense * 2;
    string info;

    int event = getRand(vector<long double>({1, 2, 12, 92})); // 1 1 10 80
    switch (event)
    {
    case 0:
        info = "MISS!";
        damage = 0;
        break;
    case 1:
        info = "Critical Hit!";
        damage *= 2;
        break;
    case 2:
        info = "Defensed!";
        damage *= 0.9;
        break;
    case 3:
        info = "";
        damage *= 1;
        break;
    }

    return pair<string, int>(info, damage);
}

string Monster::produceStatus()
{
    ss.clear();
    ss << getName() << "   Lv. " << setw(2) << getLV() << endl;
    ss << left << setw(12) << "> HP: " << getCurrentHP() << '/' << getMaxHP() << endl;

    string info = "", tmp;
    while (getline(ss, tmp))
        info += tmp + "\n";
    info.pop_back();

    return info;
}