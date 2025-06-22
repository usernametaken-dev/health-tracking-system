// Required header files
#include <iostream>     // For input/output (cin, cout)
#include <fstream>      // For file read/write operations (ifstream, ofstream)
#include <string>       // For using strings
#include <vector>       // For using dynamic arrays (vector)
#include <limits>       // For numeric_limits, helps with input validation

// Using the standard namespace to avoid std:: prefix
using namespace std;

// ======================================
// DEFINING DATA STRUCTURES FOR THE APP
// ======================================

// Struct to store vital health-related information for each user
struct HealthData {
    int systolicBP;               // Blood pressure (upper value)
    int diastolicBP;              // Blood pressure (lower value)
    int pulseRate;                // Heart beats per minute
    int respiratoryRate;          // Breaths per minute
    float bloodSugar;             // Blood sugar level in mg/dL
    float temperature;            // Body temperature in °C
    vector<string> foodLog;       // List of meals entered by the user
};

// Struct to store personal info, login credentials, and health info
struct Member {
    string username;              // Unique username for login
    string password;              // User password
    string name;                  // Full name of the user
    string medicalCondition;      // Describes existing condition (e.g. diabetes)
    HealthData health;            // Nested structure holding health metrics
};

// ==================================
// GLOBAL VARIABLES
// ==================================

// This vector holds all registered users loaded from file or newly registered
vector<Member> users;

// Constant string to hold the name of the file where all user data is stored
const string USERS_FILE = "users.txt";

// ==================================
// UTILITY FUNCTIONS
// ==================================

// Clears the screen for better user interface
void clearScreen() {
#ifdef _WIN32
    system("cls");   // Windows command to clear screen
#else
    system("clear"); // Unix/Linux/Mac command to clear screen
#endif
}

// Pauses the screen so user can read outputs before continuing
void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear leftover input
    cin.get(); // Wait for Enter
}

// ==================================
// SAVE AND LOAD USER DATA
// ==================================

// Function to save all users and their health/food data into a file
void saveUsersToFile() {
    ofstream outFile(USERS_FILE); // Opens the file for writing (overwrites existing)

    // Loop through each user in the users vector
    for (const auto &u : users) {
        // Write basic info: username, password, name, medical condition
        outFile << u.username << '\n'
                << u.password << '\n'
                << u.name << '\n'
                << u.medicalCondition << '\n';

        // Write health vitals as space-separated values
        outFile << u.health.systolicBP << ' '
                << u.health.diastolicBP << ' '
                << u.health.pulseRate << ' '
                << u.health.respiratoryRate << ' '
                << u.health.bloodSugar << ' '
                << u.health.temperature << '\n';

        // Write the number of food log entries
        outFile << u.health.foodLog.size() << '\n';

        // Write each food entry on a new line
        for (const auto &food : u.health.foodLog) {
            outFile << food << '\n';
        }
    }
    outFile.close(); // Always close files to save and release resources
}

// Function to load all user data from file when the program starts
void loadUsersFromFile() {
    ifstream inFile(USERS_FILE); // Try to open file

    if (!inFile) return; // If file doesn't exist, skip loading

    Member u;
    while (getline(inFile, u.username)) { // Read user block-by-block
        getline(inFile, u.password);
        getline(inFile, u.name);
        getline(inFile, u.medicalCondition);

        // Read health vitals
        inFile >> u.health.systolicBP >> u.health.diastolicBP
               >> u.health.pulseRate >> u.health.respiratoryRate
               >> u.health.bloodSugar >> u.health.temperature;

        size_t foodCount;
        inFile >> foodCount; // Read how many food entries there are
        inFile.ignore();     // Skip newline before reading food log

        u.health.foodLog.clear(); // Clear any old data

        // Read each food log entry line-by-line
        for (size_t i = 0; i < foodCount; ++i) {
            string food;
            getline(inFile, food);
            u.health.foodLog.push_back(food);
        }

        users.push_back(u); // Store this user into the system
    }

    inFile.close(); // Close the file after all users are loaded
}

// ==================================
// USER REGISTRATION
// ==================================

// Register a new user and collect their basic and medical info
void registerUser() {
    clearScreen();

    Member newMember;
    cout << "\n===== User Registration =====\n";

    // Collect login credentials and personal details
    cout << "Enter username: ";
    cin >> newMember.username;

    cout << "Enter password: ";
    cin >> newMember.password;

    cin.ignore(); // Clear input buffer before getline
    cout << "Enter full name: ";
    getline(cin, newMember.name);

    cout << "Enter medical condition (e.g., hypertension, diabetes, none): ";
    getline(cin, newMember.medicalCondition);

    // Initialize health values with zero/defaults
    newMember.health = {0, 0, 0, 0, 0.0, 0.0, {}};

    // Add new user to global users list
    users.push_back(newMember);

    // Save to file so data is not lost on exit
    saveUsersToFile();

    cout << "\nRegistration successful!";
    pauseScreen(); // Pause to allow user to read
}

// ==================================
// PROFILE VIEW AND HEALTH LOGGING
// ==================================

// Display current user's stored profile and vitals
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

// Allows user to enter/update their health vitals
void inputVitals(Member &member) {
    clearScreen();
    cout << "\n===== Input Vitals =====\n";

    // Prompt user to enter updated vitals
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

    saveUsersToFile(); // Save new data after update
    pauseScreen();
}

// Logs a food/meals entry from user
void logFood(Member &member) {
    clearScreen();
    cout << "\n===== Food Logging =====\n";

    cin.ignore(); // Clear input buffer
    string meal;
    cout << "Enter meal description: ";
    getline(cin, meal); // Input full line

    member.health.foodLog.push_back(meal); // Save meal
    saveUsersToFile(); // Update saved data
    pauseScreen();
}

// Placeholder for health tips (can be expanded later)
void showRecommendations() {
    clearScreen();
    cout << "\n===== Dietary Recommendations =====\n";
    cout << "- \n"; // Add real recommendations here
    cout << "- \n";
    cout << "- \n";
    pauseScreen();
}

// Deletes the current user's account from memory and file
void deleteAccount(const string &username) {
    for (auto it = users.begin(); it != users.end(); ++it) {
        if (it->username == username) {
            users.erase(it); // Remove from list
            saveUsersToFile(); // Reflect changes
            cout << "\nAccount successfully deleted.";
            pauseScreen();
            return;
        }
    }
    cout << "\nAccount not found.";
    pauseScreen();
}

// Main menu for logged-in users to access account functions
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
            case 5: deleteAccount(member.username); return; // Exit if deleted
            case 6: return; // Go back
            default: cout << "Invalid choice."; pauseScreen(); break;
        }
    } while (choice != 6);
}

// Authenticates user credentials and grants access to dashboard
void login() {
    clearScreen();
    string user, pass;
    cout << "\n===== User Login =====\n";
    cout << "Username: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;

    // Check if username and password match any existing user
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
            } while (option != 2); // Stay in dashboard until logout
            return;
        }
    }

    // Login failed
    cout << "Incorrect username or password.";
    pauseScreen();
}

// ==================================
// MAIN FUNCTION – ENTRY POINT
// ==================================

int main() {
    loadUsersFromFile(); // Load existing user data from file

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
            // Handles invalid input (e.g., letters instead of numbers)
            cin.clear(); // Clear error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
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
