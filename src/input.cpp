#include "input.h"

void Input::SetFileName(std::string_view filename) {
    file_name_ = filename;
}

void Input::OpenStream() {
    in_.open(file_name_);
}

void Input::CloseStream() {
    in_.close();
}

bool Input::Read(char& ch) {
    if (in_.get(ch)) {
        return true;
    }
    return false;
}