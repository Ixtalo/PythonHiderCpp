/**
 * Run encrypted Python program.
 * 
 * 1. generate data.h using gendatah (see gendatah.cpp!)
 * 2. compile `g++ -o runner runner.cpp AES/AES.o -fPIC -I/usr/include/python3.8 -I/usr/include/python3.8 -Wno-unused-result -Wsign-compare -g -fdebug-prefix-map=/build/python3.8-fKk4GY/python3.8-3.8.2=. -specs=/usr/share/dpkg/no-pie-compile.specs -fstack-protector -Wformat -Werror=format-security -DNDEBUG -g -fwrapv -O3 -Wall -L/usr/lib/python3.8/config-3.8-x86_64-linux-gnu -L/usr/lib -lpython3.8 -lcrypt -lpthread -ldl -lutil -lm -lm`
 *    For Python g++ flags use `python3-config --cflags --ldflags`.
 * 3. run ./runner
 * 
 * Use Docker to compile for a specific Python version:
 *   `docker run --rm -ti -v "$PWD:/x" -w "/x" python:3.6.8 g++ -o runner runner.cpp -fPIC -I/usr/local/include/python3.6m -I/usr/local/include/python3.6m  -Wno-unused-result -Wsign-compare  -DNDEBUG -g -fwrapv -O3 -Wall -L/usr/local/lib -lpython3.6m -lpthread -ldl  -lutil -lm  -Xlinker -export-dynamic`
 * To test that:
 *   `docker run --rm -ti -v "$PWD:/x" -w "/x" python:3.6.8 ./runner`
 * 
 * Ixtalo, 29.07.2020
 * 
 * TODO check: https://mli9502.github.io/2018/09/26/Hide-human-readable-information-in-c-binary/
 */

#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>
#include <stdio.h>
#include <ctype.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

// https://github.com/SergeyBel/AES
#include "AES/AES.h"

// data.h generated by gendata.cpp
#include "data.h"


using namespace std;


/// https://github.com/mafonya/c_hide_strings/blob/master/hide.cc
void c(string& str, ...)
{
	va_list args;
	va_start (args, str);
	char arg = va_arg(args, int);
	while( arg ) {
        str += arg;
		arg = va_arg(args, int);
	}
	va_end (args);
}


int main(int argc, char *argv[])
{
    // use special function to hide the key from the binary
    // the KEY macro is defined in data.h
    string str;
    c(str, KEY);

    // convert string into unsigned char*
    unsigned char *ek = new unsigned char[str.length()+1];
    strncpy((char *)ek, str.c_str(), str.length());

    // decrypt
    AES aes(128);  //128 - key length, can be 128, 192 or 256
    unsigned char *d = aes.DecryptECB(ed, el, ek);  // parameters from data.h

    // Python
    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL) {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }
    Py_SetProgramName(program);  // optional but recommended
    Py_Initialize();
    PyRun_SimpleString(reinterpret_cast<char *>(d));
    if (Py_FinalizeEx() < 0) {
        exit(120);
    }
    PyMem_RawFree(program);
    
    return 0;
}