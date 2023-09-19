#ifndef ITEM
#define ITEM

#include <iostream>
#include <string>
using namespace std;
#include "Program.h"

struct Item {
	Item(string name, string description, string location);

	string name;
	string description;
	string location;
};

#endif