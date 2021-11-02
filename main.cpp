#include <cstring>
#include "lru_for_elements.h"
#include "function.h"

using namespace std;

int main() {
    element arr[1000];
    uint8_t ram[64000];
    memcpy(ram, arr, 1000 * sizeof(element));
    lru_for_elements cache(ram);
    f(cache, 1000, 0.1);
    cache.clear();
    printf("cache misses: %.3f%%\n", cache.get_misses() * 100.0 / cache.get_queries());
    return 0;
}