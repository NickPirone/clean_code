#include <iostream>
#include <unordered_map>
#include <future>
#include <fstream>
#include <chrono>

using namespace std;
using namespace chrono;


void insertNoReserve(int max){
    unordered_map<int,int> m;
    for(int i = 0; i < max; i++){
        m[i] = i;
    }
}

void insertReserveAll(int max){
    unordered_map<int,int> m;
    m.reserve(max + 1);
    for(int i = 0; i < max; i++){
        m[i] = i;
    }
}

void insertReserveAmount(int max, int amount_reserved){
    unordered_map<int,int> m;
    m.reserve(amount_reserved);
    for(int i = 0; i < max; i++){
        m[i] = i;
    }
}

int nanosFor(packaged_task<void()>& func){
    high_resolution_clock::time_point start = high_resolution_clock::now();
    func();
    high_resolution_clock::time_point end = high_resolution_clock::now();
    return duration_cast<nanoseconds>(end-start).count();
}

void runAllTests(ofstream& out_strm, int max_elements) {
    //variables needed later:
    int quarter_sz  = (max_elements / 4) + 1;
    int half_sz     = (max_elements / 2) + 1;
    //running through tasks to make sure we have them cached:
    packaged_task<void()> pt_1(std::bind(insertNoReserve, max_elements));
    packaged_task<void()> pt_2(std::bind(insertReserveAll, max_elements));
    packaged_task<void()> pt_3(std::bind(insertReserveAmount, max_elements, 50));
    int cache_test_result = nanosFor(pt_1);
    cache_test_result = nanosFor(pt_2);
    cache_test_result = nanosFor(pt_3);
    //Output file layout:
    out_strm << "NoReserveTime,ReserveAllTime,ReserveQuarterTime,ReserverHalfTime" << endl;
    //100 iterations:
    for(int i = 0; i < 100; i++){
        packaged_task<void()> no_res_pt(std::bind(insertNoReserve, max_elements));
        packaged_task<void()> res_all_pt(std::bind(insertReserveAll, max_elements));
        packaged_task<void()> res_qt_pt(std::bind(insertReserveAmount, max_elements, quarter_sz));
        packaged_task<void()> res_half_pt(std::bind(insertReserveAmount, max_elements, half_sz));
        out_strm << nanosFor(no_res_pt) << ",";
        out_strm << nanosFor(res_all_pt) << ",";
        out_strm << nanosFor(res_qt_pt) << ",";
        out_strm << nanosFor(res_half_pt) << endl;
    }
}

int main(){
    ofstream test_100_file("Output/unordered_map_reserve/100_results.csv");
    runAllTests(test_100_file,100);
    test_100_file.close();
    ofstream test_1000_file("Output/unordered_map_reserve/1000_results.csv");
    runAllTests(test_1000_file,1000);
    test_1000_file.close();
    ofstream test_10000_file("Output/unordered_map_reserve/10000_results.csv");
    runAllTests(test_10000_file,10000);
    test_10000_file.close();
}
