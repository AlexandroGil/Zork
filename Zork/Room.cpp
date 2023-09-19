#include "Room.h"

Room::Room(string name, string description, string north, string south, string east, string west)
{
    this->name = name;
    this->description = description;
    this->north = north;
    this->south = south;
    this->east = east;
    this->west = west;
}