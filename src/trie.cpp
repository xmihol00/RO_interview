#include "trie.h"

using namespace std;

Trie::Trie() : _value{0}, _endOfWord{false}, _children(ALPHABET_SIZE) { }

Trie::Trie(string fileName) : _value{0}, _children(ALPHABET_SIZE) 
{
    ifstream file(fileName);
    if (!file.is_open())
    {
        throw runtime_error("Could not open file '" + fileName + "'.");
    }

    string line;
    while (getline(file, line))
    {
        if (!line.empty())
        {
            Trie* current = this;
            for (char c : line)
            {
                if (current->_children[c] == nullptr)
                {
                    current->_children[c] = make_unique<Trie>();
                }
                current = current->_children[c].get();
                current->_value = static_cast<int>(c);
            }
            current->_endOfWord = true;
        }
    }
}

int Trie::value() const
{
    return _value;
}

const vector<unique_ptr<INode>>& Trie::children() const
{
    return reinterpret_cast<const std::vector<std::unique_ptr<INode>> &>(_children);
}

void Trie::print()
{
    string currentWord = "";
    print(currentWord);
}

void Trie::printASCII()
{
    string currentWord = "";
    print(currentWord, [](const string& word) 
    { 
        for (char c : word)
        {
            cout << setw(4) << setfill(' ') << static_cast<int>(c);
        }
        cout << endl;
    });
}

void Trie::print(string currentWord, const function<void(const string&)>& displayFunction)
{
    if (_endOfWord)
    {
        displayFunction(currentWord);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (_children[i] != nullptr)
        {
            currentWord.push_back(static_cast<char>(i));
            _children[i]->print(currentWord, displayFunction);
            currentWord.pop_back();
        }
    }
}
