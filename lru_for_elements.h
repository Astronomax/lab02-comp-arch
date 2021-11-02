#ifndef UNTITLED1_LRU_FOR_ELEMENTS_H
#define UNTITLED1_LRU_FOR_ELEMENTS_H

#include <cstring>
#include <cstdint>

struct element {
    double x, y, ax, ay, vx, vy, a, b;
};

class lru_for_elements {
public:
    explicit lru_for_elements(uint8_t *ram);
    uint8_t read(unsigned int ind);
    void write(unsigned int ind, uint8_t new_val);
    void remove_line(unsigned int way, unsigned int line) const;
    void clear() const;
    unsigned int get_queries() const;
    unsigned int get_misses() const;

private:
    uint8_t& get(unsigned int ind);

private:
    unsigned int queries = 0, misses = 0;
    unsigned int _size[128]{};
    unsigned int ruse[128][4]{};
    uint8_t addr[128][4]{};
    uint8_t cache[4][128][64]{};
    uint8_t* ram;
};

#endif //UNTITLED1_LRU_FOR_ELEMENTS_H
