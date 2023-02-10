#include "compress.h"

bool Compress::Trie::Order::operator()(std::shared_ptr<BasicTrie::Node> const a,
                                       std::shared_ptr<BasicTrie::Node> const b) {
    return std::tie(a->cnt_, a->ch_) > std::tie(b->cnt_, b->ch_);
}

bool ByHuffmanCode(const std::pair<size_t, String>& a, const std::pair<size_t, String>& b) {
    if (a.second.size() == b.second.size()) {
        return a.first < b.first;
    }
    return a.second.size() < b.second.size();
}

std::shared_ptr<BasicTrie::Node> Compress::Trie::TakeTop() {
    std::shared_ptr<Node> node = q_.top();
    q_.pop();
    return node;
}

bool Compress::Trie::Dfs(std::shared_ptr<Node> v) {
    if (!v) {
        return true;
    }

    if (v != root_) {
        cur_bits_.push_back(v->bit_);
    }
    if (Dfs(v->left_) || Dfs(v->right_)) {
        bits_.push_back({v->ch_, cur_bits_});
    }
    if (!cur_bits_.empty()) {
        cur_bits_.pop_back();
    }
    return false;
}

void Compress::Trie::Build() {
    for (size_t i = 0; i < kMaxByteValue; ++i) {
        if (cnt_[i] != 0) {
            ++symbols_count_;
            q_.push(NewNode(cnt_[i], i));
        }
    }

    while (q_.size() > 1) {
        std::shared_ptr<Node> left = TakeTop();
        std::shared_ptr<Node> right = TakeTop();
        std::shared_ptr<Node> cur = NewNode(left->cnt_ + right->cnt_, std::min(left->ch_, right->ch_));
        cur->left_ = left;
        cur->right_ = right;
        left->bit_ = false;
        right->bit_ = true;
        q_.push(cur);
    }

    root_ = TakeTop();
    Dfs(root_);
    sort(bits_.begin(), bits_.end(), ByHuffmanCode);

    size_t code = 0;
    for (size_t i = 0; i < bits_[0].second.size(); ++i) {
        table_[bits_[0].first].push_back(false);
    }

    for (size_t i = 0; i + 1 < bits_.size(); ++i) {
        code = (code + 1) << (bits_[i + 1].second.size() - bits_[i].second.size());

        size_t plus = 0;
        String s = ToBin(code);
        while (plus + s.size() < bits_[i + 1].second.size()) {
            ++plus;
            table_[bits_[i + 1].first].push_back(false);
        }
        for (bool b : s) {
            table_[bits_[i + 1].first].push_back(b);
        }
    }

    cur_bits_.clear();
}

Compress::Compress(int argc, char** argv) : argc_(argc), argv_(argv) {
    out_.SetFileName(argv_[2]);
    out_.OpenStream();
    ToDo();
    out_.CloseStream();
}

Compress::Trie::Trie(char** arg) {
    for (size_t i = 0; i < kMaxByteValue; ++i) {
        cnt_[i] = 0;
    }
    cnt_[kFileNameEnd] = 1;
    cnt_[kOneMoreFile] = 1;
    cnt_[kArchiveEnd] = 1;

    std::string str = *arg;
    while (!str.empty() && str.back() != '/') {
        file_name_ += str.back();
        str.pop_back();
    }
    reverse(file_name_.begin(), file_name_.end());
}

std::string Compress::Trie::GetFileName() const {
    return file_name_;
}

size_t Compress::Trie::GetSymbolsCount() const {
    return symbols_count_;
}

size_t Compress::Trie::GetBits(size_t i) const {
    if (i == bits_.size()) {
        return kMaxByteValue;
    }
    return bits_[i].first;
}

const String Compress::Trie::GetCode(const size_t val) const {
    return table_[val];
}

const String Compress::Trie::GetCode(const unsigned char ch) const {
    return GetCode(static_cast<size_t>(ch));
}

void Compress::Trie::Add(const unsigned char ch) {
    size_t i = static_cast<size_t>(ch);
    ++cnt_[i];
}

void Compress::Trie::Add(std::string_view s) {
    for (unsigned char ch : s) {
        Add(ch);
    }
}

void Compress::Trie::PreCalc() {
    in_.SetFileName(file_name_);
    in_.OpenStream();

    Add(file_name_);
    char ch = 0;
    while (in_.Read(ch)) {
        Add(ch);
    }
    in_.CloseStream();
    Build();
}

void Compress::ToDo() {
    for (int idx = 3; idx < argc_; ++idx) {
        Trie trie(argv_ + idx);
        trie.PreCalc();

        out_.Write(trie.GetSymbolsCount());
        for (size_t i = 0; trie.GetBits(i) != kMaxByteValue; ++i) {
            out_.Write(trie.GetBits(i));
        }

        size_t last = 0;
        std::vector<size_t> len(kMaxByteValue, 0);
        for (size_t i = 0; i < kMaxByteValue; ++i) {
            size_t size = trie.GetCode(i).size();
            ++len[size];
            last = std::max(last, size);
        }
        for (size_t i = 1; i <= last; ++i) {
            out_.Write(len[i]);
        }
        for (unsigned char uch : trie.GetFileName()) {
            out_.Write(trie.GetCode(uch));
        }
        out_.Write(trie.GetCode(kFileNameEnd));

        trie.in_.OpenStream();
        char ch = 0;
        while (trie.in_.Read(ch)) {
            unsigned char uch = ch;
            out_.Write(trie.GetCode(uch));
        }
        if (idx + 1 < argc_) {
            out_.Write(trie.GetCode(kOneMoreFile));
        } else {
            out_.Write(trie.GetCode(kArchiveEnd));
        }
        trie.in_.CloseStream();
    }
}
