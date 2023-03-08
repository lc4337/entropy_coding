#include <iostream>
#include <fstream>
#include <string>
#include "huffman.h"
#include "ari.h" 

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        cerr << "Usage: " << argv[0] << " [-c|-d] input_file output_file" << endl;
        return 1;
    }

    string input_file = argv[2];
    string output_file = argv[3];

    Ari ari;

    if (strcmp(argv[1], "-c") == 0)
    {
        // compress data

        cout << "Compressing...\r" << flush;
        if (!ari.encode(input_file, output_file))
        {
            cerr << "Failed to compress data" << endl;
            return 1;
        }
        cout << "Successfully compressed!" << endl;

        // output details information
        // cout << "Input file size: " << compressor.getInputSize() << endl;
        // cout << "Output file size: " << compressor.getOutputSize() << endl;
        // cout << "Compression ratio: " << compressor.getCompressionRatio() << endl;
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        // decompress data

        cout << "Decompressing...\r" << flush;
        if (!ari.decode(input_file, output_file))
        {
            cerr << "Failed to decompress data" << endl;
            return 1;
        }
        cout << "Successfully decompressed!" << endl;

        // output details information
        // cout << "Input file size: " << decompressor.getInputSize() << endl;
        // cout << "Output file size: " << decompressor.getOutputSize() << endl;
        // cout << "Decompression ratio: " << decompressor.getDecompressionRatio() << endl;
    }
    else
    {
        cerr << "Unknown option: " << argv[1] << endl;
        return 1;
    }

    return 0;
}
