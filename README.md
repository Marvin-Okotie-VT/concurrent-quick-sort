# Concurrent Quicksort

## Overview
This project implements a Concurrent Quicksort algorithm in C++, combining a standard quicksort algorithm with concurrency to improve performance on large datasets. The implementation focuses on correctness, scalability, and performance evaluation.

This repository contains a cleaned, standalone version of a course project, with instructor-provided scaffolding and grading artifacts removed.

## Key Features
- Concurrent quicksort implementation using C++ templates
- Configurable concurrency to control task parallelism
- Benchmarking driver for performance measurement
- Unit tests to validate correctness
- CMake-based build system
- Docker support for reproducible builds

## Build & Run
mkdir build
cd build
cmake ..
cmake --build . --config Release
Release\cqbench.exe
Release\unittests.exe