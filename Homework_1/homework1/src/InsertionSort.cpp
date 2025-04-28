#include "header.h"

// ========== 自己寫的 to_string_custom ==========
template<typename T>
std::string to_string_custom(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

// ========== 插入函式 (傳統陣列版) ==========
template<class T>
void Insert(const T& e, T* a, int i) {
    a[0] = e;
    while (e < a[i]) {
        a[i + 1] = a[i];
        i--;
    }
    a[i + 1] = e;
}

// ========== Insertion Sort (傳統陣列版，用 Insert) ==========
template<class T>
void InsertionSort(T* a, const int n) {
    for (int j = 2; j <= n; j++) {
        T temp = a[j];
        Insert(temp, a, j - 1);
    }
}

// ========== 二分插入排序 (Binary Insertion Sort) ==========
template<typename T>
void binaryInsertionSort(T* a, int n) {
    for (int i = 1; i < n; ++i) {
        T key = a[i];
        int lo = 0, hi = i - 1;
        while (lo <= hi) {
            int mid = lo + ((hi - lo) >> 1);
            if (a[mid] > key) hi = mid - 1;
            else lo = mid + 1;
        }
        if (lo < i) {
            std::memmove(a + lo + 1, a + lo, static_cast<size_t>(i - lo) * sizeof(T));
            a[lo] = key;
        }
    }
}

// ========== STL Vector版 Insertion Sort ==========
void insertionSort(std::vector<int>& vec) {
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        auto insertion_point = std::upper_bound(vec.begin(), it, *it);
        std::rotate(insertion_point, it, it + 1);
    }
}

// ========== 讀取 TXT 測資 ==========
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
    fin.close();
    return data;
}

// ========== 取得記憶體使用量 ==========
size_t getMemoryUsageKB() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    return memInfo.WorkingSetSize / 1024;
}

// ========== 測 STL Vector版 ==========
void benchmarkInsertionSort_vector(const std::vector<int>& arr) {
    const int repeat = 1;
    long long total_time = 0;
    size_t memory_before = 0, memory_after = 0;

    for (int t = 0; t < repeat; ++t) {
        std::vector<int> temp = arr;

        memory_before = getMemoryUsageKB();

        auto start = std::chrono::high_resolution_clock::now();
        insertionSort(temp);
        auto end = std::chrono::high_resolution_clock::now();

        memory_after = getMemoryUsageKB();
        total_time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }

    std::cout << "====== STL Vector版本 Insertion Sort ======\n";
    std::cout << "平均排序時間：" << total_time / repeat << " 微秒\n";
    std::cout << "排序前記憶體使用量：" << memory_before << " KB\n";
    std::cout << "排序後記憶體使用量：" << memory_after << " KB\n";
    std::cout << "記憶體使用變化量：" << (memory_after > memory_before ? memory_after - memory_before : 0) << " KB\n\n";
}

// ========== 測 傳統 Insert+InsertionSort 陣列版 ==========
void benchmarkInsertionSort_array_insert(const std::vector<int>& arr) {
    const int repeat = 1;
    long long total_time = 0;
    size_t memory_before = 0, memory_after = 0;
    int n = arr.size();

    for (int t = 0; t < repeat; ++t) {
        int* temp = new int[n + 1];
        temp[0] = 0;
        for (int i = 0; i < n; ++i) {
            temp[i + 1] = arr[i];
        }

        memory_before = getMemoryUsageKB();

        auto start = std::chrono::high_resolution_clock::now();
        InsertionSort(temp, n);
        auto end = std::chrono::high_resolution_clock::now();

        memory_after = getMemoryUsageKB();
        total_time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        delete[] temp;
    }

    std::cout << "====== 傳統陣列版本 (Insert + InsertionSort) ======\n";
    std::cout << "平均排序時間：" << total_time / repeat << " 微秒\n";
    std::cout << "排序前記憶體使用量：" << memory_before << " KB\n";
    std::cout << "排序後記憶體使用量：" << memory_after << " KB\n";
    std::cout << "記憶體使用變化量：" << (memory_after > memory_before ? memory_after - memory_before : 0) << " KB\n\n";
}

// ========== 測 二分插入排序 (Binary Insertion Sort) ==========
void benchmarkInsertionSort_array_binary(const std::vector<int>& arr) {
    const int repeat = 1;
    long long total_time = 0;
    size_t memory_before = 0, memory_after = 0;
    int n = arr.size();

    for (int t = 0; t < repeat; ++t) {
        int* temp = new int[n];
        for (int i = 0; i < n; ++i) {
            temp[i] = arr[i];
        }

        memory_before = getMemoryUsageKB();

        auto start = std::chrono::high_resolution_clock::now();
        binaryInsertionSort(temp, n);
        auto end = std::chrono::high_resolution_clock::now();

        memory_after = getMemoryUsageKB();
        total_time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        delete[] temp;
    }

    std::cout << "====== 優化陣列版本 (Binary Insertion Sort + memmove) ======\n";
    std::cout << "平均排序時間：" << total_time / repeat << " 微秒\n";
    std::cout << "排序前記憶體使用量：" << memory_before << " KB\n";
    std::cout << "排序後記憶體使用量：" << memory_after << " KB\n";
    std::cout << "記憶體使用變化量：" << (memory_after > memory_before ? memory_after - memory_before : 0) << " KB\n\n";
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

    benchmarkInsertionSort_vector(data);
    benchmarkInsertionSort_array_insert(data);
    benchmarkInsertionSort_array_binary(data);

    return 0;
}
