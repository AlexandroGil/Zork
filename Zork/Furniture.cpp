#include "Furniture.h"

Furniture::Furniture(string name, string description, string location, vector <string> usages)
{
    this->name = name;
    this->description = description;
    this->location = location;
    this->usages = usages;
}
