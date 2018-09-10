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
    v.reserve((max / 2)+5);//included the +5 to make sure we only double size of vector once.
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
    output_file << "Iteration,NoReserve,Reserve,HalfReserve," << endl;
    //run through each once just to cache opreations
    nanosForNoReserve(50);
    nanosForReserve(50);
    nanosForHalfReserve(50);
    for(int i = -1; i < iterations; i++){
        output_file << (i + 1) << ',';
        output_file << nanosForNoReserve(max_elements) << ',';
        output_file << nanosForReserve(max_elements) << ',';
        output_file << nanosForHalfReserve(max_elements) << ',';
        output_file << endl;
    }
    output_file.close();
}


int main(){
    runTests(100, 100);
    runTests(1000, 100);
    runTests(10000, 100);
}