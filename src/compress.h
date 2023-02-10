#pragma once

#include "basic_trie.h"
#include "input.h"
#include "output.h"
#include <queue>

class Compress {
    class Trie : public BasicTrie {
        struct Order {
            bool operator()(std::shared_ptr<Node> const a, std::shared_ptr<Node> const b);
        };

        size_t symbols_count_ = 0;
        String cur_bits_;
        std::shared_ptr<Node> root_ = nullptr;

        size_t cnt_[kMaxByteValue];
        String table_[kMaxByteValue];

        std::vector<std::pair<std::size_t, String>> bits_;
        std::priority_queue<std::shared_ptr<BasicTrie::Node>, std::vector<std::shared_ptr<Node>>, Order> q_;

        std::shared_ptr<Node> TakeTop();
        bool Dfs(std::shared_ptr<Node> v);

        std::string file_name_;

    public:
        Input in_;

        void Build();

        explicit Trie(char** arg);

        std::string GetFileName() const;
        size_t GetSymbolsCount() const;

        size_t GetBits(size_t i) const;
        const String GetCode(const size_t val) const;
        const String GetCode(const unsigned char ch) const;

        void Add(const unsigned char ch);
        void Add(std::string_view s);
        void PreCalc();
    };

    int argc_;
    char** argv_;
    String s_;
    Output out_;

    void ToDo();

public:
    Compress() = delete;
    explicit Compress(int argc, char** argv);
};