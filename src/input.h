#pragma once

#include "binary_utils.h"
#include <fstream>

class Input {
    std::string file_name_;
    std::ifstream in_;

public:
    void SetFileName(std::string_view filename);
    void OpenStream();
    void CloseStream();

    bool Read(char& ch);
};