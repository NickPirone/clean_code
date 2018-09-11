#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>

using namespace std;
using namespace chrono;

const int size = 1000000;
int arr[size]; //shared across all threads.

void insertElements(int start_index, int end_index){
    for(int i = start_index; i < end_index; i++){
        arr[i] = i;
    }
}

int nanosForInsertion(int max_threads){
    high_resolution_clock::time_point start = high_resolution_clock::now();
    
    //variables we will need:
    thread  thrds[max_threads];//array of threads.
    int     partition_size      = size / max_threads; //size of each partition.
    int     start_point         = 0;
    
    //iterate through, starting a different thread for each partition.
    for(int i = 0; i < max_threads; i++){
        int end_point           = min(start_point + partition_size, size);
        thrds[i]                = thread(insertElements, start_point, end_point); 
        start_point            += partition_size;
    }
    for(int i = 0; i < max_threads; i++){
        thrds[i].join(); //need to join all the threads.
    }
    
    high_resolution_clock::time_point end = high_resolution_clock::now();
    return duration_cast<nanoseconds>(end-start).count();
}



int main(){
    nanosForInsertion(5); //run this onces to make sure cache/memory have hit those parts of the code.
    ofstream output_file("./Output/threading_max/results.csv");
    //header:
    for(int i = 1; i < 21; i++){
        output_file << i << "thread,";
    }
    output_file << endl;
    //now do tests:
    nanosForInsertion(1);//caching purposes.
    for(int i = 0; i < 50; i++){    //iterations
        for(int j = 1; j < 21; j++){//threadcount
            output_file << nanosForInsertion(j) << ",";
        }
        output_file << endl;
    }
    output_file.close();
}