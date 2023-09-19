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

void Farmer::Home(Room* room)
{
    current_farmer_room = room;
}

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

    choices = { "Hello, im lost in the forest", "Do you live here?", "Get away from me" };
    root->options[1] = new Dialog("Hello?, How can i help you?", choices);

    choices = { "Can you help get back to the city?", "Yes i lost my track and with the night it was impossible not to get lost", "*You dont say Sherlock!*" };
    root->options[1]->options[0] = new Dialog("yes its pretty easy to get lost in this forest at night", choices);

    choices = { "You have a pretty home", "Can i spend the night here and leave tomorrow morning to be safe?", "*dont say anything*" };
    root->options[1]->options[1] = new Dialog("This is my home", choices);

    choices.clear();
    root->options[1]->options[2] = new Dialog("Ok, wait here on the cold night then! (BAD ENDING)", choices);

    //Bad Way - first Node
    choices = { "Sorry, i though no one lived here", "Im lost in the forest, i need help", "Keeping my own business" };
    root->options[0] = new Dialog("WHO ARE YOU? and What are you doing inside my House? *Farmer point you with a gun*", choices);

    //Bad Way - first Node - 3 choices
    choices = { "Please, i can give you anything!", "*you leave the cabin*", "*Trys to grab the farmer gun*" };
    root->options[0]->options[0] = new Dialog("i dont care, get out of my house", choices);

    choices = { "because, i thought the cabin was abandoned", "i didnt think about that", "*dont respond*" };
    root->options[0]->options[1] = new Dialog("Ok, but why didnt you wait on the porch?", choices);

    choices = { "i dont care", "*trys to grab the farmer gun*", "*dont respond*" };
    root->options[0]->options[2] = new Dialog("i dont like your attitude", choices);

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
        root->options[0]->options[0]->options[0]->options[0] = new Dialog("YOU FOUND MY WEDDING RING?, thank you so much, i will take you to the city!", choices);
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
    cout << "Probability of success is: " << probability_success << endl;
    if (probability_success < 50) {
        choices.clear();
        root->options[0]->options[0]->options[2]->options[0] = new Dialog("*You fail to take the Farmer gun and he shots you (BAD ENDING)*", choices);
    } 
    else {
        farmer_kill = true;
        choices = { "Kill the farmer?", "Let the farmer live?"};
        root->options[0]->options[0]->options[2]->options[0] = new Dialog("You succed and take the farmers gun", choices);
        //Bad Way - First Node - First Node - Third Node - First Node(chance)

        choices.clear();
        root->options[0]->options[0]->options[2]->options[0]->options[0] = new Dialog("You kill the farmer, you can continue searching the cabin", choices);

        choices.clear();
        root->options[0]->options[0]->options[2]->options[0]->options[1] = new Dialog("You let the Farmer live, he will help you go back to the City", choices);
    }

    root->options[0]->options[0]->options[2]->options[1] = root->options[0]->options[0];


    //Bad Way - First Node - Second Node - 3 choices
    root->options[0]->options[1]->options[0] = root->options[1]->options[1];

    choices = { "Yes", "*Make weird noises*", "*dont respond*" };
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
