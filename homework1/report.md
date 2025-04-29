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
/** Merge Sort **Recursive** */

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
Quick_sort
Insert_sort



## 測試與驗證

### 測試案例



### 編譯與執行指令



### 結論



## 申論及開發報告

### 選擇遞迴的原因

