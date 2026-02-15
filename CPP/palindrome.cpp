#include <iostream>
#include <string>
#include<algorithm>
using namespace std;

int main() {
    string s;
    cin >> s;   // Read input string
    
    string rev = s;
    reverse(rev.begin(), rev.end());  // Reverse the string
    //reverse(startIterator, endIterator); 
    
    if (s == rev)
        cout << "YesYes" << endl;  // Palindrome
    else
        cout << "NoNo" << endl;    // Not palindrome
    
    return 0;
}
