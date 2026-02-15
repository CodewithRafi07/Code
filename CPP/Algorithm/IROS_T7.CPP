#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <limits>

using namespace std;

// Utility function to clear the console
void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Utility function to pause the console
void pauseConsole() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Forward declarations
class InventoryManager;

// Product class
class Product {
private:
    string productID;
    string name;
    string category;
    double price;
    int stockQuantity;
    double discount; // Product-specific discount percentage

public:
    Product(string id = "", string n = "", string cat = "", double p = 0.0, int stock = 0, double disc = 0.0)
        : productID(id), name(n), category(cat), price(p), stockQuantity(stock), discount(disc) {}

    // Getters
    string getProductID() const { return productID; }
    string getName() const { return name; }
    string getCategory() const { return category; }
    double getPrice() const { return price; }
    int getStockQuantity() const { return stockQuantity; }
    double getDiscount() const { return discount; }

    // Setters
    void setProductID(string id) { productID = id; }
    void setName(string n) { name = n; }
    void setCategory(string cat) { category = cat; }
    void setPrice(double p) { price = p; }
    void setStockQuantity(int stock) { stockQuantity = stock; }
    void setDiscount(double disc) { discount = disc; }

    // Decrease stock
    void decreaseStock(int amount) {
        if (amount <= stockQuantity) {
            stockQuantity -= amount;
        }
    }

    // Increase stock
    void increaseStock(int amount) {
        stockQuantity += amount;
    }

    // Display product information
    void displayInfo() const {
        cout << left << setw(10) << productID << setw(20) << name << setw(15) << category 
             << setw(10) << price << setw(10) << stockQuantity << setw(10) << discount << "%" << endl;
    }

    // Convert to string for file storage
    string toString() const {
        return productID + "," + name + "," + category + "," + to_string(price) + "," + 
               to_string(stockQuantity) + "," + to_string(discount);
    }

    // Create product from string (for file loading)
    static Product fromString(const string& str) {
        stringstream ss(str);
        string id, name, category, priceStr, stockStr, discountStr;
        
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, category, ',');
        getline(ss, priceStr, ',');
        getline(ss, stockStr, ',');
        getline(ss, discountStr, ',');
        
        return Product(id, name, category, stod(priceStr), stoi(stockStr), stod(discountStr));
    }
};

// Supplier class
class Supplier {
private:
    string name;
    string contactInfo;
    string address;
    vector<string> suppliedProductIDs; // IDs of products supplied by this supplier

public:
    Supplier(string n = "", string contact = "", string addr = "")
        : name(n), contactInfo(contact), address(addr) {}

    // Getters
    string getName() const { return name; }
    string getContactInfo() const { return contactInfo; }
    string getAddress() const { return address; }
    vector<string> getSuppliedProductIDs() const { return suppliedProductIDs; }

    // Setters
    void setName(string n) { name = n; }
    void setContactInfo(string contact) { contactInfo = contact; }
    void setAddress(string addr) { address = addr; }

    // Add a product ID to the list of supplied products
    void addSuppliedProduct(const string& productID) {
        if (find(suppliedProductIDs.begin(), suppliedProductIDs.end(), productID) == suppliedProductIDs.end()) {
            suppliedProductIDs.push_back(productID);
        }
    }

    // Remove a product ID from the list of supplied products
    void removeSuppliedProduct(const string& productID) {
        auto it = find(suppliedProductIDs.begin(), suppliedProductIDs.end(), productID);
        if (it != suppliedProductIDs.end()) {
            suppliedProductIDs.erase(it);
        }
    }

    // Display supplier information
    void displayInfo() const {
        cout << "Name: " << name << endl;
        cout << "Contact: " << contactInfo << endl;
        cout << "Address: " << address << endl;
        cout << "Supplied Product IDs: ";
        for (const auto& id : suppliedProductIDs) {
            cout << id << " ";
        }
        cout << endl;
    }

    // Convert to string for file storage
    string toString() const {
        string result = name + "," + contactInfo + "," + address;
        for (const auto& id : suppliedProductIDs) {
            result += "," + id;
        }
        return result;
    }

    // Create supplier from string (for file loading)
    static Supplier fromString(const string& str) {
        stringstream ss(str);
        string name, contact, address, token;
        vector<string> productIDs;
        
        getline(ss, name, ',');
        getline(ss, contact, ',');
        getline(ss, address, ',');
        
        Supplier supplier(name, contact, address);
        
        while (getline(ss, token, ',')) {
            if (!token.empty()) {
                supplier.addSuppliedProduct(token);
            }
        }
        
        return supplier;
    }
};

// Transaction class to store transaction details
class Transaction {
private:
    string invoiceID;
    string customerUsername;
    vector<pair<string, int>> purchasedItems; // Product ID and quantity
    double subtotal;
    double discountAmount;
    double totalAmount;
    int pointsRedeemed;
    int pointsEarned;
    string date;

public:
    Transaction(string invID = "", string custUN = "", vector<pair<string, int>> items = {}, 
                double sub = 0.0, double disc = 0.0, double total = 0.0, 
                int redeemed = 0, int earned = 0, string d = "")
        : invoiceID(invID), customerUsername(custUN), purchasedItems(items), subtotal(sub),
          discountAmount(disc), totalAmount(total), pointsRedeemed(redeemed), 
          pointsEarned(earned), date(d) {}

    // Getters
    string getInvoiceID() const { return invoiceID; }
    string getCustomerUsername() const { return customerUsername; }
    vector<pair<string, int>> getPurchasedItems() const { return purchasedItems; }
    double getSubtotal() const { return subtotal; }
    double getDiscountAmount() const { return discountAmount; }
    double getTotalAmount() const { return totalAmount; }
    int getPointsRedeemed() const { return pointsRedeemed; }
    int getPointsEarned() const { return pointsEarned; }
    string getDate() const { return date; }

    // Display transaction details
    void displayDetails(const InventoryManager& inventory) const;

    // Convert to string for file storage
    string toString() const {
        string result = invoiceID + "," + customerUsername + "," + to_string(subtotal) + "," + 
                       to_string(discountAmount) + "," + to_string(totalAmount) + "," + 
                       to_string(pointsRedeemed) + "," + to_string(pointsEarned) + "," + date;
        
        for (const auto& item : purchasedItems) {
            result += "," + item.first + ":" + to_string(item.second);
        }
        
        return result;
    }

    // Create transaction from string (for file loading)
    static Transaction fromString(const string& str) {
        stringstream ss(str);
        string invoiceID, customerUsername, subtotalStr, discountStr, totalStr;
        string redeemedStr, earnedStr, date, token;
        vector<pair<string, int>> purchasedItems;
        
        getline(ss, invoiceID, ',');
        getline(ss, customerUsername, ',');
        getline(ss, subtotalStr, ',');
        getline(ss, discountStr, ',');
        getline(ss, totalStr, ',');
        getline(ss, redeemedStr, ',');
        getline(ss, earnedStr, ',');
        getline(ss, date, ',');
        
        while (getline(ss, token, ',')) {
            if (!token.empty()) {
                size_t colonPos = token.find(':');
                if (colonPos != string::npos) {
                    string productID = token.substr(0, colonPos);
                    int quantity = stoi(token.substr(colonPos + 1));
                    purchasedItems.push_back(make_pair(productID, quantity));
                }
            }
        }
        
        return Transaction(
            invoiceID, customerUsername, purchasedItems,
            stod(subtotalStr), stod(discountStr), stod(totalStr),
            stoi(redeemedStr), stoi(earnedStr), date
        );
    }
};

// User class (base class)
class User {
protected:
    string username;
    string password;
    string role; // "admin" or "customer"

public:
    User(string uname = "", string pwd = "", string r = "")
        : username(uname), password(pwd), role(r) {}

    virtual ~User() {}

    // Getters
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getRole() const { return role; }

    // Setters
    void setUsername(string uname) { username = uname; }
    void setPassword(string pwd) { password = pwd; }
    void setRole(string r) { role = r; }

    // Display user information
    virtual void displayInfo() const {
        cout << "Username: " << username << endl;
        cout << "Role: " << role << endl;
    }

    // Convert to string for file storage
    string toString() const {
        return username + "," + password + "," + role;
    }

    // Create user from string (for file loading)
    static User* fromString(const string& str);
};

// Admin class (inherits from User)
class Admin : public User {
public:
    Admin(string uname = "", string pwd = "")
        : User(uname, pwd, "admin") {}

    void displayInfo() const override {
        cout << "Admin Profile" << endl;
        cout << "Username: " << username << endl;
    }
};

// Customer class (inherits from User)
class Customer : public User {
private:
    int loyaltyPoints;
    vector<string> purchaseHistory; // Invoice IDs

public:
    Customer(string uname = "", string pwd = "", int points = 0)
        : User(uname, pwd, "customer"), loyaltyPoints(points) {}

    // Getters
    int getLoyaltyPoints() const { return loyaltyPoints; }
    vector<string> getPurchaseHistory() const { return purchaseHistory; }

    // Setters
    void setLoyaltyPoints(int points) { loyaltyPoints = points; }

    // Add loyalty points
    void addLoyaltyPoints(int points) {
        loyaltyPoints += points;
    }

    // Deduct loyalty points
    bool deductLoyaltyPoints(int points) {
        if (loyaltyPoints >= points) {
            loyaltyPoints -= points;
            return true;
        }
        return false;
    }

    // Add invoice to purchase history
    void addToPurchaseHistory(const string& invoiceID) {
        purchaseHistory.push_back(invoiceID);
    }

    void displayInfo() const override {
        cout << "Customer Profile" << endl;
        cout << "Username: " << username << endl;
        cout << "Loyalty Points: " << loyaltyPoints << endl;
    }

    // Convert to string for file storage
    string toString() const {
        string result = username + "," + password + "," + role + "," + to_string(loyaltyPoints);
        for (const auto& invoiceID : purchaseHistory) {
            result += "," + invoiceID;
        }
        return result;
    }

    // Create customer from string (for file loading)
    static Customer* fromString(const string& str) {
        stringstream ss(str);
        string username, password, role, pointsStr, token;
        vector<string> purchaseHistory;
        
        getline(ss, username, ',');
        getline(ss, password, ',');
        getline(ss, role, ',');
        getline(ss, pointsStr, ',');
        
        Customer* customer = new Customer(username, password, stoi(pointsStr));
        
        while (getline(ss, token, ',')) {
            if (!token.empty()) {
                customer->addToPurchaseHistory(token);
            }
        }
        
        return customer;
    }
};

// InventoryManager class
class InventoryManager {
private:
    vector<Product> products;
    vector<Supplier> suppliers;

public:
    // Add a product
    void addProduct(const Product& product) {
        products.push_back(product);
    }

    // Remove a product by ID
    bool removeProduct(const string& productID) {
        for (auto it = products.begin(); it != products.end(); ++it) {
            if (it->getProductID() == productID) {
                products.erase(it);
                return true;
            }
        }
        return false;
    }

    // Update a product
    bool updateProduct(const string& productID, const Product& updatedProduct) {
        for (auto& product : products) {
            if (product.getProductID() == productID) {
                product = updatedProduct;
                return true;
            }
        }
        return false;
    }

    // Get product by ID (non-const version)
    Product* getProductByID(const string& productID) {
        for (auto& product : products) {
            if (product.getProductID() == productID) {
                return &product;
            }
        }
        return nullptr;
    }

    // Get product by ID (const version)
    const Product* getProductByID(const string& productID) const {
        for (const auto& product : products) {
            if (product.getProductID() == productID) {
                return &product;
            }
        }
        return nullptr;
    }

    // Get all products
    vector<Product> getProducts() const {
        return products;
    }

    // Add a supplier
    void addSupplier(const Supplier& supplier) {
        suppliers.push_back(supplier);
    }

    // Remove a supplier by name
    bool removeSupplier(const string& supplierName) {
        for (auto it = suppliers.begin(); it != suppliers.end(); ++it) {
            if (it->getName() == supplierName) {
                suppliers.erase(it);
                return true;
            }
        }
        return false;
    }

    // Update a supplier
    bool updateSupplier(const string& supplierName, const Supplier& updatedSupplier) {
        for (auto& supplier : suppliers) {
            if (supplier.getName() == supplierName) {
                supplier = updatedSupplier;
                return true;
            }
        }
        return false;
    }

    // Get supplier by name
    Supplier* getSupplierByName(const string& supplierName) {
        for (auto& supplier : suppliers) {
            if (supplier.getName() == supplierName) {
                return &supplier;
            }
        }
        return nullptr;
    }

    // Get all suppliers
    vector<Supplier> getSuppliers() const {
        return suppliers;
    }

    // Display all products
    void displayAllProducts() const {
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(15) << "Category" 
             << setw(10) << "Price" << setw(10) << "Stock" << setw(10) << "Discount" << endl;
        cout << string(75, '-') << endl;
        
        for (const auto& product : products) {
            product.displayInfo();
        }
    }

    // Display all suppliers
    void displayAllSuppliers() const {
        for (const auto& supplier : suppliers) {
            supplier.displayInfo();
            cout << endl;
        }
    }

    // Check for low stock and alert
    void checkLowStock() const {
        bool lowStockFound = false;
        cout << "\nLow Stock Alert (Stock < 15 units):" << endl;
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Stock" << endl;
        cout << string(40, '-') << endl;
        
        for (const auto& product : products) {
            if (product.getStockQuantity() < 15) {
                cout << left << setw(10) << product.getProductID() << setw(20) << product.getName() 
                     << setw(10) << product.getStockQuantity() << endl;
                lowStockFound = true;
            }
        }
        
        if (!lowStockFound) {
            cout << "No products with low stock." << endl;
        }
    }

    // Save products to file
    void saveProductsToFile(const string& filename) const {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            for (const auto& product : products) {
                outFile << product.toString() << endl;
            }
            outFile.close();
        }
    }

    // Load products from file
    void loadProductsFromFile(const string& filename) {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            products.clear();
            string line;
            while (getline(inFile, line)) {
                if (!line.empty()) {
                    products.push_back(Product::fromString(line));
                }
            }
            inFile.close();
        }
    }

    // Save suppliers to file
    void saveSuppliersToFile(const string& filename) const {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            for (const auto& supplier : suppliers) {
                outFile << supplier.toString() << endl;
            }
            outFile.close();
        }
    }

    // Load suppliers from file
    void loadSuppliersFromFile(const string& filename) {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            suppliers.clear();
            string line;
            while (getline(inFile, line)) {
                if (!line.empty()) {
                    suppliers.push_back(Supplier::fromString(line));
                }
            }
            inFile.close();
        }
    }
};

// TransactionManager class
class TransactionManager {
private:
    vector<Transaction> transactions;
    double seasonalDiscount; // Global seasonal discount percentage

public:
    TransactionManager() : seasonalDiscount(0.0) {}

    // Getters
    double getSeasonalDiscount() const { return seasonalDiscount; }
    vector<Transaction> getTransactions() const { return transactions; }

    // Set seasonal discount
    void setSeasonalDiscount(double discount) {
        seasonalDiscount = discount;
    }

    // Add a transaction
    void addTransaction(const Transaction& transaction) {
        transactions.push_back(transaction);
    }

    // Get transaction by invoice ID
    Transaction* getTransactionByID(const string& invoiceID) {
        for (auto& transaction : transactions) {
            if (transaction.getInvoiceID() == invoiceID) {
                return &transaction;
            }
        }
        return nullptr;
    }

    // Generate a unique invoice ID
    string generateInvoiceID() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        
        int year = 1900 + ltm->tm_year;
        int month = 1 + ltm->tm_mon;
        int day = ltm->tm_mday;
        
        // Format: INV_YYYYMMDD_XXX where XXX is a sequence number
        string baseID = "INV_" + to_string(year) + 
                       (month < 10 ? "0" : "") + to_string(month) + 
                       (day < 10 ? "0" : "") + to_string(day);
        
        // Find the highest sequence number for today
        int maxSeq = 0;
        for (const auto& trans : transactions) {
            string invID = trans.getInvoiceID();
            if (invID.find(baseID) == 0) {
                string seqStr = invID.substr(baseID.length() + 1); // +1 for underscore
                try {
                    int seq = stoi(seqStr);
                    if (seq > maxSeq) {
                        maxSeq = seq;
                    }
                } catch (...) {
                    // Ignore if conversion fails
                }
            }
        }
        
        // New sequence number is max + 1
        int newSeq = maxSeq + 1;
        return baseID + "_" + (newSeq < 100 ? (newSeq < 10 ? "00" : "0") : "") + to_string(newSeq);
    }

    // Get current date as string
    string getCurrentDate() const {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        
        int year = 1900 + ltm->tm_year;
        int month = 1 + ltm->tm_mon;
        int day = ltm->tm_mday;
        
        return to_string(year) + "-" + 
               (month < 10 ? "0" : "") + to_string(month) + "-" + 
               (day < 10 ? "0" : "") + to_string(day);
    }

    // Calculate final price after all discounts
    double calculateFinalPrice(const Product& product) const {
        double productDiscount = product.getDiscount() / 100.0 * product.getPrice();
        double seasonalDiscountAmount = seasonalDiscount / 100.0 * product.getPrice();
        return product.getPrice() - productDiscount - seasonalDiscountAmount;
    }

    // Process checkout
    string processCheckout(Customer& customer, const vector<pair<string, int>>& cart, InventoryManager& inventory) {
        if (cart.empty()) {
            cout << "Cart is empty. Cannot proceed with checkout." << endl;
            return "";
        }
        
        // Calculate subtotal
        double subtotal = 0.0;
        vector<pair<string, int>> validItems;
        
        for (const auto& item : cart) {
            Product* product = inventory.getProductByID(item.first);
            if (product && product->getStockQuantity() >= item.second) {
                double finalPrice = calculateFinalPrice(*product);
                subtotal += finalPrice * item.second;
                validItems.push_back(item);
            }
        }
        
        if (validItems.empty()) {
            cout << "No valid items in cart. Cannot proceed with checkout." << endl;
            return "";
        }
        
        // Ask if customer wants to redeem loyalty points
        int pointsToRedeem = 0;
        if (customer.getLoyaltyPoints() > 0) {
            cout << "You have " << customer.getLoyaltyPoints() << " loyalty points." << endl;
            cout << "Would you like to redeem points? (y/n): ";
            char choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (tolower(choice) == 'y') {
                cout << "Enter points to redeem (10 points = 1 Taka): ";
                cin >> pointsToRedeem;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                // Validate points
                if (pointsToRedeem > customer.getLoyaltyPoints()) {
                    cout << "You don't have enough points. Using maximum available points." << endl;
                    pointsToRedeem = customer.getLoyaltyPoints();
                }
                
                // Round down to nearest 10
                pointsToRedeem = pointsToRedeem / 10 * 10;
            }
        }
        
        // Calculate discount from points
        double pointsDiscount = pointsToRedeem / 10.0;
        double totalAmount = subtotal - pointsDiscount;
        
        // Ensure total is not negative
        if (totalAmount < 0) {
            totalAmount = 0;
            pointsDiscount = subtotal * 10; // Adjust points discount to match subtotal
        }
        
        // Calculate points earned (1 point per 1 Taka spent)
        int pointsEarned = static_cast<int>(totalAmount);
        
        // Generate invoice ID
        string invoiceID = generateInvoiceID();
        
        // Create transaction
        Transaction transaction(
            invoiceID, 
            customer.getUsername(), 
            validItems, 
            subtotal, 
            pointsDiscount, 
            totalAmount, 
            pointsToRedeem, 
            pointsEarned, 
            getCurrentDate()
        );
        
        // Add transaction to history
        addTransaction(transaction);
        
        // Update customer loyalty points
        customer.deductLoyaltyPoints(pointsToRedeem);
        customer.addLoyaltyPoints(pointsEarned);
        customer.addToPurchaseHistory(invoiceID);
        
        // Update inventory
        for (const auto& item : validItems) {
            Product* product = inventory.getProductByID(item.first);
            if (product) {
                product->decreaseStock(item.second);
            }
        }
        
        // Generate invoice file
        generateInvoiceFile(transaction, inventory);
        
        return invoiceID;
    }

    // Generate invoice file
    void generateInvoiceFile(const Transaction& transaction, const InventoryManager& inventory) {
        string filename = "Invoice_" + transaction.getInvoiceID().substr(4) + ".txt";
        ofstream outFile(filename);
        
        if (outFile.is_open()) {
            outFile << "========================================" << endl;
            outFile << "            I-ROS INVOICE              " << endl;
            outFile << "========================================" << endl;
            outFile << "Invoice ID: " << transaction.getInvoiceID() << endl;
            outFile << "Date: " << transaction.getDate() << endl;
            outFile << "Customer: " << transaction.getCustomerUsername() << endl;
            outFile << endl;
            outFile << "Purchased Items:" << endl;
            outFile << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Qty" 
                    << setw(10) << "Price" << setw(15) << "Total" << endl;
            outFile << string(65, '-') << endl;
            
            for (const auto& item : transaction.getPurchasedItems()) {
                const Product* product = inventory.getProductByID(item.first);
                if (product) {
                    double itemTotal = product->getPrice() * item.second;
                    outFile << left << setw(10) << product->getProductID() << setw(20) << product->getName() 
                            << setw(10) << item.second << setw(10) << product->getPrice() 
                            << setw(15) << itemTotal << endl;
                }
            }
            
            outFile << string(65, '-') << endl;
            outFile << right << setw(55) << "Subtotal: " << transaction.getSubtotal() << endl;
            outFile << right << setw(55) << "Discount: " << transaction.getDiscountAmount() << endl;
            outFile << right << setw(55) << "Total: " << transaction.getTotalAmount() << endl;
            outFile << right << setw(55) << "Points Redeemed: " << transaction.getPointsRedeemed() << endl;
            outFile << right << setw(55) << "Points Earned: " << transaction.getPointsEarned() << endl;
            outFile << endl;
            outFile << "Thank you for shopping with I-ROS!" << endl;
            outFile << "========================================" << endl;
            
            outFile.close();
        }
    }

    // Display all transactions
    void displayAllTransactions(const InventoryManager& inventory) const {
        if (transactions.empty()) {
            cout << "No transactions found." << endl;
            return;
        }
        
        for (const auto& transaction : transactions) {
            transaction.displayDetails(inventory);
            cout << endl;
        }
    }

    // Save transactions to file
    void saveTransactionsToFile(const string& filename) const {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            for (const auto& transaction : transactions) {
                outFile << transaction.toString() << endl;
            }
            outFile.close();
        }
    }

    // Load transactions from file
    void loadTransactionsFromFile(const string& filename) {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            transactions.clear();
            string line;
            while (getline(inFile, line)) {
                if (!line.empty()) {
                    transactions.push_back(Transaction::fromString(line));
                }
            }
            inFile.close();
        }
    }
};

// AuthSystem class
class AuthSystem {
private:
    vector<User*> users;

public:
    ~AuthSystem() {
        for (auto user : users) {
            delete user;
        }
    }

    // Add a user
    void addUser(User* user) {
        users.push_back(user);
    }

    // Remove a user by username
    bool removeUser(const string& username) {
        for (auto it = users.begin(); it != users.end(); ++it) {
            if ((*it)->getUsername() == username) {
                delete *it;
                users.erase(it);
                return true;
            }
        }
        return false;
    }

    // Authenticate user
    User* authenticate(const string& username, const string& password) {
        for (auto user : users) {
            if (user->getUsername() == username && user->getPassword() == password) {
                return user;
            }
        }
        return nullptr;
    }

    // Check if username exists
    bool usernameExists(const string& username) {
        for (auto user : users) {
            if (user->getUsername() == username) {
                return true;
            }
        }
        return false;
    }

    // Get user by username
    User* getUserByUsername(const string& username) {
        for (auto user : users) {
            if (user->getUsername() == username) {
                return user;
            }
        }
        return nullptr;
    }

    // Get all users
    vector<User*> getUsers() const {
        return users;
    }

    // Display all users
    void displayAllUsers() const {
        for (const auto& user : users) {
            user->displayInfo();
            cout << endl;
        }
    }

    // Save users to file
    void saveUsersToFile(const string& filename) const {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            for (const auto& user : users) {
                outFile << user->toString() << endl;
            }
            outFile.close();
        }
    }

    // Load users from file
    void loadUsersFromFile(const string& filename) {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            // Clear existing users
            for (auto user : users) {
                delete user;
            }
            users.clear();
            
            string line;
            while (getline(inFile, line)) {
                if (!line.empty()) {
                    users.push_back(User::fromString(line));
                }
            }
            inFile.close();
        }
    }
};

// LoyaltyManager class
class LoyaltyManager {
public:
    // Calculate loyalty points earned
    static int calculatePointsEarned(double amount) {
        return static_cast<int>(amount); // 1 point per 1 Taka
    }

    // Calculate discount from points
    static double calculateDiscountFromPoints(int points) {
        return points / 10.0; // 10 points = 1 Taka
    }
};

// IROS class (main system controller)
class IROS {
private:
    InventoryManager inventoryManager;
    TransactionManager transactionManager;
    AuthSystem authSystem;
    User* currentUser;

public:
    IROS() : currentUser(nullptr) {
        // Load data from files
        loadData();
    }

    ~IROS() {
        // Save data to files
        saveData();
    }

    // Load data from files
    void loadData() {
        inventoryManager.loadProductsFromFile("products.txt");
        inventoryManager.loadSuppliersFromFile("suppliers.txt");
        transactionManager.loadTransactionsFromFile("transactions.txt");
        authSystem.loadUsersFromFile("users.txt");
    }

    // Save data to files
    void saveData() {
        inventoryManager.saveProductsToFile("products.txt");
        inventoryManager.saveSuppliersToFile("suppliers.txt");
        transactionManager.saveTransactionsToFile("transactions.txt");
        authSystem.saveUsersToFile("users.txt");
    }

    // Run the system
    void run() {
        while (true) {
            if (!currentUser) {
                showLoginMenu();
            } else {
                if (currentUser->getRole() == "admin") {
                    showAdminMenu();
                } else {
                    showCustomerMenu();
                }
            }
        }
    }

    // Show login menu
    void showLoginMenu() {
        clearConsole();
        cout << "========================================" << endl;
        cout << "     Intelligent Retail Operations & System     " << endl;
        cout << "========================================" << endl;
        cout << "1. Login" << endl;
        cout << "2. Register as Customer" << endl;
        cout << "3. Exit" << endl;
        cout << "========================================" << endl;
        cout << "Enter your choice: ";
        
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                login();
                break;
            case 2:
                registerCustomer();
                break;
            case 3:
                cout << "Exiting I-ROS. Goodbye!" << endl;
                exit(0);
            default:
                cout << "Invalid choice. Please try again." << endl;
                pauseConsole();
        }
    }

    // Login
    void login() {
        clearConsole();
        cout << "========================================" << endl;
        cout << "                  LOGIN                 " << endl;
        cout << "========================================" << endl;
        
        string username, password;
        cout << "Username: ";
        getline(cin, username);
        cout << "Password: ";
        getline(cin, password);
        
        User* user = authSystem.authenticate(username, password);
        if (user) {
            currentUser = user;
            cout << "\nLogin successful! Welcome, " << user->getUsername() << "!" << endl;
        } else {
            cout << "\nLogin failed. Invalid username or password." << endl;
        }
        
        pauseConsole();
    }

    // Register a new customer
    void registerCustomer() {
        clearConsole();
        cout << "========================================" << endl;
        cout << "              CUSTOMER REGISTRATION              " << endl;
        cout << "========================================" << endl;
        
        string username, password;
        
        cout << "Enter a username: ";
        getline(cin, username);
        
        if (authSystem.usernameExists(username)) {
            cout << "Username already exists. Please choose a different one." << endl;
            pauseConsole();
            return;
        }
        
        cout << "Enter a password: ";
        getline(cin, password);
        
        Customer* newCustomer = new Customer(username, password);
        authSystem.addUser(newCustomer);
        
        cout << "\nRegistration successful! You can now login with your new account." << endl;
        pauseConsole();
    }

    // Show admin menu
    void showAdminMenu() {
        clearConsole();
        cout << "========================================" << endl;
        cout << "            ADMIN DASHBOARD            " << endl;
        cout << "========================================" << endl;
        cout << "Welcome, " << currentUser->getUsername() << "!" << endl;
        cout << "1. Inventory Management" << endl;
        cout << "2. Supplier Management" << endl;
        cout << "3. Financial Reports" << endl;
        cout << "4. Seasonal Discount Management" << endl;
        cout << "5. Low Stock Alert" << endl;
        cout << "6. Logout" << endl;
        cout << "========================================" << endl;
        cout << "Enter your choice: ";
        
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                showInventoryManagementMenu();
                break;
            case 2:
                showSupplierManagementMenu();
                break;
            case 3:
                showFinancialReports();
                break;
            case 4:
                showSeasonalDiscountMenu();
                break;
            case 5:
                showLowStockAlert();
                break;
            case 6:
                logout();
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                pauseConsole();
        }
    }

    // Show inventory management menu
    void showInventoryManagementMenu() {
        while (true) {
            clearConsole();
            cout << "========================================" << endl;
            cout << "         INVENTORY MANAGEMENT          " << endl;
            cout << "========================================" << endl;
            cout << "1. View All Products" << endl;
            cout << "2. Add New Product" << endl;
            cout << "3. Update Product" << endl;
            cout << "4. Delete Product" << endl;
            cout << "5. Back to Admin Dashboard" << endl;
            cout << "========================================" << endl;
            cout << "Enter your choice: ";
            
            int choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            switch (choice) {
                case 1:
                    viewAllProducts();
                    break;
                case 2:
                    addNewProduct();
                    break;
                case 3:
                    updateProduct();
                    break;
                case 4:
                    deleteProduct();
                    break;
                case 5:
                    return;
                default:
                    cout << "Invalid choice. Please try again." << endl;
                    pauseConsole();
            }
        }
    }

    // View all products
    void viewAllProducts() {
        clearConsole();
        cout << "========================================" << endl;
        cout << "            ALL PRODUCTS               " << endl;
        cout << "========================================" << endl;
        inventoryManager.displayAllProducts();
        pauseConsole();
    }

    // Add a new product
    void addNewProduct() {
        clearConsole();
        cout << "========================================" << endl;
        cout << "            ADD NEW PRODUCT            " << endl;
        cout << "========================================" << endl;
        
        string productID, name, category;
        double price;
        int stockQuantity;
        double discount;
        
        cout << "Enter Product ID: ";
        getline(cin, productID);
        
        // Check if product ID already exists
        if (inventoryManager.getProductByID(productID)) {
            cout << "Product ID already exists. Please use a different ID." << endl;
            pauseConsole();
            return;
        }
        
        cout << "Enter Product Name: ";
        getline(cin, name);
        
        cout << "Enter Product Category: ";
        getline(cin, category);
        
        cout << "Enter Product Price: ";
        cin >> price;
        
        cout << "Enter Stock Quantity: ";
        cin >> stockQuantity;
        
        cout << "Enter Discount Percentage (0 if none): ";
        cin >> discount;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        Product newProduct(productID, name, category, price, stockQuantity, discount);
        inventoryManager.addProduct(newProduct);
        
        cout << "\nProduct added successfully!" << endl;
        pauseConsole();
    }

    // Update a product
    void updateProduct() {
        clearConsole();
        cout << "========================================" << endl;
        cout << "            UPDATE PRODUCT             " << endl;
        cout << "========================================" << endl;
        
        string productID;
        cout << "Enter Product ID to update: ";
        getline(cin, productID);
        
        Product* product = inventoryManager.getProductByID(productID);
        if (!product) {
            cout << "Product not found." << endl;
            pauseConsole();
            return;
        }
        
        cout << "\nCurrent Product Details:" << endl;
        cout << left << setw(15) << "ID: " << product->getProductID() << endl;
        cout << left << setw(15) << "Name: " << product->getName() << endl;
        cout << left << setw(15) << "Category: " << product->getCategory() << endl;
        cout << left << setw(15) << "Price: " << product->getPrice() << endl;
        cout << left << setw(15) << "Stock: " << product->getStockQuantity() << endl;
        cout << left << setw(15) << "Discount: " << product->getDiscount() << "%" << endl;
        
        string name, category;
        double price;
        int stockQuantity;
        double discount;
        
        cout << "\nEnter new Product Name (leave blank to keep current): ";
        getline(cin, name);
        if (name.empty()) {
            name = product->getName();
        }
        
        cout << "Enter new Product Category (leave blank to keep current): ";
        getline(cin, category);
        if (category.empty()) {
            category = product->getCategory();
        }
        
        cout << "Enter new Product Price (0 to keep current): ";
        cin >> price;
        if (price <= 0) {
            price = product->getPrice();
        }
        
        cout << "Enter new Stock Quantity (0 to keep current): ";
        cin >> stockQuantity;
        if (stockQuantity <= 0) {
            stockQuantity = product->getStockQuantity();
        }
        
        cout << "Enter new Discount Percentage (0 to keep current): ";
        cin >> discount;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (discount < 0) {
            discount = product->getDiscount();
        }
        
        Product updatedProduct(productID, name, category, price, stockQuantity, discount);
        inventoryManager.updateProduct(productID, updatedProduct);
        
        cout << "\nProduct updated successfully!" << endl;
        pauseConsole();
    }

    // Delete a product
    void deleteProduct() {
        clearConsole();
        cout << "========================================" << endl;
        cout << "            DELETE PRODUCT             " << endl;
        cout << "========================================" << endl;
        
        string productID;
        cout << "Enter Product ID to delete: ";
        getline(cin, productID);
        
        if (inventoryManager.removeProduct(productID)) {
            cout << "\nProduct deleted successfully!" << endl;
        } else {
            cout << "\nProduct not found." << endl;
        }
        
        pauseConsole();
    }

    // Show supplier management menu
    void showSupplierManagementMenu() {
        while (true) {
            clearConsole();
            cout << "========================================" << endl;
            cout << "        SUPPLIER MANAGEMENT           " << endl;
            cout << "========================================" << endl;
            cout << "1. View All Suppliers" << endl;
            cout << "2. Add New Supplier" << endl;
            cout << "3. Update Supplier" << endl;
            cout << "4. Delete Supplier" << endl;
            cout << "5. Link Product to Supplier" << endl;
            cout << "6. Back to Admin Dashboard" << endl;
            cout << "========================================" << endl;
            cout << "Enter your choice: ";
            
            int choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            switch (choice) {
                case 1:
                    viewAllSuppliers();
                    break;
                case 2:
                    addNewSupplier();
                    break;
                case 3:
                    updateSupplier();
                    break;
                case 4:
                    deleteSupplier();
                    break;
                case 5:
                    linkProductToSupplier();
                    break;
                case 6:
                    return;
                default:
                    cout << "Invalid choice. Please try again." << endl;
                    pauseConsole();
            }
        }
    }

    // View all suppliers
    void viewAllSuppliers() {
        clearConsole();
        cout << "========================================" << endl;
        cout << "            ALL SUPPLIERS              " << endl;
        cout << "========================================" << endl;
        inventoryManager.displayAllSuppliers();
        pauseConsole();
    }

    // Add a new supplier
    void addNewSupplier() {
        clearConsole();
        cout << "========================================" << endl;
        cout << "            ADD NEW SUPPLIER           " << endl;
        cout << "========================================" << endl;
        
        string name, contactInfo, address;
        
        cout << "Enter Supplier Name: ";
        getline(cin, name);
        
        // Check if supplier name already exists
        if (inventoryManager.getSupplierByName(name)) {
            cout << "Supplier name already exists. Please use a different name." << endl;
            pauseConsole();
            return;
        }
        
        cout << "Enter Contact Information: ";
        getline(cin, contactInfo);
        
        cout << "Enter Address: ";
        getline(cin, address);
        
        Supplier newSupplier(name, contactInfo, address);
        inventoryManager.addSupplier(newSupplier);
        
        cout << "\nSupplier added successfully!" << endl;
        pauseConsole();
    }

    // Update a supplier
    void updateSupplier() {
        clearConsole();
        cout << "========================================" << endl;
        cout << "            UPDATE SUPPLIER            " << endl;
        cout << "========================================" << endl;
        
        string supplierName;
        cout << "Enter Supplier Name to update: ";
        getline(cin, supplierName);
        
        Supplier* supplier = inventoryManager.getSupplierByName(supplierName);
        if (!supplier) {
            cout << "Supplier not found." << endl;
            pauseConsole();
            return;
        }
        
        cout << "\nCurrent Supplier Details:" << endl;
        supplier->displayInfo();
        
        string contactInfo, address;
        
        cout << "\nEnter new Contact Information (leave blank to keep current): ";
        getline(cin, contactInfo);
        if (contactInfo.empty()) {
            contactInfo = supplier->getContactInfo();
        }
        
        cout << "Enter new Address (leave blank to keep current): ";
        getline(cin, address);
        if (address.empty()) {
            address = supplier->getAddress();
        }
        
        Supplier updatedSupplier(supplierName, contactInfo, address);
        
        // Copy supplied product IDs
        for (const auto& productID : supplier->getSuppliedProductIDs()) {
            updatedSupplier.addSuppliedProduct(productID);
        }
        
        inventoryManager.updateSupplier(supplierName, updatedSupplier);
        
        cout << "\nSupplier updated successfully!" << endl;
        pauseConsole();
    }

    // Delete a supplier
    void deleteSupplier() {
        clearConsole();
        cout << "========================================" << endl;
        cout << "            DELETE SUPPLIER            " << endl;
        cout << "========================================" << endl;
        
        string supplierName;
        cout << "Enter Supplier Name to delete: ";
        getline(cin, supplierName);
        
        if (inventoryManager.removeSupplier(supplierName)) {
            cout << "\nSupplier deleted successfully!" << endl;
        } else {
            cout << "\nSupplier not found." << endl;
        }
        
        pauseConsole();
    }

    // Link a product to a supplier
    void linkProductToSupplier() {
        clearConsole();
        cout << "========================================" << endl;
        cout << "        LINK PRODUCT TO SUPPLIER       " << endl;
        cout << "========================================" << endl;
        
        string supplierName, productID;
        
        cout << "Enter Supplier Name: ";
        getline(cin, supplierName);
        
        Supplier* supplier = inventoryManager.getSupplierByName(supplierName);
        if (!supplier) {
            cout << "Supplier not found." << endl;
            pauseConsole();
            return;
        }
        
        cout << "Enter Product ID: ";
        getline(cin, productID);
        
        Product* product = inventoryManager.getProductByID(productID);
        if (!product) {
            cout << "Product not found." << endl;
            pauseConsole();
            return;
        }
        
        supplier->addSuppliedProduct(productID);
        
        // Update supplier in inventory
        Supplier updatedSupplier = *supplier;
        inventoryManager.updateSupplier(supplierName, updatedSupplier);
        
        cout << "\nProduct linked to supplier successfully!" << endl;
        pauseConsole();
    }

    // Show financial reports
    void showFinancialReports() {
        clearConsole();
        cout << "========================================" << endl;
        cout << "          FINANCIAL REPORTS           " << endl;
        cout << "========================================" << endl;
        
        // Calculate total revenue
        double totalRevenue = 0.0;
        double totalDiscounts = 0.0;
        int totalTransactions = 0;
        
        for (const auto& transaction : transactionManager.getTransactions()) {
            totalRevenue += transaction.getTotalAmount();
            totalDiscounts += transaction.getDiscountAmount();
            totalTransactions++;
        }
        
        cout << "Total Revenue: " << totalRevenue << " Taka" << endl;
        cout << "Total Discounts: " << totalDiscounts << " Taka" << endl;
        cout << "Total Transactions: " << totalTransactions << endl;
        cout << "Average Transaction Value: " << (totalTransactions > 0 ? totalRevenue / totalTransactions : 0) << " Taka" << endl;
        
        pauseConsole();
    }

    // Show seasonal discount menu
    void showSeasonalDiscountMenu() {
        clearConsole();
        cout << "========================================" << endl;
        cout << "       SEASONAL DISCOUNT MANAGEMENT    " << endl;
        cout << "========================================" << endl;
        
        double currentDiscount = transactionManager.getSeasonalDiscount();
        cout << "Current Seasonal Discount: " << currentDiscount << "%" << endl;
        cout << endl;
        
        cout << "1. Set Seasonal Discount" << endl;
        cout << "2. Remove Seasonal Discount" << endl;
        cout << "3. Back to Admin Dashboard" << endl;
        cout << "========================================" << endl;
        cout << "Enter your choice: ";
        
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                double discount;
                cout << "Enter discount percentage (0-100): ";
                cin >> discount;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                if (discount < 0 || discount > 100) {
                    cout << "Invalid discount percentage. Please enter a value between 0 and 100." << endl;
                } else {
                    transactionManager.setSeasonalDiscount(discount);
                    cout << "Seasonal discount set to " << discount << "% successfully!" << endl;
                }
                break;
            }
            case 2:
                transactionManager.setSeasonalDiscount(0.0);
                cout << "Seasonal discount removed successfully!" << endl;
                break;
            case 3:
                return;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        pauseConsole();
    }

    // Show low stock alert
    void showLowStockAlert() {
        clearConsole();
        cout << "========================================" << endl;
        cout << "            LOW STOCK ALERT            " << endl;
        cout << "========================================" << endl;
        
        inventoryManager.checkLowStock();
        pauseConsole();
    }

    // Show customer menu
    void showCustomerMenu() {
        while (true) {
            clearConsole();
            cout << "========================================" << endl;
            cout << "           CUSTOMER DASHBOARD          " << endl;
            cout << "========================================" << endl;
            cout << "Welcome, " << currentUser->getUsername() << "!" << endl;
            Customer* customer = dynamic_cast<Customer*>(currentUser);
            cout << "Loyalty Points: " << customer->getLoyaltyPoints() << endl;
            cout << "1. Browse Products" << endl;
            cout << "2. View Cart" << endl;
            cout << "3. Checkout" << endl;
            cout << "4. View Purchase History" << endl;
            cout << "5. Logout" << endl;
            cout << "========================================" << endl;
            cout << "Enter your choice: ";
            
            int choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            switch (choice) {
                case 1:
                    browseProducts();
                    break;
                case 2:
                    viewCart();
                    break;
                case 3:
                    checkout();
                    break;
                case 4:
                    viewPurchaseHistory();
                    break;
                case 5:
                    logout();
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
                    pauseConsole();
            }
        }
    }

    // Browse products
    void browseProducts() {
        clearConsole();
        cout << "========================================" << endl;
        cout << "            BROWSE PRODUCTS            " << endl;
        cout << "========================================" << endl;
        
        vector<pair<string, int>> cart;
        bool shopping = true;
        
        while (shopping) {
            clearConsole();
            cout << "========================================" << endl;
            cout << "            BROWSE PRODUCTS            " << endl;
            cout << "========================================" << endl;
            
            // Display products
            cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(15) << "Category" 
                 << setw(10) << "Price" << setw(10) << "Stock" << setw(10) << "Discount" << endl;
            cout << string(75, '-') << endl;
            
            for (const auto& product : inventoryManager.getProducts()) {
                product.displayInfo();
            }
            
            cout << "\nCurrent Cart:" << endl;
            if (cart.empty()) {
                cout << "Cart is empty." << endl;
            } else {
                cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Qty" 
                     << setw(10) << "Price" << setw(15) << "Total" << endl;
                cout << string(65, '-') << endl;
                
                double cartTotal = 0.0;
                for (const auto& item : cart) {
                    const Product* product = inventoryManager.getProductByID(item.first);
                    if (product) {
                        double finalPrice = transactionManager.calculateFinalPrice(*product);
                        double itemTotal = finalPrice * item.second;
                        cartTotal += itemTotal;
                        
                        cout << left << setw(10) << product->getProductID() << setw(20) << product->getName() 
                             << setw(10) << item.second << setw(10) << finalPrice << setw(15) << itemTotal << endl;
                    }
                }
                
                cout << string(65, '-') << endl;
                cout << right << setw(55) << "Cart Total: " << cartTotal << endl;
            }
            
            cout << "\nOptions:" << endl;
            cout << "1. Add to Cart" << endl;
            cout << "2. Remove from Cart" << endl;
            cout << "3. Clear Cart" << endl;
            cout << "4. Proceed to Checkout" << endl;
            cout << "5. Back to Dashboard" << endl;
            cout << "========================================" << endl;
            cout << "Enter your choice: ";
            
            int choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            switch (choice) {
                case 1: {
                    string productID;
                    int quantity;
                    
                    cout << "Enter Product ID: ";
                    getline(cin, productID);
                    
                    Product* product = inventoryManager.getProductByID(productID);
                    if (!product) {
                        cout << "Product not found." << endl;
                        pauseConsole();
                        break;
                    }
                    
                    cout << "Enter Quantity: ";
                    cin >> quantity;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    
                    if (quantity <= 0) {
                        cout << "Invalid quantity." << endl;
                    } else if (quantity > product->getStockQuantity()) {
                        cout << "Not enough stock available. Available stock: " << product->getStockQuantity() << endl;
                    } else {
                        // Check if product already in cart
                        bool found = false;
                        for (auto& item : cart) {
                            if (item.first == productID) {
                                item.second += quantity;
                                found = true;
                                break;
                            }
                        }
                        
                        if (!found) {
                            cart.push_back(make_pair(productID, quantity));
                        }
                        
                        cout << "Product added to cart successfully!" << endl;
                    }
                    
                    pauseConsole();
                    break;
                }
                case 2: {
                    if (cart.empty()) {
                        cout << "Cart is empty. Nothing to remove." << endl;
                        pauseConsole();
                        break;
                    }
                    
                    string productID;
                    cout << "Enter Product ID to remove: ";
                    getline(cin, productID);
                    
                    bool removed = false;
                    for (auto it = cart.begin(); it != cart.end(); ++it) {
                        if (it->first == productID) {
                            cart.erase(it);
                            removed = true;
                            break;
                        }
                    }
                    
                    if (removed) {
                        cout << "Product removed from cart successfully!" << endl;
                    } else {
                        cout << "Product not found in cart." << endl;
                    }
                    
                    pauseConsole();
                    break;
                }
                case 3:
                    cart.clear();
                    cout << "Cart cleared successfully!" << endl;
                    pauseConsole();
                    break;
                case 4:
                    if (cart.empty()) {
                        cout << "Cart is empty. Cannot proceed to checkout." << endl;
                        pauseConsole();
                    } else {
                        string invoiceID = transactionManager.processCheckout(
                            *dynamic_cast<Customer*>(currentUser), cart, inventoryManager);
                        
                        if (!invoiceID.empty()) {
                            cout << "Checkout successful! Invoice ID: " << invoiceID << endl;
                        }
                        
                        cart.clear();
                        pauseConsole();
                    }
                    break;
                case 5:
                    shopping = false;
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
                    pauseConsole();
            }
        }
    }

    // View cart
    void viewCart() {
        clearConsole();
        cout << "========================================" << endl;
        cout << "                VIEW CART              " << endl;
        cout << "========================================" << endl;
        
        vector<pair<string, int>> cart;
        // In a real implementation, cart would be stored in the customer object
        // For simplicity, we'll just display an empty cart message
        
        cout << "Your cart is currently empty." << endl;
        cout << "Please browse products to add items to your cart." << endl;
        
        pauseConsole();
    }

    // Checkout
    void checkout() {
        clearConsole();
        cout << "========================================" << endl;
        cout << "               CHECKOUT                " << endl;
        cout << "========================================" << endl;
        
        Customer* customer = dynamic_cast<Customer*>(currentUser);
        if (!customer) {
            cout << "Error: Not a customer account." << endl;
            pauseConsole();
            return;
        }
        
        // In a real implementation, cart would be stored in the customer object
        // For simplicity, we'll just show a message
        cout << "Your cart is currently empty." << endl;
        cout << "Please browse products to add items to your cart before checkout." << endl;
        
        pauseConsole();
    }

    // View purchase history
    void viewPurchaseHistory() {
        clearConsole();
        cout << "========================================" << endl;
        cout << "          PURCHASE HISTORY             " << endl;
        cout << "========================================" << endl;
        
        Customer* customer = dynamic_cast<Customer*>(currentUser);
        if (!customer) {
            cout << "Error: Not a customer account." << endl;
            pauseConsole();
            return;
        }
        
        vector<string> purchaseHistory = customer->getPurchaseHistory();
        if (purchaseHistory.empty()) {
            cout << "You have no purchase history." << endl;
            pauseConsole();
            return;
        }
        
        cout << "Your Purchase History:" << endl;
        cout << left << setw(15) << "Invoice ID" << setw(15) << "Date" << setw(15) << "Total" << endl;
        cout << string(45, '-') << endl;
        
        for (const auto& invoiceID : purchaseHistory) {
            Transaction* transaction = transactionManager.getTransactionByID(invoiceID);
            if (transaction) {
                cout << left << setw(15) << transaction->getInvoiceID() 
                     << setw(15) << transaction->getDate() 
                     << setw(15) << transaction->getTotalAmount() << endl;
            }
        }
        
        pauseConsole();
    }

    // Logout
    void logout() {
        cout << "Logging out..." << endl;
        currentUser = nullptr;
        pauseConsole();
    }
};

// Implementation of Transaction::displayDetails
void Transaction::displayDetails(const InventoryManager& inventory) const {
    cout << "\nInvoice ID: " << invoiceID << endl;
    cout << "Date: " << date << endl;
    cout << "Customer: " << customerUsername << endl;
    cout << "Purchased Items:" << endl;
    cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Qty" 
         << setw(10) << "Price" << setw(15) << "Total" << endl;
    cout << string(65, '-') << endl;
    
    for (const auto& item : purchasedItems) {
        const Product* product = inventory.getProductByID(item.first);
        if (product) {
            double itemTotal = product->getPrice() * item.second;
            cout << left << setw(10) << product->getProductID() << setw(20) << product->getName() 
                 << setw(10) << item.second << setw(10) << product->getPrice() 
                 << setw(15) << itemTotal << endl;
        }
    }
    
    cout << string(65, '-') << endl;
    cout << right << setw(55) << "Subtotal: " << subtotal << endl;
    cout << right << setw(55) << "Discount: " << discountAmount << endl;
    cout << right << setw(55) << "Total: " << totalAmount << endl;
    cout << right << setw(55) << "Points Redeemed: " << pointsRedeemed << endl;
    cout << right << setw(55) << "Points Earned: " << pointsEarned << endl;
}

// Implementation of User::fromString
User* User::fromString(const string& str) {
    stringstream ss(str);
    string username, password, role;
    
    getline(ss, username, ',');
    getline(ss, password, ',');
    getline(ss, role, ',');
    
    if (role == "admin") {
        return new Admin(username, password);
    } else {
        return new Customer(username, password);
    }
}

int main() {
    IROS iros;
    iros.run();
    return 0;
}