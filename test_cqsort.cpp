// ------------------------------------------------
// You should not need to edit any files below here
// ------------------------------------------------

#include "catch.hpp"

#include <string>
#include <sstream>

#include "cqsort.hpp"

template <typename T>
bool is_sorted(const T &vec) {
	for (size_t i = 1; i < vec.size(); ++i) { 
		if (vec[i-1] > vec[i]) {
			return false;
		}
	}
	return true;
}

TEST_CASE( "Sequential sort of a vector of integer", "[cqsort]" ) {
	// test vector
	std::vector<int> v = {2, 4, 1, 3};

	// cqsort for a int vector with zero additional thread
	CQSort<std::vector<int>> cqsort(0);

	// sort
	cqsort(v);

	// test
	REQUIRE(is_sorted(v) == true);
}

TEST_CASE( "Sequential sort of a vector of string", "[cqsort]" ) {
	// test vector
	std::vector<std::string> v = {"d", "a", "c", "b"};

	// cqsort for a string vector with zero additional thread
	CQSort<std::vector<std::string>> cqsort(0);

	// sort
	cqsort(v);

	// test
	REQUIRE(is_sorted(v) == true);
}

TEST_CASE( "Parallel sort of a vector of integer", "[cqsort]" ) {
	// test vector
	std::vector<int> v = {2, 4, 1, 3};

	// cqsort for a int vector with two additional threads
	CQSort<std::vector<int>> cqsort(2);

	// sort
	cqsort(v);

	// test
	REQUIRE(is_sorted(v) == true);
}

TEST_CASE( "Parallel sort of a vector of string", "[cqsort]" ) {
	// test vector
	std::vector<std::string> v = {"d", "a", "c", "b"};

	// cqsort for a string vector with two additional threads
	CQSort<std::vector<std::string>> cqsort(2);

	// sort
	cqsort(v);

	// test
	REQUIRE(is_sorted(v) == true);
}
