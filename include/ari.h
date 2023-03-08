/*
* A C++ compression and decompression program based on Arithmetic Coding.
*/
#ifndef ARI_H_INCLUDED
#define ARI_H_INCLUDED

#include <bits/stdc++.h>
using namespace std;

struct probModel {
    using u64 = unsigned long long;
    using u8 = unsigned char;
//   public:
    struct prob {
        u64 low, high, count;
    };

    probModel(): numSym(257), freq(numSym, 0) {
        for(int i = 1; i < numSym; ++i)
            freq[i] = i;
    }
    ~probModel() = default;

    void update(u8 sym) {
        for (int i = sym + 1; i < numSym; ++i)
            ++freq[i];
    }

    prob getprob(int sym) {
        prob p;
        if(sym == numSym - 1)
            p = prob{freq[sym], freq[sym] + 1, freq[sym] + 1};
        else
            p =  prob{freq[sym], freq[sym + 1], freq[numSym - 1] + 1};
        return p;
    }

    int getsym(int val) {
        int sym = upper_bound(freq.begin(), freq.end(), val) - freq.begin();
        return sym - 1;
    }
//   private:
    const int numSym;
    vector<u64> freq;
};

class Ari {
    using u64 = unsigned long long;
    using u8 = unsigned char;
  public:
    Ari(): low(0), high(0xffffffffu), underflow(0), buffer(0){}
    ~Ari() = default;

    void encodeSym(probModel &model, int sym); 

    int decodeSym(probModel &model);

    bool encode(const string& in, const string& out);

    bool decode(const string& in, const string& out);

  private: 

    bool getbit();

    void writebit(const bool& bit);

    void outbit(bool bit);


  //output to console for test
  
    // bool getbit() {
    //     if(buf < int(str.size()))
    //         return (str[buf++] - '0');
    //     else
    //         return 0;
    // }
    // void outbit(bool bit) {
    //     cout << bit;
    //     while (underflow) {
    //         cout << !bit;
    //          underflow--;
    //     }
    // }
    

    void endEncode(void); 

    int initDecode(void);

    u64 low, high, code;
    int buf;
    int underflow;
    u8 buffer;
    ifstream fin;
    ofstream fout;
    string str;
};


#endif