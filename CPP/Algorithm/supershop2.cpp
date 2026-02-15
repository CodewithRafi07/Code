#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
using namespace std;

// ============================
// Product Class
// ============================
class Product {
public:
    int id;
    string name, category, supplier;
    double price;
    int stock;
    double discount;

    Product() {}
    Product(int id, string n, string c, double p, int s, string sup, double d = 0)
        : id(id), name(n), category(c), price(p), stock(s), supplier(sup), discount(d) {}

    double getFinalPrice() const {
        return price - (price * discount / 100.0);
    }
};

// ============================
// User Class
// ============================
class User {
public:
    string username, password;
    bool isAdmin;
    User() {}
    User(string u, string p, bool a) : username(u), password(p), isAdmin(a) {}
};

// ============================
// Cart Item
// ============================
class CartItem {
public:
    Product product;
    int quantity;
    CartItem(Product p, int q) : product(p), quantity(q) {}
};

// ============================
// Customer Class
// ============================
class Customer : public User {
public:
    vector<CartItem> cart;
    double loyaltyPoints = 0;

    Customer(string u, string p) : User(u, p, false) {}

    void addToCart(Product &p, int qty) {
        cart.push_back(CartItem(p, qty));
        cout << qty << " x " << p.name << " added to cart.\n";
    }

    double checkout() {
        double total = 0;
        cout << "\n--- Invoice ---\n";
        for (auto &item : cart) {
            double itemTotal = item.quantity * item.product.getFinalPrice();
            cout << item.product.name << " x " << item.quantity
                 << " = " << itemTotal << " TK\n";
            total += itemTotal;
        }
        cout << "----------------\nTotal: " << total << " TK\n";

        // Loyalty points
        loyaltyPoints += total * 0.05;
        saveSlip(total);
        cart.clear();
        return total;
    }

    void saveSlip(double total) {
        time_t now = time(0);
        string filename = "invoice_" + username + ".txt";
        ofstream out(filename, ios::app);
        out << "=== Invoice for " << username << " ===\n";
        out << "Date: " << ctime(&now);
        out << "Total: " << total << " TK\n";
        out << "Loyalty Points: " << loyaltyPoints << "\n\n";
        out.close();
    }
};

// ============================
// Admin Class
// ============================
class Admin : public User {
public:
    Admin(string u, string p) : User(u, p, true) {}
};

// ============================
// Shop System
// ============================
class SuperShop {
private:
    vector<Product> products;
    vector<User> users;

public:
    SuperShop() { loadAllData(); }

    ~SuperShop() { saveAllData(); }

    // ---------------------------
    // Product Management
    // ---------------------------
    void addProduct(Product p) {
        products.push_back(p);
        cout << "Product added successfully!\n";
    }

    void updateStock(int id, int qty) {
        for (auto &p : products) {
            if (p.id == id) {
                p.stock += qty;
                cout << "Stock updated.\n";
                return;
            }
        }
        cout << "Product not found.\n";
    }

    void showAllProducts() {
        cout << "\n--- Product List ---\n";
        cout << left << setw(5) << "ID" << setw(15) << "Name" << setw(10) << "Price"
             << setw(10) << "Stock" << setw(15) << "Category" << setw(15) << "Supplier"
             << "Discount\n";
        for (auto &p : products) {
            cout << left << setw(5) << p.id << setw(15) << p.name
                 << setw(10) << p.price << setw(10) << p.stock
                 << setw(15) << p.category << setw(15) << p.supplier
                 << p.discount << "%\n";
        }
    }

    Product* findProductById(int id) {
        for (auto &p : products)
            if (p.id == id)
                return &p;
        return nullptr;
    }

    // ---------------------------
    // Sales Handling
    // ---------------------------
    void recordSale(string user, double amount) {
        time_t now = time(0);
        ofstream out("sales.txt", ios::app);
        out << user << "," << amount << "," << ctime(&now);
        out.close();
    }

    // ---------------------------
    // User Handling
    // ---------------------------
    void addUser(User u) { users.push_back(u); }

    User* login(string u, string p) {
        for (auto &user : users) {
            if (user.username == u && user.password == p) {
                cout << "Login successful!\n";
                return &user;
            }
        }
        cout << "Invalid credentials!\n";
        return nullptr;
    }

    // ---------------------------
    // File Handling
    // ---------------------------
    void loadAllData() {
        loadProducts();
        loadUsers();
        cout << "Data loaded successfully.\n";
    }

    void saveAllData() {
        saveProducts();
        saveUsers();
        cout << "Data saved successfully.\n";
    }

    void loadProducts() {
        ifstream in("products.txt");
        if (!in) return;
        products.clear();
        int id, stock;
        string name, category, supplier;
        double price, discount;
        while (in >> id >> ws && getline(in, name, ',') &&
               getline(in, category, ',') && in >> price >> stock >> discount >> ws &&
               getline(in, supplier)) {
            products.push_back(Product(id, name, category, price, stock, supplier, discount));
        }
        in.close();
    }

    void saveProducts() {
        ofstream out("products.txt");
        for (auto &p : products)
            out << p.id << " " << p.name << "," << p.category << ","
                << p.price << " " << p.stock << " " << p.discount
                << " " << p.supplier << "\n";
        out.close();
    }

    void loadUsers() {
        ifstream in("users.txt");
        if (!in) return;
        users.clear();
        string u, p;
        int role;
        while (in >> u >> p >> role)
            users.push_back(User(u, p, role));
        in.close();
    }

    void saveUsers() {
        ofstream out("users.txt");
        for (auto &u : users)
            out << u.username << " " << u.password << " " << u.isAdmin << "\n";
        out.close();
    }
};

// ============================
// MAIN FUNCTION
// ============================
int main() {
    SuperShop shop;

    // Initial data (if file empty)
    shop.addUser(User("admin", "1234", true));
    shop.addUser(User("customer", "1111", false));

    Admin admin("admin", "1234");
    Customer customer("customer", "1111");

    cout << "\n===== Super Shop Management System =====\n";
    cout << "1. Admin Panel\n2. Customer Panel\nSelect: ";
    int choice; cin >> choice;

    if (choice == 1) {
        cout << "Admin Menu:\n1. Add Product\n2. Show All Products\n";
        int opt; cin >> opt;
        if (opt == 1) {
            int id, stock; double price, discount;
            string name, category, supplier;
            cout << "Enter ID Name Category Price Stock Supplier Discount%:\n";
            cin >> id >> name >> category >> price >> stock >> supplier >> discount;
            shop.addProduct(Product(id, name, category, price, stock, supplier, discount));
        } else {
            shop.showAllProducts();
        }
    } else if (choice == 2) {
        cout << "Customer Menu:\n1. View Products\n2. Buy Product\n";
        int opt; cin >> opt;
        shop.showAllProducts();
        if (opt == 2) {
            int id, qty;
            cout << "Enter Product ID and Quantity: ";
            cin >> id >> qty;
            Product* p = shop.findProductById(id);
            if (p && p->stock >= qty) {
                customer.addToCart(*p, qty);
                double total = customer.checkout();
                p->stock -= qty;
                shop.recordSale(customer.username, total);
            } else {
                cout << "Invalid product or insufficient stock.\n";
            }
        }
    }

    return 0;
}
