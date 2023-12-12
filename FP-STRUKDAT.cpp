#include <iostream>
#include <vector>
#include <string>

using namespace std;

class FinancialEntity {
public:
    virtual void display() const = 0;  // Abstract method
    virtual ~FinancialEntity() {}      // Virtual destructor
};

class Transaction : public FinancialEntity {
private:
    int transaction_id;
    double amount;
    string description;

public:
    Transaction(int id, double amt, const string& desc) : transaction_id(id), amount(amt), description(desc) {}

    void display() const override {
        cout << "Transaction: " << transaction_id << ": $" << amount << " - " << description << endl;
    }

    double getAmount() const { return amount; }
};

class Account : public FinancialEntity {
private:
    static int nextAccountId; // Static variable for auto-incrementing account IDs
    int account_id;
    string owner;
    vector<Transaction*> transactions; // Using vector for dynamic transactions

public:
    Account(const string& own) : account_id(nextAccountId++), owner(own) {}

    ~Account() {
        // Destructor to deallocate dynamically allocated transactions
        for (Transaction* transaction : transactions) {
            delete transaction;
        }
    }

    void addTransaction(Transaction* transaction) {
        transactions.push_back(transaction);
    }

    void displayTransactions() const {
        for (const Transaction* transaction : transactions) {
            transaction->display();
        }
    }

    void display() const override {
        cout << "Account " << account_id << " - Owner: " << owner << endl;
        if (transactions.empty()) {
            cout << "No history yet" << endl;
        } else {
            displayTransactions();
        }
        cout << endl;
    }

    // Getter methods
    int getAccountId() const { return account_id; }
    const string& getOwner() const { return owner; }
};

// Initialize static variable
int Account::nextAccountId = 1;

class FinancialTrackingSystem {
private:
    vector<Account> accounts;
    vector<vector<double>> adjacencyMatrix;

public:
    void addAccount(const Account& account) {
        accounts.push_back(account);

        // Resize the adjacency matrix to accommodate the new account
        for (auto& row : adjacencyMatrix) {
            row.resize(accounts.size(), 0.0);
        }

        // Add a new row for the new account
        adjacencyMatrix.push_back(vector<double>(accounts.size(), 0.0));
    }

    Account* findAccountByName(const string& name) {
        for (auto& account : accounts) {
            if (account.getOwner() == name) {
                return &account;
            }
        }
        return nullptr;
    }

    void addTransactionBetweenAccounts(const string& fromAccountOwner, const string& toAccountOwner,
                                   Transaction* senderTransaction, Transaction* receiverTransaction) {
    Account* fromAccount = findAccountByName(fromAccountOwner);
    Account* toAccount = findAccountByName(toAccountOwner);

    if (fromAccount && toAccount) {
        // Debit transaction (money sent) for the sender
        fromAccount->addTransaction(senderTransaction);

        // Credit transaction (money received) for the receiver
        toAccount->addTransaction(receiverTransaction);

        // Update the adjacency matrix based on the transaction type (debit or credit)
        double transactionAmount = senderTransaction->getAmount();
        if (transactionAmount < 0) {  // Debit (money sent)
            adjacencyMatrix[fromAccount->getAccountId() - 1][toAccount->getAccountId() - 1] += transactionAmount;
        } else {  // Credit (money received)
            adjacencyMatrix[toAccount->getAccountId() - 1][fromAccount->getAccountId() - 1] += transactionAmount;
        }
    } else {
        cout << "Error: One or more accounts not found." << endl;
    }
}


    void displayAccounts() const {
        for (const auto& account : accounts) {
            account.display();
        }
    }
};

int main() {
    FinancialTrackingSystem financialSystem;

    int choice;
    do {
        cout << "Options:" << endl;
        cout << "1. Create New Account" << endl;
        cout << "2. Login Into Existing Account" << endl;
        cout << "3. Display Financial Tracker" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string name;
                cout << "Input Your Name: ";
                cin.ignore(); // Ignore any newline characters in the buffer
                getline(cin, name);

                Account* newAccount = new Account(name); // Allocate account dynamically
                financialSystem.addAccount(*newAccount);

                cout << "Account Created." << endl;
                break;
            }
            case 2: {
                string name;
                cout << "Input Account's Name: ";
                cin.ignore(); // Ignore any newline characters in the buffer
                getline(cin, name);

                Account* userAccount = financialSystem.findAccountByName(name);
                if (userAccount) {
                    char optionChar;
                    do {
                        cout << "Options:" << endl;
                        cout << "A. Send Money" << endl;
                        cout << "B. Display Transaction History" << endl;
                        cout << "Enter your choice: ";
                        cin >> optionChar;

                        switch (optionChar) {
                            case 'A': {
    double amount;
    string receiverName;

    cout << "Enter amount: $";
    cin >> amount;

    cout << "Receiver's Name: ";
    cin.ignore(); // Ignore any newline characters in the buffer
    getline(cin, receiverName);

    Transaction* senderTransaction = new Transaction(userAccount->getAccountId(), -amount, "Sent money to " + receiverName);
    Transaction* receiverTransaction = new Transaction(userAccount->getAccountId(), amount, "Received money from " + userAccount->getOwner());

    financialSystem.addTransactionBetweenAccounts(userAccount->getOwner(), receiverName, senderTransaction, receiverTransaction);

    break;
}

                            case 'B':
                                userAccount->displayTransactions();
                                break;
                            default:
                                cout << "Invalid option. Please try again." << endl;
                        }
                    } while (optionChar != 'A' && optionChar != 'B');
                } else {
                    cout << "Account not found." << endl;
                }
                break;
            }
            case 3:
                financialSystem.displayAccounts();
                break;
            case 4:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    } while (choice != 4);

    return 0;
}
