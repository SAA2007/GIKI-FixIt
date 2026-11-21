// Name : Reg : Quote
// Syed Abdullah Ahmed : 2026718 : "Why did i think working in a team would be easy."
// Syed Ibrahim Kashif : 2026727 : "Why did i think working in a team would be easy. (2)"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace std;

//create parallel arrays for accounts.csv
const int MAX_USERS = 100;
string usernames[MAX_USERS], passwords[MAX_USERS], roles[MAX_USERS], regOrSkill[MAX_USERS], hostel[MAX_USERS], room[MAX_USERS];
int isApproved[MAX_USERS], tasksDone[MAX_USERS], ratingPoints[MAX_USERS], payout[MAX_USERS];
int userCount = 0;


//create parallel arrays for tasks.csv
const int MAX_TASKS = 1000;
string taskID[MAX_TASKS], studentUsername[MAX_TASKS], taskHostel[MAX_TASKS], taskRoom[MAX_TASKS], taskDesc[MAX_TASKS], workerUsername[MAX_TASKS], dateCreated[MAX_TASKS], dateCompleted[MAX_TASKS], taskType[MAX_TASKS], taskStatus[MAX_TASKS];
int taskUrgency[MAX_TASKS], allowedHours[MAX_TASKS], taskRating[MAX_TASKS]; //int arrays only where math is needed; str otherwise to keep it simple
int taskCount = 0;


//TODO create parallel arrays for complaints.csv


//prototype functions
string encryptPassword(string password, int index = 0);
void saveAccounts();
void loadAccounts();
void signUp();
void signIn();
void studentMenu(int userIndex);
void workerMenu(int userIndex);
void adminMenu(int userIndex);
void createTask(int userIndex);
void displayTasks(int userIndex);
void saveTasks();
void loadTasks();


int main() {
    loadAccounts();
    loadTasks(); //TODO might move this down

    cout << "Welcome to GIKI FixIt!" << endl;

    // first time setup: create first admin if no accounts exist
    if (userCount == 0) {
        signUp();
    }

    string choice = "";
    while (choice != "3") {
        cout << "\n--- Main Menu ---" << endl;
        cout << "1. Sign In" << endl;
        cout << "2. Sign Up" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter choice: "; cin >> choice;
        cout<<endl;

        if (choice == "1") {
            signIn();
        }
        else if (choice == "2") {
            signUp();
        }
        else if (choice == "3") {
            cout << "Goodbye!" << endl;
        }
        else {
            cout << "Try again! Invalid choice." << endl;
        }
    }

    return 0;
}

// Recursive password encryption (Feature #2)
string encryptPassword(string password, int index) {
    if (index >= password.length()) {
        return password; // Base case
    }
    password[index] = password[index] + 3; // Shift character by +3
    return encryptPassword(password, index + 1); // Recursive call
}

void saveAccounts() {
    ofstream f("accounts.csv");
    if (f.is_open()) {
        for (int i = 0; i < userCount; i++) {
            f << usernames[i] << "," << passwords[i] << "," << roles[i] << "," << regOrSkill[i] << "," << hostel[i] << "," << room[i] << "," << isApproved[i] << "," << tasksDone[i] << "," << ratingPoints[i] << "," << payout[i] << "\n";
        }
        f.close();
    }
}

void loadAccounts() {
    ifstream f("accounts.csv");
    userCount = 0;
    if (f.is_open()) {
        string line;
        while (getline(f, line) && userCount < MAX_USERS) {
            string temp_isApproved, temp_tasksDone, temp_ratingPoints, temp_payout;
            if (line.empty()) continue;
            stringstream ss(line);
            getline(ss, usernames[userCount], ',');
            getline(ss, passwords[userCount], ',');
            getline(ss, roles[userCount], ',');
            getline(ss, regOrSkill[userCount], ',');
            getline(ss, hostel[userCount], ',');
            getline(ss, room[userCount], ',');
            getline(ss, temp_isApproved, ',');
            getline(ss, temp_tasksDone, ',');
            getline(ss, temp_ratingPoints, ',');
            getline(ss, temp_payout, ',');
            isApproved[userCount] = stoi(temp_isApproved);
            tasksDone[userCount] = stoi(temp_tasksDone);
            ratingPoints[userCount] = stoi(temp_ratingPoints);
            payout[userCount] = stoi(temp_payout);
            userCount++;
        }
        f.close();
    }
}

void signUp(){

    // loading existing accounts from database
    loadAccounts();

    // checking if frist user
    if (userCount == 0){
        cout << "Signing up as admin!" << endl;
    }

    // declarations
    string temp_username, temp_password, temp_role;
    bool unique = false;

    // username
    while (unique == false){
        int i = 0; bool found = false;
        cout << "Enter a unique username: "; cin >> temp_username;
        while (i < userCount && found == false){
            if (temp_username == usernames[i]){
                found = true;
            }
            else{
                i++;
            }
        }
        if (found == true){
            cout << "Username already taken! Try again." << endl;
        }
        if (i == userCount && found == false){
            usernames[userCount] = temp_username;
            unique = true;
        }
    }

    //password
    cout << "Enter a secure password: "; cin >> temp_password;
    while (temp_password.length() < 8){
        cout << "Try again!\nEnter a secure password: "; cin >> temp_password;
    }
    passwords[userCount] = encryptPassword(temp_password);

    // filling in rest for admin
    if (userCount == 0){
        roles[0] = "Admin";
        regOrSkill[0] = "N/A";
        hostel[0] = "N/A";
        room[0] = "N/A";
        isApproved[0] = 1;
        tasksDone[0] = 0;
        ratingPoints[0] = 0;
        payout[0] = 0;
    }

    // filling in for rest of other users
    else{

        // role
        cout << "Select '1' for student or '2' for worker: "; cin >> temp_role;
        while (temp_role != "1" && temp_role != "2"){
            cout << "Try again!\nSelect '1' for student or '2' for worker: "; cin >> temp_role;
        }

        // student
        if (temp_role == "1"){
            string temp_reg, temp_hostel, temp_room;
            cout << "Enter your 7-digit Reg #: "; cin >> temp_reg;
            while (temp_reg.length() != 7){
                cout << "Try again!\nEnter your 7-digit Reg #: "; cin >> temp_reg;
            }

            cout << "Enter your Hostel (1-14 or GH or PL): "; cin >> temp_hostel; // GH is girls Hostel, and PR is Parents Lodge
            while (temp_hostel != "1" && temp_hostel != "2" && temp_hostel != "3" && temp_hostel != "4" &&
                   temp_hostel != "5" && temp_hostel != "6" && temp_hostel != "7" && temp_hostel != "8" &&
                   temp_hostel != "9" && temp_hostel != "10" && temp_hostel != "11" && temp_hostel != "12" &&
                   temp_hostel != "13" && temp_hostel != "14" && temp_hostel != "GH" && temp_hostel != "gh" &&
                   temp_hostel != "PL" && temp_hostel != "pl"){
                cout << "Try again!\nEnter your Hostel (1-14 or GH or PL): "; cin >> temp_hostel;
            }
            if (temp_hostel == "gh") temp_hostel = "GH";
            if (temp_hostel == "pl") temp_hostel = "PL";

            cout << "Enter your Room #: "; cin >> temp_room;

            roles[userCount] = "Student";
            regOrSkill[userCount] = temp_reg;
            hostel[userCount] = temp_hostel;
            room[userCount] = temp_room;
            isApproved[userCount] = 1;
            tasksDone[userCount] = 0;
            ratingPoints[userCount] = 0;
            payout[userCount] = 0;
        }

        //worker
        else{
            string temp_skill;
            cout << "Enter your specialty (e.g. Electrician, Plumber, Carpenter): "; cin >> temp_skill;

            roles[userCount] = "Worker";
            regOrSkill[userCount] = temp_skill;
            hostel[userCount] = "N/A";
            room[userCount] = "N/A";
            isApproved[userCount] = 0;
            tasksDone[userCount] = 0;
            ratingPoints[userCount] = 0;
            payout[userCount] = 0;
        }
    }

    // calling saveAccounts to update database
    userCount++;
    saveAccounts();
    cout << "\nAccount created successfully!" << endl;
}

void signIn() {

    // loading accounts
    loadAccounts();

    // login loop
    bool successful = false;
    int i = 0;
    while (successful == false){ //TODO reroute back to main menu instead of signin menu if username or password incorrect; or atleast give the user an option to do so
        string temp_username, temp_password;
        bool found = false;
        i = 0;

        cout << "Enter your username: "; cin >> temp_username; //TODO check if username exists BEFORE prompting for password
        cout << "Enter your password: "; cin >> temp_password;

        // checking if username exists
        while (i < userCount && found == false){
            if (temp_username == usernames[i]){
                found = true;
            }
            else{
                i++;
            }
        }

        if (found == false){
            cout << "\nUsername not found! Try again." << endl << endl;
        }
        // checking if password is correct
        else if (encryptPassword(temp_password) == passwords[i]){
            successful = true;
        }
        else{
            cout << "\nUsername or password incorrect!" << endl << endl;
        }
    }

    // checking if pending approval
    if (isApproved[i] == 0){
        cout << "Account pending approval." << endl;
    }
    // calling menu
    else{
        if (roles[i] == "Student"){
            studentMenu(i);
        }
        else if (roles[i] == "Worker"){
            workerMenu(i);
        }
        else{
            adminMenu(i);
        }
    }
}


void displayTasks(int userIndex){
    //TODO output in tabular format using dashes
    loadTasks();

    int temp_count = 0;
    for (int i=0; i<taskCount; i++){
        if (studentUsername[i] == usernames[userIndex]) { //only display tasks created by the same user
            cout << "\n" << taskID[i] << ", " << taskType[i] << ", " << taskDesc[i] << ", " << taskUrgency[i] << ", " << dateCreated[i] << ", " << dateCompleted[i] << ", " << taskStatus[i] <<endl; //TODO might add/remove some elements from here
            temp_count++;

        }
    }

    if (temp_count == 0) {
        cout << "\nYou havent created any tasks yet.";
    }

    cout << endl;
}

 //TODO reordering of functions; doesnt  matter now as prototyped them
void createTask(int userIndex){
    string temp_taskType, temp_taskDesc;
    int temp_taskUrgency, temp_allowedHours;

    cout << "\nSelect '1' for Plumbing, '2' for Electrical, '3' for Carpentry, '4' for Network, or '5' for HVAC issues: "; cin >> temp_taskType;
    while (temp_taskType != "1" && temp_taskType != "2" && temp_taskType != "3" && temp_taskType != "4" && temp_taskType != "5"){
        cout << "Try again!\nSelect '1' for Plumbing, '2' for Electrical, '3' for Carpentry, '4' for Network, or '5' for HVAC issues: "; cin >> temp_taskType;
    }

    if (temp_taskType == "1") temp_taskType = "Plumbing"; else if (temp_taskType == "2") temp_taskType = "Electrical"; else if (temp_taskType == "3") temp_taskType = "Carpentry"; else if (temp_taskType == "4") temp_taskType = "Network"; else if (temp_taskType == "5") temp_taskType = "HVAC";

    cout << "Enter a brief description of your issue: ";
    cin.ignore(); //ignores left over new line leftover by previous cin
    getline(cin, temp_taskDesc); //TODO perform validation checks; deal with input spaces issue in cpp

    cout << "Rate the urgency of your task from 1-3 (1 being the least urgent): "; cin >> temp_taskUrgency; //TODO work on datatype of urgency
    while (temp_taskUrgency != 1 && temp_taskUrgency != 2 && temp_taskUrgency != 3){
        cout << "Try again!\nSRate the urgency of your task from 1-3 (1 being the least urgent): "; cin >> temp_taskUrgency;
    }

    temp_allowedHours = (8-2*temp_taskUrgency)*24; //TODO revise formula


    //add task to array
    taskID[taskCount] = "#" + to_string(taskCount + 1); //TODO work on this; use taskCounter as index...
    studentUsername[taskCount] = usernames[userIndex];
    taskHostel[taskCount] = hostel[userIndex]; //TODO this works on the assumption that a student can only create a task for their own hostel and room
    taskRoom[taskCount] = room[userIndex];
    taskDesc[taskCount] = temp_taskDesc;
    workerUsername[taskCount] = "N/A";
    dateCreated[taskCount] = "DD-MM-YYYY"; //TODO work on date and time using <ctime>; used a placeholder for now
    dateCompleted[taskCount] = "N/A"; //use N/A for placeholders for string
    taskType[taskCount] = temp_taskType;
    taskUrgency[taskCount] = temp_taskUrgency;
    taskStatus[taskCount] = "In-progress";
    allowedHours[taskCount] = temp_allowedHours;
    taskRating[taskCount] = 0; //use 0 for placeholders for int

    cout << "Task created successfully!" << endl;

    //add task to tasks.csv
    taskCount++;
    saveTasks();
}

void saveTasks(){
    //overwrite tasks.csv with content of parallel arrays
    ofstream f("tasks.csv");
    if (f.is_open()){
        for (int i=0; i<taskCount; i++){
            f << taskID[i] << "," << studentUsername[i] << "," << taskHostel[i] << "," << taskRoom[i] << "," << taskDesc[i] << "," << workerUsername[i] << "," << dateCreated[i] << "," << dateCompleted[i] << "," << taskType[i] << "," << taskUrgency[i] << "," << taskStatus[i] << "," << allowedHours[i] << "," << taskRating[i] << "\n";
        }
    }
    f.close(); //rmbr

}

void loadTasks(){
    ifstream f("tasks.csv");

    taskCount=0;
    if (f.is_open()){
        string line; //temp var that holds the line being iterated over
        while (getline(f, line) && taskCount < MAX_TASKS){
            //copy tasks.csv to local parallel arrays by iterating row by row and splitting lines
            //directly copy to array from getline for string arrays; first copy to string variable from getline, then cast type to int, then copy to int array
            string temp_taskUrgency, temp_allowedHours, temp_taskRating;
            if (line.empty()) continue; //skip blank lines,
            stringstream ss(line); //converts line into an input stream variable ss, so that it can be splitted asw
            getline(ss, taskID[taskCount], ','); //split each line by ',' and store text in array element
            getline(ss, studentUsername[taskCount], ',');
            getline(ss, taskHostel[taskCount], ',');
            getline(ss, taskRoom[taskCount], ',');
            getline(ss, taskDesc[taskCount], ',');
            getline(ss, workerUsername[taskCount], ',');
            getline(ss, dateCreated[taskCount], ',');
            getline(ss, dateCompleted[taskCount], ','); //loadtasks and savetasks must follow the same order
            getline(ss, taskType[taskCount], ',');
            getline(ss, temp_taskUrgency, ',');
            getline(ss, taskStatus[taskCount], ',');
            getline(ss, temp_allowedHours, ',');
            getline(ss, temp_taskRating, ',');
            taskUrgency[taskCount] = stoi(temp_taskUrgency); //string to int
            allowedHours[taskCount] = stoi(temp_allowedHours);
            taskRating[taskCount] = stoi(temp_taskRating);


            taskCount++;
        }
        f.close();
    }
}


void studentMenu(int userIndex) {
    cout << "\nWelcome Student: " << usernames[userIndex] << endl;
    // TODO (Turn 2 - Ibrahim): Add task creation and student options

    string temp_option;

    do {
        //TODO add option to create complaint
        cout << "\nSelect '1' to create a task, '2' to load existing tasks, '3' to register a complaint, or '4' to return to main menu: "; cin >> temp_option;
        while (temp_option != "1" && temp_option != "2" && temp_option != "3"  && temp_option != "4"){
            cout << "Try again!\nSelect '1' to create a task, '2' to load existing tasks, or '3' to register a complaint: "; cin >> temp_option;
            cout << endl;
        }

        if (temp_option == "1"){
            createTask(userIndex);
        } else if (temp_option == "2") {
            displayTasks(userIndex);
        } else if (temp_option=="3") {
            // ...; //complaint
        }
    } while (temp_option != "4");

}

void workerMenu(int userIndex) {
    cout << "\nWelcome Worker: " << usernames[userIndex] << endl;
    // TODO (Turn 4 - Ibrahim): Add worker portal options

    cout << "\nThis is the worker menu! Stay tuned for further updates." <<endl;

}

void adminMenu(int userIndex) {
    cout << "\nWelcome Admin: " << usernames[userIndex] << endl;
    // TODO (Turn 5 - Abdullah): Add admin oversight options

    cout << "\nThis is the worker menu! Stay tuned for further updates." <<endl;

}
