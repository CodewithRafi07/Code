#include <iostream>
#include <vector>
using namespace std;

vector<int> invertNumbers(const vector<int>& numbers) {
    vector<int> result;
    for (int num : numbers) {
        result.push_back(-num); // Additive inverse
    }
    return result;
}

int main() {
    vector<int> input = {1, -3, 0, 5, -7};
    vector<int> output = invertNumbers(input);

    // Print result
    for (int num : output) {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}
