/*
I-ROS: Intelligent Retail Operations & System
Single-file C++ console application
Features implemented:
 - Inventory (Product CRUD), Supplier tracking
 - User authentication (Admin & Customer)
 - Role-based menus
 - Cart management, checkout, invoice generation
 - Seasonal/global discount + per-product discount
 - Loyalty points (earn and redeem)
 - File persistence for users, products, suppliers, invoices

Files used (created in program directory):
 - users.csv        (username,password,role,name,points)
 - products.csv     (id,name,category,price,stock,product_discount_percent,supplier_id)
 - suppliers.csv    (id,name,contact,address,product_ids_semi-colon-separated)
 - invoices/        (Invoice_YYYYMMDD_HHMMSS_<counter>.txt)
 - transactions.csv (append minimal transaction log)

Compile: g++ -std=c++17 -O2 I-ROS.cpp -o iros
Run: ./iros

Notes: This is a single-file, console-driven demo. It focuses on clarity and
persistence behavior rather than efficiency. Extend as needed.
*/

#include <bits/stdc++.h>
using namespace std;

// -------------------- Utilities --------------------
string trim(const string &s){auto a=s; while(!a.empty() && isspace(a.back())) a.pop_back(); size_t i=0; while(i<a.size() && isspace(a[i])) i++; return a.substr(i);}
vector<string> split(const string &s, char delim){ vector<string> out; string cur; stringstream ss(s); while(getline(ss, cur, delim)) out.push_back(cur); return out; }
string now_timestamp(){ auto t = chrono::system_clock::now(); time_t tt = chrono::system_clock::to_time_t(t); tm local_tm; #ifdef _WIN32 localtime_s(&local_tm, &tt); #else localtime_r(&tt, &local_tm); #endif char buf[64]; strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &local_tm); return string(buf); }
string now_filename_time(){ auto t = chrono::system_clock::now(); time_t tt = chrono::system_clock::to_time_t(t); tm local_tm; #ifdef _WIN32 localtime_s(&local_tm, &tt); #else localtime_r(&tt, &local_tm); #endif char buf[64]; strftime(buf, sizeof(buf), "%Y%m%d_%H%M%S", &local_tm); return string(buf); }

double stod_safe(const string &s){ try{ return stod(s); } catch(...){ return 0.0; } }
int stoi_safe(const string &s){ try{ return stoi(s); } catch(...){ return 0; } }

// -------------------- Models --------------------
struct Product {
    string id;
    string name;
    string category;
    double price = 0.0;
    int stock = 0;
    double product_discount = 0.0; // percent (0-100)
    string supplier_id; // optional

    string serialize() const {
        // CSV: id,name,category,price,stock,product_discount,supplier_id
        stringstream ss; ss<<id<<","<<name<<","<<category<<","<<fixed<<setprecision(2)<<price<<","<<stock<<","<<product_discount<<","<<supplier_id; return ss.str();
    }
    static Product deserialize(const string &line){ auto f = split(line, ','); Product p; if(f.size()>=7){ p.id=trim(f[0]); p.name=trim(f[1]); p.category=trim(f[2]); p.price=stod_safe(trim(f[3])); p.stock=stoi_safe(trim(f[4])); p.product_discount=stod_safe(trim(f[5])); p.supplier_id=trim(f[6]); } return p; }
};

struct Supplier {
    string id;
    string name;
    string contact;
    string address;
    vector<string> product_ids; // history

    string serialize() const {
        stringstream ss; ss<<id<<","<<name<<","<<contact<<","<<address<<",";
        for(size_t i=0;i<product_ids.size();++i){ if(i) ss<<";"; ss<<product_ids[i]; }
        return ss.str();
    }
    static Supplier deserialize(const string &line){ auto f=split(line, ','); Supplier s; if(f.size()>=5){ s.id=trim(f[0]); s.name=trim(f[1]); s.contact=trim(f[2]); s.address=trim(f[3]); string pids = trim(f[4]); if(!pids.empty()) s.product_ids = split(pids, ';'); } return s; }
};

struct User {
    string username;
    string password;
    string role; // ADMIN or CUSTOMER
    string name;
    int points = 0;

    string serialize() const{
        stringstream ss; ss<<username<<","<<password<<","<<role<<","<<name<<","<<points; return ss.str();
    }
    static User deserialize(const string &line){ auto f=split(line, ','); User u; if(f.size()>=5){ u.username=trim(f[0]); u.password=trim(f[1]); u.role=trim(f[2]); u.name=trim(f[3]); u.points=stoi_safe(trim(f[4])); } return u; }
};

struct CartItem { string product_id; int qty; };

struct InvoiceItem { string product_name; int qty; double unit_price; double discount_amount; double line_total; };

// -------------------- Managers --------------------
class InventoryManager {
public:
    map<string, Product> products; // id -> Product
    map<string, Supplier> suppliers; // id -> Supplier
    double seasonal_discount = 0.0; // percent
    string active_season_name = "";

    InventoryManager(){ }

    // File I/O
    void load_products(const string &fname){ products.clear(); ifstream ifs(fname); if(!ifs) return; string line; while(getline(ifs, line)){ line=trim(line); if(line.empty()) continue; Product p = Product::deserialize(line); if(!p.id.empty()) products[p.id]=p; } }
    void save_products(const string &fname){ ofstream ofs(fname); for(auto &kv: products) ofs<<kv.second.serialize()<<"\n"; }

    void load_suppliers(const string &fname){ suppliers.clear(); ifstream ifs(fname); if(!ifs) return; string line; while(getline(ifs,line)){ line=trim(line); if(line.empty()) continue; Supplier s=Supplier::deserialize(line); if(!s.id.empty()) suppliers[s.id]=s; } }
    void save_suppliers(const string &fname){ ofstream ofs(fname); for(auto &kv: suppliers) ofs<<kv.second.serialize()<<"\n"; }

    // CRUD
    string generate_product_id(){ int mx=0; for(auto &kv: products){ try{ int v=stoi(kv.first); if(v>mx) mx=v; } catch(...){} } return to_string(mx+1); }
    string generate_supplier_id(){ int mx=0; for(auto &kv: suppliers){ try{ int v=stoi(kv.first); if(v>mx) mx=v; } catch(...){} } return to_string(mx+1); }

    void add_product(const Product &p){ products[p.id]=p; if(!p.supplier_id.empty() && suppliers.count(p.supplier_id)){
        auto &s = suppliers[p.supplier_id]; if(find(s.product_ids.begin(), s.product_ids.end(), p.id)==s.product_ids.end()) s.product_ids.push_back(p.id);
    } }
    bool update_product(const Product &p){ if(products.count(p.id)){ products[p.id]=p; return true; } return false; }
    bool delete_product(const string &id){ return products.erase(id)>0; }

    void add_supplier(const Supplier &s){ suppliers[s.id]=s; }
    bool update_supplier(const Supplier &s){ if(suppliers.count(s.id)){ suppliers[s.id]=s; return true;} return false; }
    bool delete_supplier(const string &id){ return suppliers.erase(id)>0; }

    // Stock control
    bool deduct_stock(const string &product_id, int qty){ if(products.count(product_id) && products[product_id].stock>=qty){ products[product_id].stock -= qty; return true; } return false; }
    vector<Product> low_stock_alerts(int threshold=15){ vector<Product> out; for(auto &kv: products){ if(kv.second.stock < threshold) out.push_back(kv.second); } return out; }

    // Display helpers
    void print_product_short(const Product &p){ cout<<left<<setw(6)<<p.id<<" | "<<setw(20)<<p.name<<" | "<<setw(10)<<p.category<<" | Price: "<<setw(8)<<fixed<<setprecision(2)<<p.price<<" | Stock: "<<setw(4)<<p.stock<<" | PDisc:"<<p.product_discount<<"%\n"; }
    void list_products(bool show_low_alert=true){ cout<<"\nProducts:\n"; cout<<"ID     | Name                 | Category   | Price     | Stock | PDisc\n"; cout<<string(80,'-')<<"\n"; for(auto &kv: products) print_product_short(kv.second); if(show_low_alert){ auto v=low_stock_alerts(); if(!v.empty()){ cout<<"\n*** Low stock alerts (stock < 15):\n"; for(auto &p: v) cout<<" - "<<p.name<<" (ID: "<<p.id<<") stock="<<p.stock<<"\n"; } }
    }

    Product* find_product(const string &id){ if(products.count(id)) return &products[id]; return nullptr; }

    // Discounts
    double compute_final_unit_price(const Product &p){ double unit = p.price; double total_percent = p.product_discount + seasonal_discount; if(total_percent < 0) total_percent=0; if(total_percent>100) total_percent=100; double discount_amount = unit * total_percent / 100.0; return unit - discount_amount; }
};

class AuthSystem {
public:
    map<string, User> users; // username -> User
    string users_file = "users.csv";

    void load_users(){ users.clear(); ifstream ifs(users_file); if(!ifs) return; string line; while(getline(ifs,line)){ line=trim(line); if(line.empty()) continue; User u=User::deserialize(line); if(!u.username.empty()) users[u.username]=u; } }
    void save_users(){ ofstream ofs(users_file); for(auto &kv: users) ofs<<kv.second.serialize()<<"\n"; }

    bool username_exists(const string &u){ return users.count(u)>0; }
    bool register_user(const User &u){ if(username_exists(u.username)) return false; users[u.username]=u; return true; }
    User* authenticate(const string &username, const string &password){ if(users.count(username) && users[username].password==password) return &users[username]; return nullptr; }
};

class LoyaltyManager {
public:
    // conversion: 10 points = 1 Taka
    int points_per_taka = 10; // to redeem 10 points = 1 taka
    // earn rate: 1 point per 1 Taka spent
    void earn_points(User &u, double amount_spent){ int points = (int)floor(amount_spent + 1e-9); u.points += points; }
    double redeem_points(User &u, int points_to_redeem){ if(points_to_redeem <=0) return 0.0; if(points_to_redeem > u.points) points_to_redeem = u.points; // limit
        int taka = points_to_redeem / points_per_taka; int used_points = taka * points_per_taka; u.points -= used_points; return (double)taka; }
};

class TransactionManager {
public:
    InventoryManager *inv;
    LoyaltyManager *lm;
    string invoices_dir = "invoices";
    int invoice_counter = 0;

    TransactionManager(InventoryManager *im, LoyaltyManager *l): inv(im), lm(l){ // ensure invoices dir exists
        #ifdef _WIN32
            _mkdir(invoices_dir.c_str());
        #else
            mkdir(invoices_dir.c_str(), 0755);
        #endif
    }

    string generate_invoice_id(){ ++invoice_counter; string ts = now_filename_time(); stringstream ss; ss<<"INV_"<<ts<<"_"<<setw(3)<<setfill('0')<<invoice_counter; return ss.str(); }

    // Checkout flow: takes cart items, user reference, optional points redemption
    pair<string,double> checkout(User &user, const vector<CartItem> &cart, int points_to_redeem){
        // Validate stock and compute totals
        vector<InvoiceItem> items;
        double subtotal = 0.0; double total_discount = 0.0;
        for(const auto &ci: cart){ Product *p = inv->find_product(ci.product_id); if(!p) continue; if(ci.qty<=0) continue; if(p->stock < ci.qty){ cout<<"Not enough stock for product "<<p->name<<" (available="<<p->stock<<"). Skipping.\n"; continue; }
            double unit_price = p->price;
            double product_discount_amount = unit_price * p->product_discount/100.0;
            double seasonal_discount_amount = unit_price * inv->seasonal_discount/100.0;
            double total_discount_amount_per_unit = product_discount_amount + seasonal_discount_amount;
            double final_unit_price = unit_price - total_discount_amount_per_unit;
            double line_total = final_unit_price * ci.qty;
            subtotal += unit_price * ci.qty;
            total_discount += total_discount_amount_per_unit * ci.qty;
            items.push_back({p->name, ci.qty, final_unit_price, total_discount_amount_per_unit, line_total});
        }
        double payable = 0.0; for(auto &it: items) payable += it.line_total;
        // Redeem points
        double redeemed_amount = 0.0;
        if(points_to_redeem > 0){ redeemed_amount = lm->redeem_points(user, points_to_redeem); payable = max(0.0, payable - redeemed_amount); }
        // Simulate payment success
        // Deduct stock
        for(const auto &ci: cart){ Product *p = inv->find_product(ci.product_id); if(p && ci.qty>0){ p->stock -= ci.qty; } }
        // Earn points on final payable (1 point per 1 Taka)
        lm->earn_points(user, payable);
        // Generate invoice
        string invoice_id = generate_invoice_id(); string filename = invoices_dir + "/Invoice_" + invoice_id + ".txt";
        ofstream ofs(filename);
        ofs<<"Invoice ID: "<<invoice_id<<"\n";
        ofs<<"Date: "<<now_timestamp()<<"\n";
        ofs<<"Customer: "<<user.name<<" ("<<user.username<<")\n\n";
        ofs<<left<<setw(30)<<"Product"<<setw(8)<<"Qty"<<setw(12)<<"Unit Price"<<setw(14)<<"Discount/Unit"<<setw(12)<<"Line Total"<<"\n";
        ofs<<string(80,'-')<<"\n";
        for(auto &it: items){ ofs<<left<<setw(30)<<it.product_name<<setw(8)<<it.qty<<setw(12)<<fixed<<setprecision(2)<<it.unit_price<<setw(14)<<it.discount_amount<<setw(12)<<it.line_total<<"\n"; }
        ofs<<"\nSubtotal (base prices): "<<fixed<<setprecision(2)<<subtotal<<"\n";
        ofs<<"Total discounts applied: "<<fixed<<setprecision(2)<<total_discount<<"\n";
        ofs<<"Redeemed amount: "<<fixed<<setprecision(2)<<redeemed_amount<<"\n";
        ofs<<"Total payable: "<<fixed<<setprecision(2)<<payable<<"\n";
        ofs<<"Points balance after transaction: "<<user.points<<"\n";
        ofs.close();
        // Append transaction log
        ofstream tfs("transactions.csv", ios::app);
        tfs<<invoice_id<<","<<user.username<<","<<fixed<<setprecision(2)<<payable<<","<<now_timestamp()<<"\n";
        tfs.close();
        return {filename, payable};
    }
};

// -------------------- IROS Controller & UI --------------------
class IROS {
public:
    InventoryManager inv;
    AuthSystem auth;
    LoyaltyManager lm;
    unique_ptr<TransactionManager> tm;
    string products_file = "products.csv";
    string suppliers_file = "suppliers.csv";

    IROS(){ auth.users_file = "users.csv"; inv.load_products(products_file); inv.load_suppliers(suppliers_file); auth.load_users(); tm = make_unique<TransactionManager>(&inv, &lm); }
    ~IROS(){ save_all(); }

    void save_all(){ inv.save_products(products_file); inv.save_suppliers(suppliers_file); auth.save_users(); }

    // Startup helper: ensure at least one admin
    void ensure_default_admin(){ if(auth.users.empty()){ User u; u.username="admin"; u.password="admin123"; u.role="ADMIN"; u.name="Administrator"; u.points=0; auth.register_user(u); cout<<"Default admin created: username=admin password=admin123\n"; auth.save_users(); } }

    void run(){ ensure_default_admin(); while(true){ cout<<"\n=== I-ROS Main Menu ===\n1) Login\n2) Register (Customer)\n3) Exit\nChoose: "; int c; if(!(cin>>c)){ cin.clear(); string bad; getline(cin,bad); continue; } if(c==1) login_flow(); else if(c==2) register_customer(); else if(c==3){ cout<<"Saving data and exiting...\n"; save_all(); break; } else cout<<"Invalid option\n"; }
    }

    void login_flow(){ string uname, pwd; cout<<"Username: "; cin>>uname; cout<<"Password: "; cin>>pwd; User *u = auth.authenticate(uname, pwd); if(!u){ cout<<"Login failed.\n"; return; } if(u->role=="ADMIN") admin_menu(*u); else customer_menu(*u); }

    void register_customer(){ User u; cout<<"Choose username: "; cin>>u.username; if(auth.username_exists(u.username)){ cout<<"Username exists.\n"; return; } cout<<"Choose password: "; cin>>u.password; cout<<"Your name: "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); getline(cin, u.name); u.role="CUSTOMER"; u.points=0; auth.register_user(u); auth.save_users(); cout<<"Customer registered. You can login now.\n"; }

    // Admin UI
    void admin_menu(User &admin){ while(true){ cout<<"\n--- Admin Menu ("<<admin.name<<") ---\n";
            cout<<"1) Inventory Management\n2) Supplier Management\n3) Financial / Transactions\n4) Seasonal Discount Manager\n5) User Management\n6) Logout\nChoose: "; int c; if(!(cin>>c)){ cin.clear(); string bad; getline(cin,bad); continue; }
            if(c==1) admin_inventory(); else if(c==2) admin_suppliers(); else if(c==3) admin_transactions(); else if(c==4) admin_seasonal(); else if(c==5) admin_user_management(); else if(c==6) break; else cout<<"Invalid\n";
        }
    }

    void admin_inventory(){ while(true){ cout<<"\n--- Inventory ---\n1) List products\n2) Add product\n3) Update product\n4) Delete product\n5) Set product discount\n6) Back\nChoose: "; int c; if(!(cin>>c)){ cin.clear(); string bad; getline(cin,bad); continue; }
            if(c==1) inv.list_products();
            else if(c==2){ Product p; p.id = inv.generate_product_id(); cout<<"Name: "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); getline(cin,p.name); cout<<"Category: "; getline(cin,p.category); cout<<"Price: "; cin>>p.price; cout<<"Stock: "; cin>>p.stock; cout<<"Product discount percent (0-100): "; cin>>p.product_discount; cout<<"Supplier ID (or 0): "; cin>>p.supplier_id; if(p.supplier_id=="0") p.supplier_id=""; inv.add_product(p); inv.save_products(products_file); cout<<"Added product with ID "<<p.id<<"\n"; }
            else if(c==3){ string id; cout<<"Product ID to update: "; cin>>id; if(!inv.products.count(id)){ cout<<"Not found\n"; } else { Product p = inv.products[id]; cout<<"Leave blank to keep current.\n"; cout<<"Name ("<<p.name<<"): "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); string tmp; getline(cin,tmp); if(!tmp.empty()) p.name=tmp; cout<<"Category ("<<p.category<<"): "; getline(cin,tmp); if(!tmp.empty()) p.category=tmp; cout<<"Price ("<<p.price<<"): "; getline(cin,tmp); if(!tmp.empty()) p.price=stod_safe(tmp); cout<<"Stock ("<<p.stock<<"): "; getline(cin,tmp); if(!tmp.empty()) p.stock=stoi_safe(tmp); cout<<"Product discount percent ("<<p.product_discount<<"): "; getline(cin,tmp); if(!tmp.empty()) p.product_discount=stod_safe(tmp); cout<<"Supplier ID ("<<p.supplier_id<<"): "; getline(cin,tmp); if(!tmp.empty()) p.supplier_id=tmp; inv.update_product(p); inv.save_products(products_file); cout<<"Updated.\n"; } }
            else if(c==4){ string id; cout<<"Product ID to delete: "; cin>>id; if(inv.delete_product(id)) { inv.save_products(products_file); cout<<"Deleted.\n"; } else cout<<"Not found.\n"; }
            else if(c==5){ string id; cout<<"Product ID: "; cin>>id; if(!inv.products.count(id)) cout<<"Not found\n"; else{ double d; cout<<"Set product discount percent (0-100): "; cin>>d; inv.products[id].product_discount = d; inv.save_products(products_file); cout<<"Set.\n"; } }
            else if(c==6) break; else cout<<"Invalid\n";
        }
    }

    void admin_suppliers(){ while(true){ cout<<"\n--- Suppliers ---\n1) List suppliers\n2) Add supplier\n3) Update supplier\n4) Delete supplier\n5) Back\nChoose: "; int c; if(!(cin>>c)){ cin.clear(); string bad; getline(cin,bad); continue; }
            if(c==1){ cout<<"\nSuppliers:\n"; for(auto &kv: inv.suppliers){ auto &s=kv.second; cout<<"ID: "<<s.id<<" | "<<s.name<<" | "<<s.contact<<" | "<<s.address<<" | Products: "; for(size_t i=0;i<s.product_ids.size();++i){ if(i) cout<<","; cout<<s.product_ids[i]; } cout<<"\n"; } }
            else if(c==2){ Supplier s; s.id=inv.generate_supplier_id(); cout<<"Name: "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); getline(cin, s.name); cout<<"Contact: "; getline(cin, s.contact); cout<<"Address: "; getline(cin, s.address); inv.add_supplier(s); inv.save_suppliers(suppliers_file); cout<<"Added supplier with ID "<<s.id<<"\n"; }
            else if(c==3){ string id; cout<<"Supplier ID to update: "; cin>>id; if(!inv.suppliers.count(id)) cout<<"Not found\n"; else{ Supplier s = inv.suppliers[id]; cout<<"Name ("<<s.name<<"): "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); string tmp; getline(cin,tmp); if(!tmp.empty()) s.name=tmp; cout<<"Contact ("<<s.contact<<"): "; getline(cin,tmp); if(!tmp.empty()) s.contact=tmp; cout<<"Address ("<<s.address<<"): "; getline(cin,tmp); if(!tmp.empty()) s.address=tmp; inv.update_supplier(s); inv.save_suppliers(suppliers_file); cout<<"Updated.\n"; } }
            else if(c==4){ string id; cout<<"Supplier ID to delete: "; cin>>id; if(inv.delete_supplier(id)){ inv.save_suppliers(suppliers_file); cout<<"Deleted.\n"; } else cout<<"Not found.\n"; }
            else if(c==5) break; else cout<<"Invalid\n";
        }
    }

    void admin_transactions(){ cout<<"\n--- Transactions / Financial ---\n"; // simple summary
        ifstream ifs("transactions.csv"); if(!ifs){ cout<<"No transaction history yet.\n"; return; } string line; double total_revenue=0.0; int count=0; cout<<left<<setw(20)<<"Invoice"<<setw(20)<<"User"<<setw(20)<<"Amount"<<setw(30)<<"Date\n"; cout<<string(90,'-')<<"\n"; while(getline(ifs,line)){ if(line.empty()) continue; auto f=split(line,','); if(f.size()>=4){ cout<<left<<setw(20)<<trim(f[0])<<setw(20)<<trim(f[1])<<setw(20)<<trim(f[2])<<setw(30)<<trim(f[3])<<"\n"; total_revenue+=stod_safe(trim(f[2])); ++count; } } cout<<"\nTotal transactions: "<<count<<" | Revenue: "<<fixed<<setprecision(2)<<total_revenue<<"\n"; }

    void admin_seasonal(){ cout<<"\n--- Seasonal Discount Manager ---\nCurrent season: "<<(inv.active_season_name.empty()?"(none)":inv.active_season_name)<<" | Discount: "<<inv.seasonal_discount<<"%\n";
        cout<<"1) Activate / Set season\n2) Deactivate season\n3) Back\nChoose: "; int c; if(!(cin>>c)){ cin.clear(); string bad; getline(cin,bad); return; } if(c==1){ cout<<"Season name: "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); string name; getline(cin,name); double d; cout<<"Discount percent to apply to all products (0-100): "; cin>>d; inv.active_season_name = name; inv.seasonal_discount = d; inv.save_products(products_file); cout<<"Season set.\n"; } else if(c==2){ inv.active_season_name=""; inv.seasonal_discount=0.0; inv.save_products(products_file); cout<<"Season deactivated.\n"; } else return; }

    void admin_user_management(){ cout<<"\n--- User Management ---\nUsers: \n"; for(auto &kv: auth.users){ auto &u=kv.second; cout<<u.username<<" | "<<u.role<<" | "<<u.name<<" | Points: "<<u.points<<"\n"; } cout<<"(To change a user's points or role, edit users.csv and restart.)\n"; }

    // Customer UI
    void customer_menu(User &user){ vector<CartItem> cart; while(true){ cout<<"\n--- Customer Menu ("<<user.name<<") ---\n1) Browse products\n2) View cart\n3) Checkout\n4) Purchase history (invoices folder)\n5) Logout\nChoose: "; int c; if(!(cin>>c)){ cin.clear(); string bad; getline(cin,bad); continue; }
            if(c==1) customer_browse(cart);
            else if(c==2) view_cart(cart);
            else if(c==3){ if(cart.empty()){ cout<<"Cart empty.\n"; } else { cout<<"You have "<<user.points<<" loyalty points. (10 points = 1 Taka)\n"; cout<<"Enter points to redeem (0 to skip): "; int pts; cin>>pts; auto res = tm->checkout(user, cart, pts); cout<<"Invoice saved to: "<<res.first<<" | Paid: "<<fixed<<setprecision(2)<<res.second<<"\n"; // clear cart
                        cart.clear(); auth.save_users(); inv.save_products(products_file); }
            }
            else if(c==4){ cout<<"Open the invoices/ folder for your invoices.\n"; }
            else if(c==5) break; else cout<<"Invalid\n";
        }
    }

    void customer_browse(vector<CartItem> &cart){ while(true){ cout<<"\n--- Browse Products ---\n1) List all\n2) Search by name\n3) Sort options\n4) Add to cart\n5) Back\nChoose: "; int c; if(!(cin>>c)){ cin.clear(); string bad; getline(cin,bad); continue; }
            if(c==1){ inv.list_products(); }
            else if(c==2){ string q; cout<<"Search term: "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); getline(cin,q); cout<<"Results:\n"; for(auto &kv: inv.products){ if(kv.second.name.find(q)!=string::npos){ inv.print_product_short(kv.second); } } }
            else if(c==3){ cout<<"Sort by: 1) Price asc 2) Price desc 3) Stock asc 4) Stock desc 5) Back\nChoose: "; int s; cin>>s; vector<Product> list; for(auto &kv: inv.products) list.push_back(kv.second);
                    if(s==1) sort(list.begin(), list.end(), [](const Product&a,const Product&b){ return a.price < b.price; });
                    else if(s==2) sort(list.begin(), list.end(), [](const Product&a,const Product&b){ return a.price > b.price; });
                    else if(s==3) sort(list.begin(), list.end(), [](const Product&a,const Product&b){ return a.stock < b.stock; });
                    else if(s==4) sort(list.begin(), list.end(), [](const Product&a,const Product&b){ return a.stock > b.stock; });
                    for(auto &p: list) inv.print_product_short(p);
            }
            else if(c==4){ string id; int q; cout<<"Product ID to add: "; cin>>id; if(!inv.products.count(id)){ cout<<"Not found\n"; } else { cout<<"Qty: "; cin>>q; if(q<=0) cout<<"Invalid qty\n"; else { cart.push_back({id,q}); cout<<"Added to cart.\n"; } } }
            else if(c==5) break; else cout<<"Invalid\n";
        }
    }

    void view_cart(const vector<CartItem> &cart){ cout<<"\nCurrent cart:\n"; double est_total=0.0; for(auto &ci: cart){ auto it = inv.products.find(ci.product_id); if(it==inv.products.end()) continue; auto &p = it->second; double final_unit = inv.compute_final_unit_price(p); double line = final_unit * ci.qty; cout<<left<<setw(6)<<p.id<<" | "<<setw(20)<<p.name<<" | Qty: "<<setw(4)<<ci.qty<<" | Final unit: "<<fixed<<setprecision(2)<<final_unit<<" | Line: "<<line<<"\n"; est_total += line; } cout<<"Estimated total: "<<fixed<<setprecision(2)<<est_total<<"\n"; }
};

// -------------------- main --------------------
int main(){ ios::sync_with_stdio(false); cin.tie(nullptr);
    IROS app; cout<<"Welcome to I-ROS (Intelligent Retail Operations & System)\n"; app.run(); return 0; }
