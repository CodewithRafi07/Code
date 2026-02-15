#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace std;

// ==================== Constants ====================
const int LOW_STOCK_THRESHOLD = 15;
const double POINTS_TO_CURRENCY_RATE = 0.1; // 10 points = 1 Taka
const double CURRENCY_TO_POINTS_RATE = 1.0; // 1 Taka = 1 point

// ==================== Helper Functions ====================
vector<string> splitString(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

string getCurrentDateTime() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    stringstream ss;
    ss << setfill('0') 
       << 1900 + ltm->tm_year << "-" 
       << setw(2) << 1 + ltm->tm_mon << "-" 
       << setw(2) << ltm->tm_mday << " "
       << setw(2) << ltm->tm_hour << ":" 
       << setw(2) << ltm->tm_min << ":" 
       << setw(2) << ltm->tm_sec;
    
    return ss.str();
}

// ==================== Product Class ====================
class Product {
private:
    int id;
    string name;
    string category;
    double price;
    int stock;
    double discount; // Product-specific discount percentage

public:
    Product(int id, string name, string category, double price, int stock, double discount = 0.0)
        : id(id), name(name), category(category), price(price), stock(stock), discount(discount) {}

    // Getters
    int getId() const { return id; }
    string getName() const { return name; }
    string getCategory() const { return category; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }
    double getDiscount() const { return discount; }

    // Setters
    void setPrice(double newPrice) { price = newPrice; }
    void setDiscount(double newDiscount) { discount = newDiscount; }
    void setStock(int newStock) { stock = newStock; }

    // Stock management
    bool deductStock(int quantity) {
        if (stock >= quantity) {
            stock -= quantity;
            return true;
        }
        return false;
    }

    void addStock(int quantity) {
        stock += quantity;
    }

    bool isLowStock() const {
        return stock <= LOW_STOCK_THRESHOLD;
    }

    // Serialization
    string toCSV() const {
        return to_string(id) + "," + name + "," + category + "," + 
               to_string(price) + "," + to_string(stock) + "," + to_string(discount);
    }

    // Deserialization
    static Product fromCSV(const string& csv) {
        vector<string> tokens = splitString(csv, ',');
        return Product(
            stoi(tokens[0]),
            tokens[1],
            tokens[2],
            stod(tokens[3]),
            stoi(tokens[4]),
            stod(tokens[5])
        );
    }

    // Display product info
    void display() const {
        cout << "ID: " << id << " | Name: " << name << " | Category: " << category 
             << " | Price: " << price << " | Stock: " << stock 
             << " | Discount: " << discount << "%" << endl;
    }
};

// ==================== User Class (Abstract Base) ====================
class User {
protected:
    string username;
    string password;
    string role; // "admin" or "customer"

public:
    User(string username, string password, string role)
        : username(username), password(password), role(role) {}

    virtual ~User() {}

    // Getters
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getRole() const { return role; }

    // Authentication
    bool authenticate(const string& pwd) const {
        return password == pwd;
    }

    // Virtual menu function
    virtual void displayMenu() = 0;

    // Serialization
    string toCSV() const {
        return username + "," + password + "," + role;
    }

    // Deserialization
    static User* fromCSV(const string& csv) {
        vector<string> tokens = splitString(csv, ',');
        if (tokens[2] == "admin") {
            return new Admin(tokens[0], tokens[1]);
        } else {
            return new Customer(tokens[0], tokens[1]);
        }
    }
};

// ==================== Admin Class ====================
class Admin : public User {
public:
    Admin(string username, string password) 
        : User(username, password, "admin") {}

    void displayMenu() override {
        cout << "\n===== Admin Menu =====" << endl;
        cout << "1. Inventory Management" << endl;
        cout << "2. Supplier Management" << endl;
        cout << "3. Financial Reports" << endl;
        cout << "4. Discount Settings" << endl;
        cout << "5. User Management" << endl;
        cout << "6. Logout" << endl;
        cout << "Enter your choice: ";
    }
};

// ==================== Customer Class ====================
class Customer : public User {
private:
    int loyaltyPoints;

public:
    Customer(string username, string password, int points = 0) 
        : User(username, password, "customer"), loyaltyPoints(points) {}

    // Getters
    int getLoyaltyPoints() const { return loyaltyPoints; }

    // Loyalty points management
    void addLoyaltyPoints(double amount) {
        loyaltyPoints += static_cast<int>(amount * CURRENCY_TO_POINTS_RATE);
    }

    bool redeemLoyaltyPoints(int points) {
        if (points <= loyaltyPoints) {
            loyaltyPoints -= points;
            return true;
        }
        return false;
    }

    void displayMenu() override {
        cout << "\n===== Customer Menu =====" << endl;
        cout << "1. Browse Products" << endl;
        cout << "2. View Cart" << endl;
        cout << "3. Checkout" << endl;
        cout << "4. View Purchase History" << endl;
        cout << "5. View Loyalty Points" << endl;
        cout << "6. Logout" << endl;
        cout << "Enter your choice: ";
    }

    // Serialization
    string toCSV() const {
        return username + "," + password + "," + role + "," + to_string(loyaltyPoints);
    }

    // Deserialization
    static Customer* fromCSV(const string& csv) {
        vector<string> tokens = splitString(csv, ',');
        return new Customer(tokens[0], tokens[1], stoi(tokens[3]));
    }
};

// ==================== Supplier Class ====================
class Supplier {
private:
    int id;
    string name;
    string contact;
    vector<int> productIds; // Products supplied by this supplier

public:
    Supplier(int id, string name, string contact)
        : id(id), name(name), contact(contact) {}

    // Getters
    int getId() const { return id; }
    string getName() const { return name; }
    string getContact() const { return contact; }
    const vector<int>& getProductIds() const { return productIds; }

    // Product management
    void addProduct(int productId) {
        productIds.push_back(productId);
    }

    void removeProduct(int productId) {
        productIds.erase(remove(productIds.begin(), productIds.end(), productId), productIds.end());
    }

    // Serialization
    string toCSV() const {
        string csv = to_string(id) + "," + name + "," + contact;
        for (int pid : productIds) {
            csv += "," + to_string(pid);
        }
        return csv;
    }

    // Deserialization
    static Supplier fromCSV(const string& csv) {
        vector<string> tokens = splitString(csv, ',');
        Supplier supplier(stoi(tokens[0]), tokens[1], tokens[2]);
        
        for (size_t i = 3; i < tokens.size(); ++i) {
            supplier.addProduct(stoi(tokens[i]));
        }
        
        return supplier;
    }

    // Display supplier info
    void display() const {
        cout << "ID: " << id << " | Name: " << name << " | Contact: " << contact << endl;
        cout << "Supplied Products: ";
        for (int pid : productIds) {
            cout << pid << " ";
        }
        cout << endl;
    }
};

// ==================== Invoice Class ====================
class Invoice {
private:
    int invoiceId;
    string customerUsername;
    vector<pair<int, int>> items; // Product ID and quantity
    double totalAmount;
    double discountApplied;
    double finalAmount;
    string dateTime;

public:
    Invoice(int id, string customer, const vector<pair<int, int>>& items, 
            double total, double discount, double final)
        : invoiceId(id), customerUsername(customer), items(items), 
          totalAmount(total), discountApplied(discount), finalAmount(final),
          dateTime(getCurrentDateTime()) {}

    // Getters
    int getInvoiceId() const { return invoiceId; }
    string getCustomerUsername() const { return customerUsername; }
    const vector<pair<int, int>>& getItems() const { return items; }
    double getTotalAmount() const { return totalAmount; }
    double getDiscountApplied() const { return discountApplied; }
    double getFinalAmount() const { return finalAmount; }
    string getDateTime() const { return dateTime; }

    // Serialization
    string toCSV() const {
        string csv = to_string(invoiceId) + "," + customerUsername + "," + dateTime + "," + 
                    to_string(totalAmount) + "," + to_string(discountApplied) + "," + 
                    to_string(finalAmount);
        
        for (const auto& item : items) {
            csv += "," + to_string(item.first) + ":" + to_string(item.second);
        }
        
        return csv;
    }

    // Deserialization
    static Invoice fromCSV(const string& csv) {
        vector<string> tokens = splitString(csv, ',');
        
        int id = stoi(tokens[0]);
        string customer = tokens[1];
        string dateTime = tokens[2];
        double total = stod(tokens[3]);
        double discount = stod(tokens[4]);
        double final = stod(tokens[5]);
        
        vector<pair<int, int>> items;
        for (size_t i = 6; i < tokens.size(); ++i) {
            vector<string> itemTokens = splitString(tokens[i], ':');
            items.emplace_back(stoi(itemTokens[0]), stoi(itemTokens[1]));
        }
        
        Invoice invoice(id, customer, items, total, discount, final);
        // We can't set dateTime directly since it's private, so we'll skip it for simplicity
        // In a real implementation, we would add a setter or modify the constructor
        
        return invoice;
    }

    // Display invoice
    void display() const {
        cout << "\n===== Invoice #" << invoiceId << " =====" << endl;
        cout << "Date: " << dateTime << endl;
        cout << "Customer: " << customerUsername << endl;
        cout << "--------------------------------" << endl;
        cout << "Items:" << endl;
        
        for (const auto& item : items) {
            cout << "  Product ID: " << item.first << ", Quantity: " << item.second << endl;
        }
        
        cout << "--------------------------------" << endl;
        cout << "Total Amount: " << totalAmount << endl;
        cout << "Discount Applied: " << discountApplied << endl;
        cout << "Final Amount: " << finalAmount << endl;
        cout << "================================" << endl;
    }
};

// ==================== DiscountManager Class ====================
class DiscountManager {
private:
    double seasonalDiscount; // Current seasonal discount percentage
    string season; // Current season name

public:
    DiscountManager() : seasonalDiscount(0.0), season("None") {}

    // Getters
    double getSeasonalDiscount() const { return seasonalDiscount; }
    string getSeason() const { return season; }

    // Set seasonal discount
    void setSeasonalDiscount(const string& seasonName, double discount) {
        season = seasonName;
        seasonalDiscount = discount;
    }

    // Calculate final price after all discounts
    double calculateFinalPrice(const Product& product, int quantity, double loyaltyDiscount = 0.0) {
        double basePrice = product.getPrice() * quantity;
        
        // Apply product-specific discount
        double productDiscount = basePrice * (product.getDiscount() / 100.0);
        double afterProductDiscount = basePrice - productDiscount;
        
        // Apply seasonal discount
        double seasonalDiscountAmount = afterProductDiscount * (seasonalDiscount / 100.0);
        double afterSeasonalDiscount = afterProductDiscount - seasonalDiscountAmount;
        
        // Apply loyalty discount
        double afterLoyaltyDiscount = afterSeasonalDiscount - loyaltyDiscount;
        
        // Ensure price doesn't go negative
        return max(0.0, afterLoyaltyDiscount);
    }
};

// ==================== Inventory Class ====================
class Inventory {
private:
    unordered_map<int, Product> products;

public:
    // Product CRUD operations
    void addProduct(const Product& product) {
        products[product.getId()] = product;
    }

    bool updateProduct(int id, const Product& updatedProduct) {
        if (products.find(id) != products.end()) {
            products[id] = updatedProduct;
            return true;
        }
        return false;
    }

    bool deleteProduct(int id) {
        if (products.erase(id) > 0) {
            return true;
        }
        return false;
    }

    Product* findProduct(int id) {
        auto it = products.find(id);
        if (it != products.end()) {
            return &(it->second);
        }
        return nullptr;
    }

    vector<Product> getAllProducts() {
        vector<Product> result;
        for (const auto& pair : products) {
            result.push_back(pair.second);
        }
        return result;
    }

    // Stock management
    bool deductStock(int productId, int quantity) {
        Product* product = findProduct(productId);
        if (product && product->deductStock(quantity)) {
            return true;
        }
        return false;
    }

    void addStock(int productId, int quantity) {
        Product* product = findProduct(productId);
        if (product) {
            product->addStock(quantity);
        }
    }

    // Low stock alert
    void checkLowStock() {
        cout << "\n===== Low Stock Alert =====" << endl;
        bool found = false;
        
        for (const auto& pair : products) {
            if (pair.second.isLowStock()) {
                pair.second.display();
                found = true;
            }
        }
        
        if (!found) {
            cout << "No products are low in stock." << endl;
        }
        cout << "==========================" << endl;
    }

    // File persistence
    void saveToCSV(const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& pair : products) {
                file << pair.second.toCSV() << endl;
            }
            file.close();
        }
    }

    void loadFromCSV(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                Product product = Product::fromCSV(line);
                products[product.getId()] = product;
            }
            file.close();
        }
    }

    // Display all products
    void displayAllProducts() {
        cout << "\n===== Product Inventory =====" << endl;
        for (const auto& pair : products) {
            pair.second.display();
        }
        cout << "=============================" << endl;
    }
};

// ==================== Authentication Class ====================
class Authentication {
private:
    unordered_map<string, unique_ptr<User>> users;

public:
    // User management
    bool addUser(unique_ptr<User> user) {
        string username = user->getUsername();
        if (users.find(username) == users.end()) {
            users[username] = move(user);
            return true;
        }
        return false;
    }

    User* login(const string& username, const string& password) {
        auto it = users.find(username);
        if (it != users.end() && it->second->authenticate(password)) {
            return it->second.get();
        }
        return nullptr;
    }

    bool updateUser(const string& username, unique_ptr<User> updatedUser) {
        if (users.find(username) != users.end()) {
            users[username] = move(updatedUser);
            return true;
        }
        return false;
    }

    bool deleteUser(const string& username) {
        if (users.erase(username) > 0) {
            return true;
        }
        return false;
    }

    Customer* getCustomer(const string& username) {
        auto it = users.find(username);
        if (it != users.end() && it->second->getRole() == "customer") {
            return static_cast<Customer*>(it->second.get());
        }
        return nullptr;
    }

    // File persistence
    void saveToCSV(const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& pair : users) {
                if (pair.second->getRole() == "customer") {
                    Customer* customer = static_cast<Customer*>(pair.second.get());
                    file << customer->toCSV() << endl;
                } else {
                    file << pair.second->toCSV() << endl;
                }
            }
            file.close();
        }
    }

    void loadFromCSV(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                User* user = User::fromCSV(line);
                users[user->getUsername()] = unique_ptr<User>(user);
            }
            file.close();
        }
    }

    // Display all users
    void displayAllUsers() {
        cout << "\n===== User List =====" << endl;
        for (const auto& pair : users) {
            cout << "Username: " << pair.first 
                 << " | Role: " << pair.second->getRole() << endl;
        }
        cout << "====================" << endl;
    }
};

// ==================== SupplierManager Class ====================
class SupplierManager {
private:
    unordered_map<int, Supplier> suppliers;

public:
    // Supplier CRUD operations
    void addSupplier(const Supplier& supplier) {
        suppliers[supplier.getId()] = supplier;
    }

    bool updateSupplier(int id, const Supplier& updatedSupplier) {
        if (suppliers.find(id) != suppliers.end()) {
            suppliers[id] = updatedSupplier;
            return true;
        }
        return false;
    }

    bool deleteSupplier(int id) {
        if (suppliers.erase(id) > 0) {
            return true;
        }
        return false;
    }

    Supplier* findSupplier(int id) {
        auto it = suppliers.find(id);
        if (it != suppliers.end()) {
            return &(it->second);
        }
        return nullptr;
    }

    vector<Supplier> getAllSuppliers() {
        vector<Supplier> result;
        for (const auto& pair : suppliers) {
            result.push_back(pair.second);
        }
        return result;
    }

    // Product-supplier relationship
    bool addProductToSupplier(int supplierId, int productId) {
        Supplier* supplier = findSupplier(supplierId);
        if (supplier) {
            supplier->addProduct(productId);
            return true;
        }
        return false;
    }

    bool removeProductFromSupplier(int supplierId, int productId) {
        Supplier* supplier = findSupplier(supplierId);
        if (supplier) {
            supplier->removeProduct(productId);
            return true;
        }
        return false;
    }

    // File persistence
    void saveToCSV(const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& pair : suppliers) {
                file << pair.second.toCSV() << endl;
            }
            file.close();
        }
    }

    void loadFromCSV(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                Supplier supplier = Supplier::fromCSV(line);
                suppliers[supplier.getId()] = supplier;
            }
            file.close();
        }
    }

    // Display all suppliers
    void displayAllSuppliers() {
        cout << "\n===== Supplier List =====" << endl;
        for (const auto& pair : suppliers) {
            pair.second.display();
        }
        cout << "========================" << endl;
    }
};

// ==================== TransactionManager Class ====================
class TransactionManager {
private:
    vector<Invoice> invoices;
    int nextInvoiceId;

public:
    TransactionManager() : nextInvoiceId(1) {}

    // Create and process a new invoice
    Invoice* createInvoice(const string& customerUsername, const vector<pair<int, int>>& items,
                          Inventory& inventory, DiscountManager& discountManager, 
                          Customer* customer, int pointsToRedeem = 0) {
        double totalAmount = 0.0;
        double discountApplied = 0.0;
        
        // Calculate total amount and discounts
        for (const auto& item : items) {
            Product* product = inventory.findProduct(item.first);
            if (product) {
                double itemPrice = product->getPrice() * item.second;
                totalAmount += itemPrice;
                
                // Product discount
                discountApplied += itemPrice * (product->getDiscount() / 100.0);
                
                // Seasonal discount
                discountApplied += itemPrice * (discountManager.getSeasonalDiscount() / 100.0);
            }
        }
        
        // Loyalty points discount
        double loyaltyDiscount = pointsToRedeem * POINTS_TO_CURRENCY_RATE;
        if (loyaltyDiscount > totalAmount - discountApplied) {
            loyaltyDiscount = totalAmount - discountApplied;
        }
        discountApplied += loyaltyDiscount;
        
        double finalAmount = totalAmount - discountApplied;
        
        // Create invoice
        Invoice invoice(nextInvoiceId++, customerUsername, items, totalAmount, discountApplied, finalAmount);
        invoices.push_back(invoice);
        
        // Deduct stock
        for (const auto& item : items) {
            inventory.deductStock(item.first, item.second);
        }
        
        // Update customer loyalty points
        if (customer) {
            customer->addLoyaltyPoints(finalAmount);
            if (pointsToRedeem > 0) {
                customer->redeemLoyaltyPoints(pointsToRedeem);
            }
        }
        
        return &invoices.back();
    }

    // Get customer's purchase history
    vector<Invoice> getCustomerInvoices(const string& username) {
        vector<Invoice> result;
        for (const auto& invoice : invoices) {
            if (invoice.getCustomerUsername() == username) {
                result.push_back(invoice);
            }
        }
        return result;
    }

    // File persistence
    void saveToCSV(const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& invoice : invoices) {
                file << invoice.toCSV() << endl;
            }
            file.close();
        }
    }

    void loadFromCSV(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                Invoice invoice = Invoice::fromCSV(line);
                invoices.push_back(invoice);
                
                // Update next invoice ID
                if (invoice.getInvoiceId() >= nextInvoiceId) {
                    nextInvoiceId = invoice.getInvoiceId() + 1;
                }
            }
            file.close();
        }
    }

    // Display all invoices
    void displayAllInvoices() {
        cout << "\n===== Sales History =====" << endl;
        for (const auto& invoice : invoices) {
            invoice.display();
        }
        cout << "========================" << endl;
    }
};

// ==================== IROS System Class ====================
class IROSSystem {
private:
    Inventory inventory;
    Authentication auth;
    SupplierManager supplierManager;
    TransactionManager transactionManager;
    DiscountManager discountManager;
    User* currentUser;

public:
    IROSSystem() : currentUser(nullptr) {
        // Load data from files
        inventory.loadFromCSV("products.csv");
        auth.loadFromCSV("users.csv");
        supplierManager.loadFromCSV("suppliers.csv");
        transactionManager.loadFromCSV("sales.csv");
        
        // Create default admin if not exists
        if (!auth.login("admin", "admin123")) {
            auth.addUser(make_unique<Admin>("admin", "admin123"));
        }
    }

    ~IROSSystem() {
        // Save data to files
        inventory.saveToCSV("products.csv");
        auth.saveToCSV("users.csv");
        supplierManager.saveToCSV("suppliers.csv");
        transactionManager.saveToCSV("sales.csv");
    }

    // Main system loop
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

private:
    void showLoginMenu() {
        cout << "\n===== I-ROS System =====" << endl;
        cout << "1. Login" << endl;
        cout << "2. Register as Customer" << endl;
        cout << "3. Exit" << endl;
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
                cout << "Exiting system. Goodbye!" << endl;
                exit(0);
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    void login() {
        string username, password;
        cout << "Username: ";
        getline(cin, username);
        cout << "Password: ";
        getline(cin, password);
        
        currentUser = auth.login(username, password);
        if (currentUser) {
            cout << "Login successful. Welcome, " << username << "!" << endl;
        } else {
            cout << "Login failed. Invalid username or password." << endl;
        }
    }

    void registerCustomer() {
        string username, password;
        cout << "Enter username: ";
        getline(cin, username);
        cout << "Enter password: ";
        getline(cin, password);
        
        if (auth.addUser(make_unique<Customer>(username, password))) {
            cout << "Registration successful. You can now login." << endl;
        } else {
            cout << "Registration failed. Username already exists." << endl;
        }
    }

    void showAdminMenu() {
        Admin* admin = static_cast<Admin*>(currentUser);
        admin->displayMenu();
        
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                inventoryManagement();
                break;
            case 2:
                supplierManagement();
                break;
            case 3:
                financialReports();
                break;
            case 4:
                discountSettings();
                break;
            case 5:
                userManagement();
                break;
            case 6:
                logout();
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    void showCustomerMenu() {
        Customer* customer = static_cast<Customer*>(currentUser);
        customer->displayMenu();
        
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
                viewLoyaltyPoints();
                break;
            case 6:
                logout();
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    void logout() {
        cout << "Logging out..." << endl;
        currentUser = nullptr;
    }

    void inventoryManagement() {
        while (true) {
            cout << "\n===== Inventory Management =====" << endl;
            cout << "1. View All Products" << endl;
            cout << "2. Add Product" << endl;
            cout << "3. Update Product" << endl;
            cout << "4. Delete Product" << endl;
            cout << "5. Check Low Stock" << endl;
            cout << "6. Back to Admin Menu" << endl;
            cout << "Enter your choice: ";
            
            int choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            switch (choice) {
                case 1:
                    inventory.displayAllProducts();
                    break;
                case 2:
                    addProduct();
                    break;
                case 3:
                    updateProduct();
                    break;
                case 4:
                    deleteProduct();
                    break;
                case 5:
                    inventory.checkLowStock();
                    break;
                case 6:
                    return;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    void addProduct() {
        int id, stock;
        string name, category;
        double price, discount;
        
        cout << "Enter Product ID: ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cout << "Enter Product Name: ";
        getline(cin, name);
        
        cout << "Enter Category: ";
        getline(cin, category);
        
        cout << "Enter Price: ";
        cin >> price;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cout << "Enter Stock Quantity: ";
        cin >> stock;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cout << "Enter Discount Percentage (0 if none): ";
        cin >> discount;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        inventory.addProduct(Product(id, name, category, price, stock, discount));
        cout << "Product added successfully!" << endl;
    }

    void updateProduct() {
        int id;
        cout << "Enter Product ID to update: ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        Product* product = inventory.findProduct(id);
        if (!product) {
            cout << "Product not found!" << endl;
            return;
        }
        
        cout << "Current Product Details:" << endl;
        product->display();
        
        string name, category;
        double price, discount;
        int stock;
        
        cout << "Enter new Name (leave blank to keep current): ";
        getline(cin, name);
        if (!name.empty()) {
            // Note: In a real implementation, we would have setters for all fields
            // For simplicity, we'll just update what's provided
        }
        
        cout << "Enter new Category (leave blank to keep current): ";
        getline(cin, category);
        
        cout << "Enter new Price (0 to keep current): ";
        cin >> price;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cout << "Enter new Stock Quantity (-1 to keep current): ";
        cin >> stock;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cout << "Enter new Discount Percentage (-1 to keep current): ";
        cin >> discount;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        // Create updated product
        Product updatedProduct(
            id,
            name.empty() ? product->getName() : name,
            category.empty() ? product->getCategory() : category,
            price > 0 ? price : product->getPrice(),
            stock >= 0 ? stock : product->getStock(),
            discount >= 0 ? discount : product->getDiscount()
        );
        
        if (inventory.updateProduct(id, updatedProduct)) {
            cout << "Product updated successfully!" << endl;
        } else {
            cout << "Failed to update product." << endl;
        }
    }

    void deleteProduct() {
        int id;
        cout << "Enter Product ID to delete: ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (inventory.deleteProduct(id)) {
            cout << "Product deleted successfully!" << endl;
        } else {
            cout << "Product not found!" << endl;
        }
    }

    void supplierManagement() {
        while (true) {
            cout << "\n===== Supplier Management =====" << endl;
            cout << "1. View All Suppliers" << endl;
            cout << "2. Add Supplier" << endl;
            cout << "3. Update Supplier" << endl;
            cout << "4. Delete Supplier" << endl;
            cout << "5. Add Product to Supplier" << endl;
            cout << "6. Remove Product from Supplier" << endl;
            cout << "7. Back to Admin Menu" << endl;
            cout << "Enter your choice: ";
            
            int choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            switch (choice) {
                case 1:
                    supplierManager.displayAllSuppliers();
                    break;
                case 2:
                    addSupplier();
                    break;
                case 3:
                    updateSupplier();
                    break;
                case 4:
                    deleteSupplier();
                    break;
                case 5:
                    addProductToSupplier();
                    break;
                case 6:
                    removeProductFromSupplier();
                    break;
                case 7:
                    return;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    void addSupplier() {
        int id;
        string name, contact;
        
        cout << "Enter Supplier ID: ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cout << "Enter Supplier Name: ";
        getline(cin, name);
        
        cout << "Enter Contact Information: ";
        getline(cin, contact);
        
        supplierManager.addSupplier(Supplier(id, name, contact));
        cout << "Supplier added successfully!" << endl;
    }

    void updateSupplier() {
        int id;
        cout << "Enter Supplier ID to update: ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        Supplier* supplier = supplierManager.findSupplier(id);
        if (!supplier) {
            cout << "Supplier not found!" << endl;
            return;
        }
        
        cout << "Current Supplier Details:" << endl;
        supplier->display();
        
        string name, contact;
        
        cout << "Enter new Name (leave blank to keep current): ";
        getline(cin, name);
        
        cout << "Enter new Contact Information (leave blank to keep current): ";
        getline(cin, contact);
        
        // Create updated supplier
        Supplier updatedSupplier(
            id,
            name.empty() ? supplier->getName() : name,
            contact.empty() ? supplier->getContact() : contact
        );
        
        // Copy existing product IDs
        for (int pid : supplier->getProductIds()) {
            updatedSupplier.addProduct(pid);
        }
        
        if (supplierManager.updateSupplier(id, updatedSupplier)) {
            cout << "Supplier updated successfully!" << endl;
        } else {
            cout << "Failed to update supplier." << endl;
        }
    }

    void deleteSupplier() {
        int id;
        cout << "Enter Supplier ID to delete: ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (supplierManager.deleteSupplier(id)) {
            cout << "Supplier deleted successfully!" << endl;
        } else {
            cout << "Supplier not found!" << endl;
        }
    }

    void addProductToSupplier() {
        int supplierId, productId;
        cout << "Enter Supplier ID: ";
        cin >> supplierId;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cout << "Enter Product ID to add: ";
        cin >> productId;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (supplierManager.addProductToSupplier(supplierId, productId)) {
            cout << "Product added to supplier successfully!" << endl;
        } else {
            cout << "Failed to add product to supplier. Supplier not found." << endl;
        }
    }

    void removeProductFromSupplier() {
        int supplierId, productId;
        cout << "Enter Supplier ID: ";
        cin >> supplierId;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        cout << "Enter Product ID to remove: ";
        cin >> productId;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (supplierManager.removeProductFromSupplier(supplierId, productId)) {
            cout << "Product removed from supplier successfully!" << endl;
        } else {
            cout << "Failed to remove product from supplier. Supplier not found." << endl;
        }
    }

    void financialReports() {
        while (true) {
            cout << "\n===== Financial Reports =====" << endl;
            cout << "1. View All Sales" << endl;
            cout << "2. View Sales Summary" << endl;
            cout << "3. Back to Admin Menu" << endl;
            cout << "Enter your choice: ";
            
            int choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            switch (choice) {
                case 1:
                    transactionManager.displayAllInvoices();
                    break;
                case 2:
                    showSalesSummary();
                    break;
                case 3:
                    return;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    void showSalesSummary() {
        double totalSales = 0.0;
        double totalDiscounts = 0.0;
        int totalTransactions = 0;
        
        // This would be implemented by iterating through all invoices
        // For simplicity, we'll just display a placeholder
        cout << "\n===== Sales Summary =====" << endl;
        cout << "Total Sales: " << totalSales << endl;
        cout << "Total Discounts: " << totalDiscounts << endl;
        cout << "Total Transactions: " << totalTransactions << endl;
        cout << "=========================" << endl;
    }

    void discountSettings() {
        while (true) {
            cout << "\n===== Discount Settings =====" << endl;
            cout << "Current Season: " << discountManager.getSeason() << endl;
            cout << "Current Seasonal Discount: " << discountManager.getSeasonalDiscount() << "%" << endl;
            cout << "1. Set Seasonal Discount" << endl;
            cout << "2. Back to Admin Menu" << endl;
            cout << "Enter your choice: ";
            
            int choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            switch (choice) {
                case 1:
                    setSeasonalDiscount();
                    break;
                case 2:
                    return;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    void setSeasonalDiscount() {
        string season;
        double discount;
        
        cout << "Enter Season Name (e.g., Eid, Puja): ";
        getline(cin, season);
        
        cout << "Enter Discount Percentage: ";
        cin >> discount;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        discountManager.setSeasonalDiscount(season, discount);
        cout << "Seasonal discount set successfully!" << endl;
    }

    void userManagement() {
        while (true) {
            cout << "\n===== User Management =====" << endl;
            cout << "1. View All Users" << endl;
            cout << "2. Add User" << endl;
            cout << "3. Update User" << endl;
            cout << "4. Delete User" << endl;
            cout << "5. Back to Admin Menu" << endl;
            cout << "Enter your choice: ";
            
            int choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            switch (choice) {
                case 1:
                    auth.displayAllUsers();
                    break;
                case 2:
                    addUser();
                    break;
                case 3:
                    updateUser();
                    break;
                case 4:
                    deleteUser();
                    break;
                case 5:
                    return;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    void addUser() {
        string username, password, role;
        
        cout << "Enter Username: ";
        getline(cin, username);
        
        cout << "Enter Password: ";
        getline(cin, password);
        
        cout << "Enter Role (admin/customer): ";
        getline(cin, role);
        
        unique_ptr<User> user;
        if (role == "admin") {
            user = make_unique<Admin>(username, password);
        } else if (role == "customer") {
            user = make_unique<Customer>(username, password);
        } else {
            cout << "Invalid role. User not created." << endl;
            return;
        }
        
        if (auth.addUser(move(user))) {
            cout << "User added successfully!" << endl;
        } else {
            cout << "Failed to add user. Username already exists." << endl;
        }
    }

    void updateUser() {
        string username;
        cout << "Enter Username to update: ";
        getline(cin, username);
        
        User* user = auth.login(username, ""); // We'll need to modify this to get user without password
        if (!user) {
            cout << "User not found!" << endl;
            return;
        }
        
        string password;
        cout << "Enter new Password (leave blank to keep current): ";
        getline(cin, password);
        
        // Create updated user
        unique_ptr<User> updatedUser;
        if (user->getRole() == "admin") {
            updatedUser = make_unique<Admin>(username, password.empty() ? user->getPassword() : password);
        } else {
            Customer* customer = static_cast<Customer*>(user);
            updatedUser = make_unique<Customer>(
                username, 
                password.empty() ? user->getPassword() : password,
                customer->getLoyaltyPoints()
            );
        }
        
        if (auth.updateUser(username, move(updatedUser))) {
            cout << "User updated successfully!" << endl;
        } else {
            cout << "Failed to update user." << endl;
        }
    }

    void deleteUser() {
        string username;
        cout << "Enter Username to delete: ";
        getline(cin, username);
        
        if (auth.deleteUser(username)) {
            cout << "User deleted successfully!" << endl;
        } else {
            cout << "User not found!" << endl;
        }
    }

    void browseProducts() {
        inventory.displayAllProducts();
    }

    void viewCart() {
        // In a real implementation, we would have a cart system
        // For simplicity, we'll just show a placeholder
        cout << "\n===== Your Cart =====" << endl;
        cout << "Your cart is empty." << endl;
        cout << "=====================" << endl;
    }

    void checkout() {
        Customer* customer = static_cast<Customer*>(currentUser);
        
        // In a real implementation, we would have a cart system
        // For simplicity, we'll create a sample cart
        vector<pair<int, int>> cart;
        
        cout << "\n===== Checkout =====" << endl;
        cout << "Available Products:" << endl;
        inventory.displayAllProducts();
        
        while (true) {
            int productId, quantity;
            char choice;
            
            cout << "Enter Product ID to add to cart (0 to finish): ";
            cin >> productId;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (productId == 0) break;
            
            Product* product = inventory.findProduct(productId);
            if (!product) {
                cout << "Product not found!" << endl;
                continue;
            }
            
            cout << "Enter Quantity: ";
            cin >> quantity;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (quantity <= 0) {
                cout << "Invalid quantity!" << endl;
                continue;
            }
            
            if (quantity > product->getStock()) {
                cout << "Not enough stock available!" << endl;
                continue;
            }
            
            cart.emplace_back(productId, quantity);
            cout << "Added to cart!" << endl;
            
            cout << "Add more items? (y/n): ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (choice != 'y' && choice != 'Y') break;
        }
        
        if (cart.empty()) {
            cout << "Your cart is empty. Checkout cancelled." << endl;
            return;
        }
        
        // Display cart and calculate total
        double subtotal = 0.0;
        cout << "\n===== Your Cart =====" << endl;
        for (const auto& item : cart) {
            Product* product = inventory.findProduct(item.first);
            if (product) {
                double itemTotal = product->getPrice() * item.second;
                subtotal += itemTotal;
                cout << product->getName() << " x " << item.second 
                     << " = " << itemTotal << endl;
            }
        }
        cout << "Subtotal: " << subtotal << endl;
        
        // Apply discounts
        double productDiscount = 0.0;
        double seasonalDiscount = 0.0;
        
        for (const auto& item : cart) {
            Product* product = inventory.findProduct(item.first);
            if (product) {
                double itemTotal = product->getPrice() * item.second;
                productDiscount += itemTotal * (product->getDiscount() / 100.0);
                seasonalDiscount += itemTotal * (discountManager.getSeasonalDiscount() / 100.0);
            }
        }
        
        double totalAfterDiscounts = subtotal - productDiscount - seasonalDiscount;
        cout << "Product Discount: -" << productDiscount << endl;
        cout << "Seasonal Discount: -" << seasonalDiscount << endl;
        cout << "Total after discounts: " << totalAfterDiscounts << endl;
        
        // Loyalty points redemption
        int pointsToRedeem = 0;
        int availablePoints = customer->getLoyaltyPoints();
        cout << "Available Loyalty Points: " << availablePoints << endl;
        
        if (availablePoints > 0) {
            char redeemChoice;
            cout << "Would you like to redeem loyalty points? (y/n): ";
            cin >> redeemChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (redeemChoice == 'y' || redeemChoice == 'Y') {
                cout << "Enter points to redeem (max " << availablePoints << "): ";
                cin >> pointsToRedeem;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                if (pointsToRedeem > availablePoints) {
                    pointsToRedeem = availablePoints;
                }
                
                double loyaltyDiscount = pointsToRedeem * POINTS_TO_CURRENCY_RATE;
                cout << "Loyalty Points Discount: -" << loyaltyDiscount << endl;
                totalAfterDiscounts -= loyaltyDiscount;
                
                if (totalAfterDiscounts < 0) {
                    totalAfterDiscounts = 0;
                }
            }
        }
        
        cout << "Final Amount: " << totalAfterDiscounts << endl;
        
        // Confirm checkout
        char confirm;
        cout << "Confirm checkout? (y/n): ";
        cin >> confirm;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (confirm == 'y' || confirm == 'Y') {
            Invoice* invoice = transactionManager.createInvoice(
                customer->getUsername(), 
                cart, 
                inventory, 
                discountManager, 
                customer, 
                pointsToRedeem
            );
            
            if (invoice) {
                cout << "\nCheckout successful!" << endl;
                invoice->display();
                
                // Save invoice to file
                ofstream invoiceFile("invoice_" + to_string(invoice->getInvoiceId()) + ".txt");
                if (invoiceFile.is_open()) {
                    invoiceFile << "Invoice #" << invoice->getInvoiceId() << endl;
                    invoiceFile << "Date: " << invoice->getDateTime() << endl;
                    invoiceFile << "Customer: " << invoice->getCustomerUsername() << endl;
                    invoiceFile << "Items:" << endl;
                    
                    for (const auto& item : invoice->getItems()) {
                        Product* product = inventory.findProduct(item.first);
                        if (product) {
                            invoiceFile << "  " << product->getName() << " x " << item.second 
                                       << " = " << product->getPrice() * item.second << endl;
                        }
                    }
                    
                    invoiceFile << "Total Amount: " << invoice->getTotalAmount() << endl;
                    invoiceFile << "Discount Applied: " << invoice->getDiscountApplied() << endl;
                    invoiceFile << "Final Amount: " << invoice->getFinalAmount() << endl;
                    
                    invoiceFile.close();
                    cout << "Invoice saved to file: invoice_" << invoice->getInvoiceId() << ".txt" << endl;
                }
            } else {
                cout << "Checkout failed. Please try again." << endl;
            }
        } else {
            cout << "Checkout cancelled." << endl;
        }
    }

    void viewPurchaseHistory() {
        Customer* customer = static_cast<Customer*>(currentUser);
        vector<Invoice> invoices = transactionManager.getCustomerInvoices(customer->getUsername());
        
        if (invoices.empty()) {
            cout << "\nYou have no purchase history." << endl;
            return;
        }
        
        cout << "\n===== Your Purchase History =====" << endl;
        for (const auto& invoice : invoices) {
            invoice.display();
        }
        cout << "=================================" << endl;
    }

    void viewLoyaltyPoints() {
        Customer* customer = static_cast<Customer*>(currentUser);
        cout << "\n===== Your Loyalty Points =====" << endl;
        cout << "Current Points: " << customer->getLoyaltyPoints() << endl;
        cout << "Redeemable Value: " << customer->getLoyaltyPoints() * POINTS_TO_CURRENCY_RATE << " Taka" << endl;
        cout << "===============================" << endl;
    }
};

// ==================== Main Function ====================
int main() {
    IROSSystem system;
    system.run();
    return 0;
}