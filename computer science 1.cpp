#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>

using namespace std;

// Structure to represent a single bank account
struct Account {
    string ownerName;
    string password;
    double balance;
};

// Function Prototypes
void showMainMenu(vector<Account>& accounts);
void registerNewUser(vector<Account>& accounts);
int loginUser(const vector<Account>& accounts);
void showBankingMenu(Account& userAccount);

void showBalance(const Account& account);
void deposit(Account& account);
void withdraw(Account& account);

void loadAccountsFromFile(vector<Account>& accounts);
void saveAccountsToFile(const vector<Account>& accounts);

int main() {
    // A vector is a dynamic array to hold all account objects
    vector<Account> allAccounts;

    // Load existing accounts from the file when the program starts
    loadAccountsFromFile(allAccounts);

    cout << "--- Welcome to the Digital Bank ---\n";
    
    // Show the main menu until the user chooses to exit
    showMainMenu(allAccounts);

    // Save all accounts back to the file before the program closes
    saveAccountsToFile(allAccounts);
    
    cout << "All accounts have been saved. Thank you for visiting. Goodbye!\n";

    return 0;
}

// Displays the main menu for Login, Register, or Exit
void showMainMenu(vector<Account>& accounts) {
    int choice = 0;
    while (true) {
        cout << "\n** MAIN MENU **\n";
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. Exit Program\n";
        cout << "Your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                int userIndex = loginUser(accounts);
                if (userIndex != -1) {
                    // If login is successful, show the banking menu for that user
                    showBankingMenu(accounts[userIndex]);
                } else {
                    cout << "Login failed. Invalid username or password.\n";
                }
                break;
            }
            case 2:
                registerNewUser(accounts);
                break;
            case 3:
                return; // Exit the loop and the main function
            default:
                cout << "Invalid choice. Please select 1, 2, or 3.\n";
        }
    }
}

// Displays the banking menu for a logged-in user
void showBankingMenu(Account& userAccount) {
    int choice = 0;
    do {
        cout << "\n** Welcome, " << userAccount.ownerName << "! **\n";
        cout << "1. Show Balance\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cout << "Invalid Input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0; // Reset choice to avoid infinite loop
            continue;
        }

        switch (choice) {
            case 1:
                showBalance(userAccount);
                break;
            case 2:
                deposit(userAccount);
                break;
            case 3:
                withdraw(userAccount);
                break;
            case 4:
                cout << "You have been logged out.\n";
                break; // Exit the loop
            default:
                cout << "Invalid choice. Please enter a number between 1 and 4.\n";
        }
    } while (choice != 4);
}

// Handles the creation of a new user account
void registerNewUser(vector<Account>& accounts) {
    Account newAccount;
    cout << "\n--- New Account Registration ---\n";
    cout << "Please enter your full name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, newAccount.ownerName);

    cout << "Create a password: ";
    getline(cin, newAccount.password);

    cout << "Enter initial deposit amount: $";
    cin >> newAccount.balance;
    
    while(cin.fail() || newAccount.balance < 0) {
        cout << "Invalid amount. Please enter a positive number: $";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> newAccount.balance;
    }

    accounts.push_back(newAccount);
    cout << "Registration successful!\n";
}

// Handles the user login process
int loginUser(const vector<Account>& accounts) {
    string name, pass;
    cout << "\n--- User Login ---\n";
    cout << "Enter your full name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, name);
    cout << "Enter your password: ";
    getline(cin, pass);

    // Loop through all accounts to find a match
    for (int i = 0; i < accounts.size(); ++i) {
        if (accounts[i].ownerName == name && accounts[i].password == pass) {
            return i; // Return the index of the logged-in user
        }
    }
    return -1; // Return -1 if user is not found
}

// Displays the current balance of an account
void showBalance(const Account& account) {
    cout.precision(2);
    cout << fixed;
    cout << "------------------------\n";
    cout << "Account Holder: " << account.ownerName << "\n";
    cout << "Current balance is: $" << account.balance << '\n';
    cout << "------------------------\n";
}

// Adds money to an account
void deposit(Account& account) {
    double amount = 0;
    cout << "Enter amount to deposit: $";
    cin >> amount;
    if (cin.fail() || amount <= 0) {
        cout << "Invalid amount. Deposit must be positive.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } else {
        account.balance += amount;
        cout << "Deposit successful.\n";
        showBalance(account);
    }
}

// Removes money from an account
void withdraw(Account& account) {
    double amount = 0;
    cout << "Enter amount to withdraw: $";
    cin >> amount;
     if (cin.fail() || amount <= 0) {
        cout << "Invalid amount. Withdrawal must be positive.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } else if (amount > account.balance) {
        cout << "Insufficient balance.\n";
    } else {
        account.balance -= amount;
        cout << "Withdrawal successful.\n";
        showBalance(account);
    }
}

// Saves all account data to a text file
void saveAccountsToFile(const vector<Account>& accounts) {
    ofstream outFile("accounts_data.txt");
    if (!outFile) {
        cout << "Error: Unable to open file for saving.\n";
        return;
    }
    
    // Loop through the array using the classic for loop for C++98 compatibility
    for (size_t i = 0; i < accounts.size(); ++i) {
        outFile << accounts[i].ownerName << '\n';
        outFile << accounts[i].password << '\n';
        outFile << accounts[i].balance << '\n';
    }

    outFile.close();
}

// Loads all account data from a text file
void loadAccountsFromFile(vector<Account>& accounts) {
    ifstream inFile("accounts_data.txt");
    if (!inFile) {
        // If the file doesn't exist, it's not an error. It just means no accounts yet.
        return;
    }

    Account tempAccount;
    // Read from the file line by line until the end
    while (getline(inFile, tempAccount.ownerName) &&
           getline(inFile, tempAccount.password) &&
           (inFile >> tempAccount.balance)) {
        
        // This is important to consume the newline character after reading a number
        inFile.ignore(numeric_limits<streamsize>::max(), '\n'); 
        
        accounts.push_back(tempAccount);
    }

    inFile.close();
}
