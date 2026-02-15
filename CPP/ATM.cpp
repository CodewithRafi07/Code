#include <iostream>
using namespace std;

int main() {
    double balance = 1000.00; // Starting balance
    int choice;
    double amount;

    cout << "============================\n";
    cout << "    Welcome to Simple ATM   \n";
    cout << "============================\n";

    while (true) {
        cout << "\nATM Menu:\n";
        cout << "1. Check Balance\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Your current balance is: $" << balance << "\n";
            break;

        case 2:
            cout << "Enter amount to deposit: $";
            cin >> amount;
            if (amount > 0) {
                balance += amount;
                cout << "Deposit successful! New balance: $" << balance << "\n";
            } else {
                cout << "Invalid deposit amount!\n";
            }
            break;

        case 3:
            cout << "Enter amount to withdraw: $";
            cin >> amount;
            if (amount > 0 && amount <= balance) {
                balance -= amount;
                cout << "Withdrawal successful! New balance: $" << balance << "\n";
            } else if (amount > balance) {
                cout << "Insufficient balance!\n";
            } else {
                cout << "Invalid withdrawal amount!\n";
            }
            break;

        case 4:
            cout << "Thank you for using our ATM. Goodbye!\n";
            return 0;

        default:
            cout << "Invalid choice! Please try again.\n";
        }
    }

    return 0;
}
