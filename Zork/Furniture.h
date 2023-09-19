#ifndef FURNITURE
#define FURNITURE

#include <iostream>
#include <string>
using namespace std;
#include "Program.h"

struct Furniture {
	Furniture(string name, string description, string location, vector <string> usages);

	string name;
	string description;
	string location;
	vector <string> usages;
};

#endif