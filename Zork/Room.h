#ifndef ROOM
#define ROOM

#include <iostream>
#include <string>
using namespace std;
#include "Program.h"

struct Room {
	Room(string name, string description, string north, string south, string east, string west);
	string name;
	string description;
	string north;
	string south;
	string east;
	string west;

};

#endif