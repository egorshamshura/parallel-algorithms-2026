#include "quicksort.h"
#include <gtest/gtest.h>

TEST(QuicksortTest, Correctness) {
  parlay::sequence<int> arr = {5, 2, 9, 1, 5, 6};
  quicksort::quicksort(arr);
  EXPECT_EQ(arr, (parlay::sequence<int>{1, 2, 5, 5, 6, 9}));
}

TEST(RandomizedQuicksortTest, Correctness) {
  parlay::sequence<int> arr;
  for (int i = 0; i < 1000000; ++i) {
    arr.push_back(rand() % 1000);
  }
  parlay::sequence<int> expected = arr;
  std::sort(expected.begin(), expected.end());
  quicksort::quicksort(arr);
  EXPECT_EQ(arr, expected);
}

TEST(CompareTime, Performance) {
  parlay::sequence<int> arr;
  for (int i = 0; i < 100000000; ++i) {
    arr.push_back(rand() % 100000);
  }
  parlay::sequence<int> arr_copy = arr;

  auto start = std::chrono::high_resolution_clock::now();
  quicksort::quicksort(arr);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_quick = end - start;
  std::cout << "Quicksort time: " << elapsed_quick.count() << " seconds\n";
  start = std::chrono::high_resolution_clock::now();
  parlay::execute_with_scheduler(4, [&] {
        quicksort::parallel_quicksort(arr_copy);
  });
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed_parallel = end - start;

  EXPECT_EQ(arr, arr_copy);
  std::cout << "Parallel Quicksort time: " << elapsed_parallel.count() << " seconds\n";
}