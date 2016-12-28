/*

VCC CMPT 1020 - Assignment 3

Animal Guessing Game

    ******************************
    *                            *
    *  ZOO TYCOON - VCC EDITION  *
    *                            *
    ******************************

This program creates a game in which the computer asks a series of
questions in order to guess which animal the user is thinking of.
If the computer can't guess, it asks the user for the animal and
a question that distinguishes their animal from the last one the
computer asked about. The computer then remembers the question and
the new animal for the next round.

Authors: Junho Sohn, Tom Dorval, Eleanor Kim
Date: 03/13/16

*/

#include <iostream>
#include <cstdlib>
#include <limits>
#include <fstream>
#include <string>

using namespace std;

struct ListNode
{
    string text;
    string animal_name;
    ListNode *yes_answer, *no_answer;
};
typedef ListNode* ListNodePtr;


void intro_msg();
bool replay();
void print_list(ListNodePtr);
void check_if_empty(ListNodePtr&, ifstream&);
void start(ListNodePtr&);
ListNodePtr create_tree();
void check_answer(string, ListNodePtr&);
void ask_question(ListNodePtr&);
void new_question(ListNodePtr&, string);
void add_to_tree(string, string, ListNodePtr&);
void write_to_file(ListNodePtr, ofstream&);
void read_from_file(ListNodePtr&,ifstream&);


int main()
{
    ifstream in_stream;
    ofstream out_stream;
    ListNodePtr head;

//Open input file
    in_stream.open("file.txt");
    if(in_stream.fail())
    {
        cout<<"Input File Error!";
        exit(1);
    }

    intro_msg();
    check_if_empty(head,in_stream);

    do
    {
        system("CLS");
        ListNodePtr temp = head;
        start(temp);
        ask_question(temp);
    }
    while(replay());

//Open output file
    out_stream.open("file.txt");
    if(out_stream.fail())
    {
        cout<<"Output File Error!";
        exit(1);
    }
    write_to_file(head, out_stream);

    cout << "Thanks, that was easy." << endl;

    in_stream.close();
    out_stream.close();

    return 0;
}

//Intro message
//PRE: NONE
//POST: Displays message
void intro_msg()
{
    string r;
    cout << "******************************" << endl
         << "*                            *" << endl
         << "*  ZOO TYCOON - VCC EDITION  *" << endl
         << "*                            *" << endl
         << "******************************" << endl;
    cout << "\nHello, I am your computer and I can guess the animal you are thinking of.\n" << endl;
    cout << "Are you ready? ";
    cin >> r;
    if(!(r == "yes" || r== "y"))
    {
        cout << "Well too bad. Press enter to start..." << endl;
        cin.ignore();
        cin.ignore();
    }
}

//Creates initial tree
//PRE: The program is being run for the first time (length of file = 0)
//POST: Creates initial tree
ListNodePtr create_tree()
{
    ListNodePtr left,right,root;

    root = new ListNode;
    root->text = "Does your animal have 4 legs? ";
    root->yes_answer = NULL;
    root->no_answer = NULL;

    left = new ListNode;
    left->text = "Is your animal a dog? ";
    left->animal_name = "dog";
    left->yes_answer = NULL;
    left->no_answer = NULL;
    root->yes_answer = left;

    right = new ListNode;
    right->text = "Is your animal a spider? ";
    right->animal_name = "spider";
    right->yes_answer = NULL;
    right->no_answer = NULL;
    root->no_answer = right;

    return root;
}

//Checks if file is empty or not
//PRE: head tree empty/not
//POST: if empty, creates new tree from template; if not, read + create tree from file.txt
void check_if_empty(ListNodePtr& head, ifstream& in_stream)
{
    in_stream.seekg (0, in_stream.end);
    int length = in_stream.tellg();
    if(length==0)
    {
        head = create_tree();
    }
    else
    {
        in_stream.seekg(0, in_stream.beg);
        read_from_file(head, in_stream);
    }
}

//Starts with the first question
//PRE: Tree in good condition, valid user input
//POST: Checks if answer to question is yes/no, points accordingly
void start(ListNodePtr& temp)
{
    string input;
    while(temp->yes_answer != NULL && temp->no_answer != NULL)
    {
        cout << temp->text;
        cin >> input;
        check_answer(input,temp);
    }
}

//Checks user's response
//PRE: User enters valid input
//POST: Temp points to either yes/no depending on response
void check_answer(string inpt, ListNodePtr& temp)
{
    if(inpt == "y" || inpt == "yes")
        temp = temp->yes_answer;
    else
        temp = temp->no_answer;
}

//Ask user a question
//PRE: Reads out question stored at temp->text
//POST: Takes in input from user
void ask_question(ListNodePtr& temp)
{
    string inpt;
    cout << temp->text;
    cin >> inpt;

    if(inpt == "n" || inpt == "no")
        new_question(temp, temp->animal_name);
    else
        cout << "I GUESSED RIGHT!" << endl;
}

//Create a new question for a 'no' answer
//PRE: User inputs 'no' until end of tree
//POST: Creates new question with corresponding animal
void new_question(ListNodePtr& temp, string anml)
{
    string animal, q;
    cout << "I give up. What was your animal? ";
    cin >> animal;

    cout << "Type a question that can be answered as yes for a "
         << animal << " and no for a " << anml << ": " << endl;

    cin.ignore();
    getline(cin, q);
    add_to_tree(q, animal, temp);
}

//Adds a node (question) to existing tree
//PRE: User inputs valid question and answer
//POST: Adds a new node with the question and answer
void add_to_tree(string question, string answer, ListNodePtr& temp)
{
    question+=" ";
    string q2 = "Is your animal a " + answer + "? ";
    ListNodePtr left, right;

    left = new ListNode;
    left->text = q2;
    left->animal_name = answer;
    left->yes_answer = NULL;
    left->no_answer = NULL;

    right = new ListNode;
    right->text = temp->text;
    right->animal_name = temp->animal_name;
    right->yes_answer = NULL;
    right->no_answer = NULL;

    temp->text = question;
    temp->yes_answer = left;
    temp->no_answer = right;
}

//Print tree (error-checking)
//PRE: NONE
//POST: Prints out text in the tree, * if NULL
void print_list(ListNodePtr iter)
{
    if(iter==NULL)
        cout << "*" << endl;
    else
    {
        cout << iter->text << endl;
        print_list(iter->yes_answer);
        print_list(iter->no_answer);
    }
}

//Play again? Yes or no
//PRE: Yes/No question
//POST: Returns true if Y/y/yes, false if N/n/no
bool replay()
{
    string ch;
    do
    {
        cout << endl << "PLAY AGAIN? ";
        cin >> ch;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    while (!(ch=="y" || ch=="n" || ch=="Y" || ch=="N" || ch=="yes" || ch=="no"));

    if(ch=="y" || ch=="Y" || ch=="yes")
        return true;
    else
        return false;
}

//Write tree to output file
//PRE: NONE
//POST: Writes current 'head' out to file
void write_to_file(ListNodePtr head, ofstream& out_stream)
{
    if(head==NULL)
        out_stream << "*" << endl;
    else
    {
        out_stream << head->text << endl;
        write_to_file(head->yes_answer,out_stream);
        write_to_file(head->no_answer,out_stream);
    }
}

//Read tree from file
//PRE: NONE
//POST: Reads and creates a tree from file.txt
void read_from_file(ListNodePtr& head, ifstream& in_stream)
{
    string value;
    head = new ListNode;
    getline(in_stream,value);
    if(value != "*")
    {
        head -> text = value;
        read_from_file(head->yes_answer,in_stream);
        read_from_file(head->no_answer,in_stream);
    }
    else
    {
        head = NULL;
    }
}
