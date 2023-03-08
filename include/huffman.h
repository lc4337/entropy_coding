/*
* A C++ compression and decompression program based on Huffman Coding.
* 实现利用霍夫曼编码对文件进行压缩以及解压缩
*/
#ifndef HUFFMAN_H_INCLUDED
#define HUFFMAN_H_INCLUDED

#include <bits/stdc++.h>
using namespace std;

class Huffman {
  public:
    Huffman() = delete;
    Huffman(const std::string& s);
    ~Huffman();

    bool encode(const std::string &targetFile);

    bool decode(const std::string &targetFile);

  private:
    struct Node {
        char ch;
        long long freq;
        Node *left, *right;
        Node(): ch(0), freq(0), left(nullptr), right(nullptr) {}
    };
    struct cmp {
        bool operator()(Node *a, Node *b) {
            return a->freq > b->freq;
        }
    };


    Node *root, *nodes;
    unordered_map<char, string> code;
    const string fileName;

    void get_map(Node *root, std::string& s);

    bool build();

    void del(Node *root);

    void clear();

    //按频次从高到低打印编码表, 用于调试, 只有调用了build()和get_map()之后才能正确打印
    void printCode();

};

#endif
