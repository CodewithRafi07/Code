#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// ===== Abstract Base Class: Equipment =====
class Equipment {
protected:
    string equipmentID;
    int flightTime;
    int maxSpeed;
    int cameraCount;

public:
    virtual void operate() = 0;   // pure virtual
    virtual void showInfo() = 0;  // pure virtual

    // getters & setters
    void setEquipmentID(string id) { equipmentID = id; }
    string getEquipmentID() { return equipmentID; }

    void setFlightTime(int t) { flightTime = t; }
    int getFlightTime() { return flightTime; }

    void setMaxSpeed(int s) { maxSpeed = s; }
    int getMaxSpeed() { return maxSpeed; }

    void setCameraCount(int c) { cameraCount = c; }
    int getCameraCount() { return cameraCount; }
};

// ===== Insurance Class (for Multiple Inheritance) =====
class Insurance {
protected:
    bool insured;
    int monthsRemaining;

public:
    Insurance(bool ins = false, int months = 0) {
        insured = ins;
        monthsRemaining = months;
    }

    void setInsurance(bool ins, int months) {
        insured = ins;
        monthsRemaining = months;
    }

    void showInsurance() {
        cout << "Insured: " << (insured ? "Yes" : "No")
             << ", Months remaining: " << monthsRemaining << "\n";
    }
};

// ===== Derived Class: Drone =====
class Drone : public Equipment, public Insurance {
public:
    Drone(string id, int time, int speed, int cameras,
          bool ins = false, int months = 0)
        : Insurance(ins, months) {
        equipmentID = id;
        flightTime = time;
        maxSpeed = speed;
        cameraCount = cameras;
    }

    // Runtime polymorphism
    void operate() override {
        cout << "Drone " << equipmentID << " is flying at speed "
             << maxSpeed << " km/h with " << cameraCount << " cameras.\n";
    }

    void showInfo() override {
        cout << "Drone ID: " << equipmentID
             << ", Flight Time: " << flightTime
             << " mins, Max Speed: " << maxSpeed
             << " km/h, Cameras: " << cameraCount << "\n";
        showInsurance();
    }

    // Compile-time polymorphism (function overloading)
    void loadMission(string missionName) {
        cout << "Loading mission: " << missionName << "\n";
    }

    void loadMission(vector<string> waypoints) {
        cout << "Loading mission with waypoints: ";
        for (auto &wp : waypoints) cout << wp << " ";
        cout << "\n";
    }

    // Operator Overloading (+)
    Drone operator+(Drone &other) {
        string newID = equipmentID + "&" + other.equipmentID;
        int newFlightTime = flightTime + other.flightTime;
        int newMaxSpeed = max(maxSpeed, other.maxSpeed);
        int newCameras = cameraCount + other.cameraCount;

        // merged drone is NOT insured
        return Drone(newID, newFlightTime, newMaxSpeed, newCameras, false, 0);
    }
};

// ===== Main Function =====
int main() {
    Drone d1("D301", 60, 120, 2, true, 12);
    Drone d2("D302", 45, 100, 1, false, 0);

    cout << "=== Drone Information ===\n";
    d1.showInfo();
    d2.showInfo();

    cout << "\n=== Drone Operations ===\n";
    d1.operate();
    d1.loadMission("Survey Area A");
    d2.loadMission({"WP1", "WP2", "WP3"});

    cout << "\n=== Merged Drone (Operator +) ===\n";
    Drone d3 = d1 + d2;
    d3.showInfo();

    return 0;
}
