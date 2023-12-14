#include <iostream>
#include <vector>
#include <string>
#include <unistd.h> // For sleep() function

using namespace std;

class FinancialEntity
{
public:
    virtual void display() const = 0; // Abstract method
    virtual ~FinancialEntity() {}     // Virtual destructor
};

class Transaction : public FinancialEntity
{
private:
    int transaction_id;
    double amount;
    string description;

public:
    Transaction(int id, double amt, const string &desc) : transaction_id(id), amount(amt), description(desc) {}

    void display() const override
    {
        cout << "Transaction: " << transaction_id << ": $" << amount << " - " << description << endl;
    }

    double getAmount() const { return amount; }
};

class Account : public FinancialEntity
{
private:
    static int nextAccountId; // Static variable for auto-incrementing account IDs
    int account_id;
    string owner;
    vector<Transaction *> transactions; // Using vector for dynamic transactions

public:
    Account(const string &own) : account_id(nextAccountId++), owner(own) {}

    ~Account()
    {
        // Destructor to deallocate dynamically allocated transactions
        for (Transaction *transaction : transactions)
        {
            delete transaction;
        }
    }

    void addTransaction(Transaction *transaction)
    {
        transactions.push_back(transaction);
    }

    void displayTransactions() const
    {
        for (const Transaction *transaction : transactions)
        {
            transaction->display();
        }
    }

    void display() const override
    {
        cout << "Account " << account_id << " - Owner: " << owner << endl;
        if (transactions.empty())
        {
            cout << "\033[31m[!] No history yet\033[0m" << endl;
        }
        else
        {
            displayTransactions();
        }
        cout << endl;
    }

    // Getter methods
    int getAccountId() const { return account_id; }
    const string &getOwner() const { return owner; }
};

// Initialize static variable
int Account::nextAccountId = 1;

class FinancialTrackingSystem
{
private:
    vector<Account> accounts;
    vector<vector<double>> adjacencyMatrix;

public:
    void addAccount(const Account &account)
    {
        accounts.push_back(account);

        // Resize the adjacency matrix to accommodate the new account
        for (auto &row : adjacencyMatrix)
        {
            row.resize(accounts.size(), 0.0);
        }

        // Add a new row for the new account
        adjacencyMatrix.push_back(vector<double>(accounts.size(), 0.0));
    }

    Account *findAccountByName(const string &name)
    {
        for (auto &account : accounts)
        {
            if (account.getOwner() == name)
            {
                return &account;
            }
        }
        return nullptr;
    }

    void addTransactionBetweenAccounts(const string &fromAccountOwner, const string &toAccountOwner,
                                       Transaction *senderTransaction, Transaction *receiverTransaction)
    {
        Account *fromAccount = findAccountByName(fromAccountOwner);
        Account *toAccount = findAccountByName(toAccountOwner);

        if (fromAccount && toAccount)
        {
            // Debit transaction (money sent) for the sender
            fromAccount->addTransaction(senderTransaction);

            // Credit transaction (money received) for the receiver
            toAccount->addTransaction(receiverTransaction);

            // Update the adjacency matrix based on the transaction type (debit or credit)
            double transactionAmount = senderTransaction->getAmount();
            if (transactionAmount < 0)
            { // Debit (money sent)
                adjacencyMatrix[fromAccount->getAccountId() - 1][toAccount->getAccountId() - 1] += transactionAmount;
            }
            else
            { // Credit (money received)
                adjacencyMatrix[toAccount->getAccountId() - 1][fromAccount->getAccountId() - 1] += transactionAmount;
            }
        }
        else
        {
            cout << "\033[31m[!] Error: One or more accounts not found.\033[0m" << endl;
        }
    }

    void displayAccounts() const
    {
        if (accounts.empty())
        {
            cout << "\033[31m[!] Error: No accounts yet.\033[0m\n"
                 << endl;

            return;
        }
        else
        {
            for (const auto &account : accounts)
            {
                account.display();
            }
        }
    }
};

class Polymorphism
{
public:
    void getTransactionAmount(double &amount)
    {
        cout << "\033[34mEnter amount: $\033[0m";
        cin >> amount;
    }
    void getTransactionAmount(int &amount)
    {
        cout << "\033[34mEnter amount: $\033[0m";
        cin >> amount;
    }
    void getTransactionAmount(float &amount)
    {
        cout << "\033[34mEnter amount: $\033[0m";
        cin >> amount;
    }
};

int main()
{
    FinancialTrackingSystem financialSystem;

    int choice;
    do
    {
        cout << "\033[1;33m $$ Financial Tracking System $$ \033[0m" << endl;
        cout << "\033[33m=================================\033[0m" << endl;
        cout << "\033[1;34mOptions:\033[0m" << endl;
        cout << "\033[34m1. Create New Account\033[0m" << endl;
        cout << "\033[34m2. Login Into Existing Account\033[0m" << endl;
        cout << "\033[34m3. Display Financial Tracker\033[0m" << endl;
        cout << "\033[34m4. Exit\033[0m" << endl;
        cout << "\033[33m> Enter your choice: \033[0m";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            string name;
            cout << "\033[33mInput Your Name: \033[0m";
            cin.ignore(); // Ignore any newline characters in the buffer
            getline(cin, name);

            if (financialSystem.findAccountByName(name))
            {
                cout << "\033[31m[!] Error: Account with name '" << name << "' already exists.\033[0m\n"
                     << endl;
                break;
            }

            Account *newAccount = new Account(name); // Allocate account dynamically
            financialSystem.addAccount(*newAccount);

            cout << "\033[32m[!] Account with the name " << name << " created.\033[0m\n"
                 << endl;
            break;
        }
        case 2:
        {
            string name;
            cout << "\033[33m> Input Account's Name: \033[0m";
            cin.ignore(); // Ignore any newline characters in the buffer
            getline(cin, name);

            Account *userAccount = financialSystem.findAccountByName(name);
            if (userAccount)
            {
                char optionChar;
                do
                {
                    cout << "\033[32m[!] You are successfully logged in as " << name << "\033[0m\n" << endl;
                    cout << "\033[1;34mOptions:\033[0m" << endl;
                    cout << "\033[35mA. Send Money\033[0m" << endl;
                    cout << "\033[35mB. Display Transaction History\033[0m" << endl;
                    cout << "\033[33mEnter your choice: \033[0m";
                    cin >> optionChar;

                    switch (optionChar)
                    {
                    case 'A':
                    {
                        double amount;
                        string receiverName;
                        Polymorphism overloadOperator;

                        overloadOperator.getTransactionAmount(amount);

                        cout << "\033[32mReceiver's Name: \033[0m";
                        cin.ignore(); // Ignore any newline characters in the buffer
                        getline(cin, receiverName);

                        cout << "\n\033[32m[!] Transaction Successful.\033[0m\n"
                             << endl;

                        Transaction *senderTransaction = new Transaction(userAccount->getAccountId(), -amount, "\033[31m[!] Sent money to \033[0m" + receiverName);
                        Transaction *receiverTransaction = new Transaction(userAccount->getAccountId(), amount, "\033[32m[!] Received money from \033[0m" + userAccount->getOwner());

                        financialSystem.addTransactionBetweenAccounts(userAccount->getOwner(), receiverName, senderTransaction, receiverTransaction);

                        break;
                    }

                    case 'B':
                        userAccount->displayTransactions();
                        break;
                    default:
                        cout << "\033[31m[!] Error: Invalid option. Please try again.\033[0m" << endl;
                    }
                } while (optionChar != 'A' && optionChar != 'B');
            }
            else
            {
                cout << "\033[31m[!] Error: Account not found.\033[0m" << endl;
            }
            break;
        }
        case 3:
            financialSystem.displayAccounts();
            break;
        case 4:
            cout << "\033[31m[!] Exiting program.\033[0m" << endl;
            sleep(1);
            cout << "\033[31m[!] Exiting program..\033[0m" << endl;
            sleep(1);
            cout << "\033[31m[!] Exiting program...\033[0m" << endl;
            sleep(1);
            cout << "\033[1;31m[!] Program exited.\033[0m" << endl;
            break;
        default:
            cout << "\033[31m[!] Error: Invalid option. Please try again.\033[0m" << endl;
        }
    } while (choice != 4);

    return 0;
}
