// Name : Reg : Quote
// Syed Abdullah Ahmed : 2026718 : "Why did i think working in a team would be easy."

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

const int MAX_USERS = 100;
string usernames[MAX_USERS], passwords[MAX_USERS], roles[MAX_USERS], regOrSkill[MAX_USERS], hostel[MAX_USERS], room[MAX_USERS];
int isApproved[MAX_USERS], tasksDone[MAX_USERS], ratingPoints[MAX_USERS], payout[MAX_USERS];
int userCount = 0;

string encryptPassword(string password, int index = 0);
void saveAccounts();
void loadAccounts();
void signUp();
void signIn();
void studentMenu(int userIndex);
void workerMenu(int userIndex);
void adminMenu(int userIndex);

int main() {
    loadAccounts();
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
        cout << "Enter an unique username: "; cin >> temp_username;
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
    cout << "Account created successfully!" << endl;
}

void signIn() {

    // loading accounts
    loadAccounts();

    // login loop
    bool successful = false;
    int i = 0;
    while (successful == false){
        string temp_username, temp_password;
        bool found = false;
        i = 0;

        cout << "Enter your username: "; cin >> temp_username;
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
            cout << "Username not found! Try again." << endl;
        }
        // checking if password is correct
        else if (encryptPassword(temp_password) == passwords[i]){
            successful = true;
        }
        else{
            cout << "Username or password incorrect!" << endl;
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

void studentMenu(int userIndex) {
    cout << "\nWelcome Student: " << usernames[userIndex] << endl;
    // TODO (Turn 2 - Ibrahim): Add task creation and student options
}

void workerMenu(int userIndex) {
    cout << "\nWelcome Worker: " << usernames[userIndex] << endl;
    // TODO (Turn 4 - Ibrahim): Add worker portal options
}

void adminMenu(int userIndex) {
    cout << "\nWelcome Admin: " << usernames[userIndex] << endl;
    // TODO (Turn 5 - Abdullah): Add admin oversight options
}
