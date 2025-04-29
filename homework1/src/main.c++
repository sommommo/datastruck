#include "header.h"
#include "HeapSort.cpp"
#include "InsetionSort.cpp"
#include "MergeSort.cpp"
#include "QuickSort.cpp"

using namespace std;

int main() {
    int n;
    std::cout << "請輸入元素個數（會讀取 testcaseN.txt）：";
    std::cin >> n;
    std::string filename = "testcase" + to_string_custom(n) + ".txt";
    auto data = readDataFromFile(filename);
    if (data.empty()) {
        std::cerr << "資料讀取失敗，請確認檔案是否存在。\n";
        return 1;
    }
    benchmarkSTLHeap(data);
    benchmarkHeapSortBasic(data);
    benchmarkHeapSortOptimized(data);

    benchmarkInsertionSort_vector(data);
    benchmarkInsertionSort_array_insert(data);
    benchmarkInsertionSort_array_binary(data);

    benchmarkSTLMerge(data);
    benchmarkMergeSortIterative(data);
    benchmarkNaturalMergeSort(data);

    benchmarkSTLSort(data);
    benchmarkQuickSortBasic(data);
    benchmarkQuickSortOptimized(data);

    return 0;

}