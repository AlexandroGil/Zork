#ifndef PROGRAM
#define PROGRAM

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
using namespace std;

#include "Room.h"
#include "Item.h"
#include "Furniture.h"
#include "Player.h"
#include "Farmer.h"

struct Player;
struct Room;
struct Item;
struct Furniture;
struct Farmer;


class Program
{
public:
    Program();
    ~Program();
    void CreateRoom(string name, string description, string north, string south, string east, string west);
    void CreateItem(string name, string description, string location);
    void CreateFurniture(string name, string description, string location, vector <string> usages);
    bool NormalState(map<string, string> command);
    bool DialogState(map<string, string> command);
    map<string, Room*> rooms;
    map<string, Item*> items;
    map<string, Furniture*> furnitures;

    void Run();
    void Timer();

private:
    bool HandleUserInput(string userInput);

    Player* player;
    Farmer* farmer;
    bool farmer_home;
    int game_state;
    int steps;
    int probability;
    string command;
    bool status;
    bool farmer_encounter;
    map<string, string> map_command;
    bool m_done;
    string m_endRoomId;
};

class StringUtil
{
public:
    static string ToLower(const string& val);
    static string ToUpper(const string& val);
};


#endif