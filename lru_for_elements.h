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
    explicit lru_for_elements(double* _ram) {
        memset(&_size[0], 0, LINES);
        memset(&tags[0][0], 0, LINES * WAYS);
        memset(&ruse[0][0], 0, LINES * WAYS);
        memset(&cache[0][0][0], 0, WAYS * LINES * 8);
        ram = _ram;
    }
    double read(unsigned int ind) {
        return get(ind);
    }
    void write(unsigned int ind, double new_val) {
        get(ind) = new_val;
    }
    void clear() const {
        for (unsigned int line = 0; line < LINES; line++)
            for (unsigned int way = 0; way < _size[line]; way++)
                remove_data(way, line);
    }
    unsigned int get_queries() const {
        return queries;
    }
    unsigned int get_misses() const {
        return cache_misses;
    }

private:
    void load_data(unsigned int way, unsigned int line, unsigned int ind) {
        memcpy(cache[way][line], ram + ind, 8 * sizeof(double));
    }
    void remove_data(unsigned int way, unsigned int line) const {
        unsigned int rm_tag = (((tags[line][way] << LINES_P) + line) << 3u);
        memcpy(ram + rm_tag, cache[way][line], 8 * sizeof(double));
    }
    unsigned int get_mask(unsigned int ind, unsigned int l, unsigned int r) {
        return (ind >> l) & ((1u << (r - l + 1u)) - 1u);
    }
    double& get(unsigned int ind)  {
        ++queries;
        unsigned int line = get_mask(ind, 3, 3 + LINES_P - 1);
        unsigned int _tag = get_mask(ind, 3 + LINES_P, 31);
        unsigned int double_ind = get_mask(ind, 0, 2);

        for (int i = 0; i < _size[line]; i++) {
            if (tags[line][i] == _tag) {
                ruse[line][i] = queries;
                return cache[i][line][double_ind];
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
            remove_data(way, line);
        }
        load_data(way, line, (ind >> 3u) << 3u);
        tags[line][way] = _tag;
        ruse[line][way] = queries;
        return cache[way][line][double_ind];
    }
private:
    static const unsigned int WAYS = (1u << WAYS_P), LINES = (1u << LINES_P);
    unsigned int queries = 0, cache_misses = 0;
    unsigned int _size[LINES]{};
    unsigned int ruse[LINES][WAYS]{};
    unsigned int tags[LINES][WAYS]{};
    double cache[WAYS][LINES][8]{};
    double* ram;
};

#endif //_LRU_FOR_ELEMENTS_H
