#include "binary_utils.h"

size_t ToDec(String& s, size_t l, size_t r) {
    size_t val = 0;
    for (size_t i = l; i < r; ++i) {
        val = val * 2 + s[i];
    }
    return val;
}

size_t ToDec(String& s) {
    return ToDec(s, 0, s.size());
}

String ToDec(unsigned char ch) {
    String s;
    size_t val = static_cast<size_t>(ch);
    for (size_t i = 0; i < kByte; ++i) {
        s.push_back(val >> i & 1);
    }
    reverse(s.begin(), s.end());
    return s;
}

String ToBin(size_t val, size_t min_len) {
    String v;
    for (size_t i = 0; (val >> i) || i < min_len; ++i) {
        v.push_back(val >> i & 1);
    }
    reverse(v.begin(), v.end());
    return v;
}