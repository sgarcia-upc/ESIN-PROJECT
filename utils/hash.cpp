#include <iostream>
#include <string>

using namespace std;


int main (int argc, char* argv[]){
    
    string k = argv[1];
    int _M = atoi(argv[2]);

    if (_M < 5) _M = 7;

    if (_M > 0){
        int n = 0;
        for (int i=0; i < k.length(); ++i) {
            n = n + k[i]*i; // a n sumen el codi ascii
        }
        cout << n % _M << endl;
    } else {
        cout << 0 << endl;
    }

}
