#ifndef _TRIE_H_ // use guards instead of #pragma once, which is non-standard and older compilers may not support it
#define _TRIE_H_

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <functional>
#include <iomanip>

#include "INode.h"

class Trie : public INode
{
public:
    constexpr static int ALPHABET_SIZE = 256;

    Trie();
    Trie(std::string fileName);

    int value() const override;
    const std::vector<std::unique_ptr<INode>>& children() const override;

    void print();
    void printASCII();

private:
    int _value;
    bool _endOfWord;
    std::vector<std::unique_ptr<Trie>> _children;

    void print(std::string currentWord, const std::function<void(const std::string&)>& displayFunction = [](const std::string& word) { std::cout << word << std::endl; });
};


#endif // _TRIE_H_