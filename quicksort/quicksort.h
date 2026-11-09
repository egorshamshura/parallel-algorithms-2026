#include <parlay/sequence.h>

namespace quicksort {
void quicksort(parlay::sequence<int>& arr);
void parallel_quicksort(parlay::sequence<int>& arr);
} // namespace quicksort
