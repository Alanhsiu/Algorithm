// **************************************************************************
//  File       [sort_tool.cpp]
//  Author     [Yu-Hao Ho]
//  Synopsis   [The implementation of the SortTool Class]
//  Modify     [2020/9/15 Mu-Ting Wu]
// **************************************************************************

#include "sort_tool.h"
#include <iostream>

// Constructor
SortTool::SortTool() {}

// Insertsion sort method
void SortTool::InsertionSort(vector<int> &data)
{
    // Function : Insertion sort
    // TODO : Please complete insertion sort code here
    int key, i;
    int size = data.size();
    for (int j = 1; j < size; j++)
    {
        key = data[j];
        i = j - 1;
        while (data[i] > key && i >= 0)
        {
            data[i + 1] = data[i];
            i--;
        }
        data[i + 1] = key;
    }
}

// Quick sort method
void SortTool::QuickSort(vector<int> &data)
{
    QuickSortSubVector(data, 0, data.size() - 1);
}
// Sort subvector (Quick sort)
void SortTool::QuickSortSubVector(vector<int> &data, int low, int high)
{
    // Function : Quick sort subvector
    // TODO : Please complete QuickSortSubVector code here
    // Hint : recursively call itself
    //        Partition function is needed
    if (low < high)
    {
        int pivot = Partition(data, low, high);
        QuickSortSubVector(data, low, pivot - 1);
        QuickSortSubVector(data, pivot + 1, high);
    }
}

int SortTool::Partition(vector<int> &data, int low, int high)
{
    // Function : Partition the vector
    // TODO : Please complete the function
    // Hint : Textbook page 171
    int r = rand()%(high-low)+low;
    swap(data[r],data[high]);
    int pivot = data[high];
    int i = low - 1;
    for (int j = low; j < high; j++)
    {
        if (data[j] <= pivot)
        {
            i++;
            swap(data[i], data[j]);
        }
    }
    i++;
    swap(data[i], data[high]);
    return i;
}

// Merge sort method
void SortTool::MergeSort(vector<int> &data)
{
    MergeSortSubVector(data, 0, data.size() - 1);
}

// Sort subvector (Merge sort)
void SortTool::MergeSortSubVector(vector<int> &data, int low, int high)
{
    // Function : Merge sort subvector
    // TODO : Please complete MergeSortSubVector code here
    // Hint : recursively call itself
    //        Merge function is needed
    if ((high - low) >= 1)
    {
        int middle1 = (high + low) / 2;
        int middle2 = middle1 + 1;
        MergeSortSubVector(data, low, middle1);
        MergeSortSubVector(data, middle2, high);
        Merge(data, low, middle1, middle2, high);
    }
}

// Merge
void SortTool::Merge(vector<int> &data, int low, int middle1, int middle2, int high)
{
    // Function : Merge two sorted subvector
    // TODO : Please complete the function
    int leftIndex = low;                  // index into left subvector
    int rightIndex = middle2;             // index into right subvector
    int combinedIndex = 0;                // index into temporary working vector
    vector<int> combined(high - low + 1); // working vector

    while (leftIndex <= middle1 && rightIndex <= high)
        if (data[leftIndex] <= data[rightIndex])
            combined[combinedIndex++] = data[leftIndex++];
        else
            combined[combinedIndex++] = data[rightIndex++];

    if (leftIndex == middle2)
        while (rightIndex <= high)
            combined[combinedIndex++] = data[rightIndex++];
    else
        while (leftIndex <= middle1)
            combined[combinedIndex++] = data[leftIndex++];

    for (int i = low; i <= high; i++)
        data[i] = combined[i - low];
}

// Heap sort method
void SortTool::HeapSort(vector<int> &data)
{
    // Build Max-Heap
    BuildMaxHeap(data);

    for (int i = data.size() - 1; i >= 1; i--)
    {
        swap(data[0], data[i]);
        heapSize--;
        MaxHeapify(data, 0);
    }
}

// Max heapify
void SortTool::MaxHeapify(vector<int> &data, int root)
{
    // Function : Make tree with given root be a max-heap if both right and left sub-tree are max-heap
    // TODO : Please complete max-heapify code here
    int left = 2 * root + 1;
    int right = 2 * root + 2;
    int largest = root;
    if (left < heapSize && data[left] > data[root])
        largest = left;
    if (right < heapSize && data[right] > data[largest])
        largest = right;
    if (largest != root)
    {
        swap(data[root], data[largest]);
        MaxHeapify(data, largest);
    }
}

// Build max heap
void SortTool::BuildMaxHeap(vector<int> &data)
{
    heapSize = data.size(); // initialize heap size
    // Function : Make input data become a max-heap
    // TODO : Please complete BuildMaxHeap code here
    for (int i = (heapSize - 1) / 2; i >= 0; i--)
        MaxHeapify(data, i);
}
