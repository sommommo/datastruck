#include "header.h"

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
    while (fin >> num) {
        data.push_back(num);
    }
    return data;
}

// ========== 取得記憶體使用量 ==========
size_t getMemoryUsageKB() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    return memInfo.WorkingSetSize / 1024;
}

// ========== 1. STL sort ==========
void benchmarkSTLSort(const std::vector<int>& arr) {
    const int repeat = 5;
    long long total_time = 0;
    size_t mem_before = 0, mem_after = 0;

    for (int t = 0; t < repeat; ++t) {
        std::vector<int> temp = arr;
        mem_before = getMemoryUsageKB();
        auto start = std::chrono::high_resolution_clock::now();
        std::sort(temp.begin(), temp.end());
        auto end = std::chrono::high_resolution_clock::now();
        mem_after = getMemoryUsageKB();
        total_time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }

    std::cout << "====== STL std::sort ======\n";
    std::cout << "平均排序時間：" << total_time / repeat << " 微秒\n";
    std::cout << "排序前記憶體使用量：" << mem_before << " KB\n";
    std::cout << "排序後記憶體使用量：" << mem_after << " KB\n";
    std::cout << "記憶體使用變化量：" << (mem_after > mem_before ? mem_after - mem_before : 0) << " KB\n\n";
}

// ========== 2. 基本 Quick Sort ==========
template<class T>
void QuickSortBasic(T* a, int left, int right) {
    if (left < right) {
        T pivot = a[left];
        int i = left, j = right + 1;
        while (true) {
            do { i++; } while (i <= right && a[i] < pivot);
            do { j--; } while (a[j] > pivot);
            if (i >= j) break;
            std::swap(a[i], a[j]);
        }
        std::swap(a[left], a[j]);
        QuickSortBasic(a, left, j - 1);
        QuickSortBasic(a, j + 1, right);
    }
}

void benchmarkQuickSortBasic(const std::vector<int>& arr) {
    const int repeat = 5;
    long long total_time = 0;
    size_t mem_before = 0, mem_after = 0;
    int n = arr.size();

    for (int t = 0; t < repeat; ++t) {
        int* temp = new int[n];
        for (int i = 0; i < n; ++i) temp[i] = arr[i];

        mem_before = getMemoryUsageKB();
        auto start = std::chrono::high_resolution_clock::now();
        QuickSortBasic(temp, 0, n - 1);
        auto end = std::chrono::high_resolution_clock::now();
        mem_after = getMemoryUsageKB();
        total_time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        delete[] temp;
    }

    std::cout << "====== 基本 Quick Sort ======\n";
    std::cout << "平均排序時間：" << total_time / repeat << " 微秒\n";
    std::cout << "排序前記憶體使用量：" << mem_before << " KB\n";
    std::cout << "排序後記憶體使用量：" << mem_after << " KB\n";
    std::cout << "記憶體使用變化量：" << (mem_after > mem_before ? mem_after - mem_before : 0) << " KB\n\n";
}

// ========== 3. 最優化 Quick Sort (三值中位數) ==========
template<class T>
T median3(T* a, int l, int r) {
    int m = (l + r) / 2;
    if (a[m] < a[l]) std::swap(a[l], a[m]);
    if (a[r] < a[l]) std::swap(a[l], a[r]);
    if (a[r] < a[m]) std::swap(a[m], a[r]);
    // 現在 a[l] <= a[m] <= a[r]
    std::swap(a[m], a[r - 1]);  // pivot 放到 r-1
    return a[r - 1];
}

template<class T>
void QuickSortOptimized(T* a, int left, int right) {
    if (left + 10 <= right) {
        T pivot = median3(a, left, right);
        int i = left, j = right - 1;
        while (true) {
            while (a[++i] < pivot) {}
            while (a[--j] > pivot) {}
            if (i < j) std::swap(a[i], a[j]);
            else break;
        }
        std::swap(a[i], a[right - 1]);  // restore pivot
        QuickSortOptimized(a, left, i - 1);
        QuickSortOptimized(a, i + 1, right);
    }
    else {
        // 小序列改用插入排序
        for (int p = left + 1; p <= right; ++p) {
            T tmp = a[p];
            int q = p - 1;
            while (q >= left && a[q] > tmp) {
                a[q + 1] = a[q];
                --q;
            }
            a[q + 1] = tmp;
        }
    }
}

void benchmarkQuickSortOptimized(const std::vector<int>& arr) {
    const int repeat = 5;
    long long total_time = 0;
    size_t mem_before = 0, mem_after = 0;
    int n = arr.size();

    for (int t = 0; t < repeat; ++t) {
        int* temp = new int[n];
        for (int i = 0; i < n; ++i) temp[i] = arr[i];

        mem_before = getMemoryUsageKB();
        auto start = std::chrono::high_resolution_clock::now();
        QuickSortOptimized(temp, 0, n - 1);
        auto end = std::chrono::high_resolution_clock::now();
        mem_after = getMemoryUsageKB();
        total_time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        delete[] temp;
    }

    std::cout << "====== 最優化 Quick Sort (Median-of-Three) ======\n";
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
    if (data.empty()) {
        std::cerr << "資料讀取失敗，請確認檔案是否存在。\n";
        return 1;
    }

    benchmarkSTLSort(data);
    benchmarkQuickSortBasic(data);
    benchmarkQuickSortOptimized(data);

    return 0;
}
