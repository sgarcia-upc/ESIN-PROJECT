
#include <iostream>
#include <string>

using namespace std;

float sqroot(const int m) {
    float i=0;
    float x1,x2;
    while( (i*i) <= m )
            i+=0.1;
    x1=i;
    for(int j=0;j<10;j++)
    {
        x2=m;
        x2/=x1;
        x2+=x1;
        x2/=2;
        x1=x2;
    }

    return x2;
}

bool is_prime(const int num) {
    
    if ( num < 2 )
        return false;
    
    if (num == 2) return true;

    float sqrt = sqroot(num); 
    cout << sqrt << endl;
    
    if (sqrt != (int) sqrt){
        sqrt = (int) (sqrt+1);
    }
    cout << sqrt << endl;
    for (int i=2; i <= sqrt; i++){
        cout << num << " % "  << i << " = "<< num % i << endl;
        if (num % i == 0){
            return false;
        }
    }
    return true;
}

int main (int argc, char* argv[]){
    int num;
    while (cin>>num){
        cout << is_prime(num) << endl;   
    }    
}
