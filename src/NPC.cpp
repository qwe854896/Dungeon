#include "NPC.h"

NPC::NPC() : GameCharacter(), script("") {}
NPC::NPC(string name, string image, string type, string script, int LV)
    : GameCharacter(name, "NPC", image, type, LV), script(script)
{
}
NPC::NPC(string name, string image, string type, string script, vector<Item> commodity, int LV)
    : NPC(name, image, type, script, LV)
{
    this->commodity = commodity;
}

namespace
{
    stringstream ss;
};

/* Virtual function that you need to complete   */
/* In NPC, this function should deal with the   */
/* transaction in easy implementation           */
bool NPC::triggerEvent(Object *object, RenderWindow *window)
{
    Font font;
    font.loadFromFile("../Fonts/Dosis-Light.ttf");

    background.setTexture(*backgroundTexture);
    background.scale(Vector2f(window->getSize().x / backgroundTexture->getSize().x, window->getSize().y / backgroundTexture->getSize().y));
    npc = new Button(
        730, 300, 430, 480, 1, 60, &font,
        "",
        Color(128, 113, 130, 100), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
    npc->updateTexture(npcTexture);

    if (object->getTag() == "Player")
    {
        Player *player = dynamic_cast<Player *>(object);

        int ops;
        bool gainedFocus = 1;
        bool holdEnter = 1;
        bool bag = 0;

        Event sfEvent;
        Font font;
        font.loadFromFile("../Fonts/Dosis-Light.ttf");

        Button *button = new Button(
            10, 820, 1900, 100, 1,
            60, &font, script,
            Color(0, 0, 0, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
        Button *yourBagIsempty = new Button(
            10, 820, 1900, 100, 1,
            60, &font, "Your bag is empty now!",
            Color(0, 0, 0, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
        Menu *menu = new Menu(
            44.1, 930, 426, 100, 1,
            60, false, &font, window,
            Color(20, 20, 20, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
        menu->push_back("BUY");
        menu->push_back("SELL");
        menu->push_back("TALK");
        menu->push_back("EXIT");

        while (window->isOpen())
        {
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
                holdEnter = 1;
                if (bag)
                {
                    bag = 0;
                    continue;
                }
                ops = menu->getIsSelected();
                switch (ops)
                {
                case 0:
                    while (handleBuy(player, window))
                        ;
                    break;
                case 1:
                    while (handleSell(player, window))
                        ;
                    if (player->getInventory().empty())
                        bag = 1;
                    break;
                case 2:
                    while (handleTalk(window))
                        ;
                    break;
                case 3:
                    return false;
                }
            }
            if (holdEnter && !Keyboard::isKeyPressed(Keyboard::Enter))
            {
                holdEnter = 0;
            }

            menu->update();

            window->clear();
            window->draw(background);
            npc->render(window);

            if (bag)
                yourBagIsempty->render(window);
            else
                button->render(window), menu->render(window);

            window->display();
        }
        delete menu;
        delete button;
        delete yourBagIsempty;
    }

    return false;
}

void NPC::input(ifstream &in)
{
    setTag("NPC");
    inputGameCharacter(in);

    script = "";
    string tmp;
    while (1)
    {
        tmp = "";
        while (tmp == "")
            getline(in, tmp);
        if (tmp == "script end.")
            break;
        tmp += "\n";
        script += tmp;
    }

    int n;
    in >> n;
    commodity.resize(n);
    for (int i = 0; i < n; ++i)
    {
        string tag = "";
        while (tag == "")
            getline(in, tag);
        in >> commodity[i];
    }
}

void NPC::output(ofstream &out) const
{
    outputGameCharacter(out);

    out << script << "\nscript end.\n"
        << endl;

    out << commodity.size() << endl;
    for (auto good : commodity)
    {
        out << good << endl;
    }
}

/* Set & Get function*/
void NPC::setScript(string script)
{
    this->script = script;
}
void NPC::setCommodity(vector<Item> commodity)
{
    this->commodity = commodity;
}
string NPC::getScript() const
{
    return script;
}
vector<Item> NPC::getCommodity() const
{
    return commodity;
}
void NPC::setNPCTexture(Texture *npcTexture)
{
    this->npcTexture = npcTexture;
}
void NPC::setBackgroundTexture(Texture *backgroundTexture)
{
    this->backgroundTexture = backgroundTexture;
}

/* Supplement */

int NPC::listCommodity(RenderWindow *window, int gold) /* print all the Item in this NPC*/
{
    int ops;
    string info, tmp;
    Font font;
    font.loadFromFile("../Fonts/Dosis-Light.ttf");

    bool isError = 0;
    Button *title = new Button(
        10, 20, 1900, 100, 1, 60, &font,
        "Choose one item you want to buy: ",
        Color(0, 0, 0, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
    Button *error = new Button(
        10, 20, 1900, 100, 1, 60, &font,
        "Sorry, you don't have enough money!\nPlease enter operation again.",
        Color(0, 0, 0, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
    Menu *menu = new Menu(
        60, 200, 450, 500,
        0, 60, false, &font, window,
        Color(20, 20, 20, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));

    for (auto &item : commodity)
    {
        ss.clear();
        ss << item.getName() << endl;
        ss << "> type: " << item.getKind() << endl;
        if (item.getHP())
            ss << "> HP: " << item.getHP() << endl;
        if (item.getMP())
            ss << "> MP: " << item.getMP() << endl;
        if (item.getFP())
            ss << "> FP: " << item.getFP() << endl;
        if (item.getAttack())
            ss << "> Attack: " << item.getAttack() << endl;
        if (item.getDefense())
            ss << "> Defense: " << item.getDefense() << endl;
        if (item.getDurability())
            ss << "> Durability: " << item.getDurability() << endl;
        if (item.getPrice())
            ss << "> Price: " << item.getPrice() << endl;

        info = "";
        while (getline(ss, tmp))
            info += tmp + "\n";
        info.pop_back();

        menu->push_back(info);
    }
    menu->push_back("Cancel");

    bool gainedFocus = 1;
    bool holdEnter = 1;

    Event sfEvent;
    while (window->isOpen())
    {
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
            holdEnter = 1;
            if (!isError)
            {
                ops = menu->getIsSelected();
                if (ops == commodity.size() || commodity[ops].getPrice() <= gold)
                    break;
                isError = 1;
            }
            else
                isError = 0;
        }
        if (holdEnter && !Keyboard::isKeyPressed(Keyboard::Enter))
        {
            holdEnter = 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::K))
        {
            cout << Mouse::getPosition().x << ' ' << Mouse::getPosition().y << endl;
        }
        menu->update();

        window->clear();

        menu->render(window);
        if (!isError)
            title->render(window);
        else
            error->render(window);

        window->display();
    }
    delete menu;
    delete title;
    delete error;

    return ops;
}

void NPC::pushCommodity(Item item) /* push an item into commodity */
{
    commodity.emplace_back(item);
}

bool NPC::handleBuy(Player *player, RenderWindow *window)
{
    int ops = listCommodity(window, player->getGold());
    if (ops == commodity.size() || ops == -1)
        return false;

    bool gainedFocus = 1;
    bool holdEnter = 1;
    Event sfEvent;
    Font font;
    font.loadFromFile("../Fonts/Dosis-Light.ttf");
    Button *button = new Button(
        10, 450, 1900, 100, 1, 60, &font,
        "You buy " + commodity[ops].getName(),
        Color(0, 0, 0, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));

    while (window->isOpen())
    {
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
            break;
            holdEnter = 1;
        }
        if (holdEnter && !Keyboard::isKeyPressed(Keyboard::Enter))
        {
            holdEnter = 0;
        }
        window->clear();
        button->render(window);
        window->display();
    }
    delete button;

    player->addItem(commodity[ops]);
    player->decreaseGold(commodity[ops].getPrice());

    return true;
}

bool NPC::handleSell(Player *player, RenderWindow *window)
{
    return player->handleInventory("sell", window);
}

bool NPC::handleTalk(RenderWindow *window)
{
    bool gainedFocus = 1;
    bool holdEnter = 1;
    Event sfEvent;
    Font font;
    font.loadFromFile("../Fonts/Dosis-Light.ttf");
    Button *button = new Button(
        10, 820, 1900, 100, 1, 60, &font,
        "I am shy!",
        Color(0, 0, 0, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
    while (window->isOpen())
    {
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
            break;
            holdEnter = 1;
        }
        if (holdEnter && !Keyboard::isKeyPressed(Keyboard::Enter))
        {
            holdEnter = 0;
        }
        window->clear();
        button->render(window);
        window->display();
    }
    delete button;

    return false;
}
