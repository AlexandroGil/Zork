#ifndef FARMER
#define FARMER

#include <iostream>
#include <string>
#include <vector>
using namespace std;
#include "Program.h"

struct Room;
struct Item;
struct Furniture;
class Dialog;
struct Player;

struct Farmer {
	~Farmer();
	Farmer(Room* room);
	void Move(map<string, Room*> rooms);
	void Home(Room* rooms);
    void CreateDialog(Player* player);
    bool StartDialog(int choice);

    bool farmer_kill;
    int probability_success;
    bool fake_ending;
    Dialog* current_choice;
    Dialog* root;
    vector<int> dialog_steps;
	Room* current_farmer_room;
};

class Dialog {
public:
    string question;
    vector<string> choices;
    vector<Dialog*> options;
    Dialog(string val_question, vector<string> val_choices)
    {
        question = val_question;
        choices = val_choices;
        for (int i = 0; i < val_choices.size(); i++) {
            options.push_back(NULL);
        }
    }
};

#endif