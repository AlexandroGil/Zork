#include "Farmer.h"

Farmer::Farmer(Room* room)
{
    this->current_farmer_room = room;
    current_choice = NULL;
    root = NULL;
    fake_ending = false;
    farmer_kill = false;
    probability_success;
    srand(time(0));
}

Farmer::~Farmer() {
}

/*
* This function moves the farmer randomly around the cabin
* 
* @params map<string, Room*> rooms amp of all rooms
*/
void Farmer::Move(map<string, Room*> rooms)
{
    int move_prob = rand() % 10;
    if (move_prob < 5) {
        if (rooms.size()) {
            int move_room_prob = rand() % rooms.size();
            for (auto itr = rooms.begin(); itr != rooms.end(); ++itr) {
                if (move_room_prob == distance(rooms.begin(), itr)) {
                    current_farmer_room = itr->second;
                }
            }
        }
    }
}

/*
* Sets the current farmer Room
*
* @params Room* room current room farmer is in
*/
void Farmer::Home(Room* room)
{
    current_farmer_room = room;
}

/*
* Create the full set of posible dialogs like a "Binary tree" with multiple options
* , better implementation with .json next
*
* @params Player* player player data to set multiple options
*/
void Farmer::CreateDialog(Player* player)
{
    dialog_steps.push_back(0);
    vector<string> choices = { "Bad Way", "Good Way"};
    root = new Dialog("", choices);
    current_choice = root;
    vector<Item*> items;
    for (auto itr = player->items.begin(); itr != player->items.end(); ++itr) {
        items.push_back(itr->second);
    }

    //Bad Way - First Node
    choices = { "Sorry, i thought no one lived here", "Im lost, i need help", "Keeping my own business" };
    root->options[0] = new Dialog("WHO ARE YOU? and What are you doing in my House? *Farmer point a gun at you*", choices);

    //Bad Way - First Node - 3 choices
    choices = { "Please, i can give you anything!", "*You leave the cabin*", "*Trys to grab the farmer gun*" };
    root->options[0]->options[0] = new Dialog("I dont care, get out of my house!", choices);

    choices = { "Because, i thought the cabin was abandoned", "I didnt think about that", "*Dont respond*" };
    root->options[0]->options[1] = new Dialog("Ok, but why didnt you wait on the porch?", choices);

    choices = { "I dont care", "*Trys to grab the farmer gun*", "*Dont respond*" };
    root->options[0]->options[2] = new Dialog("I dont like your attitude", choices);


    //Good Way - First Node
    choices = { "Hello!, im lost in the forest!", "Do you live here?", "Get away from me!" };
    root->options[1] = new Dialog("Hello?, How can i help you?", choices);

    //Good Way - First Node - 3 choices

    choices = { "Can you help me get back to the city?", "Yes i lost my track, and with this dark night it was impossible not to get lost", "You dont say Sherlock!" };
    root->options[1]->options[0] = new Dialog("Yes its pretty easy to get lost in this forest at night", choices);

    choices = { "You have a pretty home", "Can i spend the night here and leave tomorrow morning to be safe?", "*dont say anything*" };
    root->options[1]->options[1] = new Dialog("This is my home", choices);

    choices.clear();
    root->options[1]->options[2] = new Dialog("Ok, wait here on the cold night then! (BAD ENDING)", choices);

    //Good Way - First Node - First Node
    choices.clear();
    root->options[1]->options[0]->options[0] = new Dialog("Sure!, you can spend the night here and tomorrow morning i can drop you in the city (GOOD ENDING)", choices);

    choices.clear();
    root->options[1]->options[0]->options[1] = new Dialog("Maybe you want to spent the night here and tomorrow i can take you to the city? (GOOD ENDING)", choices);

    root->options[1]->options[0]->options[2] = root->options[0]->options[2];

    //Good Way - First Node - Second Node
    choices = { "Yes", "No" };
    root->options[1]->options[1]->options[0] = new Dialog("Thank you! you want to pass inside?", choices);

    choices.clear();
    root->options[1]->options[1]->options[1] = new Dialog("Sure! (GOOD ENDING)", choices);

    root->options[1]->options[1]->options[2] = root->options[1]->options[0]->options[1];


    //Good Way - First Node - Second Node - First Node
    choices.clear();
    root->options[1]->options[1]->options[0]->options[0] = new Dialog("You get inside the House and spend a great night, next day you return to the city (GOOD ENDING)", choices);

    root->options[1]->options[1]->options[0]->options[1] = root->options[1]->options[2];

    //Bad Way - First Node - First Node - 3 choices

    choices.clear();
    string choice_1 = "<Locked>";
    string choice_2 = "<Locked>";
    if (player->items.find("Ring") != player->items.end()) {
        choice_1 = "Ring";
    }
    if (player->items.find("Picture") != player->items.end()) {
        choice_2 = "Picture";
    }
    choices = { choice_1, choice_2, "*Give him nothing*" };
    root->options[0]->options[0]->options[0] = new Dialog("What can you give me?", choices);
    root->options[0]->options[0]->options[0]->options[0] = root->options[0]->options[0]->options[0];
    root->options[0]->options[0]->options[0]->options[1] = root->options[0]->options[0]->options[0];
    if (player->items.find("Ring") != player->items.end()) {
        choices.clear();
        root->options[0]->options[0]->options[0]->options[0] = new Dialog("YOU FOUND MY WEDDING RING?, thank you so much, i will take you to the city! (GOOD ENDING)", choices);
    }
    if (player->items.find("Picture") != player->items.end()) {
        root->options[0]->options[0]->options[0]->options[1] = root->options[1];
    }

    root->options[0]->options[0]->options[0]->options[2] = root->options[0]->options[2];

    choices.clear();
    root->options[0]->options[0]->options[1] = new Dialog("Now yo have nowhere to go! (BAD ENDING)", choices);

    choices = { "Yes", "No" };
    root->options[0]->options[0]->options[2] = new Dialog("Are you sure you want to try that? Chance: " + to_string(50) + "%", choices);


    //Bad Way - First Node - First Node - Third Node - 2 choices
    probability_success = rand() % 100;
    if (probability_success < 50) {
        choices.clear();
        root->options[0]->options[0]->options[2]->options[0] = new Dialog("*You fail to take the Farmer gun and he shots you* (BAD ENDING)", choices);
    } 
    else {
        farmer_kill = true;
        choices = { "Kill the farmer?", "Let the farmer live?"};
        root->options[0]->options[0]->options[2]->options[0] = new Dialog("You succeed and take the farmers gun", choices);
        //Bad Way - First Node - First Node - Third Node - First Node(chance)

        choices.clear();
        root->options[0]->options[0]->options[2]->options[0]->options[0] = new Dialog("You kill the farmer, you can continue searching the cabin", choices);

        choices.clear();
        root->options[0]->options[0]->options[2]->options[0]->options[1] = new Dialog("You let the Farmer live, he will help you go back to the City (GOOD ENDING)", choices);
    }

    root->options[0]->options[0]->options[2]->options[1] = root->options[0]->options[0];


    //Bad Way - First Node - Second Node - 3 choices
    root->options[0]->options[1]->options[0] = root->options[1]->options[1];

    choices = { "Yes", "*Make weird noises*", "*Dont respond*" };
    root->options[0]->options[1]->options[1] = new Dialog("You are kinda slow, arent you?", choices);

    root->options[0]->options[1]->options[2] = root->options[0]->options[2];

    //Bad Way - First Node - Second Node - Second Node - 3 choices
    root->options[0]->options[1]->options[0] = root->options[0]->options[0];

    choices.clear();
    root->options[0]->options[1]->options[1] = new Dialog("The Farmer feels sorry for you and take you to the nearest Mental Hospital (WEIRD ENDING)", choices);

    root->options[0]->options[1]->options[2] = root->options[0]->options[2];

    //Bad Way - First Node - Third Node - 3 choices

    choices.clear();
    root->options[0]->options[2]->options[0] = new Dialog("The Farmer gets angry at you and shoots you! (BAD ENDING)", choices);

    root->options[0]->options[2]->options[1] = root->options[0]->options[0]->options[2];

    choices.clear();
    root->options[0]->options[2]->options[2] = root->options[1]->options[1];

}

/*
* Based on the dialgues created, evaluates the multiple options
*
* @params int choice the choice the player choose
* @return bool in case dialogue succeed
*/
bool Farmer::StartDialog(int choice) {
    for (int i = 0; i < dialog_steps.size(); i++) {
        if (i == dialog_steps.size()-1) {
            current_choice = current_choice->options[choice];
        }
        else {
            current_choice = current_choice->options[i];
        }
    }
    cout << "\n" << current_choice->question << endl;
    if (farmer_kill) {
        if (current_choice->question == root->options[0]->options[0]->options[2]->options[0]->options[0]->question) {
            fake_ending = true;
        }
    }
    if (current_choice->choices.size() > 0) {
        for (int i = 0; i < current_choice->choices.size(); i++) {
            cout << i << ". " << current_choice->choices[i] << endl;
        }
        return true;
    }
    else {
        if (!fake_ending) {
            return false;
        }
        else {
            return true;
        }

    }
}
