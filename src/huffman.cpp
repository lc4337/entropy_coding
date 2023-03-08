/*
* A C++ compression and decompression program based on Huffman Coding.
* 实现利用霍夫曼编码对文件进行压缩以及解压缩
*/

#include <bits/stdc++.h>
#include "huffman.h"


using namespace std;


Huffman::Huffman(const string& s) : root(nullptr), nodes(new Node[256]), fileName(s) {
    for(int i = 0; i < 256; ++i)
        nodes[i].ch = i;
}
Huffman::~Huffman()
{
    clear();
}

bool Huffman::encode(const string &targetFile)
{
    ifstream fin(fileName, ios::binary);
    if(!fin.is_open()) {
        cout << "Failed to open the file!" << endl;
        return false;
    }
    ofstream fout(targetFile, ios::binary);
    char buff[512];
    while(!fin.eof())
    {
        fin.read(buff, sizeof(buff));
        int n = fin.gcount();
        for(int i = 0; i < n; ++i)
            ++nodes[(unsigned char)buff[i]].freq;
    } 
    fout.write((char*)nodes, sizeof(Node) * 256);
    if(!build())
        return false;
    string s;
    get_map(root, s);
    //重置fin的读指针和标志位
    fin.clear(), fin.seekg(0);
    s.clear();
    while(!fin.eof())
    {
        fin.read(buff, sizeof(buff));
        int n = fin.gcount();
        for(int i = 0; i < n; ++i)
            s += code[buff[i]];
        while(s.size() >= 8)
        {
            fout.put((char)stoi(s.substr(0, 8), nullptr, 2));
            s.erase(0, 8);
        }
    }
    int cnt = 0;
    if(s.size() > 0)
    {
        cnt = 8 - s.size(); 
        s += string(cnt, '0');
        fout.put((char)stoi(s, nullptr, 2));
    }
    //文件末尾补0的个数
    fout.put((char)cnt);
    // printCode();

    fout.close();
    fin.close();
    return true;
}

bool Huffman::decode(const string &targetFile)
{
    ifstream fin(fileName, ios::binary);
    if(!fin.is_open()) {
        cout << "Failed to open the file!" << endl;
        return false;
    }
    ofstream fout(targetFile, ios::binary); 
    char buff[512];
    fin.read((char*)nodes, sizeof(Node) * 256);
    if(!build())
        return false;
    Node *cur = nullptr;
    fin.get(buff[0]);
    fin.get(buff[1]);
    fin.clear();
    while(!fin.eof()) {
        //保留前两个字节，用来存上一次读取的后两个字节，保证每次的buff里至少有两个字节，可以正确解码最后一个字节。
        fin.read(buff + 2, sizeof(buff) - 2);
        int n = (fin.gcount() + 2) * 8;
        //调整最后的字符
        if(fin.eof())
            n -= (unsigned int)buff[n / 8 - 1] + 7;
        else
            n -= 16;
        for(int i = 0; i < n; ++i) {
            if(cur == nullptr)
                cur = root;
            if(buff[i / 8] & (1 << (7 - (i % 8))))
                cur = cur->right;
            else
                cur = cur->left;
            if(cur->left == nullptr && cur->right == nullptr) {
                fout << cur->ch;
                cur = nullptr;
            }
        }
        buff[0] = buff[n / 8];
        buff[1] = buff[n / 8 + 1];
    } 
    // string s;
    // get_map(root, s);
    // printCode();
    fin.close();
    fout.close();
    return true;
}

void Huffman::get_map(Node *root, string& s) {
    if(root == nullptr)
        return;
    if (root->left == nullptr && root->right == nullptr) {
        code[root->ch] = s;
        return;
    }
    s.push_back('0');
    get_map(root->left, s);
    s.pop_back();
    s.push_back('1');
    get_map(root->right, s);
    s.pop_back();
}

bool Huffman::build()
{
    priority_queue<Node*, vector<Node*>, cmp> pq;
    for(int i = 0; i < 256; ++i)
        if(nodes[i].freq)
            pq.push(nodes + i);
    if(pq.empty()) {
        cout << "Empty file!" << endl;
        return false;
    }
    if(pq.size() == 1)
    {
        root = new Node();
        root->left = pq.top();
        return true;
    }

    while(pq.size() > 1)
    {
        Node *l = pq.top();
        pq.pop();
        Node *r = pq.top();
        pq.pop();
        Node *nxt = new Node();
        nxt->freq = l->freq + r->freq;
        nxt->left = l, nxt->right = r;
        pq.push(nxt);
    }
    root = pq.top();
    return true;
}

void Huffman::del(Node *root) {
    if (!root or (root->left == nullptr and root->right == nullptr)) return;
    del(root->left);
    del(root->right);
    delete root;
}
void Huffman::clear()
{
    del(root);
    delete[] nodes;
    code.clear();
}

//按频次从高到低打印编码表, 用于调试, 只有调用了build()和get_map()之后才能正确打印
void Huffman::printCode()
{
    long long sum = 0;
    vector<Node> v;
    for(int i = 0; i < 256; ++i)
    {
        sum += nodes[i].freq;
        if(nodes[i].freq)
            v.push_back(nodes[i]);
    }
    sort(v.begin(), v.end(), [](const Node &a, const Node &b) {
        return a.freq > b.freq;
    });
    cout << "filesize: " << sum << " bytes" << endl;
    cout << "huffman coding :" << endl;
    cout << "count\tchar\tcode" << endl;
    for(Node &n : v)
        if(n.freq)
        {
            cout << n.freq << "\t";
            if(isgraph(n.ch))
                cout << n.ch << "\t: " << code[n.ch] << endl;
            else
                cout << "\\" << int((unsigned char)n.ch) << "\t: " << code[n.ch] << endl;
        }
    cout << endl;
}
