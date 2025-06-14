#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

struct Account {
    int accNo;
    char name[50];
    double balance;
};

void createAccount() {
    Account acc;
    ofstream outFile("bank.dat", ios::binary | ios::app);

    cout << "\nEnter Account Number: ";
    cin >> acc.accNo;
    cout << "Enter Account Holder Name: ";
    cin.ignore();
    cin.getline(acc.name, 50);
    cout << "Enter Initial Balance: ";
    cin >> acc.balance;

    outFile.write(reinterpret_cast<char*>(&acc), sizeof(acc));
    outFile.close();
    cout << "\nAccount Created Successfully!\n";
}

void showAccount(int accNo) {
    Account acc;
    ifstream inFile("bank.dat", ios::binary);
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(acc))) {
        if (acc.accNo == accNo) {
            cout << "\nAccount No: " << acc.accNo;
            cout << "\nName: " << acc.name;
            cout << "\nBalance: " << acc.balance << "\n";
            found = true;
            break;
        }
    }

    inFile.close();
    if (!found)
        cout << "\nAccount Not Found\n";
}

void depositWithdraw(int accNo, int option) {
    Account acc;
    fstream file("bank.dat", ios::binary | ios::in | ios::out);
    bool found = false;

    while (!file.eof() && !found) {
        streampos pos = file.tellg();
        file.read(reinterpret_cast<char*>(&acc), sizeof(acc));

        if (acc.accNo == accNo) {
            double amount;
            cout << "\nCurrent Balance: " << acc.balance;

            if (option == 1) {
                cout << "\nEnter Amount to Deposit: ";
                cin >> amount;
                acc.balance += amount;
            } else {
                cout << "\nEnter Amount to Withdraw: ";
                cin >> amount;
                if (amount <= acc.balance)
                    acc.balance -= amount;
                else {
                    cout << "\nInsufficient Balance!\n";
                    return;
                }
            }

            file.seekp(pos);
            file.write(reinterpret_cast<char*>(&acc), sizeof(acc));
            cout << "\nTransaction Completed.\n";
            found = true;
        }
    }

    file.close();
    if (!found)
        cout << "\nAccount Not Found\n";
}

void deleteAccount(int accNo) {
    Account acc;
    ifstream inFile("bank.dat", ios::binary);
    ofstream outFile("temp.dat", ios::binary);
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(acc))) {
        if (acc.accNo != accNo)
            outFile.write(reinterpret_cast<char*>(&acc), sizeof(acc));
        else
            found = true;
    }

    inFile.close();
    outFile.close();

    remove("bank.dat");
    rename("temp.dat", "bank.dat");

    if (found)
        cout << "\nAccount Deleted.\n";
    else
        cout << "\nAccount Not Found.\n";
}

void listAccounts() {
    Account acc;
    ifstream inFile("bank.dat", ios::binary);

    cout << "\nAll Account Holders:\n";
    cout << left << setw(10) << "AccNo" << setw(20) << "Name" << setw(10) << "Balance" << "\n";
    cout << "------------------------------------------\n";

    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(acc))) {
        cout << left << setw(10) << acc.accNo
             << setw(20) << acc.name
             << setw(10) << acc.balance << "\n";
    }

    inFile.close();
}

void menu() {
    int choice, accNo;

    do {
        cout << "\n--- Bank Management Menu ---\n";
        cout << "1. Create Account\n";
        cout << "2. View Account\n";
        cout << "3. Deposit\n";
        cout << "4. Withdraw\n";
        cout << "5. Delete Account\n";
        cout << "6. List All Accounts\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                createAccount(); break;
            case 2:
                cout << "Enter Account No: ";
                cin >> accNo;
                showAccount(accNo);
                break;
            case 3:
                cout << "Enter Account No: ";
                cin >> accNo;
                depositWithdraw(accNo, 1);
                break;
            case 4:
                cout << "Enter Account No: ";
                cin >> accNo;
                depositWithdraw(accNo, 2);
                break;
            case 5:
                cout << "Enter Account No to delete: ";
                cin >> accNo;
                deleteAccount(accNo);
                break;
            case 6:
                listAccounts(); break;
            case 7:
                cout << "Exiting...\n"; break;
            default:
                cout << "Invalid Option\n";
        }
    } while (choice != 7);
}

int main() {
    menu();
    return 0;
}
