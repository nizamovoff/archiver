#pragma once

#include "binary_utils.h"
#include <fstream>

class Output {
    std::ofstream out_;
    std::string file_name_;
    String s_;

public:
    void SetFileName(std::string_view filename);
    void OpenStream();
    void CloseStream();

    void Write(const String& v);
    void Write(size_t val);
};