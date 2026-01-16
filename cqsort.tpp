#ifndef CQSORT_TPP
#define CQSORT_TPP

// TODO: your code here


// CONSTRUCTOR
template <typename T>
CQSort<T>::CQSort(int mthr) : max_threads(mthr), active_threads(0) {
}


// FUNCTOR OPERATOR - Entry point for sorting
template <typename T>
void CQSort<T>::operator()(T &vec) {
    if (vec.size() <= 1) {
        return; // Already sorted
    }
    
    // Start the recursive quicksort
    quicksort(vec, 0, vec.size() - 1);
}


// median-of-three pivot selection to avoid O(n^2) on sorted data
template <typename T>
int CQSort<T>::choosePivot(T &vec, int left, int right) {
    int mid = left + (right - left) / 2;
    
    // Sort left, mid, right
    if (vec[mid] < vec[left]) {
        std::swap(vec[left], vec[mid]);
    }
    if (vec[right] < vec[left]) {
        std::swap(vec[left], vec[right]);
    }
    if (vec[right] < vec[mid]) {
        std::swap(vec[mid], vec[right]);
    }
    
    //Move median to right - 1 position
    std::swap(vec[mid], vec[right - 1]);
    
    return right - 1; // Return pivot index
}


// PARTITION FUNCTION
template <typename T>
int CQSort<T>::partition(T &vec, int left, int right) {
    // Handle small subarrays 
    if (right - left < 2) {
        if (right > left && vec[right] < vec[left]) {
            std::swap(vec[left], vec[right]);
        }
        return left;
    }
    
    // Choose pivot using median-of-three
    int pivotIndex = choosePivot(vec, left, right);
    auto pivotValue = vec[pivotIndex];
    
    // Move pivot out of the way temporarily
    std::swap(vec[pivotIndex], vec[right]);
    
    //move elements smaller than pivot to the left
    int storeIndex = left;
    for (int i = left; i <= right  - 1; i++) {
        if (vec[i] < pivotValue) {
            std::swap(vec[i], vec[storeIndex]);
            storeIndex++;
        }
    }
    
    // Put pivot in its final position
    std::swap(vec[storeIndex], vec[right]);
    
    return storeIndex;
}


// RECURSIVE QUICKSORT
template <typename T>
void CQSort<T>::quicksort(T &vec, int left, int right) {
    if (left >= right) {
        return;
    }
    
    int pivotIndex = partition(vec, left, right);
    
    // Calculate partition sizes
    int leftSize = pivotIndex - left;
    int rightSize = right - pivotIndex;

    // Determine if should go parallel
    bool shouldGoParallel = (max_threads > 0) && ((leftSize > SWITCH_SIZE) || (rightSize > SWITCH_SIZE));
    
    if (shouldGoParallel && active_threads < max_threads) {
        active_threads++;
        
        // Spawn async task for left partition
        auto future_left = std::async(std::launch::async, 
            [this, &vec, left, pivotIndex]() {
                this->quicksort(vec, left, pivotIndex - 1);
            });
        
        // Process right partition in current thread
        quicksort(vec, pivotIndex + 1, right);
        
        // Wait for left partition to complete
        future_left.get();
        
        active_threads--;
    } 
    else {
        // sequential path
        quicksort(vec, left, pivotIndex - 1);
        quicksort(vec, pivotIndex + 1, right);
    }
}


#endif
