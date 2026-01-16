#ifndef CQSORT_HPP
#define CQSORT_HPP
#include <algorithm>
#include <thread>
#include <future>
#include <atomic>
#include <iostream>
#include <vector>

template <typename T>
class CQSort {
public:
	CQSort(int mthr);

	void operator()(T &vec);
private:
	int max_threads;
	std::atomic<int> active_threads;

    // Threshold below which we switch to sequential since spawning threads for tiny arrays is wasteful
    static const int SWITCH_SIZE = 1000;
    
    // Main recursive quicksort function
    void quicksort(T &vec, int left, int right);
    
    // Partition function: returns pivot index
    int partition(T &vec, int left, int right);
    
    // Median-of-three pivot selection
    int choosePivot(T &vec, int left, int right);
};
	
#include "cqsort.tpp"
#endif // CQSORT_HPP
