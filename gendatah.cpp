/**
 * Generate header file data.h with encrypted Python program.
 * 
 * 1. g++ -c AES/AES.cpp
 * 2. g++ -o gendatah gendatah.cpp AES/AES.o 
 * 3. ./gendatah <key> <file.py>
 * 
 * The resulting data.h is then to be included by runner.cpp
 * 
 * Ixtalo, 29.07.2020
 */
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <ctime>

// https://github.com/SergeyBel/AES
#include "AES/AES.h"

using namespace std;

int main(int argc, char *argv[])
{

    if (argc != 3) {
        cerr << "usage: ... <key> <file.py>" << endl;
        return 1;
    }

    size_t keyLen = strlen(argv[1]);
    if (keyLen != 16) {
        cerr << "key must be size 16! " << "(is " << keyLen << ")" << endl;;
        return 2;
    }

    // read file into string
    string content;
    try
    {
        ifstream fin(argv[2]);
        if (!fin.good()) {
            throw std::runtime_error("Could not open file!");
        }
        content = string((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
        fin.close();
    }
    catch(const exception& e)
    {
        cerr << "ERROR: " << e.what() << endl;
        return 3;
    }

    if (content.empty()) {
        cerr << "File is empty, no data!" << endl;
        return 4;
    }

    cout << "Size input file '" << argv[2] << "': " << content.length() << endl;

    // convert string into unsigned char*
    unsigned char *plain = new unsigned char[content.length()+1];
    strncpy((char *)plain, content.c_str(), content.length());

    // convert char* to unsigned char*
    unsigned char* key = reinterpret_cast<unsigned char *>(argv[1]);
    
    unsigned int plainLen = content.length();  //bytes in plaintext
    unsigned int encLen = 0;  // out param - bytes in сiphertext

    // encryption
    AES aes(128);  //128 - key length, can be 128, 192 or 256
    unsigned char *c = aes.EncryptECB(plain, plainLen, key, encLen);

    cout << "plainLen: " << plainLen << endl;
    cout << "encLen: " << encLen << endl;

    // write to header file
    ofstream fout("data.h");
    fout << "// input file: " << argv[2] << endl;
    fout << "// key: " << argv[1] << endl;
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    fout << "// time: " << std::put_time(&tm, "%FT%TZ") << endl;
    fout << endl;
    // write key, must be a macro!
    fout << "#define KEY ";
    for (size_t i = 0; i < 16-1; i++) {
        fout << "'" << key[i] << "',";
    }
    fout << "'" << key[16-1] << "'" << endl;
    // write length of encrypted data
    fout << "unsigned int el = " << encLen << ";" << endl;
    // write encrypted data as hex
    fout << "unsigned char ed[] = {";
    for (size_t i = 0; i < encLen-1; i++) {
        fout << "0x" << hex << int(c[i]);
        fout << ", ";
    }
    fout << "0x" << hex << int(c[encLen-1]) << "};" << endl;
    fout.close();

    cout << "data.h written." << endl;

    return 0;
}