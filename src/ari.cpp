#include <bits/stdc++.h>
#include "ari.h"

using namespace std;
void Ari::encodeSym(probModel &model, int sym) {
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

int Ari::decodeSym(probModel &model) {
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

bool Ari::encode(const string& in, const string& out)
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

bool Ari::decode(const string& in, const string& out)
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

bool Ari::getbit()
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

void Ari::writebit(const bool& bit)
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

void Ari::outbit(bool bit)
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



void Ari::endEncode(void) {
    underflow++;
    if(low < 0x40000000)
        outbit(0);
    else
        outbit(1);
    if(buf)
        fout.put(buffer << (7 - buf));
}

int Ari::initDecode(){
    buffer = fin.get();
    code = getbit();
    for(int i = 0; i < 31; ++i) {
        code <<= 1;
        code |= getbit();
    }
    return code;
}




/*
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

*/
