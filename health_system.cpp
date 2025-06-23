
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
    int systolicBP;               // Blood pressure (upper value)
    int diastolicBP;              // Blood pressure (lower value)
    int pulseRate;                // Heart beats per minute
    int respiratoryRate;          // Breaths per minute
    float bloodSugar;             // Blood sugar level in mg/dL
    float temperature;            // Body temperature in Â°C
    vector<string> foodLog;       // List of meals entered by the user
};

// Struct to store personal info, login credentials, and health info
struct Member {
    string username;              // Unique username for login
    string password;              // User password
    string name;                  // Full name of the user
    int age;                      // Age of the user
    float height;                 // Height in cm
    float weight;                 // Weight in kg
    string medicalCondition;      // Describes existing condition (e.g. diabetes)
    HealthData health;            // Nested structure holding health metrics
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
    system("cls");		// Windows command to clear screen
#else
    system("clear");	// Unix/Linux/Mac command to clear screen
#endif
}

// Pauses the screen so user can read outputs before continuing
void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear leftover input
    cin.get();
}

// ==================================
// SAVE AND LOAD USER DATA
// ==================================

// Function to save all users and their health/food data into a file
void saveUsersToFile() {
    ofstream outFile(USERS_FILE);	// Opens the file for writing (overwrites existing)
    
    // Loop through each user in the users vector
    for (const auto &u : users) {
    	 // Write basic info: username, password, name, age, weight, height, medical condition
        outFile << u.username << '\n'
                << u.password << '\n'
                << u.name << '\n'
                << u.age << '\n'
                << u.height << '\n'
                << u.weight << '\n'
                << u.medicalCondition << '\n'
                
        // Write health vitals as space-separated values
                << u.health.systolicBP << ' ' << u.health.diastolicBP << ' '
                << u.health.pulseRate << ' ' << u.health.respiratoryRate << ' '
                << u.health.bloodSugar << ' ' << u.health.temperature << '\n';
                
                // Write the number of food log entries
        outFile << u.health.foodLog.size() << '\n';
        
         // Write each food entry on a new line
        for (const auto &food : u.health.foodLog) {
            outFile << food << '\n';
        }
    }
    outFile.close();	// Always close files to save and release resources
}

// load all user data from file when the program starts
void loadUsersFromFile() {
    ifstream inFile(USERS_FILE);	 // Try to open file
    if (!inFile) return;	// If file doesn't exist, skip loading
    Member u;
    while (getline(inFile, u.username)) {	// Read user block-by-block
        getline(inFile, u.password);
        getline(inFile, u.name);
        inFile >> u.age >> u.height >> u.weight;
        inFile.ignore();
        getline(inFile, u.medicalCondition);
        
         // Read health vitals
        inFile >> u.health.systolicBP >> u.health.diastolicBP >> u.health.pulseRate
               >> u.health.respiratoryRate >> u.health.bloodSugar >> u.health.temperature;
        size_t foodCount;
        inFile >> foodCount;	 // Read how many food entries there are
        inFile.ignore();		 // Skip newline before reading food log
        u.health.foodLog.clear();
        
        // Read each food log entry line-by-line
        for (size_t i = 0; i < foodCount; ++i) {
            string food;
            getline(inFile, food);
            u.health.foodLog.push_back(food);
        }
        users.push_back(u);	// Store this user into the system
    }
    inFile.close();	 // Close the file after all users are loaded
}

// ==================================
// FUNCTION DECLARATIONS
// ==================================


void registerUser();
void viewProfile(const Member &member);
void inputVitals(Member &member);
void logFood(Member &member);
void showRecommendations();
void deleteAccount(const string &username);
void manageAccount(Member &member);
void login();

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
    cin.ignore();	// Clear input buffer before getline
    cout << "Enter full name: ";
    getline(cin, newMember.name);
    cout << "Enter age: ";
    cin >> newMember.age;
    cout << "Enter height (cm): ";
    cin >> newMember.height;
    cout << "Enter weight (kg): ";
    cin >> newMember.weight;
    cin.ignore();
    cout << "Enter medical condition (e.g., hypertension, diabetes, heart conditions): ";
    getline(cin, newMember.medicalCondition);
    
// Initialize health values with zero/defaults
    newMember.health = {0, 0, 0, 0, 0.0, 0.0, {}};
    
 // Add new user to global users list
    users.push_back(newMember);
    
// Save to file so data is not lost on exit
    saveUsersToFile();
    cout << "\nRegistration successful!";
    pauseScreen();	// Pause to allow user to read
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
    cout << "Age: " << member.age << "\n";
    cout << "Height: " << member.height << " cm\n";
    cout << "Weight: " << member.weight << " kg\n";
    cout << "Medical Condition: " << member.medicalCondition << "\n";
    cout << "Systolic BP: " << member.health.systolicBP << " mmHg\n";
    cout << "Diastolic BP: " << member.health.diastolicBP << " mmHg\n";
    cout << "Pulse Rate: " << member.health.pulseRate << " bpm\n";
    cout << "Respiratory Rate: " << member.health.respiratoryRate << " bpm\n";
    cout << "Blood Sugar: " << member.health.bloodSugar << " mg/dL\n";
    cout << "Temperature: " << member.health.temperature << " Â°C\n";
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
    if (member.health.systolicBP < 90 || member.health.systolicBP > 120)
        cout << "\n! Warning: Abnormal systolic blood pressure.";
    if (member.health.diastolicBP < 60 || member.health.diastolicBP > 80)
        cout << "\n! Warning: Abnormal diastolic blood pressure.";
    if (member.health.pulseRate < 60 || member.health.pulseRate > 100)
        cout << "\n! Warning: Abnormal pulse rate.";
    if (member.health.respiratoryRate < 12 || member.health.respiratoryRate > 20)
        cout << "\n! Warning: Abnormal respiratory rate.";
    if (member.health.bloodSugar > 140)
        cout << "\n! Warning: High blood sugar level.";
    if (member.health.temperature < 36.1 || member.health.temperature > 37.2)
        cout << "\n! Warning: Abnormal body temperature.";
        
    saveUsersToFile();	// Save new data after update
    pauseScreen();
}

// Logs a food/meals entry from user and checks if it's bad for their condition
void logFood(Member &member) {
    clearScreen();
    cout << "\n===== Food Logging =====\n";
    cin.ignore();  // Clear input buffer
    string meal;
    cout << "Enter meal description: ";
    getline(cin, meal);

    // Save the food log
    member.health.foodLog.push_back(meal);
    saveUsersToFile();

    // === Food Condition Warning System (Simplified) ===
    string condition = member.medicalCondition;

    // Basic warning checks using plain string comparison
    if (condition == "hypertension") {
        if (meal == "canned soup" || meal == "chips" || meal == "bacon") {
            cout << "\n? Warning: This food is high in sodium and not recommended for hypertension.";
        }
    }
    else if (condition == "diabetes") {
        if (meal == "soda" || meal == "cake" || meal == "white bread") {
            cout << "\n? Warning: This food may spike blood sugar and is not ideal for diabetes.";
        }
    }
    else if (condition == "kidney disease") {
        if (meal == "processed meat" || meal == "pickles" || meal == "dark soda") {
            cout << "\n? Warning: This food may be hard on the kidneys and should be limited.";
        }
    }
    else if (condition == "ulcer") {
        if (meal == "spicy food" || meal == "coffee" || meal == "alcohol") {
            cout << "\n? Warning: This food may irritate stomach ulcers.";
        }
    }
    else if (condition == "heart condition" || condition == "heart disease") {
        if (meal == "fried food" || meal == "red meat" || meal == "butter" || meal == "fast food") {
            cout << "\n? Warning: This food is high in saturated fat or cholesterol and may worsen heart conditions.";
        }
    }

    pauseScreen();
}


// dietary recommendations based on common medical conditions
void showRecommendations() {
    clearScreen();
    cout << "\n===== Dietary Recommendations =====\n";

    cout << "\n>> For Hypertension (High Blood Pressure):\n";
    cout << "  - Reduce sodium intake by avoiding processed foods, canned soups, pickles, soy sauce, and chips.\n";
    cout << "  - Incorporate potassium-rich foods like bananas, sweet potatoes, spinach, and white beans.\n";
    cout << "  - Eat heart-healthy options such as whole grains, lean proteins (like fish or skinless poultry), and unsalted nuts.\n";
    cout << "  - Drink plenty of water and avoid sugary drinks or excess caffeine.\n";

    cout << "\n>> For Diabetes:\n";
    cout << "  - Limit sugar-rich and refined carbohydrate foods such as white rice, white bread, pastries, and soda.\n";
    cout << "  - Opt for complex carbohydrates with low glycemic index like oats, brown rice, and whole wheat bread.\n";
    cout << "  - Include high-fiber vegetables (e.g., broccoli, spinach, carrots) and healthy fats (like avocado or olive oil).\n";
    cout << "  - Maintain consistent meal timing and portions to regulate blood sugar levels.\n";

    cout << "\n>> For Kidney Disease:\n";
    cout << "  - Avoid high-potassium foods such as bananas, oranges, potatoes, and tomatoes.\n";
    cout << "  - Limit phosphorus-rich foods like dairy products, nuts, seeds, and cola beverages.\n";
    cout << "  - Choose low-sodium, kidney-friendly meals like white rice, apples, cabbage, and lean chicken.\n";
    cout << "  - Stay hydrated but monitor fluid intake based on your doctor's advice.\n";

    cout << "\n>> For Stomach Ulcers or Peptic Ulcers:\n";
    cout << "  - Avoid acidic, spicy, and greasy foods such as citrus fruits, tomatoes, fried food, coffee, and alcohol.\n";
    cout << "  - Eat soothing, non-irritating foods like bananas, oatmeal, yogurt, boiled vegetables, and whole grains.\n";
    cout << "  - Try probiotic-rich foods (e.g., kefir, miso, sauerkraut) to support gut health.\n";
    cout << "  - Eat smaller, more frequent meals and avoid going to bed immediately after eating.\n";

    cout << "\nNote: The following dietary recommendations are based on widely accepted medical guidelines and are intended to support healthier choices. For personalized nutrition plans tailored to individual needs, consulting a healthcare professional is still advised.\n";

    pauseScreen();
}

// Deletes the current user's account from memory and file
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
            case 5: deleteAccount(member.username); return;
            case 6: return;
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
            } while (option != 2);
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
    loadUsersFromFile();	 // Load existing user data from file
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
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');	// Discard invalid input
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
    int systolicBP;               // Blood pressure (upper value)
    int diastolicBP;              // Blood pressure (lower value)
    int pulseRate;                // Heart beats per minute
    int respiratoryRate;          // Breaths per minute
    float bloodSugar;             // Blood sugar level in mg/dL
    float temperature;            // Body temperature in Â°C
    vector<string> foodLog;       // List of meals entered by the user
};

// Struct to store personal info, login credentials, and health info
struct Member {
    string username;              // Unique username for login
    string password;              // User password
    string name;                  // Full name of the user
    int age;                      // Age of the user
    float height;                 // Height in cm
    float weight;                 // Weight in kg
    string medicalCondition;      // Describes existing condition (e.g. diabetes)
    HealthData health;            // Nested structure holding health metrics
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
    system("cls");		// Windows command to clear screen
#else
    system("clear");	// Unix/Linux/Mac command to clear screen
#endif
}

// Pauses the screen so user can read outputs before continuing
void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear leftover input
    cin.get();
}

// ==================================
// SAVE AND LOAD USER DATA
// ==================================

// Function to save all users and their health/food data into a file
void saveUsersToFile() {
    ofstream outFile(USERS_FILE);	// Opens the file for writing (overwrites existing)
    
    // Loop through each user in the users vector
    for (const auto &u : users) {
    	 // Write basic info: username, password, name, age, weight, height, medical condition
        outFile << u.username << '\n'
                << u.password << '\n'
                << u.name << '\n'
                << u.age << '\n'
                << u.height << '\n'
                << u.weight << '\n'
                << u.medicalCondition << '\n'
                
        // Write health vitals as space-separated values
                << u.health.systolicBP << ' ' << u.health.diastolicBP << ' '
                << u.health.pulseRate << ' ' << u.health.respiratoryRate << ' '
                << u.health.bloodSugar << ' ' << u.health.temperature << '\n';
                
                // Write the number of food log entries
        outFile << u.health.foodLog.size() << '\n';
        
         // Write each food entry on a new line
        for (const auto &food : u.health.foodLog) {
            outFile << food << '\n';
        }
    }
    outFile.close();	// Always close files to save and release resources
}

// load all user data from file when the program starts
void loadUsersFromFile() {
    ifstream inFile(USERS_FILE);	 // Try to open file
    if (!inFile) return;	// If file doesn't exist, skip loading
    Member u;
    while (getline(inFile, u.username)) {	// Read user block-by-block
        getline(inFile, u.password);
        getline(inFile, u.name);
        inFile >> u.age >> u.height >> u.weight;
        inFile.ignore();
        getline(inFile, u.medicalCondition);
        
         // Read health vitals
        inFile >> u.health.systolicBP >> u.health.diastolicBP >> u.health.pulseRate
               >> u.health.respiratoryRate >> u.health.bloodSugar >> u.health.temperature;
        size_t foodCount;
        inFile >> foodCount;	 // Read how many food entries there are
        inFile.ignore();		 // Skip newline before reading food log
        u.health.foodLog.clear();
        
        // Read each food log entry line-by-line
        for (size_t i = 0; i < foodCount; ++i) {
            string food;
            getline(inFile, food);
            u.health.foodLog.push_back(food);
        }
        users.push_back(u);	// Store this user into the system
    }
    inFile.close();	 // Close the file after all users are loaded
}

// ==================================
// FUNCTION DECLARATIONS
// ==================================


void registerUser();
void viewProfile(const Member &member);
void inputVitals(Member &member);
void logFood(Member &member);
void showRecommendations();
void deleteAccount(const string &username);
void manageAccount(Member &member);
void login();

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
    cin.ignore();	// Clear input buffer before getline
    cout << "Enter full name: ";
    getline(cin, newMember.name);
    cout << "Enter age: ";
    cin >> newMember.age;
    cout << "Enter height (cm): ";
    cin >> newMember.height;
    cout << "Enter weight (kg): ";
    cin >> newMember.weight;
    cin.ignore();
    cout << "Enter medical condition (e.g., hypertension, diabetes, heart conditions): ";
    getline(cin, newMember.medicalCondition);
    
// Initialize health values with zero/defaults
    newMember.health = {0, 0, 0, 0, 0.0, 0.0, {}};
    
 // Add new user to global users list
    users.push_back(newMember);
    
// Save to file so data is not lost on exit
    saveUsersToFile();
    cout << "\nRegistration successful!";
    pauseScreen();	// Pause to allow user to read
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
    cout << "Age: " << member.age << "\n";
    cout << "Height: " << member.height << " cm\n";
    cout << "Weight: " << member.weight << " kg\n";
    cout << "Medical Condition: " << member.medicalCondition << "\n";
    cout << "Systolic BP: " << member.health.systolicBP << " mmHg\n";
    cout << "Diastolic BP: " << member.health.diastolicBP << " mmHg\n";
    cout << "Pulse Rate: " << member.health.pulseRate << " bpm\n";
    cout << "Respiratory Rate: " << member.health.respiratoryRate << " bpm\n";
    cout << "Blood Sugar: " << member.health.bloodSugar << " mg/dL\n";
    cout << "Temperature: " << member.health.temperature << " Â°C\n";
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
    if (member.health.systolicBP < 90 || member.health.systolicBP > 120)
        cout << "\n! Warning: Abnormal systolic blood pressure.";
    if (member.health.diastolicBP < 60 || member.health.diastolicBP > 80)
        cout << "\n! Warning: Abnormal diastolic blood pressure.";
    if (member.health.pulseRate < 60 || member.health.pulseRate > 100)
        cout << "\n! Warning: Abnormal pulse rate.";
    if (member.health.respiratoryRate < 12 || member.health.respiratoryRate > 20)
        cout << "\n! Warning: Abnormal respiratory rate.";
    if (member.health.bloodSugar > 140)
        cout << "\n! Warning: High blood sugar level.";
    if (member.health.temperature < 36.1 || member.health.temperature > 37.2)
        cout << "\n! Warning: Abnormal body temperature.";
        
    saveUsersToFile();	// Save new data after update
    pauseScreen();
}

// Logs a food/meals entry from user and checks if it's bad for their condition
void logFood(Member &member) {
    clearScreen();
    cout << "\n===== Food Logging =====\n";
    cin.ignore();  // Clear input buffer
    string meal;
    cout << "Enter meal description: ";
    getline(cin, meal);

    // Save the food log
    member.health.foodLog.push_back(meal);
    saveUsersToFile();

    // === Food Condition Warning System (Simplified) ===
    string condition = member.medicalCondition;

    // Basic warning checks using plain string comparison
    if (condition == "hypertension") {
        if (meal == "canned soup" || meal == "chips" || meal == "bacon") {
            cout << "\n? Warning: This food is high in sodium and not recommended for hypertension.";
        }
    }
    else if (condition == "diabetes") {
        if (meal == "soda" || meal == "cake" || meal == "white bread") {
            cout << "\n? Warning: This food may spike blood sugar and is not ideal for diabetes.";
        }
    }
    else if (condition == "kidney disease") {
        if (meal == "processed meat" || meal == "pickles" || meal == "dark soda") {
            cout << "\n? Warning: This food may be hard on the kidneys and should be limited.";
        }
    }
    else if (condition == "ulcer") {
        if (meal == "spicy food" || meal == "coffee" || meal == "alcohol") {
            cout << "\n? Warning: This food may irritate stomach ulcers.";
        }
    }
    else if (condition == "heart condition" || condition == "heart disease") {
        if (meal == "fried food" || meal == "red meat" || meal == "butter" || meal == "fast food") {
            cout << "\n? Warning: This food is high in saturated fat or cholesterol and may worsen heart conditions.";
        }
    }

    pauseScreen();
}


// dietary recommendations based on common medical conditions
void showRecommendations() {
    clearScreen();
    cout << "\n===== Dietary Recommendations =====\n";

    cout << "\n>> For Hypertension (High Blood Pressure):\n";
    cout << "  - Reduce sodium intake by avoiding processed foods, canned soups, pickles, soy sauce, and chips.\n";
    cout << "  - Incorporate potassium-rich foods like bananas, sweet potatoes, spinach, and white beans.\n";
    cout << "  - Eat heart-healthy options such as whole grains, lean proteins (like fish or skinless poultry), and unsalted nuts.\n";
    cout << "  - Drink plenty of water and avoid sugary drinks or excess caffeine.\n";

    cout << "\n>> For Diabetes:\n";
    cout << "  - Limit sugar-rich and refined carbohydrate foods such as white rice, white bread, pastries, and soda.\n";
    cout << "  - Opt for complex carbohydrates with low glycemic index like oats, brown rice, and whole wheat bread.\n";
    cout << "  - Include high-fiber vegetables (e.g., broccoli, spinach, carrots) and healthy fats (like avocado or olive oil).\n";
    cout << "  - Maintain consistent meal timing and portions to regulate blood sugar levels.\n";

    cout << "\n>> For Kidney Disease:\n";
    cout << "  - Avoid high-potassium foods such as bananas, oranges, potatoes, and tomatoes.\n";
    cout << "  - Limit phosphorus-rich foods like dairy products, nuts, seeds, and cola beverages.\n";
    cout << "  - Choose low-sodium, kidney-friendly meals like white rice, apples, cabbage, and lean chicken.\n";
    cout << "  - Stay hydrated but monitor fluid intake based on your doctor's advice.\n";

    cout << "\n>> For Stomach Ulcers or Peptic Ulcers:\n";
    cout << "  - Avoid acidic, spicy, and greasy foods such as citrus fruits, tomatoes, fried food, coffee, and alcohol.\n";
    cout << "  - Eat soothing, non-irritating foods like bananas, oatmeal, yogurt, boiled vegetables, and whole grains.\n";
    cout << "  - Try probiotic-rich foods (e.g., kefir, miso, sauerkraut) to support gut health.\n";
    cout << "  - Eat smaller, more frequent meals and avoid going to bed immediately after eating.\n";

    cout << "\nNote: The following dietary recommendations are based on widely accepted medical guidelines and are intended to support healthier choices. For personalized nutrition plans tailored to individual needs, consulting a healthcare professional is still advised.\n";

    pauseScreen();
}

// Deletes the current user's account from memory and file
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
            case 5: deleteAccount(member.username); return;
            case 6: return;
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
            } while (option != 2);
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
    loadUsersFromFile();	 // Load existing user data from file
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
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');	// Discard invalid input
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
    int systolicBP;               // Blood pressure (upper value)
    int diastolicBP;              // Blood pressure (lower value)
    int pulseRate;                // Heart beats per minute
    int respiratoryRate;          // Breaths per minute
    float bloodSugar;             // Blood sugar level in mg/dL
    float temperature;            // Body temperature in Â°C
    vector<string> foodLog;       // List of meals entered by the user
};

// Struct to store personal info, login credentials, and health info
struct Member {
    string username;              // Unique username for login
    string password;              // User password
    string name;                  // Full name of the user
    int age;                      // Age of the user
    float height;                 // Height in cm
    float weight;                 // Weight in kg
    string medicalCondition;      // Describes existing condition (e.g. diabetes)
    HealthData health;            // Nested structure holding health metrics
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
    system("cls");		// Windows command to clear screen
#else
    system("clear");	// Unix/Linux/Mac command to clear screen
#endif
}

// Pauses the screen so user can read outputs before continuing
void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear leftover input
    cin.get();
}

// ==================================
// SAVE AND LOAD USER DATA
// ==================================

// Function to save all users and their health/food data into a file
void saveUsersToFile() {
    ofstream outFile(USERS_FILE);	// Opens the file for writing (overwrites existing)
    
    // Loop through each user in the users vector
    for (const auto &u : users) {
    	 // Write basic info: username, password, name, age, weight, height, medical condition
        outFile << u.username << '\n'
                << u.password << '\n'
                << u.name << '\n'
                << u.age << '\n'
                << u.height << '\n'
                << u.weight << '\n'
                << u.medicalCondition << '\n'
                
        // Write health vitals as space-separated values
                << u.health.systolicBP << ' ' << u.health.diastolicBP << ' '
                << u.health.pulseRate << ' ' << u.health.respiratoryRate << ' '
                << u.health.bloodSugar << ' ' << u.health.temperature << '\n';
                
                // Write the number of food log entries
        outFile << u.health.foodLog.size() << '\n';
        
         // Write each food entry on a new line
        for (const auto &food : u.health.foodLog) {
            outFile << food << '\n';
        }
    }
    outFile.close();	// Always close files to save and release resources
}

// load all user data from file when the program starts
void loadUsersFromFile() {
    ifstream inFile(USERS_FILE);	 // Try to open file
    if (!inFile) return;	// If file doesn't exist, skip loading
    Member u;
    while (getline(inFile, u.username)) {	// Read user block-by-block
        getline(inFile, u.password);
        getline(inFile, u.name);
        inFile >> u.age >> u.height >> u.weight;
        inFile.ignore();
        getline(inFile, u.medicalCondition);
        
         // Read health vitals
        inFile >> u.health.systolicBP >> u.health.diastolicBP >> u.health.pulseRate
               >> u.health.respiratoryRate >> u.health.bloodSugar >> u.health.temperature;
        size_t foodCount;
        inFile >> foodCount;	 // Read how many food entries there are
        inFile.ignore();		 // Skip newline before reading food log
        u.health.foodLog.clear();
        
        // Read each food log entry line-by-line
        for (size_t i = 0; i < foodCount; ++i) {
            string food;
            getline(inFile, food);
            u.health.foodLog.push_back(food);
        }
        users.push_back(u);	// Store this user into the system
    }
    inFile.close();	 // Close the file after all users are loaded
}

// ==================================
// FUNCTION DECLARATIONS
// ==================================


void registerUser();
void viewProfile(const Member &member);
void inputVitals(Member &member);
void logFood(Member &member);
void showRecommendations();
void deleteAccount(const string &username);
void manageAccount(Member &member);
void login();

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
    cin.ignore();	// Clear input buffer before getline
    cout << "Enter full name: ";
    getline(cin, newMember.name);
    cout << "Enter age: ";
    cin >> newMember.age;
    cout << "Enter height (cm): ";
    cin >> newMember.height;
    cout << "Enter weight (kg): ";
    cin >> newMember.weight;
    cin.ignore();
    cout << "Enter medical condition (e.g., hypertension, diabetes, heart conditions): ";
    getline(cin, newMember.medicalCondition);
    
// Initialize health values with zero/defaults
    newMember.health = {0, 0, 0, 0, 0.0, 0.0, {}};
    
 // Add new user to global users list
    users.push_back(newMember);
    
// Save to file so data is not lost on exit
    saveUsersToFile();
    cout << "\nRegistration successful!";
    pauseScreen();	// Pause to allow user to read
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
    cout << "Age: " << member.age << "\n";
    cout << "Height: " << member.height << " cm\n";
    cout << "Weight: " << member.weight << " kg\n";
    cout << "Medical Condition: " << member.medicalCondition << "\n";
    cout << "Systolic BP: " << member.health.systolicBP << " mmHg\n";
    cout << "Diastolic BP: " << member.health.diastolicBP << " mmHg\n";
    cout << "Pulse Rate: " << member.health.pulseRate << " bpm\n";
    cout << "Respiratory Rate: " << member.health.respiratoryRate << " bpm\n";
    cout << "Blood Sugar: " << member.health.bloodSugar << " mg/dL\n";
    cout << "Temperature: " << member.health.temperature << " Â°C\n";
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
    if (member.health.systolicBP < 90 || member.health.systolicBP > 120)
        cout << "\n! Warning: Abnormal systolic blood pressure.";
    if (member.health.diastolicBP < 60 || member.health.diastolicBP > 80)
        cout << "\n! Warning: Abnormal diastolic blood pressure.";
    if (member.health.pulseRate < 60 || member.health.pulseRate > 100)
        cout << "\n! Warning: Abnormal pulse rate.";
    if (member.health.respiratoryRate < 12 || member.health.respiratoryRate > 20)
        cout << "\n! Warning: Abnormal respiratory rate.";
    if (member.health.bloodSugar > 140)
        cout << "\n! Warning: High blood sugar level.";
    if (member.health.temperature < 36.1 || member.health.temperature > 37.2)
        cout << "\n! Warning: Abnormal body temperature.";
        
    saveUsersToFile();	// Save new data after update
    pauseScreen();
}

// Logs a food/meals entry from user and checks if it's bad for their condition
void logFood(Member &member) {
    clearScreen();
    cout << "\n===== Food Logging =====\n";
    cin.ignore();  // Clear input buffer
    string meal;
    cout << "Enter meal description: ";
    getline(cin, meal);

    // Save the food log
    member.health.foodLog.push_back(meal);
    saveUsersToFile();

    // === Food Condition Warning System (Simplified) ===
    string condition = member.medicalCondition;

    // Basic warning checks using plain string comparison
    if (condition == "hypertension") {
        if (meal == "canned soup" || meal == "chips" || meal == "bacon") {
            cout << "\n? Warning: This food is high in sodium and not recommended for hypertension.";
        }
    }
    else if (condition == "diabetes") {
        if (meal == "soda" || meal == "cake" || meal == "white bread") {
            cout << "\n? Warning: This food may spike blood sugar and is not ideal for diabetes.";
        }
    }
    else if (condition == "kidney disease") {
        if (meal == "processed meat" || meal == "pickles" || meal == "dark soda") {
            cout << "\n? Warning: This food may be hard on the kidneys and should be limited.";
        }
    }
    else if (condition == "ulcer") {
        if (meal == "spicy food" || meal == "coffee" || meal == "alcohol") {
            cout << "\n? Warning: This food may irritate stomach ulcers.";
        }
    }
    else if (condition == "heart condition" || condition == "heart disease") {
        if (meal == "fried food" || meal == "red meat" || meal == "butter" || meal == "fast food") {
            cout << "\n? Warning: This food is high in saturated fat or cholesterol and may worsen heart conditions.";
        }
    }

    pauseScreen();
}


// dietary recommendations based on common medical conditions
void showRecommendations() {
    clearScreen();
    cout << "\n===== Dietary Recommendations =====\n";

    cout << "\n>> For Hypertension (High Blood Pressure):\n";
    cout << "  - Reduce sodium intake by avoiding processed foods, canned soups, pickles, soy sauce, and chips.\n";
    cout << "  - Incorporate potassium-rich foods like bananas, sweet potatoes, spinach, and white beans.\n";
    cout << "  - Eat heart-healthy options such as whole grains, lean proteins (like fish or skinless poultry), and unsalted nuts.\n";
    cout << "  - Drink plenty of water and avoid sugary drinks or excess caffeine.\n";

    cout << "\n>> For Diabetes:\n";
    cout << "  - Limit sugar-rich and refined carbohydrate foods such as white rice, white bread, pastries, and soda.\n";
    cout << "  - Opt for complex carbohydrates with low glycemic index like oats, brown rice, and whole wheat bread.\n";
    cout << "  - Include high-fiber vegetables (e.g., broccoli, spinach, carrots) and healthy fats (like avocado or olive oil).\n";
    cout << "  - Maintain consistent meal timing and portions to regulate blood sugar levels.\n";

    cout << "\n>> For Kidney Disease:\n";
    cout << "  - Avoid high-potassium foods such as bananas, oranges, potatoes, and tomatoes.\n";
    cout << "  - Limit phosphorus-rich foods like dairy products, nuts, seeds, and cola beverages.\n";
    cout << "  - Choose low-sodium, kidney-friendly meals like white rice, apples, cabbage, and lean chicken.\n";
    cout << "  - Stay hydrated but monitor fluid intake based on your doctor's advice.\n";

    cout << "\n>> For Stomach Ulcers or Peptic Ulcers:\n";
    cout << "  - Avoid acidic, spicy, and greasy foods such as citrus fruits, tomatoes, fried food, coffee, and alcohol.\n";
    cout << "  - Eat soothing, non-irritating foods like bananas, oatmeal, yogurt, boiled vegetables, and whole grains.\n";
    cout << "  - Try probiotic-rich foods (e.g., kefir, miso, sauerkraut) to support gut health.\n";
    cout << "  - Eat smaller, more frequent meals and avoid going to bed immediately after eating.\n";

    cout << "\nNote: The following dietary recommendations are based on widely accepted medical guidelines and are intended to support healthier choices. For personalized nutrition plans tailored to individual needs, consulting a healthcare professional is still advised.\n";

    pauseScreen();
}

// Deletes the current user's account from memory and file
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
            case 5: deleteAccount(member.username); return;
            case 6: return;
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
            } while (option != 2);
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
    loadUsersFromFile();	 // Load existing user data from file
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
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');	// Discard invalid input
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
