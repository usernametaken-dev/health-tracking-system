
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>

// Use the standard namespace
using namespace std;

// Health data structure
struct HealthData {
    int systolicBP;
    int diastolicBP;
    int pulseRate;
    int respiratoryRate;
    float bloodSugar;
    float temperature;
    vector<string> foodLog;
};

// User data structure
struct Member {
    string username;
    string password;
    string name;
    string medicalCondition;
    HealthData health;
};

// Global variables
vector<Member> users;
const string USERS_FILE = "users.txt";

// Utility functions
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Save users to file
void saveUsersToFile() {
    ofstream outFile(USERS_FILE);
    for (const auto &u : users) {
        outFile << u.username << '\n'
                << u.password << '\n'
                << u.name << '\n'
                << u.medicalCondition << '\n'
                << u.health.systolicBP << ' ' << u.health.diastolicBP << ' '
                << u.health.pulseRate << ' ' << u.health.respiratoryRate << ' '
                << u.health.bloodSugar << ' ' << u.health.temperature << '\n';
        outFile << u.health.foodLog.size() << '\n';
        for (const auto &food : u.health.foodLog) {
            outFile << food << '\n';
        }
    }
    outFile.close();
}

// Load users from file
void loadUsersFromFile() {
    ifstream inFile(USERS_FILE);
    if (!inFile) return;
    Member u;
    while (getline(inFile, u.username)) {
        getline(inFile, u.password);
        getline(inFile, u.name);
        getline(inFile, u.medicalCondition);
        inFile >> u.health.systolicBP >> u.health.diastolicBP >> u.health.pulseRate
               >> u.health.respiratoryRate >> u.health.bloodSugar >> u.health.temperature;
        size_t foodCount;
        inFile >> foodCount;
        inFile.ignore();
        u.health.foodLog.clear();
        for (size_t i = 0; i < foodCount; ++i) {
            string food;
            getline(inFile, food);
            u.health.foodLog.push_back(food);
        }
        users.push_back(u);
    }
    inFile.close();
}

// Register new user
void registerUser() {
    clearScreen();
    Member newMember;
    cout << "\n===== User Registration =====\n";
    cout << "Enter username: ";
    cin >> newMember.username;
    cout << "Enter password: ";
    cin >> newMember.password;
    cin.ignore();
    cout << "Enter full name: ";
    getline(cin, newMember.name);
    cout << "Enter medical condition (e.g., hypertension, diabetes, none): ";
    getline(cin, newMember.medicalCondition);
    newMember.health = {0, 0, 0, 0, 0.0, 0.0, {}};
    users.push_back(newMember);
    saveUsersToFile();
    cout << "\nRegistration successful!";
    pauseScreen();
}

// View user profile
void viewProfile(const Member &member) {
    clearScreen();
    cout << "\n===== User Profile =====\n";
    cout << "Name: " << member.name << "\n";
    cout << "Username: " << member.username << "\n";
    cout << "Medical Condition: " << member.medicalCondition << "\n";
    cout << "Systolic BP: " << member.health.systolicBP << " mmHg\n";
    cout << "Diastolic BP: " << member.health.diastolicBP << " mmHg\n";
    cout << "Pulse Rate: " << member.health.pulseRate << " bpm\n";
    cout << "Respiratory Rate: " << member.health.respiratoryRate << " bpm\n";
    cout << "Blood Sugar: " << member.health.bloodSugar << " mg/dL\n";
    cout << "Temperature: " << member.health.temperature << " °C\n";
    pauseScreen();
}

// Input vitals
void inputVitals(Member &member) {
    clearScreen();
    cout << "\n===== Input Vitals =====\n";
    cout << "Systolic Blood Pressure: ";
    cin >> member.health.systolicBP;
    cout << "Diastolic Blood Pressure: ";
    cin >> member.health.diastolicBP;
    cout << "Pulse Rate: ";
    cin >> member.health.pulseRate;
    cout << "Respiratory Rate: ";
    cin >> member.health.respiratoryRate;
    cout << "Blood Sugar Level: ";
    cin >> member.health.bloodSugar;
    cout << "Temperature: ";
    cin >> member.health.temperature;
    saveUsersToFile();
    pauseScreen();
}

// Log food
void logFood(Member &member) {
    clearScreen();
    cout << "\n===== Food Logging =====\n";
    string meal;
    cin.ignore();
    cout << "Enter meal description: ";
    getline(cin, meal);
    member.health.foodLog.push_back(meal);
    saveUsersToFile();
    pauseScreen();
}

// Show recommendations
void showRecommendations() {
    clearScreen();
    cout << "\n===== Dietary Recommendations =====\n";
    cout << "- \n";
    cout << "- \n";
    cout << "- \n";
    pauseScreen();
}

// Delete account
void deleteAccount(const string &username) {
    for (auto it = users.begin(); it != users.end(); ++it) {
        if (it->username == username) {
            users.erase(it);
            saveUsersToFile();
            cout << "\nAccount successfully deleted.";
            pauseScreen();
            return;
        }
    }
    cout << "\nAccount not found.";
    pauseScreen();
}

// Manage account
void manageAccount(Member &member) {
    int choice;
    do {
        clearScreen();
        cout << "\n===== Manage Account =====\n";
        cout << "1. View Profile\n";
        cout << "2. Update Vitals\n";
        cout << "3. Log Food\n";
        cout << "4. View Recommendations\n";
        cout << "5. Delete Account\n";
        cout << "6. Back to Dashboard\n";
        cout << "Option: ";
        cin >> choice;
        switch (choice) {
            case 1: viewProfile(member); break;
            case 2: inputVitals(member); break;
            case 3: logFood(member); break;
            case 4: showRecommendations(); break;
            case 5: deleteAccount(member.username); return;
            case 6: return;
            default: cout << "Invalid choice."; pauseScreen(); break;
        }
    } while (choice != 6);
}

// Login
void login() {
    clearScreen();
    string user, pass;
    cout << "\n===== User Login =====\n";
    cout << "Username: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;
    for (auto &u : users) {
        if (u.username == user && u.password == pass) {
            int option;
            do {
                clearScreen();
                cout << "\n===== Health Dashboard =====\n";
                cout << "1. Manage Account\n";
                cout << "2. Logout\n";
                cout << "Select option: ";
                cin >> option;
                switch (option) {
                    case 1: manageAccount(u); break;
                    case 2: cout << "Logging out...\n"; break;
                    default: cout << "Invalid option."; pauseScreen(); break;
                }
            } while (option != 2);
            return;
        }
    }
    cout << "Incorrect username or password.";
    pauseScreen();
}

// Main
int main() {
    loadUsersFromFile();
    int option;
    while (true) {
        clearScreen();
        cout << "\t==========================================\n";
        cout << "\t     HEALTH MONITORING SYSTEM MENU       \n";
        cout << "\t==========================================\n";
        cout << "\t1. Log In\n";
        cout << "\t2. Register\n";
        cout << "\t3. Exit\n";
        cout << "\tOption: ";
        cin >> option;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number.";
            pauseScreen();
            continue;
        }
        switch (option) {
            case 1: login(); break;
            case 2: registerUser(); break;
            case 3: saveUsersToFile(); cout << "Exiting system...\n"; return 0;
            default: cout << "Invalid option!"; pauseScreen(); break;
        }
    }
    return 0;
}
