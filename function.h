#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "lru_for_elements.h"
#include <string>

template<unsigned int WAYS, unsigned int LINES>
double read_double(lru_for_elements<WAYS, LINES> &cache, unsigned int ind) {
    double result = 0;
    uint8_t buffer[8];
    for (int i = 0; i < 8; i++)
        buffer[i] = cache.read(ind + i);
    memcpy(&result, buffer, sizeof(double));
    return result;
}

template<unsigned int WAYS, unsigned int LINES>
void write_double(lru_for_elements<WAYS, LINES> &cache, unsigned int ind, double new_val) {
    uint8_t buffer[8];
    memcpy(buffer, &new_val, sizeof(double));
    for (int i = 0; i < 8; i++)
        cache.write(ind + i, buffer[i]);
}

template<unsigned int WAYS, unsigned int LINES>
double read_element_field(lru_for_elements<WAYS, LINES> &cache, unsigned int ind, const std::string& field_name) {
    std::string element_field[8] = {"x", "y", "ax", "ay", "vx", "vy", "a", "b"};
    for(int i = 0; i < 8; i++)
        if(element_field[i] == field_name)
            return read_double(cache, 64 * ind + i * 8);
    return 0;
}

template<unsigned int WAYS, unsigned int LINES>
void write_element_field(lru_for_elements<WAYS, LINES> &cache, unsigned int ind, const std::string& field_name, double new_val) {
    std::string element_field[8] = {"x", "y", "ax", "ay", "vx", "vy", "a", "b"};
    for(int i = 0; i < 8; i++)
        if(element_field[i] == field_name)
            write_double(cache, 64 * ind + i * 8, new_val);
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
