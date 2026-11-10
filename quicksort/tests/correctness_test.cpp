#include "quicksort.h"
#include <gtest/gtest.h>

constexpr int N = 1;

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
  std::chrono::duration<double> total_quick{0};
  std::chrono::duration<double> total_parallel{0};

  for (int run = 0; run < N; ++run) {
    parlay::sequence<int> arr;
    for (int i = 0; i < 100000000; ++i) {
      arr.push_back(rand() % 100000);
    }
    parlay::sequence<int> arr_copy = arr;
    auto expected = arr;
    std::sort(expected.begin(), expected.end());

    auto start = std::chrono::high_resolution_clock::now();
    quicksort::quicksort(arr);
    auto end = std::chrono::high_resolution_clock::now();
    total_quick += end - start;

    start = std::chrono::high_resolution_clock::now();
    parlay::execute_with_scheduler(4, [&] {
      quicksort::parallel_quicksort(arr_copy);
    });
    end = std::chrono::high_resolution_clock::now();
    total_parallel += end - start;
    EXPECT_EQ(arr, expected) << "Ошибка в последовательном quicksort\n";
    EXPECT_EQ(arr_copy, expected) << "Ошибка в parallel_quicksort\n";
  }

  std::cout << "Average Quicksort time: " << total_quick.count() / N << " seconds\n";
  std::cout << "Average Parallel Quicksort time: " << total_parallel.count() / N
            << " seconds\n";
}