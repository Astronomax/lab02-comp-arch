#ifndef _LRU_FOR_ELEMENTS_H
#define _LRU_FOR_ELEMENTS_H

#include <cstring>
#include <cstdint>

struct element {
    double x, y, ax, ay, vx, vy, a, b;
};

template<unsigned int WAYS_P, unsigned int LINES_P>
class lru_for_elements {
public:
    explicit lru_for_elements(uint8_t* _ram) {
        memset(&_size[0], 0, LINES);
        memset(&tags[0][0], 0, LINES * WAYS);
        memset(&ruse[0][0], 0, LINES * WAYS);
        memset(&cache[0][0][0], 0, WAYS * LINES * 64);
        ram = _ram;
    }
    uint8_t read(unsigned int ind) {
        return get(ind);
    }
    void write(unsigned int ind, uint8_t new_val) {
        get(ind) = new_val;
    }
    void remove_line(unsigned int way, unsigned int line) const {
        unsigned int rm_tag = (((tags[line][way] << LINES_P) + line) << 6u);
        memcpy(ram + rm_tag, cache[way][line], 64);
    }
    void clear() const {
        for (unsigned int line = 0; line < LINES; line++)
            for (unsigned int way = 0; way < _size[line]; way++)
                remove_line(way, line);
    }
    unsigned int get_queries() const {
        return queries;
    }
    unsigned int get_misses() const {
        return cache_misses;
    }
private:
    uint8_t& get(unsigned int ind)  {
        ++queries;
        unsigned int line = (ind >> 6u) & ((1u << LINES_P) - 1u);
        unsigned int _tag = (ind >> (LINES_P + 6u));
        unsigned int byte_ind = (ind & ((1u << 6u) - 1u));

        for (int i = 0; i < _size[line]; i++) {
            if (tags[line][i] == _tag) {
                ruse[line][i] = queries;
                return cache[i][line][byte_ind];
            }
        }
        ++cache_misses;
        unsigned int way = 0;
        if (_size[line] < WAYS)
            way = _size[line]++;
        else {
            for (unsigned int i = 0; i < WAYS; i++)
                if (ruse[line][i] < ruse[line][way])
                    way = i;
            remove_line(way, line);
        }
        memcpy(cache[way][line], ram + ((ind >> 6u) << 6u), 64);
        tags[line][way] = _tag;
        ruse[line][way] = queries;
        return cache[way][line][byte_ind];
    }
private:
    static const unsigned int WAYS = (1 << WAYS_P), LINES = (1 << LINES_P);
    unsigned int queries = 0, cache_misses = 0;
    unsigned int _size[LINES]{};
    unsigned int ruse[LINES][WAYS]{};
    uint8_t tags[LINES][WAYS]{};
    uint8_t cache[WAYS][LINES][64]{};
    uint8_t* ram;
};

#endif //_LRU_FOR_ELEMENTS_H
