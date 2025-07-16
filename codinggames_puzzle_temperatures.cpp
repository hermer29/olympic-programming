#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

int main()
{
    
    int n; // the number of temperatures to analyse
    vector<int> temperatures = vector<int>(n);
    cin >> n; cin.ignore();
    for (int i = 0; i < n; i++) {
        int t; // a temperature expressed as an integer ranging from -273 to 5526
        cin >> t; cin.ignore();
        temperatures.push_back(t);
    }
    if(n == 0) {
        cout << 0 << endl;
        return 0;
    }

    int c_d = 5527;

    for(int temp : temperatures) {
        int abs_temp = abs(temp);
        if(abs_temp < abs(c_d)) {
            c_d = temp;
        } else if(abs_temp == abs(c_d)) {
            if(temp < 0 && c_d > 0) {
                continue;
            } else if(temp > 0 && c_d < 0) {
                c_d = temp;
                continue;
            }
        }
    }
    // Write an answer using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    cout << c_d << endl;
}