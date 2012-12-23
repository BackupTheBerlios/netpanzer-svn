/*
Copyright (C) 2012 by Aaron Perez <aaronps@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "WebSocketUtil.hpp"

#include "Core/CoreTypes.hpp"

static const char b2h[17] = "0123456789abcdef";
static const char b64[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const uint32_t round_1[10] =
{
    0x32353845, 0x41464135, 0x2d453931, 0x342d3437, 0x44412d39, 0x3543412d,
    0x43354142, 0x30444338, 0x35423131, 0x80000000
};

static const uint32_t round_2[80] = 
{
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x000001e0, 0x00000000, 0x00000000,
    0x000003c0, 0x00000000, 0x00000000, 0x00000780, 0x00000000, 0x000003c0,
    0x00000f00, 0x00000000, 0x00000000, 0x00001e00, 0x00000000, 0x00000cc0,
    0x00003c00, 0x00000440, 0x00000000, 0x00007800, 0x00000f00, 0x00003300,
    0x0000f000, 0x00000f00, 0x00000000, 0x0001ef00, 0x00000000, 0x0000cc00,
    0x0003c000, 0x00004380, 0x00000000, 0x00078f00, 0x0000ff00, 0x00032680,
    0x000f0000, 0x0000f000, 0x00003300, 0x001eff00, 0x00000000, 0x000cb800,
    0x003c0000, 0x00040b00, 0x0000f000, 0x0078ff00, 0x000ff000, 0x00338700,
    0x00f00000, 0x000fc300, 0x0000f000, 0x01efbb00, 0x00000000, 0x00cc0000,
    0x03c0f000, 0x00438000, 0x00000000, 0x078f0000, 0x00ff0000, 0x03269e00,
    0x0f000000, 0x00f0f000, 0x00333c00, 0x1eff8800, 0x00000000, 0x0cb88800,
    0x3c00f000, 0x040a4a00, 
};
static inline uint32_t rotate_1(uint32_t value)
{
    return (value << 1) | (value >> 31);
}

static inline uint32_t rotate_5(uint32_t value)
{
    return (value << 5) | (value >> 27);
}

static inline uint32_t rotate_30(uint32_t value)
{
    return (value << 30) | (value >> 2);
}
void WebSocket_HandShake(const char* data, char * out)
{
    uint32_t W[80]
            ,tmp
            ,A, FA = A = 0x67452301
            ,B, FB = B = 0xEFCDAB89
            ,C, FC = C = 0x98BADCFE
            ,D, FD = D = 0x10325476
            ,E, FE = E = 0xC3D2E1F0 ;

    for ( int n = 0; n < 6; n++ )
    {
        tmp  = *(data++) << 24;
        tmp |= *(data++) << 16;
        tmp |= *(data++) <<  8;
        tmp |= *(data++);
        W[n] = tmp;
    }

    memcpy(&W[6], round_1, sizeof(round_1));

    for ( int n = 16; n < 80; n++ )
    {
       W[n] = rotate_1(W[n-3] ^ W[n-8] ^ W[n-14] ^ W[n-16]);
    }

    const uint32_t * p = W;

#define BASE(F,K) tmp =  rotate_5(A) + (F) + E + (*(p++)) + (K); E=D; D=C; C=rotate_30(B); B=A; A=tmp;
#define G BASE((((C^D)&B)^D),                 0x5A827999)
#define R BASE((B ^ C ^ D),                   0x6ED9EBA1)
#define O BASE(((B & C) | (B & D) | (C & D)), 0x8F1BBCDC)
#define K BASE((B ^ C ^ D),                   0xCA62C1D6)
    G G G G G G G G G G G G G G G G G G G G
    R R R R R R R R R R R R R R R R R R R R
    O O O O O O O O O O O O O O O O O O O O
    K K K K K K K K K K K K K K K K K K K K

    A = FA += A;
    B = FB += B;
    C = FC += C;
    D = FD += D;
    E = FE += E;

    p = round_2;

    G G G G G G G G G G G G G G G G G G G G
    R R R R R R R R R R R R R R R R R R R R
    O O O O O O O O O O O O O O O O O O O O
    K K K K K K K K K K K K K K K K K K K K
#undef K
#undef O
#undef R
#undef G
#undef BASE

    FA += A;
    FB += B;
    FC += C;
    FD += D;
    FE += E;

    *(out++) = b64[FA >> 26];
    *(out++) = b64[(FA >> 20) & 0x3f];
    *(out++) = b64[(FA >> 14) & 0x3f];
    *(out++) = b64[(FA >>  8) & 0x3f];
    *(out++) = b64[(FA >>  2) & 0x3f];

    *(out++) = b64[((FA&3) << 4)|((FB>>28)&0x0f)];

    *(out++) = b64[(FB >> 22) & 0x3f];
    *(out++) = b64[(FB >> 16) & 0x3f];
    *(out++) = b64[(FB >> 10) & 0x3f];
    *(out++) = b64[(FB >>  4) & 0x3f];

    *(out++) = b64[((FB&0xf) << 2)|((FC>>30)&0x03)];

    *(out++) = b64[(FC >> 24) & 0x3f];
    *(out++) = b64[(FC >> 18) & 0x3f];
    *(out++) = b64[(FC >> 12) & 0x3f];
    *(out++) = b64[(FC >>  6) & 0x3f];
    *(out++) = b64[FC & 0x3f];

    *(out++) = b64[FD >> 26];
    *(out++) = b64[(FD >> 20) & 0x3f];
    *(out++) = b64[(FD >> 14) & 0x3f];
    *(out++) = b64[(FD >>  8) & 0x3f];
    *(out++) = b64[(FD >>  2) & 0x3f];

    *(out++) = b64[((FD&3) << 4)|((FE>>28)&0x0f)];

    *(out++) = b64[(FE >> 22) & 0x3f];
    *(out++) = b64[(FE >> 16) & 0x3f];
    *(out++) = b64[(FE >> 10) & 0x3f];
    *(out++) = b64[(FE >>  4) & 0x3f];
    *(out++) = b64[(FE <<  2) & 0x3f];

    *(out++) = '=';

}
