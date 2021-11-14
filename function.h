#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "lru_for_elements.h"
#include <string>

template<unsigned int WAYS, unsigned int LINES>
double read_element_field(lru_for_elements<WAYS, LINES> &cache, unsigned int ind, const std::string& field_name) {
    std::string element_field[8] = {"x", "y", "ax", "ay", "vx", "vy", "a", "b"};
    for(int i = 0; i < 8; i++)
        if(element_field[i] == field_name)
            return cache.read(8 * ind + i);
    return 0;
}

template<unsigned int WAYS, unsigned int LINES>
void write_element_field(lru_for_elements<WAYS, LINES> &cache, unsigned int ind, const std::string& field_name, double new_val) {
    std::string element_field[8] = {"x", "y", "ax", "ay", "vx", "vy", "a", "b"};
    for(int i = 0; i < 8; i++)
        if(element_field[i] == field_name)
            cache.write(8 * ind + i, new_val);
}

template<unsigned int WAYS, unsigned int LINES>
void f(lru_for_elements<WAYS, LINES> &cache, int n, double asqr) {
    for (int i = 0; i < n; ++i) {
        double x = read_element_field(cache, i, "x"),
                ax = read_element_field(cache, i, "ax"),
                vx = read_element_field(cache, i, "vx");
        write_element_field(cache, i, "x", x + vx * asqr + 0.5 * ax * asqr * asqr);
    }
    for (int i = 0; i < n; ++i) {
        double y = read_element_field(cache, i, "y"),
                ay = read_element_field(cache, i, "ay"),
                vy = read_element_field(cache, i, "vy");
        write_element_field(cache, i, "y", y + vy * asqr + 0.5 * ay * asqr * asqr);
    }
}

#endif //_FUNCTION_H
