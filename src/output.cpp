#include "output.h"

void Output::SetFileName(std::string_view filename) {
    file_name_ = filename;
}

void Output::OpenStream() {
    out_.open(file_name_, std::ios::out | std::ios::binary);
}

void Output::CloseStream() {
    String v;
    while (s_.size() + v.size() < kByte) {
        v.push_back(false);
    }
    Write(v);
    out_.close();
}

void Output::Write(const String& v) {
    if (s_.size() + v.size() < kByte) {
        for (bool b : v) {
            s_.push_back(b);
        }
        return;
    }

    size_t i = 0;
    while (s_.size() < kByte) {
        s_.push_back(v[i]);
        ++i;
    }

    do {
        unsigned char ch = ToDec(s_);
        out_.write(reinterpret_cast<char*>(&ch), sizeof(char));

        s_.clear();
        for (; i < v.size() && s_.size() < kByte; ++i) {
            s_.push_back(v[i]);
        }
    } while (i < v.size());
}

void Output::Write(size_t val) {
    String v = ToBin(val, kBitsCount);
    Write(v);
}
