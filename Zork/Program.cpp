#include "Program.h"

Program::Program() {
    status = false;
    m_done = false;

    CreateRoom("MoreForest", "A Dark Forest with tall trees", "Null", "Null", "Null", "Null");
    CreateRoom("Forest", "A Dark Forest with tall trees", "Porch", "Null", "Garden", "Null");
    CreateRoom("Porch", "Small Porch of the  with one rocking chair", "LivingRoom", "Forest", "Null", "Null");
    CreateRoom("LivingRoom", "Cozy  livinroom with an unlit fireplace and a big leather couch", "MainRoom", "Porch", "Garage", "Null");
    CreateRoom("MainRoom", "The Main room with a bed, a nightstand and a wardrobe", "Null", "LivingRoom", "Null", "BathRoom");
    CreateRoom("BathRoom", "Small Bathroom with a simple shower, sink, toilet and a trash can", "Null", "Null", "MainRoom", "Null");
    CreateRoom("Garage", "Cozy  livinroom with an unlit fireplace and a big leather couche", "Null", "Garden", "Null", "LivingRoom");
    CreateRoom("Garden", "Cozy  livinroom with an unlit fireplace and a big leather couche", "Garage", "Null", "Null", "Forest");

    CreateFurniture("Tree", "A big and confy leather couch", "Forest");
    CreateFurniture("Chair", "A big and confy leather couch", "Porch");
    CreateFurniture("Couch", "A big and confy leather couch", "LivingRoom");
    CreateFurniture("Fireplace", "An old but really pretty fireplace, maybe you can light it", "LivingRoom");
    CreateFurniture("Bed", "An old but really pretty fireplace, maybe you can light it", "MainRoom");
    CreateFurniture("Nightstand", "An old but really pretty fireplace, maybe you can light it", "MainRoom");
    CreateFurniture("Wardrobe", "An old but really pretty fireplace, maybe you can light it", "MainRoom");
    CreateFurniture("Shower", "An old but really pretty fireplace, maybe you can light it", "BathRoom");
    CreateFurniture("Sink", "An old but really pretty fireplace, maybe you can light it", "BathRoom");
    CreateFurniture("Toilet", "An old but really pretty fireplace, maybe you can light it", "BathRoom");
    CreateFurniture("Car", "An old but really pretty fireplace, maybe you can light it", "Garage");

    CreateItem("keys", "Looks like some old car keys", "Sink");
    CreateItem("axe", "Looks like some old car keys", "Garage");
    CreateItem("wood", "Looks like some old car keys", "Tree");
    CreateItem("lighter", "Looks like some old car keys", "Nightstand");
    CreateItem("picture", "Looks like some old car keys", "Couch");
    CreateItem("ring", "Looks like some old car keys", "Fireplace");

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


void Program::Run()
{
    cout << "Lost in the Forest" << endl;
    cout << "You and your partner are lost in a scary forest late at night, but with your compass heading north you can see a small cavin at the distance and you both decide to approach the cavin" << endl;
    while (!m_done)
    {
        cout << "\n>";
        getline(cin, command);
        status = HandleUserInput(command);
    }

    cout << endl << "The end." << endl;
}

void Program::Timer()
{
    if (!farmer->fake_ending) {
        if (!farmer_home) {
            int val = steps * 5;
            probability = rand() % 100;
            if (probability < val) {
                cout << "You hear some footsteps at the distance" << endl;
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
        cout << "Your Room is: " << player->current_room->name << " Farmer current Room is: " << farmer->current_farmer_room->name << endl;
        if ((farmer->current_farmer_room == player->current_room) && !farmer_encounter) {
            cout << "You see an old man with dirty clothes and with a gun pointing at you!" << endl;
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

void Program::CreateFurniture(string name, string description, string location)
{
    Furniture* furniture = new Furniture(name, description, location);
    furnitures.insert({ StringUtil::ToLower(name), furniture });
}

string StringUtil::ToLower(const string& val)
{
    string upper = "";
    for (unsigned int i = 0; i < val.size(); i++)
    {
        upper += tolower(val[i]);
    }
    return upper;
}


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
                cout << "Where do you want to go?" << endl;
            }
            if (can_move) {
                Timer();
            }
            else {
                cout << "Cant go there!" << endl;
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
            cout << "You decide to wait some time" << endl;
            Timer();
            return true;
        }
        if (itr->first == "quit") {
            m_done = true;
        }
        if (itr->first == "drop") {
            if (player->items.find(itr->second) != player->items.end()) {
                string new_location = player->Drop(player->items.find(itr->second)->second);
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
        return false;
    }
}

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