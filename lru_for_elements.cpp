#include "lru_for_elements.h"

lru_for_elements::lru_for_elements(uint8_t *ram) {
    memset(&_size[0], 0, 128);
    memset(&addr[0][0], 0, 128 * 4);
    memset(&ruse[0][0], 0, 128 * 4);
    memset(&cache[0][0][0], 0, 4 * 128 * 64);
    this->ram = ram;
}

uint8_t lru_for_elements::read(unsigned int ind) {
    return get(ind);
}

void lru_for_elements::write(unsigned int ind, uint8_t new_val) {
    get(ind) = new_val;
}

void lru_for_elements::remove_line(unsigned int way, unsigned int line) const {
    unsigned int rm_addr = (((addr[line][way] << 7u) + line) << 6u);
    memcpy(ram + rm_addr, cache[way][line], 64);
}

void lru_for_elements::clear() const {
    for (unsigned int line = 0; line < 128; line++)
        for (unsigned int way = 0; way < _size[line]; way++)
            remove_line(way, line);
}

unsigned int lru_for_elements::get_queries() const {
    return queries;
}

unsigned int lru_for_elements::get_misses() const {
    return misses;
}

uint8_t &lru_for_elements::get(unsigned int ind) {
    unsigned int line = (ind >> 6u) & ((1u << 7u) - 1u);
    unsigned int _addr = (ind >> 13u);
    unsigned int byte_ind = (ind & ((1u << 6u) - 1u));

    for (int i = 0; i < _size[line]; i++) {
        if (addr[line][i] == _addr) {
            ruse[line][i] = queries++;
            return cache[i][line][byte_ind];
        }
    }
    ++misses;
    unsigned int way = 0;
    if (_size[line] < 4)
        way = _size[line]++;
    else {
        for (unsigned int i = 0; i < 4; i++)
            if (ruse[line][i] < ruse[line][way])
                way = i;
        remove_line(way, line);
    }
    memcpy(cache[way][line], ram + ((ind >> 6u) << 6u), 64);
    addr[line][way] = _addr;
    ruse[line][way] = queries++;
    return cache[way][line][byte_ind];
}