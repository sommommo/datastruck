#include "header.h"
#define NOMINMAX


// ========== 自訂 to_string ==========
template<typename T>
std::string to_string_custom(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

// ========== 讀取資料 ==========
std::vector<int> readDataFromFile(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin) {
        std::cerr << "開啟檔案失敗！\n";
        return {};
    }
    std::vector<int> data;
    int num;
    while (fin >> num) data.push_back(num);
    return data;
}

// ========== 取得記憶體使用量 ==========
size_t getMemoryUsageKB() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    return memInfo.WorkingSetSize / 1024;
}

// ========== 1. STL Merge Sort (stable_sort) ==========
void benchmarkSTLMerge(const std::vector<int>& arr) {
    const int repeat = 5;
    long long total_time = 0;
    size_t mem_before = 0, mem_after = 0;
    for (int t = 0; t < repeat; ++t) {
        std::vector<int> temp = arr;
        mem_before = getMemoryUsageKB();
        auto start = std::chrono::high_resolution_clock::now();
        std::stable_sort(temp.begin(), temp.end());
        auto end = std::chrono::high_resolution_clock::now();
        mem_after = getMemoryUsageKB();
        total_time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }
    std::cout << "====== STL Merge Sort (stable_sort) ======\n";
    std::cout << "平均排序時間：" << total_time / repeat << " 微秒\n";
    std::cout << "排序前記憶體使用量：" << mem_before << " KB\n";
    std::cout << "排序後記憶體使用量：" << mem_after << " KB\n";
    std::cout << "記憶體使用變化量：" << (mem_after > mem_before ? mem_after - mem_before : 0) << " KB\n\n";
}

// ========== 2. Iterative Merge Sort ==========
template<class T>
void Merge(T* initList, T* mergedList, int l, int m, int n) {
    int i1 = l, i2 = m + 1, iRes = l;
    while (i1 <= m && i2 <= n) {
        if (initList[i1] <= initList[i2]) mergedList[iRes++] = initList[i1++];
        else mergedList[iRes++] = initList[i2++];
    }
    std::copy(initList + i1, initList + m + 1, mergedList + iRes);
    iRes += (m - i1 + 1);
    std::copy(initList + i2, initList + n + 1, mergedList + iRes);
}

template<class T>
void MergePass(T* initList, T* resultList, int n, int s) {
    int i;
    for (i = 0; i <= n - 2 * s; i += 2 * s) {
        Merge(initList, resultList, i, i + s - 1, i + 2 * s - 1);
    }
    if (i + s - 1 < n) {
        Merge(initList, resultList, i, i + s - 1, n);
    }
    else {
        std::copy(initList + i, initList + n + 1, resultList + i);
    }
}

template<class T>
void MergeSortIterative(T* a, int n) {
    T* temp = new T[n + 1];
    for (int sz = 1; sz < n; sz *= 2) {
        MergePass(a, temp, n - 1, sz);
        MergePass(temp, a, n - 1, sz);
    }
    delete[] temp;
}

void benchmarkMergeSortIterative(const std::vector<int>& arr) {
    const int repeat = 5; long long total_time = 0; size_t mem_before = 0, mem_after = 0;
    int n = arr.size();
    for (int t = 0; t < repeat; ++t) {
        int* temp = new int[n];
        for (int i = 0; i < n; ++i) temp[i] = arr[i];
        mem_before = getMemoryUsageKB();
        auto start = std::chrono::high_resolution_clock::now();
        MergeSortIterative(temp, n);
        auto end = std::chrono::high_resolution_clock::now();
        mem_after = getMemoryUsageKB();
        total_time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        delete[] temp;
    }
    std::cout << "====== Iterative Merge Sort ======\n";
    std::cout << "平均排序時間：" << total_time / repeat << " 微秒\n";
    std::cout << "排序前記憶體使用量：" << mem_before << " KB\n";
    std::cout << "排序後記憶體使用量：" << mem_after << " KB\n";
    std::cout << "記憶體使用變化量：" << (mem_after > mem_before ? mem_after - mem_before : 0) << " KB\n\n";
}

// ========== 3. Natural Merge Sort ==========
template<class T>
void naturalMergeSort(T* a, int n) {
    if (n <= 1) return;
    T* buffer = new T[n];
    std::vector<std::pair<int, int>> runs;
    // 檢測自然遞增區段
    int start = 0;
    while (start < n) {
        int end = start + 1;
        while (end < n && a[end - 1] <= a[end]) ++end;
        runs.emplace_back(start, end);
        start = end;
    }
    while (runs.size() > 1) {
        std::vector<std::pair<int, int>> newRuns;
        for (size_t i = 0; i + 1 < runs.size(); i += 2) {
            int l = runs[i].first;
            int m = runs[i].second;
            int r = runs[i + 1].second;
            int idx = l, i1 = l, i2 = runs[i + 1].first;
            while (i1 < m && i2 < r) buffer[idx++] = (a[i1] <= a[i2] ? a[i1++] : a[i2++]);
            while (i1 < m) buffer[idx++] = a[i1++];
            while (i2 < r) buffer[idx++] = a[i2++];
            for (int k = l; k < r; ++k) a[k] = buffer[k];
            newRuns.emplace_back(l, r);
        }
        if (runs.size() % 2 == 1) newRuns.push_back(runs.back());
        runs.swap(newRuns);
    }
    delete[] buffer;
}

void benchmarkNaturalMergeSort(const std::vector<int>& arr) {
    const int repeat = 5;
    long long total_time = 0;
    size_t mem_before = 0, mem_after = 0;
    int n = arr.size();
    for (int t = 0; t < repeat; ++t) {
        int* temp = new int[n];
        for (int i = 0; i < n; ++i) temp[i] = arr[i];
        mem_before = getMemoryUsageKB();
        auto start = std::chrono::high_resolution_clock::now();
        naturalMergeSort(temp, n);
        auto end = std::chrono::high_resolution_clock::now();
        mem_after = getMemoryUsageKB();
        total_time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        delete[] temp;
    }
    std::cout << "====== Natural Merge Sort ======\n";
    std::cout << "平均排序時間：" << total_time / repeat << " 微秒\n";
    std::cout << "排序前記憶體使用量：" << mem_before << " KB\n";
    std::cout << "排序後記憶體使用量：" << mem_after << " KB\n";
    std::cout << "記憶體使用變化量：" << (mem_after > mem_before ? mem_after - mem_before : 0) << " KB\n\n";
}

// ========== 主程式 ==========
int main() {
    int n;
    std::cout << "請輸入元素個數（會讀取 testcaseN.txt）：";
    std::cin >> n;
    std::string filename = "testcase" + to_string_custom(n) + ".txt";
    auto data = readDataFromFile(filename);
    if (data.empty()) { std::cerr << "資料讀取失敗，請確認檔案是否存在。\n"; return 1; }
    benchmarkSTLMerge(data);
    benchmarkMergeSortIterative(data);
    benchmarkNaturalMergeSort(data);
    return 0;
}
