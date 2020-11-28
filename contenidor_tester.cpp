#include "contenidor.cpp"
#include <iostream>

using namespace std; 

void comparatoria( contenidor &c1, contenidor &c2)
{
    cout << (c1 == c2) <<endl;
    cout << (c1 != c2) <<endl;
    cout << (c1 < c2) <<endl;
    cout << (c1 <= c2) <<endl;
    cout << (c1 > c2) <<endl;
    cout << (c1 >= c2) <<endl;
    cout<<"-----"<<endl;
}

int main()
{
    contenidor c1 = contenidor("1234BC", 2);
    contenidor c2 = contenidor("1234BC", 2);
    
    // si c1 == c2
    comparatoria( c1, c2 );
    
    //si c1 != c2 y si c1 < c2 (matricula)
    //matricula diferente, long igual
    c1 = contenidor("a1234BC", 2);
    c2 = contenidor("b1234BC", 2);
    comparatoria( c1, c2 );
    
    //si c1 != c2 y si c1 < c2 (longitud)
    c1 = contenidor("a1234BC", 1);
    c2 = contenidor("a1234BC", 2);
    comparatoria( c1, c2 );
    
    //si c2 != c1 y si c2 < c1 (matricula)
    //matricula diferente, long igual
    c1 = contenidor("b1234BC", 2);
    c2 = contenidor("a1234BC", 2);
    comparatoria( c1, c2 );
    
    //si c2 != c1 y si c2 < c1 (longitud)
    c1 = contenidor("a1234BC", 2);
    c2 = contenidor("a1234BC", 1);
    comparatoria( c1, c2 );
}
