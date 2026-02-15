#include <iostream>
#include <vector>
using namespace std;

class Gadget {
protected:
    string gadgetID_g;
    int batteryCapacity_h;
    double processorSpeed_i;

public:
    Gadget(string g, int h, double i) : gadgetID_g(g), batteryCapacity_h(h), processorSpeed_i(i) {}

    virtual void operate() = 0;
    virtual void showInfo() = 0;
    virtual ~Gadget() {}
};

class Warranty {
protected:
    bool warrantyStatus_j;
    int warrantyYears_k;

public:
    Warranty(bool j = false, int k = 0) : warrantyStatus_j(j), warrantyYears_k(k) {}

    void setWarranty(bool j, int k) {
        warrantyStatus_j = j;
        warrantyYears_k = k;
    }

    void displayWarrantyInfo() const {
        if (warrantyStatus_j)
            cout << "Warranty Status: Active\nWarranty Expiry: " << warrantyYears_k << " years" << endl;
        else
            cout << "Warranty Status: No Warranty" << endl;
    }
};

class Smartphone : public Gadget, public Warranty {
private:
    int numberOfCameras_j;

public:
    Smartphone(string g, int h, double i, int j, bool warrantyStatus, int warrantyYears)
        : Gadget(g, h, i), Warranty(warrantyStatus, warrantyYears), numberOfCameras_j(j) {}

    void operate() override {
        cout << "Operating Smartphone: Launching apps and managing calls..." << endl;
    }

    void showInfo() override {
        cout << "Gadget ID: " << gadgetID_g << endl;
        cout << "Gadget Type: Smartphone" << endl;
        cout << "Processor Speed: " << processorSpeed_i << " GHz" << endl;
        cout << "Battery Capacity: " << batteryCapacity_h << " mAh" << endl;
        cout << "Number of Cameras: " << numberOfCameras_j << endl;
        displayWarrantyInfo();
        cout << "-------------------------------------" << endl;
    }

    int getBatteryCapacity() const { return batteryCapacity_h; }
    void setBatteryCapacity(int h) { batteryCapacity_h = h; }

    double getProcessorSpeed() const { return processorSpeed_i; }
    void setProcessorSpeed(double i) { processorSpeed_i = i; }

    int getNumberOfCameras() const { return numberOfCameras_j; }
    void setNumberOfCameras(int j) { numberOfCameras_j = j; }

    string getGadgetID() const { return gadgetID_g; }

    // Overloaded method to install a single app

    void installApp(string appName_g) {
        cout << "Installing app: " << appName_g << endl;
    }

    // Overloaded method to install multiple apps

    void installApp(vector<string> appList_h) {
        cout << "Installing apps: ";
        for (auto app : appList_h) {
            cout << app << ", ";
        }
        cout << "\b\b \n";
    }

    // Overloaded + operator to combine two Smartphones

    Smartphone operator+(const Smartphone& other) {
        string newID = gadgetID_g + "&" + other.gadgetID_g;
        int combinedBattery = batteryCapacity_h + other.batteryCapacity_h;
        double maxProcessor = (processorSpeed_i > other.processorSpeed_i) ? processorSpeed_i : other.processorSpeed_i;
        int totalCameras = numberOfCameras_j + other.numberOfCameras_j;
        Smartphone combined(newID, combinedBattery, maxProcessor, totalCameras, false, 0);
        return combined;
    }
};

int main() {
    Smartphone phone1("G101", 4000, 2.8, 3, true, 2);
    Smartphone phone2("G102", 3500, 2.5, 2, false, 0);

    vector<Gadget*> gadgets;
    gadgets.push_back(&phone1);
    gadgets.push_back(&phone2);

    cout << "Displaying Smartphone Information:" << endl;
    for (auto g : gadgets) {
        g->showInfo();
        g->operate();
        cout << endl;
    }

    cout << "Demonstrating App Installation:" << endl;
    phone1.installApp("WhatsApp");
    vector<string> apps = {"Gmail", "Instagram", "Maps"};
    phone2.installApp(apps);
    cout << endl;

    cout << "Combining Smartphones:" << endl;
    Smartphone combinedPhone = phone1 + phone2;
    combinedPhone.showInfo();
    combinedPhone.operate();

    return 0;
}
