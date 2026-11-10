#include "quicksort.h"
#include "parlay/parallel.h"
#include "parlay/sequence.h"
#include <parlay/primitives.h>

namespace quicksort {
namespace {
void quicksort_helper(parlay::sequence<int>& arr, int left, int right) {
  if (left < right) {
    int pivot = arr[right];
    int i = left - 1;
    for (int j = left; j < right; j++) {
      if (arr[j] <= pivot) {
        i++;
        std::swap(arr[i], arr[j]);
      }
    }
    std::swap(arr[i + 1], arr[right]);
    int partition_index = i + 1;

    quicksort_helper(arr, left, partition_index - 1);
    quicksort_helper(arr, partition_index + 1, right);
  }
}
} // anonymous namespace

void quicksort(parlay::sequence<int>& arr) {
  quicksort_helper(arr, 0, arr.size() - 1);
}

void parallel_quicksort(parlay::sequence<int>& arr) {
    if (arr.size() <= 1000) {
        quicksort(arr);
        return;
    }
    
    int pivot = arr[arr.size() / 2];
    parlay::sequence<int> less, equal, greater;
    for (size_t i = 0; i < arr.size(); ++i) {
        if (arr[i] < pivot) {
            less.push_back(arr[i]);
        } else if (arr[i] == pivot) {
            equal.push_back(arr[i]);
        } else {
            greater.push_back(arr[i]);
        }
    }
    
    parlay::par_do(
        [&] { parallel_quicksort(less); },
        [&] { parallel_quicksort(greater); }
    );

    parlay::sequence<int> result(arr.size());
    size_t pos = 0;

    for (int x : less) result[pos++] = x;
    for (int x : equal) result[pos++] = x;
    for (int x : greater) result[pos++] = x;

    arr = std::move(result);
}
} // namespace quicksort
