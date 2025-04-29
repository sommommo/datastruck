#include "header.h"

// ========== �ۤv�g�� to_string_custom ==========
template<typename T>
std::string to_string_custom(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

// ========== ���J�禡 (�ǲΰ}�C��) ==========
template<class T>
void Insert(const T& e, T* a, int i) {
    a[0] = e;
    while (e < a[i]) {
        a[i + 1] = a[i];
        i--;
    }
    a[i + 1] = e;
}

// ========== Insertion Sort (�ǲΰ}�C���A�� Insert) ==========
template<class T>
void InsertionSort(T* a, const int n) {
    for (int j = 2; j <= n; j++) {
        T temp = a[j];
        Insert(temp, a, j - 1);
    }
}

// ========== �G�����J�Ƨ� (Binary Insertion Sort) ==========
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

// ========== STL Vector�� Insertion Sort ==========
void insertionSort(std::vector<int>& vec) {
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        auto insertion_point = std::upper_bound(vec.begin(), it, *it);
        std::rotate(insertion_point, it, it + 1);
    }
}

// ========== Ū�� TXT ���� ==========
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
    fin.close();
    return data;
}

// ========== ���o�O����ϥζq ==========
size_t getMemoryUsageKB() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    return memInfo.WorkingSetSize / 1024;
}

// ========== �� STL Vector�� ==========
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

    std::cout << "====== STL Vector���� Insertion Sort ======\n";
    std::cout << "�����ƧǮɶ��G" << total_time / repeat << " �L��\n";
    std::cout << "�Ƨǫe�O����ϥζq�G" << memory_before << " KB\n";
    std::cout << "�Ƨǫ�O����ϥζq�G" << memory_after << " KB\n";
    std::cout << "�O����ϥ��ܤƶq�G" << (memory_after > memory_before ? memory_after - memory_before : 0) << " KB\n\n";
}

// ========== �� �ǲ� Insert+InsertionSort �}�C�� ==========
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

    std::cout << "====== �ǲΰ}�C���� (Insert + InsertionSort) ======\n";
    std::cout << "�����ƧǮɶ��G" << total_time / repeat << " �L��\n";
    std::cout << "�Ƨǫe�O����ϥζq�G" << memory_before << " KB\n";
    std::cout << "�Ƨǫ�O����ϥζq�G" << memory_after << " KB\n";
    std::cout << "�O����ϥ��ܤƶq�G" << (memory_after > memory_before ? memory_after - memory_before : 0) << " KB\n\n";
}

// ========== �� �G�����J�Ƨ� (Binary Insertion Sort) ==========
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

    std::cout << "====== �u�ư}�C���� (Binary Insertion Sort + memmove) ======\n";
    std::cout << "�����ƧǮɶ��G" << total_time / repeat << " �L��\n";
    std::cout << "�Ƨǫe�O����ϥζq�G" << memory_before << " KB\n";
    std::cout << "�Ƨǫ�O����ϥζq�G" << memory_after << " KB\n";
    std::cout << "�O����ϥ��ܤƶq�G" << (memory_after > memory_before ? memory_after - memory_before : 0) << " KB\n\n";
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

    benchmarkInsertionSort_vector(data);
    benchmarkInsertionSort_array_insert(data);
    benchmarkInsertionSort_array_binary(data);

    return 0;
}
