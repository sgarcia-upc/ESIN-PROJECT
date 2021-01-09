#include <iostream>
#include <list>

using namespace std;

list<string>::iterator partition(list<string>::iterator start, list<string>::iterator end_it){
     list<string>::iterator partition_it = start;
     for(list<string>::iterator i = start; i != end_it; i++){
         if(*i <= *end_it){
             std::iter_swap(i, partition_it);
             ++partition_it;
         }
     }
     std::iter_swap(partition_it, end_it);
     return partition_it;
}

void quick_sort(list<string>::iterator start, list<string>::iterator end_it){
    int size = std::distance(start, end_it);
    if (size <= 1)
        return;

    list<string>::iterator partition_it = partition(start, end_it);
    --partition_it;

    quick_sort(start, partition_it);
    quick_sort(partition_it, end_it);
}

int main(){
    list<string> l;
    l.push_back("J");
    l.push_back("H");
    l.push_back("Z");
    l.push_back("A");
    l.push_back("B");

    list<string>::iterator end = l.end();
    --end;

    quick_sort(l.begin(), end);
    list<string>::iterator it = l.begin();
    while ( it != l.end() )
    {
            cout<<*it<<" ";
            ++it;
    }
    cout << endl;
}
