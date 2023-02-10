#pragma once

#include "basic_trie.h"

size_t ToDec(String& s, size_t l, size_t r);
size_t ToDec(String& s);
String ToDec(unsigned char ch);
String ToBin(size_t val, size_t min_len = 0);