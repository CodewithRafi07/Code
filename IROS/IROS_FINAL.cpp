#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <filesystem>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <limits>

using namespace std;
namespace fs = std::filesystem;

// ---------- STRUCTS ----------
struct Product {
    string id, name, category, supplierName;
    double price;
    int stock;
    double discount;
};

struct Customer {
    string username, password;
    bool isMember = false;
    int loyaltyPoints = 0;
};

struct CartItem {
    string productId;
    int quantity;
};

struct Admin {
    string username, password;
};

struct Branch {
    string id;
    string name;
    string location;
};

// ---------- GLOBAL VARIABLES ----------
vector<Product> products;
vector<Admin> admins;
vector<Customer> customers;
vector<Branch> branches;
Customer currentUser;
vector<CartItem> cart;
double seasonalDiscount = 0.0;
double totalSales = 0.0;
Branch currentBranch;

// ---------- FILE PATHS ----------
const string ADMIN_FILE = "admins.csv";
const string PRODUCT_FILE = "products.csv";
const string LOGIN_FILE = "admin_login_history.csv";
const string CUSTOMER_LOGIN_FILE = "customer_login_history.csv";
const string CUSTOMER_FILE = "customers.csv";
const string WISHLIST_FILE = "wishlist.csv";
const string SALES_FILE = "sales.csv";
const string BRANCH_FILE = "branches.csv";
const string SECRET_CODE = "IROSUPER4";

// ---------- FUNCTION PROTOTYPES ----------
void customerMenu();
void paymentProcess(double total);
void saveCustomer(const Customer &c);
void addToCart(const string &pid, int qty);
void viewCart();
void checkoutCart();
void selectBranch();
void loadBranches();
void saveBranches();
void viewCombinedSalesReport();
string getBranchFilePath(const string& filename);
void addBranch();
void updateStockAfterPurchase();
void viewCustomerProfile();
void removeFromCart();
void updateCartQuantity();
void clearCart();
void changeCustomerPassword();
void viewCustomers();
void viewAdmins();
void deleteAdmin();
void deleteBranch();
void updateBranch();
void viewBranches();
void resetCustomerPassword();
void resetAdminPassword();
void viewSalesHistory();
void viewCustomerLoginHistory();

// ---------- UTILITY ----------
void ensureDataFolder() {
    if (!fs::exists("data")) fs::create_directory("data");
    
    // Create branch directories if they don't exist
    for (const auto& branch : branches) {
        string branchDir = "data/branch_" + branch.id;
        if (!fs::exists(branchDir)) fs::create_directory(branchDir);
        if (!fs::exists(branchDir + "/invoices")) fs::create_directory(branchDir + "/invoices");
    }
}

string getBranchFilePath(const string& filename) {
    return "data/branch_" + currentBranch.id + "/" + filename;
}

// ---------- FILE HANDLING ----------
void loadAdmins() {
    admins.clear();
    ifstream fin(getBranchFilePath(ADMIN_FILE));
    if (!fin) return;
    
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string user, pass;
        
        try {
            if (ss >> user >> pass) {
                admins.push_back({user, pass});
            }
        } catch (...) {
            cout << "Warning: Failed to parse admin line: " << line << "\n";
        }
    }
    fin.close();
}

void saveAdmins() {
    ofstream fout(getBranchFilePath(ADMIN_FILE));
    for (auto &a : admins)
        fout << a.username << " " << a.password << "\n";
    fout.close();
}

void loadProducts() {
    products.clear();
    ifstream fin(getBranchFilePath(PRODUCT_FILE));
    if (!fin) return;
    
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string id, name, category, supplier;
        double price, discount;
        int stock;
        
        try {
            if (getline(ss, id, ',') && 
                getline(ss, name, ',') && 
                ss >> price && 
                ss.ignore() && 
                ss >> stock && 
                ss.ignore() && 
                getline(ss, category, ',') && 
                getline(ss, supplier, ',') && 
                ss >> discount) {
                products.push_back({id, name, category, supplier, price, stock, discount});
            }
        } catch (...) {
            cout << "Warning: Failed to parse product line: " << line << "\n";
        }
    }
    fin.close();
}

void saveProducts() {
    ofstream fout(getBranchFilePath(PRODUCT_FILE));
    for (auto &p : products)
        fout << p.id << "," << p.name << "," << p.price << "," << p.stock << ","
             << p.category << "," << p.supplierName << "," << p.discount << "\n";
    fout.close();
}

void loadCustomers() {
    customers.clear();
    ifstream fin(getBranchFilePath(CUSTOMER_FILE));
    if (!fin) return;

    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string user, pass;
        int memInt, points;
        
        try {
            if (ss >> user >> pass >> memInt >> points) {
                bool isMem = (memInt == 1);
                customers.push_back({user, pass, isMem, points});
            }
        } catch (...) {
            cout << "Warning: Failed to parse customer line: " << line << "\n";
        }
    }
    fin.close();
}

void saveCustomers() {
    ofstream fout(getBranchFilePath(CUSTOMER_FILE));
    for (auto &c : customers)
        fout << c.username << " " << c.password << " " << (c.isMember ? 1 : 0)
             << " " << c.loyaltyPoints << "\n";
    fout.close();
}

void saveCustomer(const Customer &c) {
    // Check if customer exists
    bool exists = false;
    for (auto &cust : customers) {
        if (cust.username == c.username) {
            cust = c;
            exists = true;
            break;
        }
    }
    if (!exists) customers.push_back(c);
    saveCustomers();
}

void loadBranches() {
    branches.clear();
    ifstream fin("data/" + BRANCH_FILE);
    if (!fin) {
        // Create default branch if none exists
        branches.push_back({"BR001", "Main Branch", "Downtown"});
        branches.push_back({"BR002", "North Branch", "Uptown"});
        branches.push_back({"BR003", "South Branch", "Suburb"});
        branches.push_back({"BR004", "East Branch", "Industrial Area"});
        branches.push_back({"BR005", "West Branch", "Shopping District"});
        saveBranches();
        return;
    }
    
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string id, name, location;
        
        try {
            if (getline(ss, id, ',') && getline(ss, name, ',') && getline(ss, location)) {
                branches.push_back({id, name, location});
            }
        } catch (...) {
            cout << "Warning: Failed to parse branch line: " << line << "\n";
        }
    }
    fin.close();
}

void saveBranches() {
    ofstream fout("data/" + BRANCH_FILE);
    for (auto &b : branches) {
        fout << b.id << "," << b.name << "," << b.location << "\n";
    }
    fout.close();
}

// ---------- BRANCH SELECTION ----------
void selectBranch() {
    if (branches.empty()) {
        cout << "No branches available!\n";
        return;
    }
    
    while (true) {
        cout << "\n===== SELECT BRANCH =====\n";
        for (size_t i = 0; i < branches.size(); i++) {
            cout << i+1 << ". " << branches[i].name << " (" << branches[i].location << ")\n";
        }
        
        int choice;
        cout << "Enter choice (1-" << branches.size() << "): ";
        cin >> choice;
        
         if (choice >= 1 && static_cast<size_t>(choice) <= branches.size()) {
            currentBranch = branches[choice-1];
            cout << "Selected branch: " << currentBranch.name << "\n";
            
            // Ensure branch directory exists
            string branchDir = "data/branch_" + currentBranch.id;
            if (!fs::exists(branchDir)) fs::create_directory(branchDir);
            if (!fs::exists(branchDir + "/invoices")) fs::create_directory(branchDir + "/invoices");
            break;
        } else {
            cout << "Invalid choice! Please try again.\n";
        }
    }
}

// ---------- ADMIN FUNCTIONS ----------
void registerAdmin() {
    string user, pass;
    cout << "\nEnter new username: ";
    cin >> user;
    
    // Check if admin with the same username already exists
    for (auto &a : admins) {
        if (a.username == user) {
            cout << " An admin with username " << user << " already exists!\n";
            return;
        }
    }
    
    cout << "Enter password: ";
    cin >> pass;

    admins.push_back({user, pass});
    saveAdmins();
    cout << "Admin registered successfully!\n";
}

bool loginAdmin() {
    string user, pass;
    cout << "\nUsername: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;

    for (auto &a : admins) {
        if (a.username == user && a.password == pass) {
            ofstream fout(getBranchFilePath(LOGIN_FILE), ios::app);
            fout << user << "\n";
            fout.close();
            cout << "Login successful! Welcome, " << user << " at " << currentBranch.name << "\n";
            return true;
        }
    }
    cout << "Invalid Password!\n";
    return false;
}

// ---------- PRODUCT MANAGEMENT ----------
void addProduct() {
    Product p;
    cout << "\nEnter Product ID: ";
    cin >> p.id;
    
    // Check if product with the same ID already exists
    for (auto &product : products) {
        if (product.id == p.id) {
            cout << " A product with ID " << p.id << " already exists!\n";
            return;
        }
    }
    
    cout << "Enter Name: ";
    cin >> p.name;
    cout << "Enter Price: ";
    cin >> p.price;
    cout << "Enter Stock: ";
    cin >> p.stock;
    cout << "Enter Category: ";
    cin >> p.category;
    cout << "Enter Supplier Name: ";
    cin >> p.supplierName;
    cout << "Enter Discount (%): ";
    cin >> p.discount;

    products.push_back(p);
    saveProducts();
    cout << "Product added successfully!\n";
}

void viewProducts() {
    // Get unique categories
    vector<string> categories;
    for (auto &p : products) {
        bool found = false;
        for (auto &cat : categories) {
            if (cat == p.category) {
                found = true;
                break;
            }
        }
        if (!found) {
            categories.push_back(p.category);
        }
    }
    
    int filterOption;
    cout << "\nFilter products by:\n";
    cout << "1. All Products\n";
    cout << "2. Search by Name or ID\n";
    for (size_t i = 0; i < categories.size(); i++) {
        cout << i+3 << ". " << categories[i] << "\n";
    }
    cout << "Enter choice: ";
    if (!(cin >> filterOption) || filterOption < 1 || filterOption > (int)categories.size()+2) {
        cout << "Invalid input! Showing all products.\n";
        filterOption = 1;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    // Create a copy of products to filter and sort
    vector<Product> filteredProducts;
    
    // Filter the products based on the selected option
    if (filterOption == 1) {
        filteredProducts = products;
    } else if (filterOption == 2) {
        string searchTerm;
        cout << "Enter product name or ID to search: ";
        cin >> ws;
        getline(cin, searchTerm);
        
        // Convert search term to lowercase for case-insensitive search
        string searchTermLower = searchTerm;
        transform(searchTermLower.begin(), searchTermLower.end(), searchTermLower.begin(), ::tolower);
        
        for (auto &p : products) {
            // Convert product name and ID to lowercase for case-insensitive search
            string nameLower = p.name;
            transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
            string idLower = p.id;
            transform(idLower.begin(), idLower.end(), idLower.begin(), ::tolower);
            
            if (nameLower.find(searchTermLower) != string::npos || idLower.find(searchTermLower) != string::npos) {
                filteredProducts.push_back(p);
            }
        }
    } else {
        string selectedCategory = categories[filterOption-3];
        for (auto &p : products) {
            if (p.category == selectedCategory) {
                filteredProducts.push_back(p);
            }
        }
    }
    
    if (filteredProducts.empty()) {
        cout << "No products found matching your criteria.\n";
        return;
    }
    
    int sortOption;
    cout << "\nSort products by:\n";
    cout << "1. ID\n";
    cout << "2. Name\n";
    cout << "3. Price (Low to High)\n";
    cout << "4. Price (High to Low)\n";
    cout << "5. Stock (Low to High)\n";
    cout << "6. Stock (High to Low)\n";
    cout << "Enter choice: ";
    if (!(cin >> sortOption) || sortOption < 1 || sortOption > 6) {
        cout << "Invalid input! Showing products in default order.\n";
        sortOption = 1;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    // Sort the products based on the selected option
    switch (sortOption) {
        case 1: // Sort by ID
            sort(filteredProducts.begin(), filteredProducts.end(), [](const Product &a, const Product &b) {
                return a.id < b.id;
            });
            break;
        case 2: // Sort by Name
            sort(filteredProducts.begin(), filteredProducts.end(), [](const Product &a, const Product &b) {
                return a.name < b.name;
            });
            break;
        case 3: // Sort by Price (Low to High)
            sort(filteredProducts.begin(), filteredProducts.end(), [](const Product &a, const Product &b) {
                return a.price < b.price;
            });
            break;
        case 4: // Sort by Price (High to Low)
            sort(filteredProducts.begin(), filteredProducts.end(), [](const Product &a, const Product &b) {
                return a.price > b.price;
            });
            break;
        case 5: // Sort by Stock (Low to High)
            sort(filteredProducts.begin(), filteredProducts.end(), [](const Product &a, const Product &b) {
                return a.stock < b.stock;
            });
            break;
        case 6: // Sort by Stock (High to Low)
            sort(filteredProducts.begin(), filteredProducts.end(), [](const Product &a, const Product &b) {
                return a.stock > b.stock;
            });
            break;
    }
    
    // Pagination
    const int pageSize = 10;
    int totalPages = (filteredProducts.size() + pageSize - 1) / pageSize;
    int currentPage = 1;
    
    while (true) {
        cout << "\n===== ALL PRODUCTS (Page " << currentPage << " of " << totalPages << ") =====\n";
        cout << left << setw(10) << "ID" << setw(20) << "Name"
             << setw(15) << "Category" << setw(15) << "Supplier"
             << setw(10) << "Price" << setw(10) << "Stock"
             << setw(10) << "Discount(%)" << endl;
        cout << string(90, '-') << endl;
        
        int startIdx = (currentPage - 1) * pageSize;
        int endIdx = min(startIdx + pageSize, (int)filteredProducts.size());
        
        for (int i = startIdx; i < endIdx; i++) {
            auto &p = filteredProducts[i];
            cout << left << setw(10) << p.id << setw(20) << p.name
                 << setw(15) << p.category << setw(15) << p.supplierName
                 << setw(10) << p.price << setw(10) << p.stock
                 << setw(10) << p.discount << endl;
        }
        
        cout << "\nNavigation:\n";
        if (currentPage > 1) cout << "1. Previous Page\n";
        if (currentPage < totalPages) cout << "2. Next Page\n";
        cout << "3. Go to Page\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        
        int navOption;
        if (!(cin >> navOption)) {
            cout << "Invalid input!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        
        if (navOption == 1 && currentPage > 1) {
            currentPage--;
        } else if (navOption == 2 && currentPage < totalPages) {
            currentPage++;
        } else if (navOption == 3) {
            cout << "Enter page number (1-" << totalPages << "): ";
            int pageNum;
            if (!(cin >> pageNum) || pageNum < 1 || pageNum > totalPages) {
                cout << "Invalid page number!\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else {
                currentPage = pageNum;
            }
        } else if (navOption == 4) {
            break;
        } else {
            cout << "Invalid choice!\n";
        }
    }
}

void updateProduct() {
    string id;
    cout << "\nEnter Product ID to update: ";
    cin >> id;
    for (auto &p : products) {
        if (p.id == id) {
            cout << "\nCurrent Product Information:\n";
            cout << "Name: " << p.name << "\n";
            cout << "Price: " << p.price << "\n";
            cout << "Stock: " << p.stock << "\n";
            cout << "Category: " << p.category << "\n";
            cout << "Supplier: " << p.supplierName << "\n";
            cout << "Discount: " << p.discount << "%\n";
            
            cout << "\nEnter new name (leave blank to keep current): ";
            string newName;
            cin >> ws;
            getline(cin, newName);
            if (!newName.empty()) p.name = newName;
            
            cout << "Enter new price (enter -1 to keep current): ";
            double newPrice;
            cin >> newPrice;
            if (newPrice >= 0) p.price = newPrice;
            
            cout << "Enter new stock (enter -1 to keep current): ";
            int newStock;
            cin >> newStock;
            if (newStock >= 0) p.stock = newStock;
            
            cout << "Enter new category (leave blank to keep current): ";
            string newCategory;
            cin >> ws;
            getline(cin, newCategory);
            if (!newCategory.empty()) p.category = newCategory;
            
            cout << "Enter new supplier (leave blank to keep current): ";
            string newSupplier;
            cin >> ws;
            getline(cin, newSupplier);
            if (!newSupplier.empty()) p.supplierName = newSupplier;
            
            cout << "Enter new discount % (enter -1 to keep current): ";
            double newDiscount;
            cin >> newDiscount;
            if (newDiscount >= 0) p.discount = newDiscount;
            
            saveProducts();
            cout << "Product updated successfully!\n";
            return;
        }
    }
    cout << "Product not found!\n";
}

void deleteProduct() {
    string id;
    cout << "\nEnter Product ID to delete: ";
    cin >> id;
    for (auto it = products.begin(); it != products.end(); ++it) {
        if (it->id == id) {
            cout << "Are you sure you want to delete the following product?\n";
            cout << "ID: " << it->id << "\n";
            cout << "Name: " << it->name << "\n";
            cout << "Price: " << it->price << "\n";
            cout << "Stock: " << it->stock << "\n";
            cout << "Category: " << it->category << "\n";
            cout << "Supplier: " << it->supplierName << "\n";
            cout << "Discount: " << it->discount << "%\n";
            
            cout << "Enter 'y' to confirm deletion: ";
            char confirm;
            cin >> confirm;
            
            if (confirm == 'y' || confirm == 'Y') {
                products.erase(it);
                saveProducts();
                cout << "Product deleted successfully!\n";
            } else {
                cout << "Deletion cancelled.\n";
            }
            return;
        }
    }
    cout << "Product not found!\n";
}

// ---------- ADMIN REPORTS ----------
void lowStockAlert() {
    cout << "\n===== LOW STOCK ALERTS (<15 units) =====\n";
    bool found = false;
    for (auto &p : products) {
        if (p.stock < 15) {
            cout << p.name << " (Stock: " << p.stock << ")\n";
            found = true;
        }
    }
    if (!found) cout << "All products are well-stocked.\n";
}

void viewFinancialReport() {
    totalSales = 0.0;
    int salesCount = 0;
    ifstream fin(getBranchFilePath(SALES_FILE));
    if (!fin) { cout << "\nNo sales record found.\n"; return; }

    string line;
    while (getline(fin, line)) {
        size_t pos = line.find_last_of(',');
        if (pos != string::npos) {
            double sale = stod(line.substr(pos + 1));
            totalSales += sale;
            salesCount++;
        }
    }
    fin.close();

    cout << "\n===== FINANCIAL REPORT FOR " << currentBranch.name << " =====\n";
    cout << fixed << setprecision(2);
    cout << "Number of Sales: " << salesCount << "\n";
    cout << "Total Sales: " << totalSales << " Taka\n";
    if (salesCount > 0) {
        cout << "Average Sale: " << totalSales / salesCount << " Taka\n";
    }
}

void viewCombinedSalesReport() {
    double combinedTotal = 0.0;
    int combinedSalesCount = 0;
    
    cout << "\n===== COMBINED SALES REPORT =====\n";
    cout << left << setw(15) << "Branch ID" << setw(20) << "Branch Name" 
         << setw(15) << "Sales Count" << setw(15) << "Total Sales" << endl;
    cout << string(65, '-') << endl;
    
    for (auto &branch : branches) {
        double branchTotal = 0.0;
        int branchSalesCount = 0;
        string salesFile = "data/branch_" + branch.id + "/" + SALES_FILE;
        ifstream fin(salesFile);
        
        if (fin) {
            string line;
            while (getline(fin, line)) {
                size_t pos = line.find_last_of(',');
                if (pos != string::npos) {
                    double sale = stod(line.substr(pos + 1));
                    branchTotal += sale;
                    branchSalesCount++;
                }
            }
            fin.close();
        }
        
        combinedTotal += branchTotal;
        combinedSalesCount += branchSalesCount;
        cout << left << setw(15) << branch.id << setw(20) << branch.name 
             << setw(15) << branchSalesCount << setw(15) << fixed << setprecision(2) << branchTotal << endl;
    }
    
    cout << string(65, '-') << endl;
    cout << right << setw(35) << "Combined Total: " << fixed << setprecision(2) << combinedTotal << " Taka\n";
    cout << right << setw(35) << "Combined Sales Count: " << combinedSalesCount << "\n";
    if (combinedSalesCount > 0) {
        cout << right << setw(35) << "Average Sale: " << fixed << setprecision(2) << combinedTotal / combinedSalesCount << " Taka\n";
    }
}

void setSeasonalDiscount() {
    double discount;
    cout << "\nEnter Seasonal Discount Percentage (0 to deactivate): ";
    cin >> discount;
    
    if (discount < 0 || discount > 100) {
        cout << "Invalid discount percentage! Please enter a value between 0 and 100.\n";
        return;
    }
    
    seasonalDiscount = discount;

    for (auto &p : products) p.discount = seasonalDiscount;
    saveProducts();
    cout << "Seasonal discount of " << seasonalDiscount
         << "% applied to all products!\n";
}

bool verifySecret() { 
    cout << "\nEnter secret code to continue export: ";
    string code;
    cin >> ws;
    getline(cin, code);
    if (code == SECRET_CODE) {
        cout << "Secret verified.\n";
        return true;
    } else {
        cout << "Wrong code! Export cancelled.\n";
        return false;
    }
}

// Export Products
void exportProducts() {
    if (!verifySecret()) return; // must pass secret

    string filepath;
    cout << "\nEnter file path to save Products (e.g. C:\\\\Users\\\\Rafi\\\\Desktop\\\\products.csv): ";
    cin >> ws;
    getline(cin, filepath);

    ofstream fout(filepath);
    if (!fout) {
        cout << "Failed to open file. Check path again.\n";
        return;
    }

    fout << "Product ID,Name,Category,Supplier,Price,Stock,Discount(%)\n";
    for (auto &p : products) {
        fout << p.id << "," << p.name << "," << p.category << "," 
             << p.supplierName << "," << p.price << "," << p.stock << "," 
             << p.discount << "\n";
    }

    fout.close();
    cout << "Products successfully exported to: " << filepath << endl;
}

// Export Admins
void exportAdmins() {
    if (!verifySecret()) return; // must pass secret

    string filepath;
    cout << "\nEnter file path to save Admins (e.g. C:\\\\Users\\\\Rafi\\\\Desktop\\\\admins.csv): ";
    cin >> ws;
    getline(cin, filepath);

    ofstream fout(filepath);
    if (!fout) {
        cout << "Failed to open file. Check path again.\n";
        return;
    }

    fout << "Username,Password\n";
    for (auto &a : admins) {
        fout << a.username << "," << a.password << "\n";
    }

    fout.close();
    cout << "Admin list successfully exported to: " << filepath << endl;
}

// Export Login History
void exportLoginHistory() {
    if (!verifySecret()) return; // must pass secret

    string filepath;
    cout << "\nEnter file path to save Login History (e.g. C:\\\\Users\\\\Rafi\\\\Desktop\\\\login_history.csv): ";
    cin >> ws;
    getline(cin, filepath);

    ifstream fin(getBranchFilePath(LOGIN_FILE));
    if (!fin) {
        cout << "No login history found!\n";
        return;
    }

    ofstream fout(filepath);
    if (!fout) {
        cout << "Failed to open file. Check path again.\n";
        return;
    }

    fout << "Logged-in Username\n";
    string line;
    while (getline(fin, line)) {
        fout << line << "\n";
    }

    fin.close();
    fout.close();
    cout << "Login history successfully exported to: " << filepath << endl;
}

// ---------- BRANCH MANAGEMENT ----------
void addBranch() {
    Branch b;
    cout << "\nEnter Branch ID: ";
    cin >> b.id;
    
    // Check if branch with the same ID already exists
    for (auto &branch : branches) {
        if (branch.id == b.id) {
            cout << "A branch with ID " << b.id << " already exists!\n";
            return;
        }
    }
    
    cout << "Enter Branch Name: ";
    cin >> b.name;
    cout << "Enter Branch Location: ";
    cin >> b.location;

    branches.push_back(b);
    saveBranches();
    
    // Create branch directory
    string branchDir = "data/branch_" + b.id;
    if (!fs::exists(branchDir)) fs::create_directory(branchDir);
    if (!fs::exists(branchDir + "/invoices")) fs::create_directory(branchDir + "/invoices");
    
    cout << "Branch added successfully!\n";
}

void viewBranches() {
    cout << "\n===== ALL BRANCHES =====\n";
    cout << left << setw(15) << "Branch ID" << setw(20) << "Branch Name" 
         << setw(20) << "Location" << endl;
    cout << string(55, '-') << endl;
    for (auto &b : branches) {
        cout << left << setw(15) << b.id << setw(20) << b.name 
             << setw(20) << b.location << endl;
    }
}

void updateBranch() {
    string id;
    cout << "\nEnter Branch ID to update: ";
    cin >> id;
    
    // Check if the branch exists
    bool found = false;
    for (auto &b : branches) {
        if (b.id == id) {
            found = true;
            break;
        }
    }
    
    if (!found) {
        cout << "❌ Branch not found!\n";
        return;
    }
    
    // Find the branch and update it
    for (auto &b : branches) {
        if (b.id == id) {
            cout << "\nCurrent Branch Information:\n";
            cout << "ID: " << b.id << "\n";
            cout << "Name: " << b.name << "\n";
            cout << "Location: " << b.location << "\n";
            
            cout << "\nEnter new name (leave blank to keep current): ";
            string newName;
            cin >> ws;
            getline(cin, newName);
            if (!newName.empty()) b.name = newName;
            
            cout << "Enter new location (leave blank to keep current): ";
            string newLocation;
            cin >> ws;
            getline(cin, newLocation);
            if (!newLocation.empty()) b.location = newLocation;
            
            saveBranches();
            cout << "Branch updated successfully!\n";
            return;
        }
    }
}

void deleteBranch() {
    string id;
    cout << "\nEnter Branch ID to delete: ";
    cin >> id;
    
    // Check if the branch exists
    bool found = false;
    for (auto &b : branches) {
        if (b.id == id) {
            found = true;
            break;
        }
    }
    
    if (!found) {
        cout << "Branch not found!\n";
        return;
    }
    
    // Ask for confirmation
    cout << "Are you sure you want to delete branch " << id << "? (y/n): ";
    char confirm;
    cin >> confirm;
    
    if (confirm == 'y' || confirm == 'Y') {
        for (auto it = branches.begin(); it != branches.end(); ++it) {
            if (it->id == id) {
                branches.erase(it);
                saveBranches();
                cout << "Branch deleted successfully!\n";
                return;
            }
        }
    } else {
        cout << "Deletion cancelled.\n";
    }
}

// ---------- CUSTOMER FUNCTIONS ----------
void registerCustomer() {
    string user, pass;
    cout << "\nEnter new username: ";
    cin >> user;
    
    // Check if customer with the same username already exists
    for (auto &c : customers) {
        if (c.username == user) {
            cout << "A customer with username " << user << " already exists!\n";
            return;
        }
    }
    
    cout << "Enter password: ";
    cin >> pass;

    currentUser = {user, pass, false, 0};
    saveCustomer(currentUser);
    cout << "Customer registered successfully!\n";
}

bool loginCustomer() {
    string user, pass;
    cout << "\nUsername: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;

    for (auto &c : customers) {
        if (c.username == user && c.password == pass) {
            currentUser = c;
            ofstream fout(getBranchFilePath(CUSTOMER_LOGIN_FILE), ios::app);
            fout << user << "\n";
            fout.close();
            cout << "Login successful! Welcome, " << user << " at " << currentBranch.name << "\n";
            return true;
        }
    }
    cout << "Invalid username or password!\n";
    return false;
}

bool loginOrRegister() {
    int choice;
    cout << "\n1. Login\n2. Register\nChoose: ";
    cin >> choice;
    if (choice == 1) return loginCustomer();
    else if (choice == 2) { registerCustomer(); return true; }
    else return false;
}

void viewCustomerProfile() {
    cout << "\n===== CUSTOMER PROFILE =====\n";
    cout << "Username: " << currentUser.username << "\n";
    cout << "Membership Status: " << (currentUser.isMember ? "Member" : "Non-Member") << "\n";
    cout << "Loyalty Points: " << currentUser.loyaltyPoints << "\n";
    
    if (currentUser.isMember) {
        cout << "Membership Benefits:\n";
        cout << "- Earn 1 loyalty point for every 100 BDT spent\n";
        cout << "- Exclusive discounts and offers\n";
    } else {
        cout << "Become a member by spending 6000 BDT or more in a single purchase!\n";
    }
}

void changeCustomerPassword() {
    string currentPass, newPass, confirmPass;
    
    cout << "\nEnter current password: ";
    cin >> currentPass;
    
    if (currentPass != currentUser.password) {
        cout << "Incorrect current password!\n";
        return;
    }
    
    cout << "Enter new password: ";
    cin >> newPass;
    
    cout << "Confirm new password: ";
    cin >> confirmPass;
    
    if (newPass != confirmPass) {
        cout << "Passwords do not match!\n";
        return;
    }
    
    currentUser.password = newPass;
    saveCustomer(currentUser);
    cout << "Password changed successfully!\n";
}

// ---------- CART & PAYMENT ----------
void addToCart(const string &pid, int qty) {
    if (qty <= 0) {
        cout << "Invalid quantity! Please enter a positive number.\n";
        return;
    }
    
    // Check if product exists
    bool productFound = false;
    for (auto &p : products) {
        if (p.id == pid) {
            productFound = true;
            break;
        }
    }
    
    if (!productFound) {
        cout << "Product not found!\n";
        return;
    }
    
    // Check if there's enough stock
    for (auto &p : products) {
        if (p.id == pid) {
            if (p.stock < qty) {
                cout << "Not enough stock available! Only " << p.stock << " units in stock.\n";
                return;
            }
            break;
        }
    }
    
    // Add to cart or update quantity
    for (auto &item : cart) {
        if (item.productId == pid) { 
            // Check if adding more quantity exceeds available stock
            for (auto &p : products) {
                if (p.id == pid && p.stock >= (item.quantity + qty)) {
                    item.quantity += qty; 
                    cout << "🛒 Quantity updated in cart!\n"; 
                    return;
                } else if (p.id == pid) {
                    cout << "Not enough stock to add more quantity! Only " << p.stock << " units in stock.\n";
                    return;
                }
            }
        }
    }
    cart.push_back({pid, qty});
    cout << "Added to cart!\n";
}

void viewCart() {
    if (cart.empty()) { cout << "Your cart is empty.\n"; return; }
    double total = 0;
    double totalWithoutDiscount = 0;
    cout << "\n===== YOUR CART =====\n";
    cout << left << setw(10) << "ID" << setw(20) << "Name"
         << setw(10) << "Price" << setw(10) << "Discount" << setw(10) << "Qty" << setw(15) << "Total\n";
    cout << string(75, '-') << endl;
    for (auto &item : cart) {
        for (auto &p : products) {
            if (p.id == item.productId) {
                double price = p.price * (1 - p.discount / 100);
                double subtotal = price * item.quantity;
                double subtotalWithoutDiscount = p.price * item.quantity;
                total += subtotal;
                totalWithoutDiscount += subtotalWithoutDiscount;
                cout << left << setw(10) << p.id << setw(20) << p.name
                     << setw(10) << fixed << setprecision(2) << p.price 
                     << setw(10) << fixed << setprecision(2) << p.discount << "%"
                     << setw(10) << item.quantity
                     << setw(15) << fixed << setprecision(2) << subtotal << "\n";
            }
        }
    }
    cout << string(75, '-') << "\n";
    cout << "Subtotal: " << fixed << setprecision(2) << totalWithoutDiscount << " BDT\n";
    cout << "Discount: " << fixed << setprecision(2) << (totalWithoutDiscount - total) << " BDT\n";
    cout << "Cart Total: " << fixed << setprecision(2) << total << " BDT\n";
}

double calculateCartTotal() {
    double total = 0;
    for (auto &item : cart) {
        for (auto &p : products) {
            if (p.id == item.productId) {
                double price = p.price * (1 - p.discount / 100);
                total += price * item.quantity;
            }
        }
    }
    return total;
}

void removeFromCart() {
    if (cart.empty()) {
        cout << "Your cart is empty.\n";
        return;
    }
    
    viewCart();
    
    string pid;
    cout << "\nEnter Product ID to remove from cart: ";
    cin >> pid;
    
    for (auto it = cart.begin(); it != cart.end(); ++it) {
        if (it->productId == pid) {
            cart.erase(it);
            cout << "✅ Product removed from cart!\n";
            return;
        }
    }
    
    cout << "Product not found in cart!\n";
}

void updateCartQuantity() {
    if (cart.empty()) {
        cout << "Your cart is empty.\n";
        return;
    }
    
    viewCart();
    
    string pid;
    cout << "\nEnter Product ID to update quantity: ";
    cin >> pid;
    
    for (auto &item : cart) {
        if (item.productId == pid) {
            int newQty;
            cout << "Enter new quantity (0 to remove): ";
            if (!(cin >> newQty) || newQty < 0) {
                cout << "Invalid input! Please enter a positive number or 0.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return;
            }
            
            if (newQty == 0) {
                cart.erase(remove_if(cart.begin(), cart.end(), [&pid](const CartItem &item) {
                    return item.productId == pid;
                }), cart.end());
                cout << "Product removed from cart!\n";
            } else {
                // Check if there's enough stock
                for (auto &p : products) {
                    if (p.id == pid && p.stock >= newQty) {
                        item.quantity = newQty;
                        cout << "Quantity updated!\n";
                        return;
                    } else if (p.id == pid) {
                        cout << "Not enough stock! Only " << p.stock << " units available.\n";
                        return;
                    }
                }
            }
            return;
        }
    }
    
    cout << "Product not found in cart!\n";
}

void clearCart() {
    if (cart.empty()) {
        cout << "Your cart is already empty.\n";
        return;
    }
    
    cout << "Are you sure you want to clear your cart? (y/n): ";
    char confirm;
    cin >> confirm;
    
    if (confirm == 'y' || confirm == 'Y') {
        cart.clear();
        cout << "Cart cleared!\n";
    } else {
        cout << "Operation cancelled.\n";
    }
}

void updateStockAfterPurchase() {
    for (auto &item : cart) {
        bool productFound = false;
        for (auto &p : products) {
            if (p.id == item.productId) {
                productFound = true;
                if (p.stock >= item.quantity) {
                    p.stock -= item.quantity;
                } else {
                    cout << "Warning: Not enough stock for product " << p.name << ". Current stock: " << p.stock << ", Requested: " << item.quantity << "\n";
                    p.stock = 0; // Set to 0 if not enough stock
                }
                break;
            }
        }
        if (!productFound) {
            cout << "Warning: Product with ID " << item.productId << " not found in inventory.\n";
        }
    }
    saveProducts();
}

void paymentProcess(double total) {
    // Check if all products in the cart are still available in the inventory
    for (auto &item : cart) {
        bool productFound = false;
        for (auto &p : products) {
            if (p.id == item.productId) {
                productFound = true;
                if (p.stock < item.quantity) {
                    cout << "Not enough stock for product " << p.name << "! Only " << p.stock << " units available.\n";
                    return;
                }
                break;
            }
        }
        if (!productFound) {
            cout << "Product with ID " << item.productId << " not found in inventory!\n";
            return;
        }
    }
    
    cout << "\nPayment Methods:\n1. Cash\n2. Bkash\n3. Nagad\n4. Card\nChoose: ";
    int opt; 
    if (!(cin >> opt)) {
        cout << "Invalid input! Please enter a number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    
    bool success = false;
    string method;
    double change = 0.0;

    if (opt == 1) {
        method = "Cash";
        double paid; 
        cout << "Enter amount: "; 
        if (!(cin >> paid)) {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        
        if (paid < total) { 
            cout << "Not enough cash.\n"; 
            return; 
        }
        change = paid - total;
        success = true;
    } else if (opt == 2 || opt == 3) {
        method = (opt==2?"Bkash":"Nagad");
        string num; 
        cout << "Enter 11-digit number: "; 
        cin >> num;
        success = (num.size() == 11 && num.find_first_not_of("0123456789") == string::npos);
        if (!success) {
            cout << "Invalid phone number! Please enter an 11-digit number.\n";
            return;
        }
    } else if (opt == 4) {
        method = "Card";
        string card; 
        cout << "Enter 16-digit card number: "; 
        cin >> card;
        success = (card.size() == 16 && card.find_first_not_of("0123456789") == string::npos);
        if (!success) {
            cout << "Invalid card number! Please enter a 16-digit number.\n";
            return;
        }
    } else {
        cout << "Invalid payment method!\n";
        return;
    }

    cout << (success?" Payment Successful!\n":" Payment failed!\n");
    if (!success) return;

    // Generate unique invoice ID
    string invoiceId = "INV-" + currentBranch.id + "-" + to_string(time(0));
    
    // Get current date and time
    time_t now = time(0);
    char* dt = ctime(&now);
    
    ofstream fout(getBranchFilePath(SALES_FILE), ios::app);
    fout << invoiceId << "," << currentUser.username << "," << total << "\n";
    fout.close();

    //Membership & loyalty
    if (!currentUser.isMember && total >= 6000) {
        cout << "Eligible for membership! Accept? (y/n): ";
        char ch; cin >> ch;
        if (ch == 'y' || ch == 'Y') currentUser.isMember = true;
    }

    if (currentUser.isMember)
        currentUser.loyaltyPoints += (int)(total / 100);

    saveCustomer(currentUser);

    // Update stock after purchase
    updateStockAfterPurchase();

    // Generate invoice
    string filename = "data/branch_" + currentBranch.id + "/invoices/Invoice_" + currentUser.username + "_" + to_string(time(0)) + ".txt";
    ofstream inv(filename);
    
    // Display detailed invoice in terminal
    cout << "\n===== IROS - INVOICE =====\n";
    cout << "Invoice ID: " << invoiceId << "\n";
    cout << "Date: " << dt;
    cout << "Customer: " << currentUser.username << "\n";
    cout << "Branch: " << currentBranch.name << "\n";
    cout << "Payment Method: " << method << "\n";
    
    inv << "\n===== IROS - INVOICE =====\n";
    inv << "Invoice ID: " << invoiceId << "\n";
    inv << "Date: " << dt;
    inv << "Customer: " << currentUser.username << "\n";
    inv << "Branch: " << currentBranch.name << "\n";
    inv << "Payment Method: " << method << "\n";
    
    cout << left << setw(10) << "ID" << setw(20) << "Name"
         << setw(10) << "Price" << setw(10) << "Qty" << setw(15) << "Total\n";
    cout << string(65, '-') << endl;
    
    inv << left << setw(10) << "ID" << setw(20) << "Name"
        << setw(10) << "Price" << setw(10) << "Qty" << setw(15) << "Total\n";
    inv << string(65, '-') << endl;
    
    double invoiceTotal = 0.0;
    for (auto &item : cart) {
        for (auto &p : products) {
            if (p.id == item.productId) {
                double price = p.price * (1 - p.discount / 100);
                double subtotal = price * item.quantity;
                invoiceTotal += subtotal;
                
                cout << left << setw(10) << p.id << setw(20) << p.name
                     << setw(10) << fixed << setprecision(2) << price 
                     << setw(10) << item.quantity
                     << setw(15) << fixed << setprecision(2) << subtotal << "\n";
                
                inv << left << setw(10) << p.id << setw(20) << p.name
                    << setw(10) << fixed << setprecision(2) << price 
                    << setw(10) << item.quantity
                    << setw(15) << fixed << setprecision(2) << subtotal << "\n";
            }
        }
    }
    
    cout << string(65, '-') << "\n";
    cout << "Subtotal: " << fixed << setprecision(2) << invoiceTotal << " BDT\n";
    
    inv << string(65, '-') << "\n";
    inv << "Subtotal: " << fixed << setprecision(2) << invoiceTotal << " BDT\n";
    
    if (change > 0) {
        cout << "Amount Paid: " << fixed << setprecision(2) << (invoiceTotal + change) << " BDT\n";
        cout << "Change: " << fixed << setprecision(2) << change << " BDT\n";
        
        inv << "Amount Paid: " << fixed << setprecision(2) << (invoiceTotal + change) << " BDT\n";
        inv << "Change: " << fixed << setprecision(2) << change << " BDT\n";
    }
    
    cout << "Membership: " << (currentUser.isMember?"Yes":"No") << "\n";
    cout << "Loyalty Points: " << currentUser.loyaltyPoints << "\n";
    
    inv << "Membership: " << (currentUser.isMember?"Yes":"No") << "\n";
    inv << "Loyalty Points: " << currentUser.loyaltyPoints << "\n";
    inv.close();
    
    cout << "🧾 Invoice saved to: " << filename << "\n";

    cart.clear(); // empty cart
}

void checkoutCart() {
    if (cart.empty()) { cout << "Cart is empty!\n"; return; }
    
    // Display cart summary
    cout << "\n===== CHECKOUT SUMMARY =====\n";
    double total = calculateCartTotal();
    double totalWithoutDiscount = 0;
    
    for (auto &item : cart) {
        for (auto &p : products) {
            if (p.id == item.productId) {
                double subtotalWithoutDiscount = p.price * item.quantity;
                totalWithoutDiscount += subtotalWithoutDiscount;
            }
        }
    }
    
    cout << "Subtotal: " << fixed << setprecision(2) << totalWithoutDiscount << " BDT\n";
    cout << "Discount: " << fixed << setprecision(2) << (totalWithoutDiscount - total) << " BDT\n";
    cout << "Total: " << fixed << setprecision(2) << total << " BDT\n";
    
    cout << "\nProceed to payment? (y/n): ";
    char confirm;
    cin >> confirm;
    
    if (confirm == 'y' || confirm == 'Y') {
        paymentProcess(total);
    } else {
        cout << "Checkout cancelled.\n";
    }
}

// ---------- ADMIN MANAGEMENT ----------
void viewCustomers() {
    cout << "\n===== ALL CUSTOMERS =====\n";
    cout << left << setw(20) << "Username" << setw(15) << "Membership"
         << setw(15) << "Loyalty Points" << endl;
    cout << string(50, '-') << endl;
    for (auto &c : customers) {
        cout << left << setw(20) << c.username 
             << setw(15) << (c.isMember ? "Member" : "Non-Member")
             << setw(15) << c.loyaltyPoints << endl;
    }
}

void viewAdmins() {
    cout << "\n===== ALL ADMINS =====\n";
    cout << left << setw(20) << "Username" << endl;
    cout << string(20, '-') << endl;
    for (auto &a : admins) {
        cout << left << setw(20) << a.username << endl;
    }
}

void deleteAdmin() {
    string user;
    cout << "\nEnter Admin username to delete: ";
    cin >> user;
    
    // Check if the admin exists
    bool found = false;
    for (auto &a : admins) {
        if (a.username == user) {
            found = true;
            break;
        }
    }
    
    if (!found) {
        cout << "Admin not found!\n";
        return;
    }
    
    // Ask for confirmation
    cout << "Are you sure you want to delete admin " << user << "? (y/n): ";
    char confirm;
    cin >> confirm;
    
    if (confirm == 'y' || confirm == 'Y') {
        for (auto it = admins.begin(); it != admins.end(); ++it) {
            if (it->username == user) {
                admins.erase(it);
                saveAdmins();
                cout << "Admin deleted successfully!\n";
                return;
            }
        }
    } else {
        cout << "Deletion cancelled.\n";
    }
}

void resetCustomerPassword() {
    string user;
    cout << "\nEnter Customer username to reset password: ";
    cin >> user;
    
    // Check if the customer exists
    bool found = false;
    for (auto &c : customers) {
        if (c.username == user) {
            found = true;
            break;
        }
    }
    
    if (!found) {
        cout << "Customer not found!\n";
        return;
    }
    
    // Ask for confirmation
    cout << "Are you sure you want to reset password for customer " << user << "? (y/n): ";
    char confirm;
    cin >> confirm;
    
    if (confirm == 'y' || confirm == 'Y') {
        string newPass;
        cout << "Enter new password: ";
        cin >> newPass;
        
        for (auto &c : customers) {
            if (c.username == user) {
                c.password = newPass;
                saveCustomers();
                cout << "Password reset successfully!\n";
                return;
            }
        }
    } else {
        cout << "Password reset cancelled.\n";
    }
}

void resetAdminPassword() {
    string user;
    cout << "\nEnter Admin username to reset password: ";
    cin >> user;
    
    // Check if the admin exists
    bool found = false;
    for (auto &a : admins) {
        if (a.username == user) {
            found = true;
            break;
        }
    }
    
    if (!found) {
        cout << "Admin not found!\n";
        return;
    }
    
    // Ask for confirmation
    cout << "Are you sure you want to reset password for admin " << user << "? (y/n): ";
    char confirm;
    cin >> confirm;
    
    if (confirm == 'y' || confirm == 'Y') {
        string newPass;
        cout << "Enter new password: ";
        cin >> newPass;
        
        for (auto &a : admins) {
            if (a.username == user) {
                a.password = newPass;
                saveAdmins();
                cout << "Password reset successfully!\n";
                return;
            }
        }
    } else {
        cout << "Password reset cancelled.\n";
    }
}

void viewSalesHistory() {
    ifstream fin(getBranchFilePath(SALES_FILE));
    if (!fin) { 
        cout << "\nNo sales record found.\n"; 
        return; 
    }

    cout << "\n===== SALES HISTORY FOR " << currentBranch.name << " =====\n";
    cout << left << setw(20) << "Invoice ID" << setw(20) << "Customer"
         << setw(15) << "Amount" << endl;
    cout << string(55, '-') << endl;
    
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue; // Skip empty lines
        
        stringstream ss(line);
        string invoiceId, customer;
        double amount;
        
        try {
            if (getline(ss, invoiceId, ',') && 
                getline(ss, customer, ',') && 
                ss >> amount) {
                cout << left << setw(20) << invoiceId << setw(20) << customer
                     << setw(15) << fixed << setprecision(2) << amount << endl;
            }
        } catch (...) {
            cout << "Warning: Failed to parse sales line: " << line << "\n";
        }
    }
    fin.close();
}

void viewCustomerLoginHistory() {
    ifstream fin(getBranchFilePath(CUSTOMER_LOGIN_FILE));
    if (!fin) { 
        cout << "\nNo customer login history found.\n"; 
        return; 
    }

    cout << "\n===== CUSTOMER LOGIN HISTORY FOR " << currentBranch.name << " =====\n";
    cout << left << setw(20) << "Username" << endl;
    cout << string(20, '-') << endl;
    
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue; // Skip empty lines
        
        cout << left << setw(20) << line << endl;
    }
    fin.close();
}

// ---------- ADMIN MENU ----------
void adminMenu() {
    int choice;
    do {
        cout << "\n========== ADMIN PANEL (" << currentBranch.name << ") ==========\n";
        cout << "1. Add Product\n";
        cout << "2. View All Products\n";
        cout << "3. Update Product\n";
        cout << "4. Delete Product\n";
        cout << "5. Low Stock Alerts\n";
        cout << "6. Set Seasonal Discount\n";
        cout << "7. View Financial Report\n";
        cout << "8. Export Products (protected)\n";
        cout << "9. Export Admins (protected)\n";
        cout << "10. Export Login History (protected)\n";
        cout << "11. View Combined Sales Report\n";
        cout << "12. Add New Branch\n";
        //cout << "13. View All Customers\n";
        //cout << "14. View All Admins\n";
        //cout << "15. Delete Admin\n";
        cout << "13. Delete Branch\n";
        cout << "14. Update Branch\n";
        cout << "15. View All Branches\n";
        //cout << "19. Reset Customer Password\n";
        //cout << "20. Reset Admin Password\n";
        cout << "16. View Sales History\n";
        //cout << "22. View Customer Login History\n";
        cout << "17. Logout\n";
        cout << "Enter choice: ";
        if (!(cin >> choice)) {
            cout << " Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: addProduct(); break;
            case 2: viewProducts(); break;
            case 3: updateProduct(); break;
            case 4: deleteProduct(); break;
            case 5: lowStockAlert(); break;
            case 6: setSeasonalDiscount(); break;
            case 7: viewFinancialReport(); break;
            case 8: exportProducts(); break;
            case 9: exportAdmins(); break;
            case 10: exportLoginHistory(); break;
            case 11: viewCombinedSalesReport(); break;
            case 12: addBranch(); break;
           // case 13: viewCustomers(); break;
           // case 14: viewAdmins(); break;
           // case 15: deleteAdmin(); break;
            case 13: deleteBranch(); break;
            case 14: updateBranch(); break;
            case 15: viewBranches(); break;
           // case 19: resetCustomerPassword(); break;
           // case 20: resetAdminPassword(); break;
            case 16: viewSalesHistory(); break;
           // case 22: viewCustomerLoginHistory(); break;
            case 17: cout << "Log out.\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 17);
}

// ---------- CUSTOMER MENU ----------
void customerMenu() {
    int choice;
    do {
        cout << "\n===== CUSTOMER PANEL (" << currentBranch.name << ") =====\n";
        cout << "1. View Products\n";
        cout << "2. Add to Cart\n";
        cout << "3. View Cart\n";
        cout << "4. Remove from Cart\n";
        cout << "5. Update Cart Quantity\n";
        cout << "6. Clear Cart\n";
        cout << "7. Checkout\n";
        cout << "8. View Profile\n";
        cout << "9. Change Password\n";
        cout << "10. Logout\n";
        cout << "Enter choice: ";
        if (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 1) viewProducts();
        else if (choice == 2) {
            string pid; int qty;
            cout << "Enter Product ID: "; cin >> pid;
            cout << "Enter Quantity: "; 
            if (!(cin >> qty)) {
                cout << "Invalid input! Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            addToCart(pid, qty);
        }
        else if (choice == 3) viewCart();
        else if (choice == 4) removeFromCart();
        else if (choice == 5) updateCartQuantity();
        else if (choice == 6) clearCart();
        else if (choice == 7) checkoutCart();
        else if (choice == 8) viewCustomerProfile();
        else if (choice == 9) changeCustomerPassword();
        else if (choice == 10) cout << "Logging out...\n";
        else cout << "Invalid choice!\n";
    } while (choice != 10);
}

// ---------- MAIN ----------
int main() {
    ensureDataFolder();
    loadBranches();
    
    cout << "==========================================\n";
    cout << "      I-ROS ADMIN & CUSTOMER SYSTEM      \n";
    cout << "              Version 1.0                \n";
    cout << "==========================================\n";
    int mainChoice;
    do {
        cout << "\n1. Admin Panel\n";
        cout << "2. Customer Panel\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        if (!(cin >> mainChoice)) {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (mainChoice) {
            case 1: {
                selectBranch();
                loadAdmins();
                loadProducts();
                
                int adminOption;
                do {
                    cout << "\n1. Register Admin\n";
                    cout << "2. Login as Admin\n";
                    cout << "3. Back to Main\n";
                    cout << "Enter option: ";
                    if (!(cin >> adminOption)) {
                        cout << "Invalid input! Please enter a number.\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;
                    }

                    switch (adminOption) {
                        case 1: registerAdmin(); break;
                        case 2:
                            if (loginAdmin())
                                adminMenu();
                            break;
                        case 3: break;
                        default: cout << "Invalid option!\n";
                    }
                } while (adminOption != 3);
                break;
            }
            case 2:
                selectBranch();
                loadCustomers();
                loadProducts();
                
                if (loginOrRegister()) {
                    customerMenu();
                }
                break;
            case 3:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (mainChoice != 3);

    return 0;
}