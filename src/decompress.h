#pragma once

#include "basic_trie.h"
#include <fstream>

class DeCompress {
    class Trie : public BasicTrie {
        bool is_name_ = true;
        std::string name_;
        std::shared_ptr<Node> root_ = nullptr;
        std::shared_ptr<Node> v_;

    public:
        enum class Verdict {
            kArchiveEnd,
            NOTHING,
            TEXT,
            NEW_FILE,
        };

        explicit Trie();

        void Add(const String& s, size_t val);
        Verdict Go(bool go);
        void Output();
        void Output(unsigned char ch);

        bool is_text_ = false;
        size_t symbols_count_ = 0;

        size_t count_ = 0;
        size_t code_ = 0;
        size_t len_ = 1;
        size_t last_len_ = 1;
        std::vector<size_t> val_;
        std::vector<unsigned char> out_v_;
    };

    char** argv_;
    String s_;
    String saved_v_;

    std::ifstream in_;

    bool Read();
    void Cut(size_t idx);

public:
    DeCompress() = delete;
    explicit DeCompress(char** argv);

    void ToDo();
};