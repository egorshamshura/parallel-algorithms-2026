# parallel-algorithms-2026

## QuickSort

Сборка в релизе:

```
$ cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
$ cmake --build build
$ ./build/quicksort/tests/Quicksort_test
```

Мои результаты 2,3 GHz Quad-Core Intel Core i5:
```
Average Quicksort time: 66.1572 seconds
Average Parallel Quicksort time: 16.415 seconds
```
