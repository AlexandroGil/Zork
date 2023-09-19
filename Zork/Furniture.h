#ifndef FURNITURE
#define FURNITURE

#include <iostream>
#include <string>
using namespace std;
#include "Program.h"

struct Furniture {
	Furniture(string name, string description, string location);

	string name;
	string description;
	string location;
};

#endif