
#include <iostream>
#include <string>

using namespace std;

int main (int argc, char* argv[]){

    string ***arr;
    int col = 4;
    int row = 5;
    int depth = 3;

    arr = new string **[depth]();
    for (int i = 0; i < depth; i++)
    {
        arr[i] = new string *[row]();
        for (int j = 0; j < row; j++)
            arr[i][j] = new string [col]();
    }


// ----------------
    for (int i = 0; i < depth; i++){
        cout << "Fila " << i << endl;
        for (int j = 0; j < row; j++){
            for (int k = 0; k < col; k++){
                cout << arr[i][j][k] << " ";
            }
            cout << endl;
        } 
        cout << endl;
    }

// ----------------



//Drownamicallrow deallocate a 3D arrarow

    //for (i = 0; i < depth; i++)
    //{
    //    for (j = 0; j < row; j++)
    //        delete[] arr[i][j];
    //    delete[] arr[i];
    //}
    //delete[] arr;
}
