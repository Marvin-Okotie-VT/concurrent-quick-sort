#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <random>
#include <limits>
#include <tuple>
#include <iomanip>
#include "cqsort.hpp"

std::vector<int> generateRandomData(int size) {
    std::vector<int> data(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000);
    
    for (int i = 0; i < size; i++) {
        data[i] = dis(gen);
    }
    
    return data;
}

//benchmark a single run
double benchmarkSort(std::vector<int> data, int numThreads) {
    // Create sorter with specified thread count
    CQSort<std::vector<int>> sorter(numThreads);
    
    auto start = std::chrono::high_resolution_clock::now();
    sorter(data);
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double, std::milli> elapsed = end - start; // sort time in milliseconds
    return elapsed.count();
}


int main(int, char *[])
{
	// TODO: your code here

    std::vector<int> sizes = {1000, 10000, 100000};
    
    const int NUM_TRIALS = 3;
    
    unsigned int N = std::thread::hardware_concurrency();
    if (N == 0) N = 4; // Default if can't detect hardware concurrency
    
    std::cout << "CQSort Benchmark" << std::endl;
    std::cout << "=================" << std::endl;
    std::cout << "Hardware concurrency: " << N << " threads" << std::endl;
    std::cout << "Trials per configuration: " << NUM_TRIALS << std::endl;
    std::cout << std::endl;
    
    // Print headers
    std::cout << std::setw(10) << "ListSize" 
              << std::setw(15) << "SeqAvg(ms)" 
              << std::setw(15) << "ParAvg(ms)" 
              << std::setw(15) << "Speedup"
              << std::endl;
    std::cout << std::string(55, '-') << std::endl;
    
    for (int size : sizes) {
        double seqTotal = 0.0;
        double parTotal = 0.0;
        
        for (int trial = 0; trial < NUM_TRIALS; trial++) {
            std::vector<int> data = generateRandomData(size);
            std::vector<int> dataCopy = data;	// Copy for parallel sort
            
            // Sequential sort max threads = 0
            double seqTime = benchmarkSort(data, 0);
            seqTotal += seqTime;
            
            // Parallel sort max threads = N
            double parTime = benchmarkSort(dataCopy, N);
            parTotal += parTime;
        }
        
        double seqAvg = seqTotal / NUM_TRIALS;
        double parAvg = parTotal / NUM_TRIALS;
        double speedup = seqAvg / parAvg;
        
        std::cout << std::setw(10) << size
                  << std::setw(15) << std::fixed << std::setprecision(2) << seqAvg
                  << std::setw(15) << std::fixed << std::setprecision(2) << parAvg
                  << std::setw(15) << std::fixed << std::setprecision(2) << speedup << "x"
                  << std::endl;
    }

	return EXIT_SUCCESS;
}
