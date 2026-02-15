#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

string modifyFirstSortedString(vector<string> arr) {
    // Step 1: Sort the array (case-sensitive, ASCII based)
    sort(arr.begin(), arr.end());

    // Step 2: Get the first string after sorting
    string first = arr[0];

    // Step 3: Insert "***" between each character
    string result = "";
    for (size_t i = 0; i < first.length(); i++) {
        result += first[i];
        if (i != first.length() - 1) {
            result += "***";
        }
    }

    return result;
}

int main() {
    vector<string> input = {"apple", "Banana", "cherry"};
    string result = modifyFirstSortedString(input);
    cout << result << endl; // Output: B***a***n***a***n***a
    return 0;
}
