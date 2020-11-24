#include "ubicacio.cpp"
#include <iostream>

using namespace std; 


int main()
{
    //row ==      place ==      floor ==
    ubicacio a = ubicacio(1,2,3);
    ubicacio b = ubicacio(1,2,3);
    //~ a.mostra();
    //~ cout<<endl;
    //~ b.mostra();
    //~ cout<<endl;
    cout << (a == b) << endl;
    cout << (a != b) << endl;
    cout << (a >= b) << endl;
    cout << (a <= b) << endl;
    cout << (a < b) << endl;
    cout << (a > b) << endl;
    cout<<"-----"<<endl;
    //en tots els altres casos es comprova el not equal
    
    //row < 
    a = ubicacio(0,2,3);
    b = ubicacio(2,2,3);
    cout << (a == b) << endl;
    cout << (a != b) << endl;
    cout << (a >= b) << endl;
    cout << (a <= b) << endl;
    cout << (a < b) << endl;
    cout << (a > b) << endl;
    cout<<"-----"<<endl;
    
    //row ==      place <
    a = ubicacio(0,1,3);
    b = ubicacio(0,2,3);
    cout << (a == b) << endl;
    cout << (a != b) << endl;
    cout << (a >= b) << endl;
    cout << (a <= b) << endl;
    cout << (a < b) << endl;
    cout << (a > b) << endl;
    cout<<"-----"<<endl;
    
    //row >        
    a = ubicacio(7,1,2);
    b = ubicacio(0,1,3);
    cout << (a == b) << endl;
    cout << (a != b) << endl;
    cout << (a >= b) << endl;
    cout << (a <= b) << endl;
    cout << (a < b) << endl;
    cout << (a > b) << endl;
    cout<<"-----"<<endl;
    
    //row ==     place ==    floor <
    a = ubicacio(0,1,2);
    b = ubicacio(0,1,3);
    cout << (a == b) << endl;
    cout << (a != b) << endl;
    cout << (a >= b) << endl;
    cout << (a <= b) << endl;
    cout << (a < b) << endl;
    cout << (a > b) << endl;
    cout<<"-----"<<endl;
    
    //row ==     place >     
    a = ubicacio(0,1,3);
    b = ubicacio(0,2,3);
    cout << (a == b) << endl;
    cout << (a != b) << endl;
    cout << (a >= b) << endl;
    cout << (a <= b) << endl;
    cout << (a < b) << endl;
    cout << (a > b) << endl;
    cout<<"-----"<<endl;
    
    //row ==     place ==     floor >
    a = ubicacio(0,1,5);
    b = ubicacio(0,1,3);
    cout << (a == b) << endl;
    cout << (a != b) << endl;
    cout << (a >= b) << endl;
    cout << (a <= b) << endl;
    cout << (a < b) << endl;
    cout << (a > b) << endl;
    cout<<"-----"<<endl;
    
}
