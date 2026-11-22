#include "quicksort.h"
#include <gtest/gtest.h>

constexpr int N = 5;

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

TEST(ParallelQuicksortTest, SortsBothPartitions) {
  parlay::sequence<int> arr;
  for (int i = 0; i < 150000; ++i) {
    arr.push_back((i * 7919) % 10007);
  }
  parlay::sequence<int> expected = arr;
  std::sort(expected.begin(), expected.end());

  parlay::execute_with_scheduler(4, [&] {
    quicksort::parallel_quicksort(arr);
  });

  EXPECT_EQ(arr, expected);
}

TEST(ParallelQuicksortTest, HandlesEdgeCases) {
  const parlay::sequence<int> cases[] = {
      {}, {42}, {1, 2, 3, 4, 5}, {5, 4, 3, 2, 1}, {7, 7, 7, 7}};

  parlay::execute_with_scheduler(4, [&] {
    for (auto arr : cases) {
      auto expected = arr;
      std::sort(expected.begin(), expected.end());
      quicksort::parallel_quicksort(arr);
      EXPECT_EQ(arr, expected);
    }
  });
}

TEST(CompareTime, Performance) {
  std::chrono::duration<double> total_quick{0};
  std::chrono::duration<double> total_parallel{0};

  for (int run = 0; run < N; ++run) {
    parlay::sequence<int> arr;
    for (int i = 0; i < 100000000; ++i) {
        auto x = rand() % 10000000;
        arr.push_back(x);
    }
    parlay::sequence<int> arr_copy = arr;
    auto expected = arr;
    std::sort(expected.begin(), expected.end());

    auto start = std::chrono::high_resolution_clock::now();
    quicksort::quicksort(arr);
    auto end = std::chrono::high_resolution_clock::now();
    total_quick += end - start;

    start = std::chrono::high_resolution_clock::now();
    quicksort::parallel_quicksort(arr_copy);
    end = std::chrono::high_resolution_clock::now();
    total_parallel += end - start;
    EXPECT_EQ(arr, expected) << "quicksort error\n";
    EXPECT_EQ(arr_copy, expected) << "parallel_quicksort error\n";
  }

  std::cout << "Average Quicksort time: " << total_quick.count() / N << " seconds\n";
  std::cout << "Average Parallel Quicksort time: " << total_parallel.count() / N
            << " seconds\n";
}