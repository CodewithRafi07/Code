#include <iostream>
using namespace std;

// Forward declaration of friend class
class VolumeCalculator;

class Box {
private:
    float length;
    float width;
    float height;

public:
    // Constructor to initialize dimensions
    Box(float l, float w, float h) {
        length = l;
        width = w;
        height = h;
    }

    // Friend function to print dimensions
    friend void printDimensions(Box b);

    // Friend class to calculate volume
    friend class VolumeCalculator;
};

// Friend function definition
void printDimensions(Box b) {
    cout << "Box Dimensions: " 
         << b.length << " X" 
         << b.width << " X " 
         << b.height << endl;
}

// Friend class definition
class VolumeCalculator {
public:
    float calculateVolume(Box b) {
        return b.length * b.width * b.height;
    }
};

int main() {
    // Creating Box objects
    Box b1(5.0, 3.0, 2.0);
    Box b2(4.5, 2.5, 3.0);
    Box b3(6.0, 4.0, 1.5);

    // Using friend function to print dimensions
    printDimensions(b1);
    printDimensions(b2);
    printDimensions(b3);

    // Using friend class to calculate volume
    VolumeCalculator vc;
    cout << "Volume of Box 1: " << vc.calculateVolume(b1) << endl;
    cout << "Volume of Box 2: " << vc.calculateVolume(b2) << endl;
    cout << "Volume of Box 3: " << vc.calculateVolume(b3) << endl;

    return 0;
}
