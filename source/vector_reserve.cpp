#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>

using namespace std;
using namespace chrono;

void insertElements(vector<int>& v, int max){
    for(int i = 0; i < max; i++){
        v.push_back(i);
    }
}

int nanosForNoReserve(int max){
    //returns the number of nanoseconds it takes to insert [max] integers into a vector.
    high_resolution_clock::time_point start = high_resolution_clock::now();
    vector<int> v;
    insertElements(v,max);
    high_resolution_clock::time_point end = high_resolution_clock::now();
    int nanos = duration_cast<nanoseconds>(end-start).count();
    return nanos;
}

int nanosForReserve(int max){
    //returns the number of nanoseconds it takes to insert [max] integers into a vector, 
    //calling reserve first.
    high_resolution_clock::time_point start = high_resolution_clock::now();
    vector<int> v;
    v.reserve(max);
    insertElements(v,max);
    high_resolution_clock::time_point end = high_resolution_clock::now();
    int nanos = duration_cast<nanoseconds>(end-start).count();
    return nanos;
}

int nanosForHalfReserve(int max){
    //returns the number of nanoseconds it takes to insert [max] integers into a vector, 
    //calling reserve but only for half of our amount first.
    high_resolution_clock::time_point start = high_resolution_clock::now();
    vector<int> v;
    v.reserve(max / 2);
    insertElements(v,max);
    high_resolution_clock::time_point end = high_resolution_clock::now();
    int nanos = duration_cast<nanoseconds>(end-start).count();
    return nanos;
}

void runTests(int max_elements, int iterations){
    string filepth = "./Output/vector_reserve/";
    filepth += to_string(max_elements);
    filepth += "_elements.csv";
    cout << filepth << endl;
    ofstream output_file(filepth);
    output_file << "Iteration, NoReserve, Reserve, HalfReserve," << endl;
    for(int i = 0; i < iterations; i++){
        output_file << (i + 1) << ',';
        output_file << nanosForNoReserve(max_elements) << ',';
        output_file << nanosForReserve(max_elements) << ',';
        output_file << nanosForHalfReserve(max_elements) << ',';
        output_file << endl;
    }
    output_file.close();
}

void cacheOperations(){
    //run through this first to make sure that our application has already loaded into memory/cache
    //methods such as constructors, dynamic reallocation, etc.
    vector<int> v;
    insertElements(v, 50);
    high_resolution_clock::time_point s = high_resolution_clock::now(); //make sure accessing the chrono libs is also cached.
    
}

int main(){
    cacheOperations();
    runTests(100, 100);
    runTests(1000, 100);
    runTests(10000, 100);
}