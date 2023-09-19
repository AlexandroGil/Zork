#include "Program.h"

/*
* Constructor that creates all the game objects
*/
Program::Program() {
    status = false;
    m_done = false;

    CreateRoom("MoreForest", "You dont want to go there", "Null", "Null", "Null", "Null");
    CreateRoom("Forest", "A Dark Forest with tall trees", "Porch", "Null", "Garden", "Null");
    CreateRoom("Porch", "Small Porch of the Cabin with one rocking chair", "LivingRoom", "Forest", "Null", "Null");
    CreateRoom("LivingRoom", "Cozy livinroom with a fireplace and a big leather couch", "MainRoom", "Porch", "Garage", "Null");
    CreateRoom("MainRoom", "The beautiful Main room with a bed, a nightstand and a wardrobe", "Null", "LivingRoom", "Null", "BathRoom");
    CreateRoom("BathRoom", "Small Bathroom with a simple shower, sink and a toilet", "Null", "Null", "MainRoom", "Null");
    CreateRoom("Garage", "A big garage with a lot of tools and a big car, percfect to explore the forest", "Null", "Garden", "Null", "LivingRoom");
    CreateRoom("Garden", "A beatiful garden with some tires trails in the middle", "Garage", "Null", "Null", "Forest");

    vector <string> usages = {};
    CreateFurniture("Tree", "A big beautiful pine", "Forest", usages);
    CreateFurniture("Chair", "A simple and old rocking chair", "Porch", usages);
    CreateFurniture("Couch", "A big and confy leather couch", "LivingRoom", usages);
    CreateFurniture("Fireplace", "An old but really pretty fireplace", "LivingRoom", usages);
    CreateFurniture("Bed", "A giant bed", "MainRoom", usages);
    usages = { "Lighter" };
    CreateFurniture("Nightstand", "Old Nightstand, looks like it was made by hand", "MainRoom", usages);
    CreateFurniture("Wardrobe", "Another furniture that was made by hand", "MainRoom", usages);
    usages.clear();
    CreateFurniture("Shower", "Simple old rusty shower", "BathRoom", usages);
    CreateFurniture("Sink", "Simple sink", "BathRoom", usages);
    CreateFurniture("Toilet", "Simple Toilet", "BathRoom", usages);
    usages = { "Keys" };
    CreateFurniture("Car", "An old but big and powerful truck", "Garage", usages);

    CreateItem("Keys", "Looks like some old car keys", "Sink");
    CreateItem("Axe", "A pretty used axe", "Garage");
    CreateItem("Wood", "Some leftover wood sticks", "Tree");
    CreateItem("Lighter", "A simple lighter", "Nightstand");
    CreateItem("Picture", "A pretty old photo of a couple", "Couch");
    CreateItem("Ring", "Looks like a weeding ring", "Fireplace");

    game_state = 0;
    farmer_home = false;
    Room* current_room = rooms.find("Forest")->second;
    map<string, Item*> player_items;
    player = new Player("Hero", current_room, player_items);
    Room* current_farmer_room = rooms.find("MoreForest")->second;
    farmer = new Farmer(current_farmer_room);
}

Program::~Program() {
}

/*
* Main Run Class
*/
void Program::Run()
{
    cout << "Lost in the Forest" << endl;
    cout << "You are lost in a scary forest late at night, but with your \ncompass heading north you can see a small cavin at the \ndistance and you decide to approach the cavin\n" << endl;
    cout << player->current_room->name << endl;
    cout << player->current_room->description << endl;
    while (!m_done)
    {
        cout << "\n>";
        getline(cin, command);
        status = HandleUserInput(command);
    }

    cout << endl << "THE END!" << endl;
}

/*
* Simple "Timer"to move the farmer
*/
void Program::Timer()
{
    if (!farmer->fake_ending) {
        if (!farmer_home) {
            int val = steps * 5;
            probability = rand() % 100;
            if (probability < val) {
                cout << "\nYou hear some footsteps at the distance" << endl;
                Room* current_farmer_room = rooms.find("Forest")->second;
                farmer->Home(current_farmer_room);
                farmer_home = true;
            }
            steps++;
        }
        else {
            map<string, Room*> posible_rooms;
            if (farmer->current_farmer_room->north != "Null") {
                posible_rooms.insert({ farmer->current_farmer_room->north, rooms.find(farmer->current_farmer_room->north)->second});
            }
            if (farmer->current_farmer_room->south != "Null") {
                posible_rooms.insert({ farmer->current_farmer_room->south, rooms.find(farmer->current_farmer_room->south)->second });
            }
            if (farmer->current_farmer_room->east != "Null") {
                posible_rooms.insert({ farmer->current_farmer_room->east, rooms.find(farmer->current_farmer_room->east)->second });
            }
            if (farmer->current_farmer_room->west != "Null") {
                posible_rooms.insert({ farmer->current_farmer_room->west, rooms.find(farmer->current_farmer_room->west)->second });
            }
            farmer->Move(posible_rooms);
        }
        if ((farmer->current_farmer_room == player->current_room) && !farmer_encounter) {
            cout << "\nYou see an old man with dirty clothes and with a gun pointing at you!" << endl;
            farmer_encounter = true;
            farmer->CreateDialog(player);
            if (player->current_room->name == "Forest" || player->current_room->name == "Garden" || player->current_room->name == "Porch") {
                farmer->StartDialog(1);
            }
            else {
                farmer->StartDialog(0);
            }
            game_state = 1;
        }
    }
}

void Program::CreateRoom(string name, string description, string north, string south, string east, string west)
{
    Room* room = new Room(name, description, north, south, east, west);
    rooms.insert({ name, room });
}

void Program::CreateItem(string name, string description, string location)
{
    Item* item = new Item(name, description, location);
    items.insert({ StringUtil::ToLower(name), item });
}

void Program::CreateFurniture(string name, string description, string location, vector <string> usages)
{
    Furniture* furniture = new Furniture(name, description, location, usages);
    furnitures.insert({ StringUtil::ToLower(name), furniture });
}

/*
* Lower cases of all latters in a word
*/
string StringUtil::ToLower(const string& val)
{
    string lower = "";
    for (unsigned int i = 0; i < val.size(); i++)
    {
        lower += tolower(val[i]);
    }
    return lower;
}

/*
* Upper the first case of a word
*/
string StringUtil::ToUpper(const string& val)
{
    string upper = val;
    if (val.length() != 0) {
        upper[0] = toupper(val[0]);
    }

    return upper;
}

/*
* Handle the user input on console
*/
bool Program::HandleUserInput(string userInput) {
    map_command.clear();
    bool status = false;
    vector<string> arr;
    string command = "";
    userInput.push_back('.');
    for (auto x : StringUtil::ToLower(userInput))
    {
        if (x == ' ' || x == '.') {
            arr.push_back(command);
            command = "";
        }
        else {
            command = command + x;
        }
    }

    switch (arr.size()) {
    case 1:
        map_command.insert({ arr[0], " " });
        break;
    case 2:
        map_command.insert({ arr[0], arr[1] });
        break;
    }
    switch (game_state) {
        case 0:
            status = NormalState(map_command);
            break;
        case 1:
            status = DialogState(map_command);
            break;
    }
    
    return status;
}

/*
* Normal explore the map state
*/
bool Program::NormalState(map<string, string> command) {
    for (auto itr = command.begin(); itr != command.end(); ++itr) {
        if (itr->first == "go") {
            bool can_move = false;
            if (itr->second == "north") {
                can_move = player->Go("north", rooms);
            }
            else if (itr->second == "south") {
                can_move = player->Go("south", rooms);
            }
            else if (itr->second == "west") {
                can_move = player->Go("west", rooms);
            }
            else if (itr->second == "east") {
                can_move = player->Go("east", rooms);
            }
            else {
                cout << "\nWhere do you want to go?" << endl;
            }
            if (can_move) {
                cout << endl;
                cout << player->current_room->name << endl;
                cout << player->current_room->description << endl;
                Timer();
            }
            else {
                cout << "\nCant go there!" << endl;
            }
            return true;
        }
        else if (itr->first == "info") {
            player->Info(furnitures, items);
            return true;
        }
        else if (itr->first == "inventory") {
            player->Inventory();
        }
        if (itr->first == "take") {
            if (items.find(itr->second) != items.end()) {
                bool find_item = player->Take(items.find(itr->second)->second);
                if (find_item) {
                    items.find(itr->second)->second->location = "Player";
                }
                else {
                    cout << "Cant find item in Current Room" << endl;
                }
            }
            else {
                cout << "Cant find item" << endl;
            }
            return true;
        }
        if (itr->first == "search") {
            if (furnitures.find(itr->second) != furnitures.end()) {
                player->Search(furnitures.find(itr->second)->second, items);
            }
            else {
                cout << "Cant find object" << endl;
            }
            Timer();
            return true;
        }
        if (itr->first == "wait") {
            cout << "\nYou decide to wait some time" << endl;
            Timer();
            return true;
        }
        if (itr->first == "quit") {
            m_done = true;
        }
        if (itr->first == "drop") {
            if (player->items.find(StringUtil::ToUpper(itr->second)) != player->items.end()) {
                string new_location = player->Drop(player->items.find(StringUtil::ToUpper(itr->second))->second);
                cout << "You are droping this in " << new_location << endl;
                if (items.find(itr->second) != items.end()) {
                    items.find(itr->second)->second->location = new_location;
                }
            }
            else {
                cout << "You dont have that item" << endl;
            }
            return true;
        }      
        if (itr->first == "use") {
            if (player->items.find(StringUtil::ToUpper(itr->second)) != player->items.end()) {
                if (furnitures.find(StringUtil::ToLower(player->temporal_location)) != furnitures.end()) {
                    int can_use = player->Use(player->items.find(StringUtil::ToUpper(itr->second))->second, furnitures.find(StringUtil::ToLower(player->temporal_location))->second);
                    switch (can_use) {
                        case 0:
                            cout << "Cant use this item here!" << endl;
                            break;
                        case 1:
                            cout << "You just want to watch the world Burn (MEME ENDING)" << endl;
                            m_done = true;
                            break;
                        case 2:
                            cout << "You start the car and leave that pleace" << endl;
                            if (farmer->fake_ending) {
                                cout << "but you commit a crime (BAD ENDING)" << endl;
                            }
                            else {
                                cout << "no one was harm (GOOD ENDING)" << endl;
                            }
                            m_done = true;
                            break;
                    }
                }
                else {
                    cout << "Cant use this item here!" << endl;
                }
            }
            else {
                cout << "You dont have that item" << endl;
            }
            return true;
        }
        else {
            cout << "Thats not a command!" << endl;
            return false;
        }
        return false;
    }
}

/*
* Dialogue state when the player encounters the farmer
*/
bool Program::DialogState(map<string, string> command) {
    for (auto itr = command.begin(); itr != command.end(); ++itr) {
        for (int i = 0; i <= farmer->current_choice->choices.size(); i++) {
            string command_str = to_string(i);
            if (itr->first == command_str) {
                if (i < farmer->current_choice->choices.size()) {
                    bool ending = farmer->StartDialog(i);
                    if (farmer->fake_ending) {
                        game_state = 0;
                        break;
                    }
                    m_done = !ending;
                }
                else {
                    cout << "No choice selected!" << endl;
                }
            }
        }
    }
    return true;
}