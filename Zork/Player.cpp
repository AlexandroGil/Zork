#include "Player.h"

Player::Player(string name, Room* room, map<string, Item*> items)
{
    this->current_room = room;
    this->items = items;
}

Player::~Player() {
}

/*
* Player movement function
*
* @params string direction direction to get
* @params map<string, Room*> rooms posible rooms in that direction
* @return bool in the player can or cant advance
*/
bool Player::Go(string direction, map<string, Room*> rooms)
{
    if (direction == "north") {
        if (rooms.find(current_room->north) != rooms.end()) {
            current_room = rooms.find(current_room->north)->second;
            temporal_location = "";
            return true;
        }
        else {
            return false;
        }
    }
    else if (direction == "south") {
        if (rooms.find(current_room->south) != rooms.end()) {
            current_room = rooms.find(current_room->south)->second;
            temporal_location = "";
            return true;
        }
        else {
            return false;
        }
    }
    else if (direction == "west") {
        if (rooms.find(current_room->west) != rooms.end()) {
            current_room = rooms.find(current_room->west)->second;
            temporal_location = "";
            return true;
        }
        else {
            return false;
        }
    }
    else if (direction == "east") {
        if (rooms.find(current_room->east) != rooms.end()) {
            current_room = rooms.find(current_room->east)->second;
            temporal_location = "";
            return true;
        }
        else {
            return false;
        }
    }
}

/*
* Player info function, to get info about the current room he is in.
*
* @params map<string, Furniture*> furnitures 
* @params map<string, Item*> items
*/
void Player::Info(map<string, Furniture*> furnitures, map<string, Item*> items)
{
    vector<Furniture*> furniture_room = { };
    vector<Item*> items_room = { };
    for (auto itr = furnitures.begin(); itr != furnitures.end(); ++itr) {
        if (itr->second->location == current_room->name) {
                furniture_room.push_back(itr->second);
        }
    }
    if (furniture_room.size() > 0) {
        for (int i = 0; i < furniture_room.size(); i++) {
            for (auto itr_s = items.begin(); itr_s != items.end(); ++itr_s) {
                if (itr_s->second->location == furniture_room[i]->name) {
                    items_room.push_back(itr_s->second);
                }
            }
        }
    }

    for (auto itr_s = items.begin(); itr_s != items.end(); ++itr_s) {
        if (itr_s->second->location == current_room->name) {
            items_room.push_back(itr_s->second);
        }
    }

    cout << "\nYou are in " << current_room->name << endl;
    cout << current_room->description << endl;
    if (temporal_location != "") {
        cout << "and searching inside " << temporal_location << endl;
    }
    if (items_room.size() > 0) {
        cout << "Objects in this room: ";
        for (int i = 0; i < items_room.size(); i++) {
            cout << items_room[i]->name << " ";
        }
        cout << endl;
    }
}

/*
* Player inventory function, to get all items the player have
*
*/
void Player::Inventory()
{
    cout << "\nYou have: " << endl;
    for(auto itr = items.begin(); itr != items.end(); ++itr)
    {
        cout << "- " << itr->first << endl;
    }
}


/*
* Player take function, to take object from the grounf or furniture
*
* @params Item* item the item to take
* @return bool to successful take item
*/
bool Player::Take(Item* item)
{
    if (item->location == current_room->name || item->location == temporal_location) {
        cout << "\nTaking " << item->name << " from " << item->location << endl;
        cout << item->description << endl;
        items.insert({ item->name, item });
        temporal_location = "";
        return true;
    }
    else {
        return false;
    }
}

/*
* Player use function, to use an item inside a furniture
*
* @params Item* item to use
* @params Furniture* furniture in which the player is going to use the item
* @return int value of the possible reaction of the action
*/
int Player::Use(Item* item, Furniture* furniture)
{
    if (furniture->usages.size() > 0) {
        for (int i = 0; i < furniture->usages.size(); i++) {
            if (furniture->usages[i] == item->name) {
                if (item->name == "Lighter") {
                    return 1;
                }
                if (item->name == "Keys") {
                    return 2;
                }
            }
        }
    }
    return 0;
}

/*
* Player drop function, to drop items throught the map
*
* @params Item* item to use
* @return string of the new location of the item
*/
string Player::Drop(Item* item)
{
    items.erase(item->name);
    if (temporal_location != "") {
        return temporal_location;
    }
    else {
        return current_room->name;
    }
}

/*
* Player search function, to search inside furniture
*
* @params Furniture* furniture to search on
* @params map<string, Item*> items all the items to filter with furniture
*/
void Player::Search(Furniture* furniture, map<string, Item*> items)
{
    if (furniture->location == current_room->name) {
        cout << "\nSearching " << temporal_location << endl;
        bool check_items = Checkitems(furniture, items);
        if (!check_items) {
            cout << "Nothing found in " << furniture->name << endl;
        }
        temporal_location = furniture->name;
    }
    else {
        cout << "No object found" << endl;
    }
}

/*
* Player search function, to search inside furniture
*
* @params Furniture* furniture to search on
* @params map<string, Item*> items all the items to filter with furniture
* @return bool item is inside furniture
*/
bool Player::Checkitems(Furniture* furniture, map<string, Item*> items) {
    for (auto itr = items.begin(); itr != items.end(); ++itr) {
        if (itr->second->location == furniture->name) {
            cout << furniture->name << " has " << itr->second->name << endl;
            return true;
        }
    }
    return false;
}