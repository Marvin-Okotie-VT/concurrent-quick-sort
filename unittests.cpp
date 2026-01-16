#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"

#include "cqsort.hpp"
#include <string>
#include <vector>
#include <algorithm>

template <typename T>
bool is_sorted_check(const T &vec) {
    for (size_t i = 1; i < vec.size(); ++i) {
        if (vec[i-1] > vec[i]) {
            return false;
        }
    }
    return true;
}

// integer tests
TEST_CASE("Sequential sort of integers - small", "[integers]") {
    std::vector<int> v = {5, 2, 8, 1, 9, 3};
    CQSort<std::vector<int>> sorter(0);
    sorter(v);
    REQUIRE(is_sorted_check(v));
}

TEST_CASE("Concurrent sort of integers - small", "[integers]") {
    std::vector<int> v = {5, 2, 8, 1, 9, 3};
    CQSort<std::vector<int>> sorter(4);
    sorter(v);
    REQUIRE(is_sorted_check(v));
}

TEST_CASE("Sequential sort of integers - large", "[integers]") {
    std::vector<int> v(5000);
    for (int i = 0; i < 5000; i++) {
        v[i] = 5000 - i;
    }
    CQSort<std::vector<int>> sorter(0);
    sorter(v);
    REQUIRE(is_sorted_check(v));
}

TEST_CASE("Concurrent sort of integers - large", "[integers]") {
    std::vector<int> v(5000);
    for (int i = 0; i < 5000; i++) {
        v[i] = 5000 - i;
    }
    CQSort<std::vector<int>> sorter(8);
    sorter(v);
    REQUIRE(is_sorted_check(v));
}

// string tests
TEST_CASE("Sequential sort of strings", "[strings]") {
    std::vector<std::string> v = {"zebra", "apple", "banana", "cherry"};
    CQSort<std::vector<std::string>> sorter(0);
    sorter(v);
    REQUIRE(is_sorted_check(v));
}

TEST_CASE("Concurrent sort of strings", "[strings]") {
    std::vector<std::string> v = {"zebra", "apple", "banana", "cherry", "date", "fig"};
    CQSort<std::vector<std::string>> sorter(4);
    sorter(v);
    REQUIRE(is_sorted_check(v));
}

// custom type with operator<
struct Point {
    int x, y;
    Point(int x_, int y_) : x(x_), y(y_) {}
    
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
    
    bool operator>(const Point& other) const {
        return other < *this;
    }
};

TEST_CASE("Sequential sort of custom type", "[custom]") {
    std::vector<Point> v = {Point(3,4), Point(1,2), Point(5,1), Point(1,1)};
    CQSort<std::vector<Point>> sorter(0);
    sorter(v);
    REQUIRE(is_sorted_check(v));
}

TEST_CASE("Concurrent sort of custom type", "[custom]") {
    std::vector<Point> v = {Point(3,4), Point(1,2), Point(5,1), Point(1,1)};
    CQSort<std::vector<Point>> sorter(2);
    sorter(v);
    REQUIRE(is_sorted_check(v));
}

// edge cases
TEST_CASE("Empty vector", "[edge]") {
    std::vector<int> v;
    CQSort<std::vector<int>> sorter(4);
    sorter(v);
    REQUIRE(v.empty());
}

TEST_CASE("Single element", "[edge]") {
    std::vector<int> v = {42};
    CQSort<std::vector<int>> sorter(4);
    sorter(v);
    REQUIRE(v.size() == 1);
    REQUIRE(v[0] == 42);
}

TEST_CASE("Already sorted", "[edge]") {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    CQSort<std::vector<int>> sorter(4);
    sorter(v);
    REQUIRE(is_sorted_check(v));
}

TEST_CASE("Reverse sorted", "[edge]") {
    std::vector<int> v = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    CQSort<std::vector<int>> sorter(4);
    sorter(v);
    REQUIRE(is_sorted_check(v));
}

TEST_CASE("All duplicates", "[edge]") {
    std::vector<int> v = {5, 5, 5, 5, 5, 5};
    CQSort<std::vector<int>> sorter(4);
    sorter(v);
    REQUIRE(is_sorted_check(v));
}

TEST_CASE("Many duplicates mixed", "[edge]") {
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8, 9, 7, 9};
    CQSort<std::vector<int>> sorter(4);
    sorter(v);
    REQUIRE(is_sorted_check(v));
}

TEST_CASE("Two elements - ascending", "[edge]") {
    std::vector<int> v = {1, 2};
    CQSort<std::vector<int>> sorter(4);
    sorter(v);
    REQUIRE(is_sorted_check(v));
}

TEST_CASE("Two elements - descending", "[edge]") {
    std::vector<int> v = {2, 1};
    CQSort<std::vector<int>> sorter(4);
    sorter(v);
    REQUIRE(is_sorted_check(v));
}

// verify different thread counts give same result
TEST_CASE("Different thread counts produce same result", "[threads]") {
    std::vector<int> original = {9, 3, 7, 1, 5, 2, 8, 4, 6};
    
    std::vector<int> v0 = original;
    std::vector<int> v2 = original;
    std::vector<int> v4 = original;
    std::vector<int> v8 = original;
    
    CQSort<std::vector<int>> sorter0(0);
    CQSort<std::vector<int>> sorter2(2);
    CQSort<std::vector<int>> sorter4(4);
    CQSort<std::vector<int>> sorter8(8);
    
    sorter0(v0);
    sorter2(v2);
    sorter4(v4);
    sorter8(v8);
    
    REQUIRE(v0 == v2);
    REQUIRE(v2 == v4);
    REQUIRE(v4 == v8);
    REQUIRE(is_sorted_check(v0));
} 