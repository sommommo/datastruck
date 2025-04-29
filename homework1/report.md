# 41243232
# 41243235



# 作業一 : Heap_sort、Merge_sort、Quick_sort、Insert_sort。


## 解題說明

本題要求實現多個排序演算法，並比較它們的效能。

### 解題策略

1. **Heap Sort**: 使用最大堆或最小堆的概念，將數列轉換為堆結構，然後進行排序。
2. **Merge Sort**: 使用分治法，將數列分成兩半，然後遞迴地排序每一半，最後合併已排序的兩半。
3. **Quick Sort**: 選擇一個基準值，將數列分成兩部分，一部分小於基準值，另一部分大於基準值，然後遞迴地排序這兩部分。
4. **Insert Sort**: 將數列分為已排序和未排序兩部分，然後將未排序的元素插入到已排序的部分中，直到所有元素都被排序。
5.呼叫testcase 獲取測資，並進行排序。
6.收集各排序法及不同演算法所需的時間複雜度，並進行比較。


## 程式實作
以下為各排序演算法的實作程式碼：

**basic Heap_sort**
```cpp
template<class T>
void Adjust(T* a, const int root, const int n)
{
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

    for (int i = n / 2; i >= 1; --i) {
        Adjust(a, i, n);
    }

    for (int i = n; i >= 2; --i) {
        std::swap(a[1], a[i]);
        Adjust(a, 1, i - 1);
    }
}
```

**Optimized Heap_sort**
```cpp
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
```


**Iterative Merge Sort**
```cpp
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
```
**Natural Merge Sort**
```cpp
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
```
**Basic Quick sort**
```cpp
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
```
**Optimization Quick sort**
```cpp
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
```
**Insert sort**
```cpp
template<class T>
void InsertionSort(T* a, const int n) {
    for (int j = 2; j <= n; j++) {
        T temp = a[j];
        Insert(temp, a, j - 1);
    }
}
```
**Binary Insert sort**
```cpp
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
```

## 效能分析

## 測試與驗證

### 測試案例



### 編譯與執行指令



### 結論



## 申論及開發報告

### 選擇遞迴的原因

