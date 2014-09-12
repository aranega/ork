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

#include <iostream>

using namespace std;

#include <stdlib.h>

#include <cstring>

#include "libork.h"

orknumber::orknumber()
{
    value = 0.0;
}

orknumber *orknumber::operator=(double arg)
{
    value = arg;
    return this;
}

orknumber *orknumber::operator=(orknumber &arg)
{
    value = arg.value;
    return this;
}

void scribe::ORK_write(string &arg)
{
    cout << arg;
}

void scribe::ORK_write(const char *arg)
{
    cout << arg;
}

DTON(scribe, ORK_write)
void scribe::ORK_write(orknumber &arg)
{
    cout << arg.value;
}

DTON(scribe, ORK_asciiWrite)
void scribe::ORK_asciiWrite(orknumber &arg)
{
    cout << (char) arg.value;
}

inputter::inputter()
{
    it_s_done = 0;
    it_s_not_done = 1;
}

void inputter::ORK_read(string &arg)
{
    if (cin.eof()) {
        it_s_done = 1;
        it_s_not_done = 0;
    } else {
        cin >> arg;
    }
}

void inputter::ORK_readOne(string &arg)
{
    if (cin.eof()) {
        it_s_done = 1;
        it_s_not_done = 0;
    } else {
        char outarg[2];
        cin.get(outarg[0]);
        outarg[1] = '\0';
        arg = outarg;
        if (cin.eof()) {
            it_s_done = 1;
            it_s_not_done = 0;
        }
    }
}

DTON(inputter, ORK_readOne)
void inputter::ORK_readOne(orknumber &arg)
{
    if (cin.eof()) {
        it_s_done = 1;
        it_s_not_done = 0;
    } else {
        char outarg;
        cin.get(outarg);
        arg = outarg;
        if (cin.eof()) {
            it_s_done = 1;
            it_s_not_done = 0;
        }
    }
}

DTON(inputter, ORK_read)
void inputter::ORK_read(orknumber &arg)
{
    if (cin.eof()) {
        it_s_done = 1;
        it_s_not_done = 0;
    } else {
        cin >> arg.value;
    }
}

void mathematician::ORK_compare()
{
    if (first_operand.value < second_operand.value) {
        it_s_less = 1; it_s_greater = 0; it_s_equal = 0;
    }
    if (first_operand.value == second_operand.value) {
        it_s_less = 0; it_s_greater = 0; it_s_equal = 1;
    }
    if (first_operand.value > second_operand.value) {
        it_s_less = 0; it_s_greater = 1; it_s_equal = 0;
    }
}
    
void mathematician::ORK_add()
{
    result = first_operand.value + second_operand.value;
}
    
void mathematician::ORK_subtract()
{
    result = first_operand.value - second_operand.value;
}

void mathematician::ORK_multiply()
{
    result = first_operand.value * second_operand.value;
}
    
void mathematician::ORK_divide()
{
    result = first_operand.value / second_operand.value;
}
    
void mathematician::ORK_modulo()
{
    result = (int) first_operand.value % (int) second_operand.value;
}

DTON(mathematician, ORK_floor)
void mathematician::ORK_floor(orknumber &arg)
{
    arg.value = (double) ((int) arg.value);
}

void linguist::ORK_concatenate()
{
    result = first_operand + second_operand;
}

void linguist::ORK_compare()
{
    if (first_operand == second_operand) {
        it_s_equal = 1;
        it_s_not_equal = 0;
    } else {
        it_s_equal = 0;
        it_s_not_equal = 1;
    }
}


word_array::word_array()
{
    elems = NULL;
}

DTON(word_array, ORK_instantiate)
void word_array::ORK_instantiate(orknumber &arg)
{
    int len = (int) arg.value;
    if (elems) free(elems);
    elems = (string *) malloc(sizeof(string) * len);
    current = 0;
    length = len;
}

void word_array::ORK_get(string &arg)
{
    arg = elems[(int) current.value];
}

void word_array::ORK_set(string &arg)
{
    elems[(int) current.value] = arg;
}

number_array::number_array()
{
    elems = NULL;
}

DTON(number_array, ORK_instantiate)
void number_array::ORK_instantiate(orknumber &arg)
{
    int len = (int) arg.value;
    if (elems) free(elems);
    elems = (orknumber *) malloc(sizeof(orknumber) * len);
    current = 0;
    length = len;
}

void number_array::ORK_import(string &arg)
{
    const char *cptr;
    int len, i;
    
    cptr = arg.c_str();
    len = strlen(cptr) + 1;
    
    if (elems) free(elems);
    elems = (orknumber *) malloc(sizeof(orknumber) * len);
    
    for (i = 0; i < len; i++) {
        elems[i].value = (double) cptr[i];
    }
    
    current = 0;
    length = len - 1;
}

DTON(number_array, ORK_get)
void number_array::ORK_get(orknumber &arg)
{
    arg = elems[(int) current.value];
}

DTON(number_array, ORK_set)
void number_array::ORK_set(orknumber &arg)
{
    elems[(int) current.value] = arg;
}
