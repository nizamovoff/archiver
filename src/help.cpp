#include "help.h"
#include <iostream>

void Help() {
    std::cout << "use \"-c archive_name file1 [file2 ...]\" to create an archive with name <archive_name> containing "
                 "file1, file2, ...\n";
    std::cout << "use \"-d archive_name\" to unzip archive\n";
    std::cout << "use \"-h\" for help\n";
}