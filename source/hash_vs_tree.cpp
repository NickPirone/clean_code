#include <iostream>
#include <unordered_map>
#include <map>
#include <future>
#include <fstream>
#include <vector>
#include <string>

using namespace std;
using namespace chrono;

/*
    The idea here is to test the performance of the hash-table (std::unordered_map)
    versus the red-black tree (std::map).  In theory, the hash-table should outperform
    the tree in times for insertion, deletion, access, etc (based on time complexity).
    
    But let's test it out--maybe for small N tree will be faster?

*/

//A templated function to test insertions up to [count], returns nanoseconds for completion.
template<typename Map_T>
void TestInsertion(Map_T& containr, int count){
    for(int i = 0; i < count; i++){
        containr[count] = count;
    }
}

template<typename Map_T>
void TestAccess(Map_T& containr, int count){
    for(int i = 0; i < count; i++){
        int a = containr[i];
    }
}

template<typename Map_T>
void TestConstructor(){
    Map_T my_map;
}

//Bind the arguments and package up the tasks; then pass them to this function.
int nanosFor(packaged_task<void()>& func){
    high_resolution_clock::time_point start = high_resolution_clock::now();
    func();
    high_resolution_clock::time_point end = high_resolution_clock::now();
    return duration_cast<nanoseconds>(end-start).count();
}

template<typename Map_T>
void runAllTests(ofstream& out_file){
    //we want to perform 100 iterations of each test:
        //  10 elements, 20 elements, 40 elements, 80 elements, 160 elements.
        
    //Step 1:  write the output header:
    out_file << "Constructor";
    vector<int> counts = {10,20,40,80,160,320,640};
    for(int cnt : counts){
        out_file << ",Insert" << cnt << ",Access" << cnt;
    }
    out_file << endl;
    
    //Step 2:  run through all functions to make sure they are fresh in memory/cache.
    Map_T cachemap;
    packaged_task<void()> const_cache(TestConstructor<Map_T>);
    packaged_task<void()> insert_cache(std::bind(TestInsertion<Map_T>,std::ref(cachemap), 160));
    packaged_task<void()> access_cache(std::bind(TestAccess<Map_T>,std::ref(cachemap), 160));
    int nans = nanosFor(const_cache);
    nans = nanosFor(insert_cache);
    nans = nanosFor(access_cache);
    
    for(int i = 0; i < 100; i++){
        packaged_task<void()> construct_pt(TestConstructor<Map_T>);
        out_file << nanosFor(construct_pt);
        for(int cnt : counts){
            Map_T my_map;
            packaged_task<void()> insert_pt(std::bind(TestInsertion<Map_T>,std::ref(my_map), cnt));
            packaged_task<void()> access_pt(std::bind(TestAccess<Map_T>,std::ref(my_map), cnt));
            out_file << "," << nanosFor(insert_pt);
            out_file << "," <<nanosFor(access_pt);
        }
        out_file << endl;
    }
}


int main(){
    ofstream hash_file("Output/hash_vs_tree/hash_results.csv");
    runAllTests<unordered_map<int,int> >(hash_file);
    hash_file.close();
    ofstream tree_file("Output/hash_vs_tree/tree_results.csv");
    runAllTests<map<int,int> >(tree_file);
    tree_file.close();
}