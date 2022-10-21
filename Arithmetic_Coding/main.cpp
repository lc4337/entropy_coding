#include <bits\stdc++.h>
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

    void encodeSym(probModel &model, int sym) {
        auto p = model.getprob(sym);
        u64 range = high - low;
        high = low + (range * p.high) / p.count;
        low = low + (range * p.low) / p.count;
        for(;;) {
            if(high < 0x80000000) {
                outbit(0);
            } else if(low >= 0x80000000) {
                outbit(1);
                low -= 0x80000000;
                high -= 0x80000000;
            } else if(low >= 0x40000000 && high < 0xc0000000) {
                ++underflow;
                low -= 0x40000000;
                high -= 0x40000000;
            } else {
                break;
            }
            low <<= 1;
            high <<= 1;
        }
    }

    int decodeSym(probModel &model) {
        u64 range = high - low;
        //why +1 -1  ??
        u64 cum = ((code - low + 1) * (model.freq[model.numSym - 1] + 1) - 1) / range; 
        int sym = model.getsym(cum);
        auto prob = model.getprob(sym);
        high = low + (range * prob.high) / prob.count;
        low = low + (range * prob.low) / prob.count;
        for(;;) {
            if(high < 0x80000000) {
            } else if(low >= 0x80000000) {
                code -= 0x80000000;
                low -= 0x80000000;
                high -= 0x80000000;
            } else if(low >= 0x40000000 && high < 0xc0000000) {
                code -= 0x40000000;
                low -= 0x40000000;
                high -= 0x40000000;
            } else {
                break;
            }
            low <<= 1;
            high <<= 1;
            code <<= 1;
            code |= getbit();
        }
        return sym;
    }

    bool encode(const string& in, const string& out)
    {
        buf = buffer = 0;
        fin.open(in, ios::binary | ios::in);
        fout.open(out, ios::binary | ios::out);
        if(!fin || !fout) {
            // cout << "open file failed" << endl;
            return false;
        }
        low = 0, high = 0xffffffffu, underflow = 0;
        probModel model;
        // for(auto& c: s) {
        //     encodeSym(model, (u8)c);
        //     model.update(c);
        // }
        while(true) {
            u8 c = fin.get();
            if(fin.eof())
                break;
            encodeSym(model, c);
            model.update(c);
        }
        encodeSym(model, 256);
        endEncode();
        fin.close();
        fout.close();
        return true;
    }

    bool decode(const string& in, const string& out)
    {
        // str = s, buf = 0;
        buf = buffer = 0;
        fin.open(in, ios::binary);
        fout.open(out, ios::binary);
        if(!fin.is_open() || !fout.is_open()) {
            // cout << "open file failed" << endl;
            return false;
        }
        low = 0, high = 0xffffffffu, underflow = 0;
        probModel model;
        initDecode();
        // while(true)
        // {
        //     int sym = decodeSym(model);
        //     if(sym == 256) break;
        //     cout << (sym);
        //     model.update(sym);
        // }
        while(true) {
            int sym = decodeSym(model);
            if(sym == 256) break;
            fout.put(u8(sym));
            model.update(sym);
        }
        fin.close();
        fout.close();
        return true;
    }

  private: 

    bool getbit()
    {
        if(buf == 8) {
            buffer = fin.get();
            if(fin.eof())
                buffer = 0;
            buf = 0;
        }
        //cout << ((buffer >> (7 - buf)) & 1);
        return (buffer >> (7 - buf++)) & 1;
    }

    void writebit(const bool& bit)
    {
        // cout << bit;
        buffer |= bit;
        buf++;
        if(buf == 8) {
            fout.put(buffer);
            buf = buffer = 0;
            return;
        }
        buffer <<= 1;
    }

    void outbit(bool bit)
    {
        writebit(bit);
        while(underflow) {
            writebit(!bit);
            --underflow;
        }
    }


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
    


    void endEncode(void) {
        underflow++;
        if(low < 0x40000000)
            outbit(0);
        else
            outbit(1);
        if(buf)
            fout.put(buffer << (7 - buf));
    }

    int initDecode(){
        buffer = fin.get();
        code = getbit();
        for(int i = 0; i < 31; ++i) {
            code <<= 1;
            code |= getbit();
        }
        return code;
    }

    u64 low, high, code;
    int buf;
    int underflow;
    u8 buffer;
    ifstream fin;
    ofstream fout;
    string str;
};



//get file size
long long getFileSize(const string& filePath)
{
    std::ifstream file(filePath.c_str(), std::ifstream::in | std::ifstream::binary);
    if(!file.is_open())
        return -1;
    file.seekg(0, std::ios::end);
    long long size = file.tellg();
    file.close();
    return size;
}

// to compare if two files are the same
bool isFilesEqual(const std::string& lFilePath, const std::string& rFilePath)
{
    std::ifstream lFile(lFilePath.c_str(), std::ifstream::in | std::ifstream::binary);
    std::ifstream rFile(rFilePath.c_str(), std::ifstream::in | std::ifstream::binary);

    if(!lFile.is_open() || !rFile.is_open())
    {
        return false;
    }

    char *lBuffer = new char[512]();
    char *rBuffer = new char[512]();

    do {
        lFile.read(lBuffer, 512);
        rFile.read(rBuffer, 512);

        if (std::memcmp(lBuffer, rBuffer, 512) != 0)
        {
            delete[] lBuffer;
            delete[] rBuffer;
            return false;
        }
    } while (lFile.good() || rFile.good());

    delete[] lBuffer;
    delete[] rBuffer;
    return true;
}

int main(void) 
{
    string in, out;
    int op = 1;
    while(op)
    {
        cout << "input 1 to compress, 2 to decompress, 0 to exit" << endl;
        cin >> op;
        if(!op)
            break;
        cout << "Please input two files name (inputfile ouputfile separate with space)" << endl;
        cin >> in >> out;
        Ari ari;
        bool ok = false;
        if(op == 1)
        {
            cout << "Compressing...\r" << flush;
            clock_t start = clock();
            ok = ari.encode(in, out);
            clock_t end = clock();
            if(!ok) {
                cout << "Error!" << endl;
                break;
            }
            cout << "Successfully compressed!" << endl;
            cout << "Time consumption: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
            cout << "Original file size: " << getFileSize(in) << " Bytes" << endl;
            cout << "Compressed file size: " << getFileSize(out) << " Bytes" << endl;
            cout << "Compression ratio: " << (double)getFileSize(out) / getFileSize(in) << endl;
        }
        else if(op == 2)
        {
            cout << "Decompressing...\r" << flush;
            clock_t start = clock();
            ok = ari.decode(in, out);
            clock_t end = clock();
            if(!ok) {
                cout << "Error!" << endl;
                break;
            }
            cout << "Successfully decompressed!" << endl;
            cout << "Time consumption: " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
        }
    }
    return 0;
}

