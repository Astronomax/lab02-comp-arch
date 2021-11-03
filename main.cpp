#include "lru_for_elements.h"
#include "function.h"

using namespace std;

int main() {
    const int N = 1000;
    element arr[N];
    uint8_t ram[sizeof(element) * N];
    memcpy(ram, arr, sizeof(element) * N);
    lru_for_elements<2, 7> cache(ram);
    f(cache, N, 0.1);
    cache.clear();
    printf("%d %.3f%%\n", cache.get_misses(), cache.get_misses() * 100.0 / cache.get_queries());
    return 0;
}