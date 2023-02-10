#include "compress.h"
#include "decompress.h"
#include "errors.h"
#include "help.h"

const std::string kCmdC = "-c";
const std::string kCmdD = "-d";
const std::string kCmdH = "-h";

int main(int argc, char** argv) {
    if (argc >= 4 && argv[1] == kCmdC) {
        Compress archive(argc, argv);
    } else if (argc == 3 && argv[1] == kCmdD) {
        DeCompress archive(argv);
    } else if (argc == 2 && argv[1] == kCmdH) {
        Help();
    } else {
        IncorrectCommand();
    }
    return 0;
}