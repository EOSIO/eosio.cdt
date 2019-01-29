/**
 * Copyright (c) 2012-2014 Luke Dashjr
 * Copyright (c) 2013-2014 Pavol Rusnak
 *
 * Minor modifications by Kedar Iyer (2019)
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stddef.h>
#include <stdbool.h>

namespace eosio {

    void memzero(void * const pnt, const size_t len);

    const char b58digits_ordered[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
    const int8_t b58digits_map[] = {
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1, 0, 1, 2, 3, 4, 5, 6, 7, 8,-1,-1,-1,-1,-1,-1,
        -1, 9,10,11,12,13,14,15,16,-1,17,18,19,20,21,-1,
        22,23,24,25,26,27,28,29,30,31,32,-1,-1,-1,-1,-1,
        -1,33,34,35,36,37,38,39,40,41,42,43,-1,44,45,46,
        47,48,49,50,51,52,53,54,55,56,57,-1,-1,-1,-1,-1,
    };

    bool b58enc(char *b58, size_t *b58sz, const uint8_t *bin, size_t binsz);

    bool b58tobin(void *bin, size_t *binszp, const char *b58);
}
