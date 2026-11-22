#include "quicksort.h"
#include "parlay/parallel.h"
#include "parlay/sequence.h"
#include <algorithm>
#include <parlay/primitives.h>

namespace quicksort {
namespace {
void quicksort_helper(parlay::sequence<int>& arr, int left, int right) {
  if (right - left > 1) {
    int pivot = arr[left + (right - left) / 2];
    int i = left;
    int j = right - 1;

    while (i <= j) {
      while (i < right && arr[i] < pivot) {
        i++;
      }
      while (j >= left && arr[j] > pivot) {
        j--;
      }

      if (i <= j) {
        std::swap(arr[i], arr[j]);
        i++;
        j--;
      }
    }

    if (left < j) {
      quicksort_helper(arr, left, j + 1);
    }
    if (i < right) {
      quicksort_helper(arr, i, right);
    }
  }
}

void parallel_quicksort_helper(parlay::sequence<int>& arr, int left, int right) {
  if (right - left > 1) {
    int pivot = arr[left + (right - left) / 2];
    int i = left;
    int j = right - 1;

    while (i <= j) {
      while (i < right && arr[i] < pivot) {
        i++;
      }
      while (j >= left && arr[j] > pivot) {
        j--;
      }

      if (i <= j) {
        std::swap(arr[i], arr[j]);
        i++;
        j--;
      }
    }
    
    parlay::par_do([&] {
      if (left < j) {
        parallel_quicksort_helper(arr, left, j + 1);
      }
    }, [&] {
      if (i < right) {
        parallel_quicksort_helper(arr, i, right);
      }
    });
  }}
} // anonymous namespace

void quicksort(parlay::sequence<int>& arr) {
  quicksort_helper(arr, 0, arr.size());
}

void parallel_quicksort(parlay::sequence<int>& arr) {
  parallel_quicksort_helper(arr, 0, arr.size());
}
} // namespace quicksort
