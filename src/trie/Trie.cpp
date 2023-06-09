//
// Created by owen on 09.05.22.
//

#include <utility>
#include "Trie.h"

Trie::Trie(string alphabet, const vector<Word *> &words) : alphabet_(std::move(alphabet)) {
    this->root_ = new TrieNode<Word *>(nullptr, false, this->alphabet_.length());

    for (auto word: words) {
        this->insert(*word);
    }
}

Trie::~Trie() {
    delete root_;
}

void Trie::insert(const Word &word) const {
    auto current = this->root_;

    for (unsigned int i = 0; i < word.length(); i++) {
        auto indexInAlphabet = this->alphabet_.find(word.getText()[i]);

        if (indexInAlphabet == string::npos) continue;

        if (current->getChild(indexInAlphabet) == nullptr) {
            current->setChild(
                    indexInAlphabet,
                    new TrieNode<Word *>(
                            new Word(word.getText().substr(0, i + 1)),
                            i == word.length() - 1,
                            this->alphabet_.length()
                    )
            );
        } else if (i == word.length() - 1) {
            current->getChild(indexInAlphabet)->setIsTerminal(true);
        }

        current = current->getChild(indexInAlphabet);
    }
}

void
Trie::recursiveAutocomplete(
        TrieNode<Word *> *currentWord,
        Word **suggestions,
        unsigned int &wordCount,
        unsigned int maxWords,
        unsigned int &depth,
        unsigned int maxDepth
) const {
    if (currentWord == nullptr) return;

    for (unsigned int i = 0; i < this->alphabet_.length(); i++) {
        if (currentWord->getChild(i) == nullptr) continue;

        if (currentWord->getChild(i)->getIsTerminal()) {
            suggestions[wordCount] = currentWord->getChild(i)->getData();
            wordCount++;

            if (wordCount >= maxWords) {
                return;
            }
        }
    }

    depth++;

    if (depth >= maxDepth) {
        return;
    }

    for (unsigned int i = 0; i < this->alphabet_.length(); i++) {
        recursiveAutocomplete(currentWord->getChild(i), suggestions, wordCount, maxWords,
                              depth, maxDepth);
    }
}

Word *Trie::search(const string &word) const {
    auto current = this->root_;

    for (auto i: word) {
        auto indexInAlphabet = this->alphabet_.find(i);

        if (current->getChild(indexInAlphabet) == nullptr) {
            return nullptr;
        }

        current = current->getChild(indexInAlphabet);
    }

    if (!current->getIsTerminal()) {
        return nullptr;
    }

    return current->getData();
}

Word **Trie::autocomplete(const string &word, unsigned int maxWords, unsigned int maxDepth) const {
    auto current = this->root_;

    auto *suggestions = Utils::initializeArray<Word *>(maxWords, nullptr);
    unsigned int wordCount = 0;

    for (auto i: word) {
        auto indexInAlphabet = this->alphabet_.find(i);

        if (current->getChild(indexInAlphabet) == nullptr) {
            return suggestions;
        }

        current = current->getChild(indexInAlphabet);
    }

    unsigned int depth = 0;
    recursiveAutocomplete(current, suggestions, wordCount, maxWords, depth, maxDepth);

    return suggestions;
}

ostream &operator<<(ostream &os, const Trie &t) {
    return os;
}
