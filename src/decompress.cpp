#include "decompress.h"
#include "binary_utils.h"

DeCompress::Trie::Trie() {
    root_ = NewNode();
    v_ = root_;
}

void DeCompress::Trie::Add(const String& s, size_t val) {
    std::shared_ptr<Node> v = root_;
    for (bool b : s) {
        if (b) {
            if (!v->right_) {
                std::shared_ptr<Node> ptr = NewNode();
                v->right_ = ptr;
            }
            v = v->right_;
        } else {
            if (!v->left_) {
                std::shared_ptr<Node> ptr = NewNode();
                v->left_ = ptr;
            }
            v = v->left_;
        }
    }
    v->ch_ = val;
}

DeCompress::Trie::Verdict DeCompress::Trie::Go(bool go) {
    if (go) {
        v_ = v_->right_;
    } else {
        v_ = v_->left_;
    }

    size_t vch = v_->ch_;
    if (vch == kNoText) {
        return Trie::Verdict::NOTHING;
    }
    if (vch == kOneMoreFile) {
        return Trie::Verdict::NEW_FILE;
    }
    if (vch == kArchiveEnd) {
        return Trie::Verdict::kArchiveEnd;
    }

    if (vch == kFileNameEnd) {
        std::ofstream fout(name_, std::ios_base::app);
        fout.close();
        is_name_ = false;
    } else {
        unsigned char ch = static_cast<unsigned char>(vch);
        if (is_name_) {
            name_ += ch;
        } else {
            Output(ch);
        }
    }
    v_ = root_;
    return Trie::Verdict::TEXT;
}

void DeCompress::Trie::Output() {
    std::ofstream fout(name_, std::ios_base::app);
    for (unsigned char uch : out_v_) {
        fout << uch;
    }
    fout.close();
    out_v_.clear();
}

void DeCompress::Trie::Output(unsigned char ch) {
    out_v_.push_back(ch);
    if (out_v_.size() == kBufSize) {
        Output();
    }
}

bool DeCompress::Read() {
    if (s_.size() < kBufSize * kBitsCount) {
        char buf[kBufSize];
        in_.read(buf, kBufSize);

        for (size_t i = 0; i < kBufSize; ++i) {
            String str = ToDec(static_cast<unsigned char>(buf[i]));
            for (bool b : str) {
                s_.push_back(b);
            }
        }
    }
    return true;
}

void DeCompress::Cut(size_t idx) {
    if (!idx) {
        return;
    }
    String ns = s_;
    s_.clear();
    for (size_t j = idx; j < ns.size(); ++j) {
        s_.push_back(ns[j]);
    }
}

DeCompress::DeCompress(char** argv) : argv_(argv) {
    in_.open(argv_[2], std::ios::binary | std::ios::out);
    ToDo();
    in_.close();
}

void DeCompress::ToDo() {
    Trie trie;
    while (Read()) {
        bool& is_text = trie.is_text_;
        size_t& symbols_count = trie.symbols_count_;
        size_t& code = trie.code_;
        size_t& len = trie.len_;
        size_t& last_len = trie.last_len_;
        size_t& count = trie.count_;
        std::vector<size_t>& val = trie.val_;

        if (!is_text) {
            size_t l = 0;
            while (l + kBitsCount <= s_.size()) {
                size_t value = ToDec(s_, l, l + kBitsCount);
                l += kBitsCount;

                if (!symbols_count) {
                    symbols_count = value;
                } else {
                    if (val.size() < symbols_count) {
                        val.push_back(value);
                    } else {
                        for (size_t i = 0; i < value; ++i) {
                            code = (code + 1) << (len - last_len);
                            if (i == 0 && count == 0) {
                                code = 0;
                            }
                            last_len = len;

                            String scode = ToBin(code, len);
                            trie.Add(scode, val[count + i]);
                        }

                        ++len;
                        count += value;
                        if (count == symbols_count) {
                            is_text = true;
                            break;
                        }
                    }
                }
            }
            Cut(l);
        }
        if (is_text) {
            size_t i = 0;
            for (; i < s_.size(); ++i) {
                Trie::Verdict ver = trie.Go(s_[i]);
                if (ver == Trie::Verdict::kArchiveEnd) {
                    trie.Output();
                    return;
                } else if (ver == Trie::Verdict::NOTHING || ver == Trie::Verdict::TEXT) {
                    continue;
                } else {
                    Trie new_trie;
                    trie.Output();
                    std::swap(trie, new_trie);
                    break;
                }
            }
            Cut(i + 1);
        }
    }
}