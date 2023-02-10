#pragma once

#include <stdio.h>
#include <memory>
#include <string>
#include <vector>

const size_t kBufSize = 1024;

const size_t kByte = 8;
const size_t kBitsCount = 9;

const size_t kFileNameEnd = 256;
const size_t kOneMoreFile = 257;
const size_t kArchiveEnd = 258;
const size_t kMaxByteValue = 259;
const size_t kNoText = 300;

using String = std::vector<bool>;

class BasicTrie {
protected:
    struct Node {
        size_t cnt_ = 0;
        size_t ch_ = kNoText;
        bool bit_ = false;
        std::shared_ptr<Node> left_ = nullptr;
        std::shared_ptr<Node> right_ = nullptr;
    };

    std::shared_ptr<Node> NewNode() const;
    std::shared_ptr<Node> NewNode(const size_t cnt, const size_t ch) const;
};