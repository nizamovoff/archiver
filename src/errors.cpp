#include "errors.h"
#include <iostream>

void IncorrectCommand() {
    std::cout << "Command is incorrect\n";
    std::cout << "For more information use command -h\n";
}