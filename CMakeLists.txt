project(archiver)

add_executable(
        archiver
        src/archiver.cpp
        src/binary_utils.cpp
        src/compress.cpp
        src/decompress.cpp
        src/errors.cpp
        src/help.cpp
        src/basic_trie.cpp
        src/input.cpp
        src/output.cpp
)