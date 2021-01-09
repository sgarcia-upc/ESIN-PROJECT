//~ #include <iostream>
//~ #include <list>
//~ using namespace std;


//~ list<double>::iterator partition (vector<double> &arr, list<double>::iterator low, list<double>::iterator pivot) 
//~ {
    //~ int i = (low - 1);  // Index of smaller element
    //~ --low; 
  
    //~ for (int j = low; j <= high- 1; j++) 
    //~ { 
        //~ // If current element is smaller than or 
        //~ // equal to pivot 
        //~ if (arr[j] <= pivot) 
        //~ { 
            //~ i++;    // increment index of smaller element 
            //~ swap(arr[i], arr[j]); 
        //~ } 
    //~ } 
    //~ swap(arr[i + 1], arr[high]); 
    //~ return (i + 1); 
//~ } 
  

//~ void quickSort(list<double> &arr, int low, int high) 
//~ { 
    //~ if (low < high) 
    //~ { 
        //~ /* pi is partitioning index, arr[p] is now 
           //~ at right place */
        //~ int pi = partition(arr, low, high); 
  
        //~ // Separately sort elements before 
        //~ // partition and after partition 
        //~ quickSort(arr, low, pi - 1); 
        //~ quickSort(arr, pi + 1, high); 
    //~ } 
//~ } 

//~ void ordena_per_fusio( list<double> &v)
//~ {
    
    //~ quickSort(v,0,v.size()-1);
//~ }

//~ int main() {
    //~ int n,k;
    //~ while (cin >> n) {
        //~ list<double> l;
        //~ for (int i=0; i<n; ++i) {
            //~ cin >> k;
            //~ l.push_back(k);
        //~ }
        //~ ordena_per_fusio(l);
        //~ list<double>::iterator it = l.begin();
        //~ while ( it != l.end() )
        //~ {
            //~ cout<<*it<<" ";
            //~ ++it;
        //~ }
        //~ cout << endl;
    //~ }
//~ }

#include <vector>
#include <iostream>
#include <algorithm>
#include <list>

using namespace std;

list<int>::iterator partition(list<int>::iterator start, list<int>::iterator end_it){
   list<int>::iterator partition_idx = start;
   for(list<int>::iterator i = start; i != end_it; i++){
     if(*i <= *end_it){
       std::iter_swap(i, partition_idx);
       ++partition_idx;
     }
   }
   std::iter_swap(partition_idx, end_it);
   return partition_idx;
}

void quick_sort(list<int>::iterator start, list<int>::iterator end_it){
  int size = std::distance(start, end_it);
  if (size <= 1) return;
  list<int>::iterator partition_idx = partition(start, end_it);
  --partition_idx;
  quick_sort(start, partition_idx);
  quick_sort(partition_idx, end_it);
}

int main(){
     int n,k;
    while (cin >> n) {
        list<int> l;
        for (int i=0; i<n; ++i) {
            cin >> k;
            l.push_back(k);
        }
        list<int>::iterator end = l.end();
        --end;
        quick_sort(l.begin(), end);
        list<int>::iterator it = l.begin();
        while ( it != l.end() )
        {
            cout<<*it<<" ";
            ++it;
        }
        cout << endl;
    }
}
