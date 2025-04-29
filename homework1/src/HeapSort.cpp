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
    while (fin >> num) data.push_back(num);
    return data;
}

// ========== ���o�O����ϥζq ==========
size_t getMemoryUsageKB() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    return memInfo.WorkingSetSize / 1024;
}

// ========== �Ƨǥ��T���ˬd ==========
template<class T>
bool isSorted(const T* a, int n) {
    for (int i = 1; i < n; ++i) {
        if (a[i] > a[i + 1]) return false;
    }
    return true;
}

// ========== 1. STL Heap Sort (make_heap + sort_heap) ==========
void benchmarkSTLHeap(const std::vector<int>& arr) {
    const int repeat = 5;
    long long total_time = 0;
    size_t mem_before = 0, mem_after = 0;
    for (int t = 0; t < repeat; ++t) {
        std::vector<int> temp = arr;
        mem_before = getMemoryUsageKB();
        auto start = std::chrono::high_resolution_clock::now();
        std::make_heap(temp.begin(), temp.end());
        std::sort_heap(temp.begin(), temp.end());
        auto end = std::chrono::high_resolution_clock::now();
        mem_after = getMemoryUsageKB();
        total_time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }
    std::cout << "====== STL Heap Sort (make_heap + sort_heap) ======\n";
    std::cout << "�����ƧǮɶ��G" << total_time / repeat << " �L��\n";
    std::cout << "�Ƨǫe�O����ϥζq�G" << mem_before << " KB\n";
    std::cout << "�Ƨǫ�O����ϥζq�G" << mem_after << " KB\n";
    std::cout << "�O����ϥ��ܤƶq�G" << (mem_after > mem_before ? mem_after - mem_before : 0) << " KB\n\n";
}

// ========== 2. �� Heap Sort ========== 
template<class T>
void Adjust(T* a, const int root, const int n) {
    T e = a[root];
    int j = 2 * root;
    while (j <= n) {
        if (j < n && a[j] < a[j + 1]) j++;
        if (e >= a[j]) break;
        a[j / 2] = a[j];
        j *= 2;
    }
    a[j / 2] = e;
}

template<class T>
void HeapSort(T* a, const int n) {
    // �ذ�
    for (int i = n / 2; i >= 1; --i) {
        Adjust(a, i, n);
    }
    // �Ƨ�
    for (int i = n; i >= 2; --i) {
        std::swap(a[1], a[i]);
        Adjust(a, 1, i - 1);
    }
}

void benchmarkHeapSortBasic(const std::vector<int>& arr) {
    const int repeat = 5;
    long long total_time = 0;
    size_t mem_before = 0, mem_after = 0;
    int n = arr.size();
    for (int t = 0; t < repeat; ++t) {
        int* temp = new int[n + 1];
        for (int i = 0; i < n; ++i) temp[i + 1] = arr[i];
        mem_before = getMemoryUsageKB();
        auto start = std::chrono::high_resolution_clock::now();
        HeapSort(temp, n);
        auto end = std::chrono::high_resolution_clock::now();
        mem_after = getMemoryUsageKB();
        if (!isSorted(temp, n)) {
            std::cerr << ">>> ���~�GBasic Heap Sort �S�Ʀn�I\n";
        }
        total_time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        delete[] temp;
    }
    std::cout << "\n";
    std::cout << "====== �� Heap Sort ======\n";
    std::cout << "�����ƧǮɶ��G" << total_time / repeat << " �L��\n";
    std::cout << "�Ƨǫe�O����ϥζq�G" << mem_before << " KB\n";
    std::cout << "�Ƨǫ�O����ϥζq�G" << mem_after << " KB\n";
    std::cout << "�O����ϥ��ܤƶq�G" << (mem_after > mem_before ? mem_after - mem_before : 0) << " KB\n\n";
}

// ========== 3. Optimized Heap Sort (Floyd's Sift-Down + Bottom-Up Build) ==========
template<class T>
void SiftDown(T* a, int start, int n) {
    int root = start;
    while (2 * root <= n) {
        int child = 2 * root;
        if (child + 1 <= n && a[child] < a[child + 1]) child++;
        if (a[root] < a[child]) {
            std::swap(a[root], a[child]);
            root = child;
        }
        else {
            return;
        }
    }
}

template<class T>
void HeapSortOptimized(T* a, int n) {
    for (int start = n / 2; start >= 1; --start) {
        SiftDown(a, start, n);
    }
    for (int end = n; end > 1; --end) {
        std::swap(a[1], a[end]);
        SiftDown(a, 1, end - 1);
    }
}

void benchmarkHeapSortOptimized(const std::vector<int>& arr) {
    const int repeat = 5;
    long long total_time = 0;
    size_t mem_before = 0, mem_after = 0;
    int n = arr.size();
    for (int t = 0; t < repeat; ++t) {
        int* temp = new int[n + 1];
        for (int i = 0; i < n; ++i) temp[i + 1] = arr[i];
        mem_before = getMemoryUsageKB();
        auto start = std::chrono::high_resolution_clock::now();
        HeapSortOptimized(temp, n);
        auto end = std::chrono::high_resolution_clock::now();
        mem_after = getMemoryUsageKB();
        if (!isSorted(temp, n)) {
            std::cerr << ">>> ���~�GOptimized Heap Sort �S�Ʀn�I\n";
        }
        total_time += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        delete[] temp;
    }
    std::cout << "====== Optimized Heap Sort (Floyd + Bottom-Up Build) ======\n";
    std::cout << "�����ƧǮɶ��G" << total_time / repeat << " �L��\n";
    std::cout << "�Ƨǫe�O����ϥζq�G" << mem_before << " KB\n";
    std::cout << "�Ƨǫ�O����ϥζq�G" << mem_after << " KB\n";
    std::cout << "�O����ϥ��ܤƶq�G" << (mem_after > mem_before ? mem_after - mem_before : 0) << " KB\n\n";
}

// ========== �D�{�� ==========
/*
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
    benchmarkSTLHeap(data);
    benchmarkHeapSortBasic(data);
    benchmarkHeapSortOptimized(data);
    return 0;
}
    */
