/*
 * Copyright (c) 2005 Gregor Richards
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LIBORK_H
#define LIBORK_H

#include <string>

using namespace std;

class orknumber {
    public:
    orknumber();
    orknumber *operator=(double arg);
    orknumber *operator=(orknumber &arg);
    double value;
};

class scribe {
    public:
    void ORK_write(string &arg);
    void ORK_write(const char *arg);
    void ORK_write(double arg);
    void ORK_write(orknumber &arg);
    void ORK_asciiWrite(double arg);
    void ORK_asciiWrite(orknumber &arg);
};

/* I need a better name here ... */
class inputter {
    public:
    inputter();
    void ORK_read(string &arg);
    void ORK_readOne(string &arg);
    void ORK_readOne(double arg);
    void ORK_readOne(orknumber &arg);
    void ORK_read(double arg);
    void ORK_read(orknumber &arg);

    int it_s_done;
    int it_s_not_done;
};

class mathematician {
    public:
    void ORK_compare();
    void ORK_add();
    void ORK_subtract();
    void ORK_multiply();
    void ORK_divide();
    void ORK_modulo();
    void ORK_floor(double arg);
    void ORK_floor(orknumber &arg);

    int it_s_less;
    int it_s_greater;
    int it_s_equal;
    orknumber first_operand;
    orknumber second_operand;
    orknumber result;
};

class linguist {
    public:
    void ORK_concatenate();
    void ORK_compare();
    
    string first_operand;
    string second_operand;
    string result;
    int it_s_equal;
    int it_s_not_equal;
};


class word_array {
    public:
    word_array();
    void ORK_instantiate(double arg);
    void ORK_instantiate(orknumber &arg);
    void ORK_get(string &arg);
    void ORK_set(string &arg);
    
    string *elems;
    orknumber current;
    orknumber length;
};

class number_array {
    public:
    number_array();
    void ORK_instantiate(double arg);
    void ORK_instantiate(orknumber &arg);
    void ORK_import(string &arg);
    void ORK_get(double arg);
    void ORK_get(orknumber &arg);
    void ORK_set(double arg);
    void ORK_set(orknumber &arg);
    
    orknumber *elems;
    orknumber current;
    orknumber length;
};

#define DTON(classn, func) \
void classn::func(double arg) \
{ \
    orknumber temp; \
    temp = arg; \
    func(temp); \
}

#endif

// Gregor can't spell ...
#define ORK_instanciate ORK_instantiate
