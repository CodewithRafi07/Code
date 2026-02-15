// super_shop.cpp
// Super Shop Management System (file-based, console)
// Author: ChatGPT (example implementation)
// Compile: g++ -std=c++17 super_shop.cpp -o super_shop

#include <bits/stdc++.h>
using namespace std;

// --------------------------- Utility: DateTime --------------------------------
string now_datetime() {
    time_t t = time(nullptr);
    tm *lt = localtime(&t);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", lt);
    return string(buf);
}
string now_date() {
    time_t t = time(nullptr);
    tm *lt = localtime(&t);
    char buf[16];
    strftime(buf, sizeof(buf), "%Y-%m-%d", lt);
    return string(buf);
}
string date_month_key(const string &datetime) {
    // expects YYYY-MM-DD ... -> returns YYYY-MM
    if (datetime.size() >= 7) return datetime.substr(0,7);
    return datetime;
}

// --------------------------- Product -----------------------------------------
struct Product {
    int id;
    string name;
    string category;
    double price;
    int stock;
    double discount; // percentage 0-100
    string supplier;

    Product(): id(0), name(""), category(""), price(0), stock(0), discount(0), supplier("") {}
    string to_csv() const {
        // id,name,category,price,stock,discount,supplier
        string esc_name = name;
        replace(esc_name.begin(), esc_name.end(), ',', ';');
        string esc_cat = category; replace(esc_cat.begin(), esc_cat.end(), ',', ';');
        string esc_sup = supplier; replace(esc_sup.begin(), esc_sup.end(), ',', ';');
        ostringstream oss;
        oss << id << "," << esc_name << "," << esc_cat << "," << price << "," << stock << "," << discount << "," << esc_sup;
        return oss.str();
    }
    static Product from_csv(const string &line) {
        Product p;
        stringstream ss(line);
        string token;
        getline(ss, token, ','); p.id = stoi(token);
        getline(ss, token, ','); p.name = token;
        getline(ss, token, ','); p.category = token;
        getline(ss, token, ','); p.price = stod(token);
        getline(ss, token, ','); p.stock = stoi(token);
        getline(ss, token, ','); p.discount = stod(token);
        getline(ss, token, ','); p.supplier = token;
        return p;
    }
};

// --------------------------- User --------------------------------------------
struct User {
    string username;
    string password;
    string role; // "admin" or "customer"
    int loyaltyPoints;
    User():username(""),password(""),role("customer"),loyaltyPoints(0){}
    string to_csv() const {
        string u = username, p = password;
        replace(u.begin(), u.end(), ',', ';');
        replace(p.begin(), p.end(), ',', ';');
        ostringstream oss;
        oss << u << "," << p << "," << role << "," << loyaltyPoints;
        return oss.str();
    }
    static User from_csv(const string &line) {
        User u;
        stringstream ss(line);
        string token;
        getline(ss, token, ','); u.username = token;
        getline(ss, token, ','); u.password = token;
        getline(ss, token, ','); u.role = token;
        getline(ss, token, ','); u.loyaltyPoints = stoi(token);
        return u;
    }
};

// --------------------------- Supplier ----------------------------------------
struct Supplier {
    string name;
    string contact;
    vector<int> suppliedIDs;
    string to_csv() const {
        string n = name, c = contact;
        replace(n.begin(), n.end(), ',', ';');
        replace(c.begin(), c.end(), ',', ';');
        ostringstream oss;
        oss << n << "," << c << ",";
        for (size_t i=0;i<suppliedIDs.size();++i){
            oss << suppliedIDs[i];
            if (i+1<suppliedIDs.size()) oss << ";";
        }
        return oss.str();
    }
    static Supplier from_csv(const string &line) {
        Supplier s;
        stringstream ss(line);
        string token;
        getline(ss, token, ','); s.name = token;
        getline(ss, token, ','); s.contact = token;
        getline(ss, token, ',');
        if (!token.empty()) {
            stringstream ids(token);
            string id;
            while (getline(ids, id, ';')) {
                s.suppliedIDs.push_back(stoi(id));
            }
        }
        return s;
    }
};

// --------------------------- CartItem & Invoice -------------------------------
struct CartItem {
    int productID;
    string productName;
    double unitPrice;
    int qty;
    double discountPercent;
    CartItem():productID(0),productName(""),unitPrice(0),qty(0),discountPercent(0){}
    double total() const {
        double after = unitPrice * (1.0 - discountPercent/100.0);
        return after * qty;
    }
};

struct Invoice {
    int invoiceID;
    string username;
    vector<CartItem> items;
    double totalAmount;
    string datetime;
    Invoice():invoiceID(0),username(""),totalAmount(0),datetime(""){}
    string to_csv() const {
        // invoiceID,username,totalAmount,datetime,items: id|name|unit|qty|disc;...
        ostringstream oss;
        oss << invoiceID << "," << username << "," << totalAmount << "," << datetime << ",";
        for (size_t i=0;i<items.size();++i){
            const CartItem &it = items[i];
            oss << it.productID << "|" << it.productName << "|" << it.unitPrice << "|" << it.qty << "|" << it.discountPercent;
            if (i+1<items.size()) oss << ";";
        }
        return oss.str();
    }
    static Invoice from_csv(const string &line) {
        Invoice inv;
        stringstream ss(line);
        string token;
        getline(ss, token, ','); inv.invoiceID = stoi(token);
        getline(ss, token, ','); inv.username = token;
        getline(ss, token, ','); inv.totalAmount = stod(token);
        getline(ss, token, ','); inv.datetime = token;
        getline(ss, token, ','); // items
        if (!token.empty()) {
            stringstream its(token);
            string it;
            while (getline(its, it, ';')) {
                stringstream iis(it);
                string f;
                CartItem ci;
                getline(iis, f, '|'); ci.productID = stoi(f);
                getline(iis, f, '|'); ci.productName = f;
                getline(iis, f, '|'); ci.unitPrice = stod(f);
                getline(iis, f, '|'); ci.qty = stoi(f);
                getline(iis, f, '|'); ci.discountPercent = stod(f);
                inv.items.push_back(ci);
            }
        }
        return inv;
    }
};

// --------------------------- Data Files & FileManager -------------------------
const string PRODUCTS_FILE = "products.csv";
const string USERS_FILE = "users.csv";
const string SALES_FILE = "sales.csv";
const string SUPPLIERS_FILE = "suppliers.csv";
const string BACKUP_DIR = "backup/";

struct FileManager {
    static vector<Product> loadProducts() {
        vector<Product> v;
        ifstream ifs(PRODUCTS_FILE);
        if (!ifs) return v;
        string line;
        while (getline(ifs, line)) {
            if (line.empty()) continue;
            v.push_back(Product::from_csv(line));
        }
        return v;
    }
    static void saveProducts(const vector<Product> &v) {
        ofstream ofs(PRODUCTS_FILE);
        for (auto &p: v) ofs << p.to_csv() << "\n";
    }
    static vector<User> loadUsers() {
        vector<User> v;
        ifstream ifs(USERS_FILE);
        if (!ifs) return v;
        string line;
        while (getline(ifs, line)) {
            if (line.empty()) continue;
            v.push_back(User::from_csv(line));
        }
        return v;
    }
    static void saveUsers(const vector<User> &v) {
        ofstream ofs(USERS_FILE);
        for (auto &u: v) ofs << u.to_csv() << "\n";
    }
    static vector<Invoice> loadSales() {
        vector<Invoice> v;
        ifstream ifs(SALES_FILE);
        if (!ifs) return v;
        string line;
        while (getline(ifs, line)) {
            if (line.empty()) continue;
            v.push_back(Invoice::from_csv(line));
        }
        return v;
    }
    static void appendSale(const Invoice &inv) {
        ofstream ofs(SALES_FILE, ios::app);
        ofs << inv.to_csv() << "\n";
    }
    static vector<Supplier> loadSuppliers() {
        vector<Supplier> v;
        ifstream ifs(SUPPLIERS_FILE);
        if (!ifs) return v;
        string line;
        while (getline(ifs, line)) {
            if (line.empty()) continue;
            v.push_back(Supplier::from_csv(line));
        }
        return v;
    }
    static void saveSuppliers(const vector<Supplier> &v) {
        ofstream ofs(SUPPLIERS_FILE);
        for (auto &s: v) ofs << s.to_csv() << "\n";
    }
    static void backupAll() {
        // Simple backup: copy files to backup/ with timestamp
        system(("mkdir -p " + BACKUP_DIR).c_str());
        string ts = now_datetime();
        for (auto fname : {PRODUCTS_FILE, USERS_FILE, SALES_FILE, SUPPLIERS_FILE}) {
            ifstream src(fname, ios::binary);
            if (!src) continue;
            string dst = BACKUP_DIR + fname + "." + ts;
            ofstream out(dst, ios::binary);
            out << src.rdbuf();
        }
    }
};

// --------------------------- Inventory & Managers ----------------------------
struct Inventory {
    vector<Product> products;
    int nextID;
    Inventory():nextID(1){}
    void load() {
        products = FileManager::loadProducts();
        nextID = 1;
        for (auto &p : products) if (p.id >= nextID) nextID = p.id + 1;
    }
    void save() {
        FileManager::saveProducts(products);
    }
    Product* findByID(int id) {
        for (auto &p: products) if (p.id == id) return &p;
        return nullptr;
    }
    vector<Product*> searchByName(const string &kw) {
        vector<Product*> res;
        string low = kw; transform(low.begin(), low.end(), low.begin(), ::tolower);
        for (auto &p: products) {
            string n = p.name; transform(n.begin(), n.end(), n.begin(), ::tolower);
            if (n.find(low) != string::npos) res.push_back(&p);
        }
        return res;
    }
    vector<Product*> filterByCategory(const string &cat) {
        vector<Product*> res;
        string low = cat; transform(low.begin(), low.end(), low.begin(), ::tolower);
        for (auto &p: products) {
            string c = p.category; transform(c.begin(), c.end(), c.begin(), ::tolower);
            if (c == low) res.push_back(&p);
        }
        return res;
    }
    Product addProduct(const Product &p_in) {
        Product p = p_in;
        p.id = nextID++;
        products.push_back(p);
        save();
        return p;
    }
    bool removeProduct(int id) {
        auto it = remove_if(products.begin(), products.end(), [&](const Product &p){ return p.id==id; });
        bool changed = it != products.end();
        if (changed) {
            products.erase(it, products.end());
            save();
        }
        return changed;
    }
    bool updateProduct(const Product &p) {
        for (auto &q: products) if (q.id==p.id) {
            q = p;
            save();
            return true;
        }
        return false;
    }
    vector<Product> allProducts() const { return products; }
};

// --------------------------- Discount & Loyalty ------------------------------
struct DiscountManager {
    string currentSeason; // "NONE", "EID_FITR", "EID_ADHA", "PUJA"
    double percent; // applied globally for season
    DiscountManager():currentSeason("NONE"),percent(0){}
    double applySeasonal(double price) const {
        return price * (1.0 - percent/100.0);
    }
    void setSeason(const string &season, double p) {
        currentSeason = season;
        percent = p;
    }
};

struct LoyaltySystem {
    int pointsPerTaka = 1; // 1 point per unit (you can adjust)
    int pointsToTaka = 10; // redeem 10 points => 1 unit discount
    int calculatePoints(double total) {
        return static_cast<int>(total) * pointsPerTaka;
    }
    double redeem(int &pointsAvailable) {
        int redeemable = (pointsAvailable / pointsToTaka) * (1); // 1 unit per 10 points
        pointsAvailable -= (redeemable * pointsToTaka);
        return redeemable;
    }
};

// --------------------------- SalesReport ------------------------------------
struct SalesReport {
    vector<Invoice> invoices;
    void load() { invoices = FileManager::loadSales(); }
    void addInvoice(const Invoice &inv) { invoices.push_back(inv); FileManager::appendSale(inv); }
    void dailyReport(const string &date) {
        double total = 0;
        unordered_map<int,int> countByProduct;
        for (auto &inv: invoices) {
            if (inv.datetime.substr(0,10) == date) {
                total += inv.totalAmount;
                for (auto &it: inv.items) countByProduct[it.productID] += it.qty;
            }
        }
        cout << "Daily Report for " << date << "\n";
        cout << "Total Revenue: " << total << "\n";
        // top sold product
        if (!countByProduct.empty()) {
            vector<pair<int,int>> v(countByProduct.begin(), countByProduct.end());
            sort(v.begin(), v.end(), [](auto &a, auto &b){ return a.second>b.second; });
            cout << "Top Selling Product ID: " << v[0].first << " (qty: " << v[0].second << ")\n";
        } else cout << "No sales today.\n";
    }
    void monthlyReport(const string &ym) {
        double total = 0;
        unordered_map<int,int> countByProduct;
        for (auto &inv: invoices) {
            if (date_month_key(inv.datetime) == ym) {
                total += inv.totalAmount;
                for (auto &it: inv.items) countByProduct[it.productID] += it.qty;
            }
        }
        cout << "Monthly Report for " << ym << "\n";
        cout << "Total Revenue: " << total << "\n";
        if (!countByProduct.empty()) {
            vector<pair<int,int>> v(countByProduct.begin(), countByProduct.end());
            sort(v.begin(), v.end(), [](auto &a, auto &b){ return a.second>b.second; });
            cout << "Top Selling Product ID: " << v[0].first << " (qty: " << v[0].second << ")\n";
        } else cout << "No sales this month.\n";
    }
};

// --------------------------- Auth System ------------------------------------
struct AuthSystem {
    vector<User> users;
    void load() { users = FileManager::loadUsers(); }
    void save() { FileManager::saveUsers(users); }
    User* findUser(const string &uname) {
        for (auto &u: users) if (u.username==uname) return &u;
        return nullptr;
    }
    bool registerUser(const string &uname, const string &pass, const string &role) {
        if (findUser(uname)) return false;
        User u; u.username = uname; u.password = pass; u.role = role; u.loyaltyPoints = 0;
        users.push_back(u);
        save();
        return true;
    }
    User* login(const string &uname, const string &pass) {
        for (auto &u: users) if (u.username==uname && u.password==pass) return &u;
        return nullptr;
    }
};

// --------------------------- Main System ------------------------------------
struct SuperShopSystem {
    Inventory inventory;
    AuthSystem auth;
    SalesReport sales;
    DiscountManager discount;
    LoyaltySystem loyalty;
    vector<Supplier> suppliers;
    int nextInvoiceID;

    SuperShopSystem():nextInvoiceID(1) {}

    void init() {
        inventory.load();
        auth.load();
        sales.load();
        suppliers = FileManager::loadSuppliers();
        // ensure at least one admin exists
        if (!auth.findUser("admin")) {
            auth.registerUser("admin", "admin123", "admin");
            cout << "Default admin created: username=admin password=admin123\n";
        }
        // invoice id
        for (auto &inv : FileManager::loadSales()) if (inv.invoiceID >= nextInvoiceID) nextInvoiceID = inv.invoiceID + 1;
    }

    void adminMenu(User *adminUser) {
        if (!adminUser || adminUser->role!="admin") return;
        while (true) {
            cout << "\n--- Admin Menu ---\n1. Add Product\n2. Update Product\n3. Delete Product\n4. View Products\n5. Low Stock Alert\n6. Manage Suppliers\n7. Sales Report (Daily/Monthly)\n8. Set Seasonal Discount\n9. Backup Data\n0. Logout\nChoose: ";
            int ch; if (!(cin>>ch)) { cin.clear(); string dump; getline(cin, dump); continue; }
            if (ch==0) break;
            if (ch==1) addProductFlow();
            else if (ch==2) updateProductFlow();
            else if (ch==3) deleteProductFlow();
            else if (ch==4) viewProducts();
            else if (ch==5) lowStockAlert();
            else if (ch==6) manageSuppliers();
            else if (ch==7) salesReportFlow();
            else if (ch==8) setSeasonFlow();
            else if (ch==9) { FileManager::backupAll(); cout<<"Backup completed.\n"; }
            else cout<<"Invalid choice\n";
        }
    }

    void customerMenu(User *custUser) {
        if (!custUser || custUser->role!="customer") return;
        vector<CartItem> cart;
        while (true) {
            cout << "\n--- Customer Menu ("<<custUser->username<<") ---\n1. Browse Products\n2. Search Product\n3. Filter by Category\n4. View Cart\n5. Checkout\n6. View Purchase History\n0. Logout\nChoose: ";
            int ch; if (!(cin>>ch)) { cin.clear(); string s; getline(cin,s); continue; }
            if (ch==0) break;
            if (ch==1) browseProducts(cart);
            else if (ch==2) searchProductFlow(cart);
            else if (ch==3) filterCategoryFlow(cart);
            else if (ch==4) viewCart(cart);
            else if (ch==5) checkoutFlow(custUser, cart);
            else if (ch==6) viewPurchaseHistory(custUser);
            else cout<<"Invalid\n";
        }
    }

    // ---------------- admin actions ----------------
    void addProductFlow() {
        Product p;
        string tmp;
        cout << "Product name: "; getline(cin,tmp); if(tmp=="") getline(cin,tmp);
        p.name = tmp;
        cout << "Category: "; getline(cin,tmp); p.category = tmp;
        cout << "Price: "; cin >> p.price;
        cout << "Stock: "; cin >> p.stock;
        cout << "Discount% (0 if none): "; cin >> p.discount;
        cin.ignore();
        cout << "Supplier name: "; getline(cin, tmp); p.supplier = tmp;
        Product added = inventory.addProduct(p);
        cout << "Added product ID: " << added.id << "\n";
    }
    void updateProductFlow() {
        cout << "Enter product ID to update: ";
        int id; cin >> id;
        Product *p = inventory.findByID(id);
        if (!p) { cout<<"Not found\n"; return; }
        cout << "Updating product: " << p->name << "\n";
        string tmp; cin.ignore();
        cout << "New name (blank skip): "; getline(cin,tmp); if (!tmp.empty()) p->name = tmp;
        cout << "New category (blank skip): "; getline(cin,tmp); if (!tmp.empty()) p->category = tmp;
        cout << "New price (0 skip): "; double pr; cin>>pr; if (pr>0) p->price = pr;
        cout << "New stock (-1 skip): "; int st; cin>>st; if (st>=0) p->stock = st;
        cout << "New discount% (-1 skip): "; double d; cin>>d; if (d>=0) p->discount = d;
        cin.ignore();
        cout << "New supplier (blank skip): "; getline(cin,tmp); if (!tmp.empty()) p->supplier = tmp;
        inventory.updateProduct(*p);
        cout << "Updated.\n";
    }
    void deleteProductFlow() {
        cout << "Enter product ID to delete: ";
        int id; cin >> id;
        if (inventory.removeProduct(id)) cout << "Deleted.\n"; else cout << "Not found.\n";
    }
    void viewProducts() {
        cout << "\n--- Products ---\n";
        for (auto &p: inventory.allProducts()) {
            cout << "ID: " << p.id << " | " << p.name << " | Cat: " << p.category << " | Price: " << p.price
                 << " | Stock: " << p.stock << " | Disc%: " << p.discount << " | Supplier: " << p.supplier << "\n";
        }
    }
    void lowStockAlert() {
        cout << "Low stock products (threshold 15):\n";
        for (auto &p: inventory.allProducts()) {
            if (p.stock <= 15) cout << "ID:" << p.id << " " << p.name << " stock:" << p.stock << "\n";
        }
    }
    void manageSuppliers() {
        while (true) {
            cout << "\n1. View Suppliers\n2. Add Supplier\n3. Back\nChoose: ";
            int ch; cin >> ch;
            cin.ignore();
            if (ch==1) {
                for (auto &s: suppliers) {
                    cout << "Name: " << s.name << " | Contact: " << s.contact << " | SuppliedIDs: ";
                    for (auto id: s.suppliedIDs) cout << id << " ";
                    cout << "\n";
                }
            } else if (ch==2) {
                Supplier s;
                cout << "Supplier name: "; getline(cin,s.name);
                cout << "Contact: "; getline(cin,s.contact);
                suppliers.push_back(s);
                FileManager::saveSuppliers(suppliers);
                cout << "Added.\n";
            } else break;
        }
    }
    void salesReportFlow() {
        while (true) {
            cout << "1. Daily Report\n2. Monthly Report\n0. Back\nChoose: ";
            int ch; cin >> ch;
            cin.ignore();
            if (ch==0) break;
            if (ch==1) {
                cout << "Enter date (YYYY-MM-DD) or 0 for today: ";
                string d; getline(cin,d);
                if (d=="0"||d.empty()) d = now_date();
                sales.dailyReport(d);
            } else if (ch==2) {
                cout << "Enter month (YYYY-MM) or 0 for current month: ";
                string m; getline(cin,m);
                if (m=="0"||m.empty()) m = date_month_key(now_datetime());
                sales.monthlyReport(m);
            }
        }
    }
    void setSeasonFlow() {
        cout << "Set seasonal discount: 0 NONE, 1 Eid ul Fitr, 2 Eid ul Adha, 3 Puja\nChoice: ";
        int c; cin >> c;
        double p=0; string name="NONE";
        if (c==1) { name="Eid ul Fitr"; cout<<"Percent: "; cin>>p; }
        else if (c==2) { name="Eid ul Adha"; cout<<"Percent: "; cin>>p; }
        else if (c==3) { name="Puja"; cout<<"Percent: "; cin>>p; }
        else { p=0; name="NONE"; }
        discount.setSeason(name, p);
        cout<<"Season set: "<<name<<" "<<p<<"%\n";
    }

    // ---------------- customer actions ----------------
    void browseProducts(vector<CartItem> &cart) {
        viewProducts();
        cout << "To add to cart, enter product ID (0 to skip): ";
        int id; cin >> id;
        if (id==0) return;
        Product *p = inventory.findByID(id);
        if (!p) { cout<<"Not found\n"; return; }
        cout << "Qty: "; int q; cin >> q;
        if (q <= 0 || q > p->stock) { cout << "Invalid qty or insufficient stock\n"; return; }
        CartItem it; it.productID = p->id; it.productName = p->name; it.unitPrice = p->price; it.qty = q;
        it.discountPercent = p->discount + discount.percent; // combine product discount & seasonal
        cart.push_back(it);
        cout<<"Added to cart.\n";
    }
    void searchProductFlow(vector<CartItem> &cart) {
        cout << "Search keyword: ";
        string kw; cin.ignore(); getline(cin, kw);
        auto res = inventory.searchByName(kw);
        for (auto p: res) {
            cout << "ID: " << p->id << " | " << p->name << " | Price: " << p->price << " | Stock: " << p->stock << "\n";
        }
        if (!res.empty()) {
            cout << "Enter ID to add to cart (0 skip): ";
            int id; cin >> id;
            if (id==0) return;
            Product *prd = inventory.findByID(id);
            if (!prd) { cout<<"Not found\n"; return; }
            cout << "Qty: "; int q; cin >> q;
            if (q <= 0 || q > prd->stock) { cout << "Invalid qty or insufficient stock\n"; return; }
            CartItem it; it.productID = prd->id; it.productName = prd->name; it.unitPrice = prd->price; it.qty = q;
            it.discountPercent = prd->discount + discount.percent;
            cart.push_back(it);
            cout<<"Added to cart.\n";
        }
    }
    void filterCategoryFlow(vector<CartItem> &cart) {
        cout << "Category: "; string cat; cin.ignore(); getline(cin, cat);
        auto res = inventory.filterByCategory(cat);
        for (auto p: res) {
            cout << "ID: " << p->id << " | " << p->name << " | Price: " << p->price << " | Stock: " << p->stock << "\n";
        }
        if (!res.empty()) {
            cout << "Enter ID to add to cart (0 skip): ";
            int id; cin >> id;
            if (id==0) return;
            Product *prd = inventory.findByID(id);
            if (!prd) { cout<<"Not found\n"; return; }
            cout << "Qty: "; int q; cin >> q;
            if (q <= 0 || q > prd->stock) { cout << "Invalid qty or insufficient stock\n"; return; }
            CartItem it; it.productID = prd->id; it.productName = prd->name; it.unitPrice = prd->price; it.qty = q;
            it.discountPercent = prd->discount + discount.percent;
            cart.push_back(it);
            cout<<"Added to cart.\n";
        }
    }
    void viewCart(const vector<CartItem> &cart) {
        if (cart.empty()) { cout<<"Cart empty\n"; return; }
        double sum = 0;
        cout << "\n-- Cart --\n";
        for (size_t i=0;i<cart.size();++i) {
            auto &it = cart[i];
            cout << i+1 << ". " << it.productName << " | unit: " << it.unitPrice << " | qty: " << it.qty
                 << " | disc%: " << it.discountPercent << " | total: " << it.total() << "\n";
            sum += it.total();
        }
        cout << "Cart Total: " << sum << "\n";
    }
    void checkoutFlow(User *custUser, vector<CartItem> &cart) {
        if (cart.empty()) { cout<<"Cart empty\n"; return; }
        // Validate stock
        for (auto &it: cart) {
            Product *p = inventory.findByID(it.productID);
            if (!p || p->stock < it.qty) {
                cout << "Insufficient stock for product ID: " << it.productID << "\n";
                return;
            }
        }
        double total = 0;
        for (auto &it: cart) total += it.total();
        cout << "Cart Total: " << total << "\n";
        cout << "Use loyalty points? (y/n): ";
        char ch; cin >> ch;
        double redeemAmount = 0;
        if (ch=='y' || ch=='Y') {
            int &pts = custUser->loyaltyPoints;
            double redeemed = loyalty.redeem(pts);
            if (redeemed>0) {
                cout << "Redeemed: " << redeemed << " (points left: " << pts << ")\n";
                redeemAmount = redeemed;
            } else cout << "No sufficient points to redeem.\n";
            auth.save(); // update user points
        }
        total -= redeemAmount;
        if (total < 0) total = 0;
        cout << "Final total after redeem: " << total << "\n";
        cout << "Confirm checkout? (y/n): ";
        cin >> ch;
        if (ch!='y' && ch!='Y') { cout << "Cancelled.\n"; return; }

        // Deduct stock
        for (auto &it: cart) {
            Product *p = inventory.findByID(it.productID);
            p->stock -= it.qty;
            // update supplier history
            for (auto &s: suppliers) {
                if (s.name == p->supplier) {
                    s.suppliedIDs.push_back(p->id);
                    break;
                }
            }
        }
        inventory.save();
        FileManager::saveSuppliers(suppliers);

        // Create invoice
        Invoice inv;
        inv.invoiceID = nextInvoiceID++;
        inv.username = custUser->username;
        inv.items = cart;
        inv.totalAmount = total;
        inv.datetime = now_datetime();
        sales.addInvoice(inv);

        // Increase loyalty points
        int gained = loyalty.calculatePoints(total);
        custUser->loyaltyPoints += gained;
        auth.save();

        // Save invoice as text (slip)
        string fname = "invoice_" + to_string(inv.invoiceID) + ".txt";
        ofstream ofs(fname);
        ofs << "Invoice ID: " << inv.invoiceID << "\n";
        ofs << "Customer: " << inv.username << "\n";
        ofs << "Date: " << inv.datetime << "\n";
        ofs << "------------------------------------\n";
        for (auto &it: inv.items) {
            ofs << it.productName << " | unit: " << it.unitPrice << " | qty: " << it.qty << " | disc%: " << it.discountPercent
                << " | total: " << it.total() << "\n";
        }
        ofs << "------------------------------------\n";
        ofs << "Total: " << inv.totalAmount << "\n";
        ofs << "Loyalty points gained: " << gained << "\n";
        ofs.close();

        cout << "Checkout complete. Invoice saved: " << fname << "\n";
        cart.clear();
    }
    void viewPurchaseHistory(User *custUser) {
        cout << "Purchase history for: " << custUser->username << "\n";
        auto invs = FileManager::loadSales();
        for (auto &inv: invs) {
            if (inv.username == custUser->username) {
                cout << "Invoice: " << inv.invoiceID << " | Total: " << inv.totalAmount << " | Date: " << inv.datetime << "\n";
            }
        }
    }

    // ---------------- top-level run ----------------
    void run() {
        init();
        while (true) {
            cout << "\n=== Super Shop System ===\n1. Login\n2. Register (customer)\n3. Exit\nChoose: ";
            int ch; if (!(cin >> ch)) { cin.clear(); string s; getline(cin,s); continue; }
            if (ch==3) break;
            if (ch==2) {
                string uname, pass;
                cout << "Choose username: "; cin >> uname;
                cout << "Choose password: "; cin >> pass;
                if (auth.registerUser(uname, pass, "customer")) cout<<"Registered. You can login now.\n";
                else cout<<"Username taken.\n";
            } else if (ch==1) {
                string uname, pass;
                cout << "Username: "; cin >> uname;
                cout << "Password: "; cin >> pass;
                User *u = auth.login(uname, pass);
                if (!u) { cout<<"Invalid credentials\n"; continue; }
                if (u->role == "admin") adminMenu(u);
                else customerMenu(u);
            }
        }
        cout << "Goodbye.\n";
    }
};

// --------------------------- main -------------------------------------------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    SuperShopSystem sys;
    sys.run();
    return 0;
}
