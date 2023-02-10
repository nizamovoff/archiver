#include "basic_trie.h"

std::shared_ptr<BasicTrie::Node> BasicTrie::NewNode() const {
    std::shared_ptr<Node> node = std::make_shared<Node>();
    return node;
}

std::shared_ptr<BasicTrie::Node> BasicTrie::NewNode(const size_t cnt, const size_t ch) const {
    std::shared_ptr<Node> node = std::make_shared<Node>();
    node->cnt_ = cnt;
    node->ch_ = ch;
    return node;
}
