#ifndef PLAYER
#define PLAYER

#include <iostream>
#include <string>
using namespace std;
#include "Program.h"

struct Room;
struct Item;
struct Furniture;

struct Player {
	~Player();
	Player(string name, Room* room, map<string, Item*> items);
	bool Go(string name, map<string, Room*> rooms);
	bool Take(Item* item);
	void Search(Furniture* furniture, map<string, Item*> items);
	bool Checkitems(Furniture* furniture, map<string, Item*> items);
	void Inventory();
	void Info();

	Room* current_room;
	string temporal_location;
	map<string, Item*> items;
};

#endif