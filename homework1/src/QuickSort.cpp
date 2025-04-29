#include "header.h"

// ========== �ۭq to_string ==========
template<typename T>
std::string to_string_custom(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

// ========== Ū����� ==========
std::vector<int> readDataFromFile(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin) {
        std::cerr << "�}���ɮץ��ѡI\n";
        return {};
    }
    std::vector<int> data;
    int num;
    while (fin >> num) {
        data.push_back(num);
    }
    return data;
}

// ========== ���o�O����ϥζq ==========
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
    std::cout << "�����ƧǮɶ��G" << total_time / repeat << " �L��\n";
    std::cout << "�Ƨǫe�O����ϥζq�G" << mem_before << " KB\n";
    std::cout << "�Ƨǫ�O����ϥζq�G" << mem_after << " KB\n";
    std::cout << "�O����ϥ��ܤƶq�G" << (mem_after > mem_before ? mem_after - mem_before : 0) << " KB\n\n";
}

// ========== 2. �� Quick Sort ==========
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

    std::cout << "====== �� Quick Sort ======\n";
    std::cout << "�����ƧǮɶ��G" << total_time / repeat << " �L��\n";
    std::cout << "�Ƨǫe�O����ϥζq�G" << mem_before << " KB\n";
    std::cout << "�Ƨǫ�O����ϥζq�G" << mem_after << " KB\n";
    std::cout << "�O����ϥ��ܤƶq�G" << (mem_after > mem_before ? mem_after - mem_before : 0) << " KB\n\n";
}

// ========== 3. ���u�� Quick Sort (�T�Ȥ����) ==========
template<class T>
T median3(T* a, int l, int r) {
    int m = (l + r) / 2;
    if (a[m] < a[l]) std::swap(a[l], a[m]);
    if (a[r] < a[l]) std::swap(a[l], a[r]);
    if (a[r] < a[m]) std::swap(a[m], a[r]);
    // �{�b a[l] <= a[m] <= a[r]
    std::swap(a[m], a[r - 1]);  // pivot ��� r-1
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
        // �p�ǦC��δ��J�Ƨ�
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

    std::cout << "====== ���u�� Quick Sort (Median-of-Three) ======\n";
    std::cout << "�����ƧǮɶ��G" << total_time / repeat << " �L��\n";
    std::cout << "�Ƨǫe�O����ϥζq�G" << mem_before << " KB\n";
    std::cout << "�Ƨǫ�O����ϥζq�G" << mem_after << " KB\n";
    std::cout << "�O����ϥ��ܤƶq�G" << (mem_after > mem_before ? mem_after - mem_before : 0) << " KB\n\n";
}

// ========== �D�{�� ==========
int main() {
    int n;
    std::cout << "�п�J�����Ӽơ]�|Ū�� testcaseN.txt�^�G";
    std::cin >> n;

    std::string filename = "testcase" + to_string_custom(n) + ".txt";
    auto data = readDataFromFile(filename);
    if (data.empty()) {
        std::cerr << "���Ū�����ѡA�нT�{�ɮ׬O�_�s�b�C\n";
        return 1;
    }

    benchmarkSTLSort(data);
    benchmarkQuickSortBasic(data);
    benchmarkQuickSortOptimized(data);

    return 0;
}
