#include "Record.h"

void Record::savePlayer(Player* player, ofstream& out) {
    out << (*player) << endl;
}

void Record::loadPlayer(Player* player, ifstream& in) {
    string tag = ""; while (tag == "") getline(in, tag);
    in >> (*player);
    in >> CRIndex >> PRIndex;
}

void Record::saveRooms(vector<Room>& rooms, ofstream& out) {
    out << rooms.size() << endl;
    for (auto room : rooms) {
        out << room << endl;
    }
}

void Record::loadRooms(vector<Room>& rooms, ifstream& in) {
    int n;
    in >> n;
    rooms.resize(n);
    for (int i = 0; i < n; ++i) {
        in >> rooms[i];

        vector<Object*> objs;
        int m;
        in >> m;
        objs.resize(m);
        for (int j = 0; j < m; ++j) {
            string tag = "";
            while (tag == "") getline(in, tag);

            if (tag == "Item")          objs[j] = new Item();
            else if (tag == "Monster")  objs[j] = new Monster();
            else if (tag == "NPC")      objs[j] = new NPC();

            in >> (*objs[j]);
        }
        rooms[i].setObjects(objs);

        int uId, dId, lId, rId;
        in >> uId >> dId >> lId >> rId;
        if (uId + 1) rooms[i].setUpRoom(&rooms[uId]);
        if (dId + 1) rooms[i].setDownRoom(&rooms[dId]);
        if (lId + 1) rooms[i].setLeftRoom(&rooms[lId]);
        if (rId + 1) rooms[i].setRightRoom(&rooms[rId]);
    }
}

void Record::saveVis(vector<bool>& vis, ofstream& out)
{
    out << vis.size() << endl;
    for (auto v : vis) out << v << ' ';
    out << endl;
}

void Record::loadVis(vector<bool>& vis, ifstream& in)
{
    int n, v;
    in >> n;
    vis.resize(n);
    for (int i = 0; i < n; ++i) in >> v, vis[i] = v;
}

void Record::saveCoord(map <coord, int>& coordToIndex, ofstream& out)
{
    out << coordToIndex.size() << endl;
    for (auto& c : coordToIndex) {
        out << c.first.first << ' ' << c.first.second << ' ' << c.second << endl;
    }
    out << endl;
}

void Record::loadCoord(map <coord, int>& coordToIndex, ifstream& in)
{
    int n, X, Y, index;
    in >> n;
    for (int i = 0; i < n; ++i) {
        in >> X >> Y >> index;
        coordToIndex[ coord(X, Y) ] = index;
    }
}

void Record::saveFileNames()
{
    ofstream fout("../Record/file");
    for (int i = 0; i < (int)fileList.size(); ++i)
    {
        fout << i << endl;

        if (fileList[i].first.first == "")
        {
            fout << "EMPTY_USER\n";
            continue;
        }

        fout << fileList[i].first.first << endl;
        fout << fileList[i].first.second << endl;
        fout << fileList[i].second << endl;
    }
    fout.close();
}

void Record::loadFileNames()
{
    ifstream fin("../Record/file");
    int i;
    while (fin >> i)
    {
        string name = ""; while (name == "") getline(fin, name);
        if (name == "EMPTY_USER")
        {
            fileList[i] = pair<pair<string, int>, string>(pair<string, int>("", 0), "");
            continue;
        }

        int LV; fin >> LV;

        string T = ""; while (T == "") getline(fin, T);

        fileList[i] = pair<pair<string, int>, string>(pair<string, int>(name, LV), T);
    }
    fin.close();
}

void Record::listFileNames()
{
    menu = new Menu(20, 150, 1880, 100, 0, 60, true, font, _window, Color(20, 20, 20, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
    char ops = 'A' - 1;
    stringstream ss;
    string name;
    for (auto data : fileList)
    {
        if (data.first.first == "")
        {
            cout << (++ops) << ": " << left << setw(10) << "empty" << endl;
            menu->push_back("empty");
        }
        else {
            cout << (++ops) << ": " << left << setw(10) << data.first.first << "   LV. " << setw(2) << data.first.second << "   " << data.second << endl;
            ss.clear();
            ss << left << setw(10) << data.first.first << "   LV. " << setw(2) << data.first.second << "   " << data.second << endl;
            getline(ss, name);
            menu->push_back( name );
        }
    }
    cout << "\n" << (++ops) << ": Cancel.\n";
    menu->push_back("Cancel");
}

void Record::initFileNames(int number)
{
    ofstream fout("../Record/file");
    for (int i = 0; i < number; ++i)
    {
        fout << i << endl;
        fout << "EMPTY_USER\n";
    }
    fout.close();
}

inline bool exists(const string& name) {
    ifstream fin(name.c_str());
    return fin.good();
}

Record::Record() {}
Record::Record(int number, RenderWindow* _window)
: _window(_window)
{
    font = new Font();
    font->loadFromFile("../fonts/Dosis-Light.ttf");
    if (!exists("../Record/file")) initFileNames(number);
    fileList.resize(number);
    loadFileNames();
}

void Record::saveToFile(Player* player, vector<Room>& rooms, vector<bool>& vis, int& usedIndex, map <coord, int>& coordToIndex) {
    button = new Button(10, 20, 1900, 100, 1, 60, font, "Choose one file to load", Color(0, 0, 0, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
    cout << "Choose one file to save:\n";
    listFileNames();

    holdEnter = 1;
    gainedFocus = 1;
    while (_window->isOpen()) {
        while (_window->pollEvent(sfEvent)) {
            if ((sfEvent.type == Event::KeyPressed) && (sfEvent.key.code == Keyboard::Escape)) {
                _window->close();
            }
            switch (sfEvent.type) {
                case Event::Closed:
                    _window->close();
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
            option = menu->getIsSelected();
            break;
        }
        if (holdEnter && !Keyboard::isKeyPressed(Keyboard::Enter)) {
            holdEnter = 0;
        }
        menu->update();

        _window->clear();
        button->render(_window);
        menu->render(_window);

        _window->display();
    }
    delete menu;
    delete button;

    /* string ops; getline(cin, ops);
    while (ops[0] < 'A' || OPS > fileList.size()) {
        cout << "Error, please enter operation again.\n";
        getline(cin, ops);
    } */
    if (option == fileList.size()) return;

    ofstream fout("../Record/file" + string(1, 'A' + option));

    cout << "Saving...\n";

    auto end = std::chrono::system_clock::now();
    time_t end_time = chrono::system_clock::to_time_t(end);
    fileList[option] = pair<pair<string, int>, string>(pair<string, int>(player->getName(), player->getLV()), ctime(&end_time));

    saveFileNames();
    savePlayer(player, fout);
    saveRooms(rooms, fout);
    saveVis(vis, fout);
    saveCoord(coordToIndex, fout);
    fout << usedIndex << endl;

    cout << "Done.\n";

    fout.close();
}
bool Record::loadFromFile(Player* player, vector<Room>& rooms, vector<bool>& vis, int& usedIndex, map <coord, int>& coordToIndex) {
    button = new Button(10, 20, 1900, 100, 1, 60, font, "Choose one file to load", Color(0, 0, 0, 200), Color(150, 150, 150, 200), Color(70, 70, 70, 255));
    cout << "Choose one file to load:\n";
    listFileNames();

    gainedFocus = 1;
    holdEnter = 1;
    while (_window->isOpen()) {
        while (_window->pollEvent(sfEvent)) {
            if ((sfEvent.type == Event::KeyPressed) && (sfEvent.key.code == Keyboard::Escape)) {
                _window->close();
            }
            switch (sfEvent.type) {
                case Event::Closed:
                    _window->close();
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
            option = menu->getIsSelected();
            if (fileList[option].first.first != "") break;
            holdEnter = 1;
        }
        if (holdEnter && !Keyboard::isKeyPressed(Keyboard::Enter)) {
            holdEnter = 0;
        }
        menu->update();

        _window->clear();
        button->render(_window);
        menu->render(_window);

        _window->display();
    }
    delete menu;
    delete button;

    // string ops; getline(cin, ops);
    // while (ops[0] < 'A' || OPS > fileList.size() || fileList[OPS].first.first == "") {
    //     cout << "Error, please enter operation again.\n";
    //     getline(cin, ops);
    // }
    if (option == fileList.size()) return 0;

    ifstream fin("../Record/file" + string(1, 'A' + option));

    cout << "Loading...\n";

    loadPlayer(player, fin);
    loadRooms(rooms, fin);
    loadVis(vis, fin);
    loadCoord(coordToIndex, fin);
    fin >> usedIndex;

    cout << "Done.\n";

    player->setCurrentRoom(&rooms[CRIndex]);
    player->setPreviousRoom(&rooms[PRIndex]);

    fin.close();
    return 1;
}

void Record::saveToFile(Player* player, vector<Room>& rooms, vector<bool>& vis, int& usedIndex, map <coord, int>& coordToIndex, ofstream& fout) {
    savePlayer(player, fout);
    saveRooms(rooms, fout);
    saveVis(vis, fout);
    saveCoord(coordToIndex, fout);
    fout << usedIndex << endl;
}
