#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

class Account {
private:
    long long accountNumber;
    string accountHolderName;
    string accountType;
    double balance;
    string password;
    vector<string> transactions; // Transaction history

    string getCurrentTime() const {
        time_t now = time(nullptr);
        char buf[80];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        return buf;
    }

public:
    Account() : accountNumber(0), balance(0.0) {}

    void createAccount() {
        cout << "\nEnter Account Number: ";
        cin >> accountNumber;
        cin.ignore();
        cout << "Enter Account Holder Name: ";
        getline(cin, accountHolderName);
        cout << "Enter Account Type (Savings/Current): ";
        cin >> accountType;
        cout << "Enter Initial Balance: ";
        cin >> balance;
        cin.ignore();
        cout << "Set Account Password: ";
        getline(cin, password);
        transactions.push_back(getCurrentTime() + " | Account created with balance $" + to_string(balance));
        cout << "\nAccount created successfully!\n";
    }

    void showAccount() const {
        cout << "\nAccount Number: " << accountNumber
             << "\nHolder Name: " << accountHolderName
             << "\nAccount Type: " << accountType
             << "\nBalance: $" << balance << "\n";
    }

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            transactions.push_back(getCurrentTime() + " | Deposited $" + to_string(amount));
            cout << "Deposit successful!\n";
        } else {
            cout << "Invalid deposit amount!\n";
        }
    }

    void withdraw(double amount) {
        if (amount <= 0) {
            cout << "Invalid withdrawal amount!\n";
        } else if (amount > balance) {
            cout << "Insufficient funds!\n";
        } else {
            balance -= amount;
            transactions.push_back(getCurrentTime() + " | Withdrawn $" + to_string(amount));
            cout << "Withdrawal successful!\n";
        }
    }

    void checkBalance() {
        cout << "\n--- Account Details ---\n";
        cout << "Account Holder: " << accountHolderName << "\n";
        cout << "Account Number: " << accountNumber << "\n";
        cout << "Current Balance: $" << balance << "\n";
        transactions.push_back(getCurrentTime() + " | Balance checked");
    }

    long long getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }
    string getHolderName() const { return accountHolderName; }

    bool verifyPassword(const string &pass) const {
        return pass == password;
    }

    void modifyAccount() {
        cin.ignore();
        cout << "Enter New Account Holder Name: ";
        getline(cin, accountHolderName);
        cout << "Enter New Account Type: ";
        cin >> accountType;
        cout << "Account modified successfully!\n";
    }

    void showTransactionHistory() const {
        cout << "\n--- Transaction History for Account " << accountNumber << " ---\n";
        for (const auto &t : transactions) {
            cout << t << "\n";
        }
    }

    // File writing/reading
    void writeToFile(ofstream &out) const {
        size_t nameLen = accountHolderName.size();
        size_t typeLen = accountType.size();
        size_t passLen = password.size();
        size_t txnCount = transactions.size();

        out.write((char*)&accountNumber, sizeof(accountNumber));
        out.write((char*)&balance, sizeof(balance));

        out.write((char*)&nameLen, sizeof(nameLen));
        out.write(accountHolderName.c_str(), nameLen);

        out.write((char*)&typeLen, sizeof(typeLen));
        out.write(accountType.c_str(), typeLen);

        out.write((char*)&passLen, sizeof(passLen));
        out.write(password.c_str(), passLen);

        out.write((char*)&txnCount, sizeof(txnCount));
        for (const auto &txn : transactions) {
            size_t len = txn.size();
            out.write((char*)&len, sizeof(len));
            out.write(txn.c_str(), len);
        }
    }

    void readFromFile(ifstream &in) {
        size_t nameLen, typeLen, passLen, txnCount;
        char buffer[1024];

        in.read((char*)&accountNumber, sizeof(accountNumber));
        in.read((char*)&balance, sizeof(balance));

        in.read((char*)&nameLen, sizeof(nameLen));
        in.read(buffer, nameLen);
        accountHolderName.assign(buffer, nameLen);

        in.read((char*)&typeLen, sizeof(typeLen));
        in.read(buffer, typeLen);
        accountType.assign(buffer, typeLen);

        in.read((char*)&passLen, sizeof(passLen));
        in.read(buffer, passLen);
        password.assign(buffer, passLen);

        in.read((char*)&txnCount, sizeof(txnCount));
        transactions.clear();
        for (size_t i = 0; i < txnCount; i++) {
            size_t len;
            in.read((char*)&len, sizeof(len));
            in.read(buffer, len);
            transactions.push_back(string(buffer, len));
        }
    }
};

// Global functions
void saveAccountsToFile(const vector<Account> &accounts) {
    ofstream file("accounts.dat", ios::binary | ios::trunc);
    if (!file) {
        cerr << "Error opening file for saving!\n";
        return;
    }
    for (const auto &acc : accounts) {
        acc.writeToFile(file);
    }
    file.close();
}

void loadAccountsFromFile(vector<Account> &accounts) {
    ifstream file("accounts.dat", ios::binary);
    if (!file) {
        return;
    }
    Account temp;
    while (true) {
        temp.readFromFile(file);
        if (!file) break;
        accounts.push_back(temp);
    }
    file.close();
}

Account* findAccount(vector<Account> &accounts, int accNum) {
    for (auto &acc : accounts) {
        if (acc.getAccountNumber() == accNum)
            return &acc;
    }
    return nullptr;
}

// Menus
void administratorMenu(vector<Account> &accounts) {
    int choice;
    do {
        cout << "\n--- Administrator Menu ---\n";
        cout << "1. Create New Account\n";
        cout << "2. View All Accounts\n";
        cout << "3. Search for an Account\n";
        cout << "4. Delete an Account\n";
        cout << "5. Modify an Account\n";
        cout << "6. View Transaction History\n";
        cout << "7. Back to Main Menu\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            Account acc;
            acc.createAccount();
            accounts.push_back(acc);
            saveAccountsToFile(accounts);
        }
        else if (choice == 2) {
            for (const auto &acc : accounts) {
                acc.showAccount();
            }
        }
        else if (choice == 3) {
            int num;
            cout << "Enter Account Number: ";
            cin >> num;
            Account *acc = findAccount(accounts, num);
            if (acc) acc->showAccount();
            else cout << "Account not found!\n";
        }
        else if (choice == 4) {
            int num;
            cout << "Enter Account Number to delete: ";
            cin >> num;
            bool found = false;
            for (auto it = accounts.begin(); it != accounts.end(); ++it) {
                if (it->getAccountNumber() == num) {
                    accounts.erase(it);
                    saveAccountsToFile(accounts);
                    cout << "Account deleted successfully!\n";
                    found = true;
                    break;
                }
            }
            if (!found) cout << "Account not found!\n";
        }
        else if (choice == 5) {
            int num;
            cout << "Enter Account Number to modify: ";
            cin >> num;
            Account *acc = findAccount(accounts, num);
            if (acc) {
                acc->modifyAccount();
                saveAccountsToFile(accounts);
            } else {
                cout << "Account not found!\n";
            }
        }
        else if (choice == 6) {
            int num;
            cout << "Enter Account Number to view history: ";
            cin >> num;
            Account *acc = findAccount(accounts, num);
            if (acc) acc->showTransactionHistory();
            else cout << "Account not found!\n";
        }
    } while (choice != 7);
}

void customerMenu(vector<Account> &accounts) {
    long long accNum;
    string pass;
    cout << "Enter your Account Number: ";
    cin >> accNum;
    Account *acc = findAccount(accounts, accNum);
    if (!acc) {
        cout << "Account not found!\n";
        return;
    }
    cin.ignore();
    cout << "Enter Password: ";
    getline(cin, pass);
    if (!acc->verifyPassword(pass)) {
        cout << "Incorrect password!\n";
        return;
    }

    int choice;
    do {
        cout << "\n--- Customer Menu ---\n";
        cout << "1. Deposit Money\n";
        cout << "2. Withdraw Money\n";
        cout << "3. Check Balance\n";
        cout << "4. Back to Main Menu\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            double amt;
            cout << "Enter amount to deposit: ";
            cin >> amt;
            acc->deposit(amt);
            saveAccountsToFile(accounts);
        }
        else if (choice == 2) {
            double amt;
            cout << "Enter amount to withdraw: ";
            cin >> amt;
            acc->withdraw(amt);
            saveAccountsToFile(accounts);
        }
        else if (choice == 3) {
            acc->checkBalance();
            saveAccountsToFile(accounts);
        }
    } while (choice != 4);
}

int main() {
    vector<Account> accounts;
    loadAccountsFromFile(accounts);

    int choice;
    do {
        cout << "\n--- Main Menu ---\n";
        cout << "1. Administrator Menu\n";
        cout << "2. Customer Menu\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) administratorMenu(accounts);
        else if (choice == 2) customerMenu(accounts);
    } while (choice != 3);

    cout << "Thank you for Banking with Glowzy Bank PLC!\n";
    return 0;
}

