#include "header.h"

// ========== ï¿½Û­q to_string ==========
template<typename T>
std::string to_string_custom(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

// ========== Åªï¿½ï¿½ï¿½ï¿½ï¿? ==========
std::vector<int> readDataFromFile(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin) {
        std::cerr << "ï¿½}ï¿½ï¿½ï¿½É®×¥ï¿½ï¿½Ñ¡I\n";
        return {};
    }
    std::vector<int> data;
    int num;
    while (fin >> num) {
        data.push_back(num);
    }
    return data;
}

// ========== ï¿½ï¿½ï¿½oï¿½Oï¿½ï¿½ï¿½ï¿½Ï¥Î¶q ==========
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
    std::cout << "ï¿½ï¿½ï¿½ï¿½ï¿½Æ§Ç®É¶ï¿½ï¿½G" << total_time / repeat << " ï¿½Lï¿½ï¿½\n";
    std::cout << "ï¿½Æ§Ç«eï¿½Oï¿½ï¿½ï¿½ï¿½Ï¥Î¶qï¿½G" << mem_before << " KB\n";
    std::cout << "ï¿½Æ§Ç«ï¿½Oï¿½ï¿½ï¿½ï¿½Ï¥Î¶qï¿½G" << mem_after << " KB\n";
    std::cout << "ï¿½Oï¿½ï¿½ï¿½ï¿½Ï¥ï¿½ï¿½Ü¤?¶qï¿½G" << (mem_after > mem_before ? mem_after - mem_before : 0) << " KB\n\n";
}

// ========== 2. ï¿½ï¿½ Quick Sort ==========
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

    std::cout << "====== ï¿½ï¿½ Quick Sort ======\n";
    std::cout << "ï¿½ï¿½ï¿½ï¿½ï¿½Æ§Ç®É¶ï¿½ï¿½G" << total_time / repeat << " ï¿½Lï¿½ï¿½\n";
    std::cout << "ï¿½Æ§Ç«eï¿½Oï¿½ï¿½ï¿½ï¿½Ï¥Î¶qï¿½G" << mem_before << " KB\n";
    std::cout << "ï¿½Æ§Ç«ï¿½Oï¿½ï¿½ï¿½ï¿½Ï¥Î¶qï¿½G" << mem_after << " KB\n";
    std::cout << "ï¿½Oï¿½ï¿½ï¿½ï¿½Ï¥ï¿½ï¿½Ü¤?¶qï¿½G" << (mem_after > mem_before ? mem_after - mem_before : 0) << " KB\n\n";
}

// ========== 3. ï¿½ï¿½ï¿½uï¿½ï¿½ Quick Sort (ï¿½Tï¿½È¤ï¿½ï¿½ï¿½ï¿?) ==========
template<class T>
T median3(T* a, int l, int r) {
    int m = (l + r) / 2;
    if (a[m] < a[l]) std::swap(a[l], a[m]);
    if (a[r] < a[l]) std::swap(a[l], a[r]);
    if (a[r] < a[m]) std::swap(a[m], a[r]);
    // ï¿½{ï¿½b a[l] <= a[m] <= a[r]
    std::swap(a[m], a[r - 1]);  // pivot ï¿½ï¿½ï¿? r-1
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
        // ï¿½pï¿½Ç¦Cï¿½ï¿½Î´ï¿½ï¿½Jï¿½Æ§ï¿½
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

    std::cout << "======  Quick Sort (Median-of-Three) ======\n";
    std::cout << "¥­§¡±Æ§Ç®É¶¡¡G" << total_time / repeat << " ï¿½Lï¿½ï¿½\n";
    std::cout << "±Æ§Ç«e°O¾ÐÅé¨Ï¥Î¶q" << mem_before << " KB\n";
    std::cout << "±Æ§Ç«á°O¾ÐÅé¨Ï¥Î¶q" << mem_after << " KB\n";
    std::cout << "°O¾ÐÅé¨Ï¥ÎÅÜ¤Æ¶q" << (mem_after > mem_before ? mem_after - mem_before : 0) << " KB\n\n";
}

// ========== ¥Dµ{¦¡ ==========
/*
int main() {
    int n;
    std::cout << "½Ð¿é¤J¤¸¯À­Ó¼Æ¡]·|Åª¨ú testcaseN.txt¡^¡G";
    std::cin >> n;
    std::string filename = "testcase" + to_string_custom(n) + ".txt";
    auto data = readDataFromFile(filename);
    if (data.empty()) {
        std::cerr << "¸ê®ÆÅª¨ú¥¢±Ñ¡A½Ð½T»{ÀÉ®×¬O§_¦s¦b¡C\n";
        return 1;
    }

    benchmarkSTLSort(data);
    benchmarkQuickSortBasic(data);
    benchmarkQuickSortOptimized(data);

    return 0;
}
*/