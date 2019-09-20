/*
Copyright (c) 2018 Amir Hossein Alikhah Mishamandani
Copyright (c) 2018 Algorand LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <string.h>

#include "common.h"

/*
 h = 0
 */

static inline void fe25519_0(fe25519 h)
{
    memset(&h[0], 0, 10 * sizeof h[0]);
}

/*
 h = 1
 */

static inline void fe25519_1(fe25519 h)
{
    h[0] = 1;
    h[1] = 0;
    memset(&h[2], 0, 8 * sizeof h[0]);
}

/*
 h = f + g
 Can overlap h with f or g.
 *
 Preconditions:
 |f| bounded by 1.1*2^25,1.1*2^24,1.1*2^25,1.1*2^24,etc.
 |g| bounded by 1.1*2^25,1.1*2^24,1.1*2^25,1.1*2^24,etc.
 *
 Postconditions:
 |h| bounded by 1.1*2^26,1.1*2^25,1.1*2^26,1.1*2^25,etc.
 */

static inline void
fe25519_add(fe25519 h, const fe25519 f, const fe25519 g)
{
    int32_t h0 = f[0] + g[0];
    int32_t h1 = f[1] + g[1];
    int32_t h2 = f[2] + g[2];
    int32_t h3 = f[3] + g[3];
    int32_t h4 = f[4] + g[4];
    int32_t h5 = f[5] + g[5];
    int32_t h6 = f[6] + g[6];
    int32_t h7 = f[7] + g[7];
    int32_t h8 = f[8] + g[8];
    int32_t h9 = f[9] + g[9];

    h[0] = h0;
    h[1] = h1;
    h[2] = h2;
    h[3] = h3;
    h[4] = h4;
    h[5] = h5;
    h[6] = h6;
    h[7] = h7;
    h[8] = h8;
    h[9] = h9;
}

/*
 h = f - g
 Can overlap h with f or g.
 *
 Preconditions:
 |f| bounded by 1.1*2^25,1.1*2^24,1.1*2^25,1.1*2^24,etc.
 |g| bounded by 1.1*2^25,1.1*2^24,1.1*2^25,1.1*2^24,etc.
 *
 Postconditions:
 |h| bounded by 1.1*2^26,1.1*2^25,1.1*2^26,1.1*2^25,etc.
 */

static void fe25519_sub(fe25519 h, const fe25519 f, const fe25519 g)
{
    int32_t h0 = f[0] - g[0];
    int32_t h1 = f[1] - g[1];
    int32_t h2 = f[2] - g[2];
    int32_t h3 = f[3] - g[3];
    int32_t h4 = f[4] - g[4];
    int32_t h5 = f[5] - g[5];
    int32_t h6 = f[6] - g[6];
    int32_t h7 = f[7] - g[7];
    int32_t h8 = f[8] - g[8];
    int32_t h9 = f[9] - g[9];

    h[0] = h0;
    h[1] = h1;
    h[2] = h2;
    h[3] = h3;
    h[4] = h4;
    h[5] = h5;
    h[6] = h6;
    h[7] = h7;
    h[8] = h8;
    h[9] = h9;
}

/*
 h = -f
 *
 Preconditions:
 |f| bounded by 1.1*2^25,1.1*2^24,1.1*2^25,1.1*2^24,etc.
 *
 Postconditions:
 |h| bounded by 1.1*2^25,1.1*2^24,1.1*2^25,1.1*2^24,etc.
 */

static inline void fe25519_neg(fe25519 h, const fe25519 f)
{
    int32_t h0 = -f[0];
    int32_t h1 = -f[1];
    int32_t h2 = -f[2];
    int32_t h3 = -f[3];
    int32_t h4 = -f[4];
    int32_t h5 = -f[5];
    int32_t h6 = -f[6];
    int32_t h7 = -f[7];
    int32_t h8 = -f[8];
    int32_t h9 = -f[9];

    h[0] = h0;
    h[1] = h1;
    h[2] = h2;
    h[3] = h3;
    h[4] = h4;
    h[5] = h5;
    h[6] = h6;
    h[7] = h7;
    h[8] = h8;
    h[9] = h9;
}

/*
 Replace (f,g) with (g,g) if b == 1;
 replace (f,g) with (f,g) if b == 0.
 *
 Preconditions: b in {0,1}.
 */

static void fe25519_cmov(fe25519 f, const fe25519 g, unsigned int b)
{
    const uint32_t mask = (uint32_t) (-(int32_t) b);

    int32_t f0 = f[0];
    int32_t f1 = f[1];
    int32_t f2 = f[2];
    int32_t f3 = f[3];
    int32_t f4 = f[4];
    int32_t f5 = f[5];
    int32_t f6 = f[6];
    int32_t f7 = f[7];
    int32_t f8 = f[8];
    int32_t f9 = f[9];

    int32_t x0 = f0 ^ g[0];
    int32_t x1 = f1 ^ g[1];
    int32_t x2 = f2 ^ g[2];
    int32_t x3 = f3 ^ g[3];
    int32_t x4 = f4 ^ g[4];
    int32_t x5 = f5 ^ g[5];
    int32_t x6 = f6 ^ g[6];
    int32_t x7 = f7 ^ g[7];
    int32_t x8 = f8 ^ g[8];
    int32_t x9 = f9 ^ g[9];

    x0 &= mask;
    x1 &= mask;
    x2 &= mask;
    x3 &= mask;
    x4 &= mask;
    x5 &= mask;
    x6 &= mask;
    x7 &= mask;
    x8 &= mask;
    x9 &= mask;

    f[0] = f0 ^ x0;
    f[1] = f1 ^ x1;
    f[2] = f2 ^ x2;
    f[3] = f3 ^ x3;
    f[4] = f4 ^ x4;
    f[5] = f5 ^ x5;
    f[6] = f6 ^ x6;
    f[7] = f7 ^ x7;
    f[8] = f8 ^ x8;
    f[9] = f9 ^ x9;
}

/*
 h = f
 */

static inline void fe25519_copy(fe25519 h, const fe25519 f)
{
    int32_t f0 = f[0];
    int32_t f1 = f[1];
    int32_t f2 = f[2];
    int32_t f3 = f[3];
    int32_t f4 = f[4];
    int32_t f5 = f[5];
    int32_t f6 = f[6];
    int32_t f7 = f[7];
    int32_t f8 = f[8];
    int32_t f9 = f[9];

    h[0] = f0;
    h[1] = f1;
    h[2] = f2;
    h[3] = f3;
    h[4] = f4;
    h[5] = f5;
    h[6] = f6;
    h[7] = f7;
    h[8] = f8;
    h[9] = f9;
}

/*
 return 1 if f is in {1,3,5,...,q-2}
 return 0 if f is in {0,2,4,...,q-1}

 Preconditions:
 |f| bounded by 1.1*2^26,1.1*2^25,1.1*2^26,1.1*2^25,etc.
 */

static inline int fe25519_isnegative(const fe25519 f)
{
    unsigned char s[32];

    fe25519_tobytes(s, f);

    return s[0] & 1;
}

/*
 return 1 if f == 0
 return 0 if f != 0

 Preconditions:
 |f| bounded by 1.1*2^26,1.1*2^25,1.1*2^26,1.1*2^25,etc.
 */


static inline int local_is_zero(const unsigned char *n, const size_t nlen)
{
    size_t                 i;
    volatile unsigned char d = 0U;

    for (i = 0U; i < nlen; i++) {
        d |= n[i];
    }
    return 1 & ((d - 1) >> 8);
} 
 
static inline int fe25519_iszero(const fe25519 f)
{
    unsigned char s[32];

    fe25519_tobytes(s, f);

    return local_is_zero(s, 32);
}
