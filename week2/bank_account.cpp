#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

// Transaction class to store transaction history
class Transaction {
private:
    std::string type;        // "deposit", "withdrawal", "transfer"
    double amount;
    std::string description;
    std::string date;        // In a real system, this would be a proper date type
    
public:
    // Constructor
    Transaction(std::string transType, double transAmount, 
                std::string transDescription, std::string transDate)
        : type(transType), amount(transAmount), 
          description(transDescription), date(transDate) {}
    
    // Getter methods
    std::string getType() const { return type; }
    double getAmount() const { return amount; }
    std::string getDescription() const { return description; }
    std::string getDate() const { return date; }
    
    // Display transaction details
    void display() const {
        std::cout << std::left << std::setw(12) << date 
                  << std::setw(12) << type
                  << std::right << std::setw(10) << std::fixed << std::setprecision(2) << amount
                  << "  " << std::left << description << std::endl;
    }
};

// BankAccount class demonstrating encapsulation
class BankAccount {
private:
    // Private data members
    std::string accountNumber;
    std::string accountHolderName;
    double balance;
    std::string accountType;  // "checking", "savings", etc.
    double interestRate;      // Annual interest rate (for savings accounts)
    std::string pin;          // PIN for account access
    bool locked;              // Account lock status
    std::vector<Transaction> transactionHistory;
    
    // Private helper methods
    bool validatePin(std::string enteredPin) const {
        return enteredPin == pin;
    }
    
    void recordTransaction(std::string type, double amount, std::string description) {
        // In a real system, we would get the current date
        std::string currentDate = "2025-06-27";  // Hardcoded for simplicity
        
        Transaction transaction(type, amount, description, currentDate);
        transactionHistory.push_back(transaction);
    }
    
public:
    // Constructor
    BankAccount(std::string number, std::string holderName, 
                std::string type, std::string initialPin)
        : accountNumber(number), accountHolderName(holderName), 
          accountType(type), pin(initialPin), locked(false) {
        
        balance = 0.0;
        
        // Set interest rate based on account type
        if (accountType == "savings") {
            interestRate = 0.025;  // 2.5% for savings
        } else if (accountType == "checking") {
            interestRate = 0.001;  // 0.1% for checking
        } else {
            interestRate = 0.0;    // No interest for other types
        }
    }
    
    // Getter methods
    std::string getAccountNumber() const {
        return accountNumber;
    }
    
    std::string getAccountHolderName() const {
        return accountHolderName;
    }
    
    std::string getAccountType() const {
        return accountType;
    }
    
    double getInterestRate() const {
        return interestRate;
    }
    
    bool isLocked() const {
        return locked;
    }
    
    // Balance can only be accessed after PIN verification
    double getBalance(std::string enteredPin) const {
        if (validatePin(enteredPin)) {
            return balance;
        } else {
            std::cout << "Error: Invalid PIN. Access denied." << std::endl;
            return -1.0;  // Indicate error
        }
    }
    
    // Setter methods
    void setAccountHolderName(std::string newName, std::string enteredPin) {
        if (validatePin(enteredPin)) {
            accountHolderName = newName;
            std::cout << "Account holder name updated successfully." << std::endl;
        } else {
            std::cout << "Error: Invalid PIN. Cannot update account holder name." << std::endl;
        }
    }
    
    void changePin(std::string currentPin, std::string newPin) {
        if (validatePin(currentPin)) {
            pin = newPin;
            std::cout << "PIN changed successfully." << std::endl;
        } else {
            std::cout << "Error: Invalid current PIN. Cannot change PIN." << std::endl;
        }
    }
    
    // Account operations
    void deposit(double amount, std::string description = "Deposit") {
        if (locked) {
            std::cout << "Error: Account is locked. Cannot make deposit." << std::endl;
            return;
        }
        
        if (amount <= 0) {
            std::cout << "Error: Deposit amount must be positive." << std::endl;
            return;
        }
        
        balance += amount;
        recordTransaction("deposit", amount, description);
        std::cout << "Deposit of $" << amount << " successful. New balance: $" << balance << std::endl;
    }
    
    bool withdraw(double amount, std::string enteredPin, std::string description = "Withdrawal") {
        if (locked) {
            std::cout << "Error: Account is locked. Cannot make withdrawal." << std::endl;
            return false;
        }
        
        if (!validatePin(enteredPin)) {
            std::cout << "Error: Invalid PIN. Withdrawal denied." << std::endl;
            return false;
        }
        
        if (amount <= 0) {
            std::cout << "Error: Withdrawal amount must be positive." << std::endl;
            return false;
        }
        
        if (amount > balance) {
            std::cout << "Error: Insufficient funds. Withdrawal denied." << std::endl;
            return false;
        }
        
        balance -= amount;
        recordTransaction("withdrawal", -amount, description);
        std::cout << "Withdrawal of $" << amount << " successful. New balance: $" << balance << std::endl;
        return true;
    }
    
    bool transfer(BankAccount& recipient, double amount, std::string enteredPin, 
                  std::string description = "Transfer") {
        if (locked) {
            std::cout << "Error: Account is locked. Cannot make transfer." << std::endl;
            return false;
        }
        
        if (recipient.isLocked()) {
            std::cout << "Error: Recipient account is locked. Cannot make transfer." << std::endl;
            return false;
        }
        
        if (!validatePin(enteredPin)) {
            std::cout << "Error: Invalid PIN. Transfer denied." << std::endl;
            return false;
        }
        
        if (amount <= 0) {
            std::cout << "Error: Transfer amount must be positive." << std::endl;
            return false;
        }
        
        if (amount > balance) {
            std::cout << "Error: Insufficient funds. Transfer denied." << std::endl;
            return false;
        }
        
        balance -= amount;
        recordTransaction("transfer", -amount, 
                          description + " to " + recipient.getAccountNumber());
        
        recipient.deposit(amount, "Transfer from " + accountNumber);
        
        std::cout << "Transfer of $" << amount << " to account " 
                  << recipient.getAccountNumber() << " successful. New balance: $" 
                  << balance << std::endl;
        return true;
    }
    
    void applyInterest() {
        if (locked) {
            std::cout << "Error: Account is locked. Cannot apply interest." << std::endl;
            return;
        }
        
        double interestAmount = balance * interestRate;
        balance += interestAmount;
        recordTransaction("interest", interestAmount, "Interest payment");
        std::cout << "Interest of $" << std::fixed << std::setprecision(2) << interestAmount 
                  << " applied. New balance: $" << balance << std::endl;
    }
    
    void lockAccount(std::string enteredPin) {
        if (validatePin(enteredPin)) {
            locked = true;
            std::cout << "Account locked successfully." << std::endl;
        } else {
            std::cout << "Error: Invalid PIN. Cannot lock account." << std::endl;
        }
    }
    
    void unlockAccount(std::string enteredPin) {
        if (validatePin(enteredPin)) {
            locked = false;
            std::cout << "Account unlocked successfully." << std::endl;
        } else {
            std::cout << "Error: Invalid PIN. Cannot unlock account." << std::endl;
        }
    }
    
    void displayTransactionHistory(std::string enteredPin) const {
        if (!validatePin(enteredPin)) {
            std::cout << "Error: Invalid PIN. Cannot display transaction history." << std::endl;
            return;
        }
        
        std::cout << "\n===== Transaction History for Account " << accountNumber << " =====" << std::endl;
        std::cout << std::left << std::setw(12) << "Date" 
                  << std::setw(12) << "Type"
                  << std::right << std::setw(10) << "Amount"
                  << "  " << std::left << "Description" << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        
        for (const auto& transaction : transactionHistory) {
            transaction.display();
        }
        
        std::cout << std::string(60, '-') << std::endl;
        std::cout << "Current Balance: $" << std::fixed << std::setprecision(2) << balance << std::endl;
    }
    
    void displayAccountInfo(std::string enteredPin) const {
        if (!validatePin(enteredPin)) {
            std::cout << "Error: Invalid PIN. Cannot display account information." << std::endl;
            return;
        }
        
        std::cout << "\n===== Account Information =====" << std::endl;
        std::cout << "Account Number: " << accountNumber << std::endl;
        std::cout << "Account Holder: " << accountHolderName << std::endl;
        std::cout << "Account Type: " << accountType << std::endl;
        std::cout << "Interest Rate: " << (interestRate * 100) << "%" << std::endl;
        std::cout << "Balance: $" << std::fixed << std::setprecision(2) << balance << std::endl;
        std::cout << "Status: " << (locked ? "Locked" : "Active") << std::endl;
        std::cout << "Transaction Count: " << transactionHistory.size() << std::endl;
    }
};

int main() {
    std::cout << "===== Bank Account Encapsulation Example =====" << std::endl;
    
    // Create a checking account
    BankAccount checkingAccount("C12345", "John Smith", "checking", "1234");
    
    // Create a savings account
    BankAccount savingsAccount("S67890", "John Smith", "savings", "5678");
    
    // Deposit money into accounts
    std::cout << "\n1. Making deposits:" << std::endl;
    checkingAccount.deposit(1000.0, "Initial deposit");
    savingsAccount.deposit(5000.0, "Initial deposit");
    
    // Display account information
    std::cout << "\n2. Account information:" << std::endl;
    checkingAccount.displayAccountInfo("1234");
    savingsAccount.displayAccountInfo("5678");
    
    // Try to access balance with incorrect PIN
    std::cout << "\n3. Trying to access balance with incorrect PIN:" << std::endl;
    double balance = checkingAccount.getBalance("9999");  // Wrong PIN
    
    // Access balance with correct PIN
    std::cout << "\n4. Accessing balance with correct PIN:" << std::endl;
    balance = checkingAccount.getBalance("1234");  // Correct PIN
    std::cout << "Checking account balance: $" << balance << std::endl;
    
    // Make withdrawals
    std::cout << "\n5. Making withdrawals:" << std::endl;
    checkingAccount.withdraw(300.0, "1234", "ATM withdrawal");
    
    // Try withdrawal with insufficient funds
    checkingAccount.withdraw(2000.0, "1234", "Large purchase");
    
    // Transfer between accounts
    std::cout << "\n6. Transferring between accounts:" << std::endl;
    checkingAccount.transfer(savingsAccount, 200.0, "1234", "Monthly savings");
    
    // Apply interest to savings account
    std::cout << "\n7. Applying interest to savings account:" << std::endl;
    savingsAccount.applyInterest();
    
    // Lock and unlock account
    std::cout << "\n8. Testing account locking:" << std::endl;
    checkingAccount.lockAccount("1234");
    
    // Try operations on locked account
    checkingAccount.deposit(100.0);
    checkingAccount.withdraw(50.0, "1234");
    
    // Unlock account
    checkingAccount.unlockAccount("1234");
    
    // Try operations after unlocking
    checkingAccount.deposit(100.0, "After unlocking");
    
    // Display transaction history
    std::cout << "\n9. Transaction history:" << std::endl;
    checkingAccount.displayTransactionHistory("1234");
    savingsAccount.displayTransactionHistory("5678");
    
    // Change PIN
    std::cout << "\n10. Changing PIN:" << std::endl;
    checkingAccount.changePin("1234", "4321");
    
    // Verify new PIN works
    checkingAccount.displayAccountInfo("4321");
    
    return 0;
}
