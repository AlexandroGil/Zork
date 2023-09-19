#include "Player.h"

Player::Player(string name, Room* room, map<string, Item*> items)
{
    this->current_room = room;
    this->items = items;
}

Player::~Player() {

}

bool Player::Go(string direction, map<string, Room*> rooms)
{
    if (direction == "north") {
        if (rooms.find(current_room->north) != rooms.end()) {
            cout << "Going to: " << current_room->north << endl;
            current_room = rooms.find(current_room->north)->second;
            temporal_location = "";
            return true;
        }
        else {
            cout << "Cant go there!" << endl;
            return false;
        }
    }
    else if (direction == "south") {
        if (rooms.find(current_room->south) != rooms.end()) {
            cout << "Going to: " << current_room->south << endl;
            current_room = rooms.find(current_room->south)->second;
            temporal_location = "";
            return true;
        }
        else {
            cout << "Cant go there!" << endl;
            return false;
        }
    }
    else if (direction == "west") {
        if (rooms.find(current_room->west) != rooms.end()) {
            cout << "Going to: " << current_room->west << endl;
            current_room = rooms.find(current_room->west)->second;
            temporal_location = "";
            return true;
        }
        else {
            cout << "Cant go there!" << endl;
            return false;
        }
    }
    else if (direction == "east") {
        if (rooms.find(current_room->east) != rooms.end()) {
            cout << "Going to: " << current_room->east << endl;
            current_room = rooms.find(current_room->east)->second;
            temporal_location = "";
            return true;
        }
        else {
            cout << "Cant go there!" << endl;
            return false;
        }
    }
}

void Player::Info()
{
    cout << "You are in " << current_room->name << endl;
    if (temporal_location != "") {
        cout << "and searching inside " << temporal_location << endl;
    }
}

void Player::Inventory()
{
    cout << "You have: " << endl;
    for(auto itr = items.begin(); itr != items.end(); ++itr)
    {
        cout << "- " << itr->first << endl;
    }
}

bool Player::Take(Item* item)
{
    cout << "Temporal in: " << temporal_location << endl;

    if (item->location == current_room->name || item->location == temporal_location) {
        cout << "Taking " << item->name << " from " << item->location << endl;
        items.insert({ item->name, item });
        temporal_location = "";
        return true;
    }
    else {
        return false;
    }
}

void Player::Search(Furniture* furniture, map<string, Item*> items)
{
    if (furniture->location == current_room->name) {
        cout << "Searching " << temporal_location << endl;
        bool check_items = Checkitems(furniture, items);
        if (check_items) {
            temporal_location = furniture->name;
        }
        else{
            cout << "Nothing found in " << furniture->name << endl;
            temporal_location = "";
        }
    }
    else {
        cout << "No object found" << endl;
    }
}

bool Player::Checkitems(Furniture* furniture, map<string, Item*> items) {
    for (auto itr = items.begin(); itr != items.end(); ++itr) {
        if (itr->second->location == furniture->name) {
            cout << furniture->name << " has " << itr->second->name << endl;
            return true;
        }
    }
    return false;
}