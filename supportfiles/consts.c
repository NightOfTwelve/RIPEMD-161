#include "ripemd.h"

int rho[16] = {7, 4, 13, 1, 10, 6, 15, 3, 12, 0, 9, 5, 2, 14, 11, 8};
int pi[16]  = {5, 14, 7, 0, 9, 2, 11, 4, 13, 6, 15, 8, 1, 10, 3, 12};
int lsh_amt[5][16] = {
/*rnd 1*/       {11, 14, 15, 12, 5, 8, 7, 9, 11, 13, 14, 15, 6, 7, 9, 8},
/*rnd 2*/       {12, 13, 11, 15, 6, 9, 9, 7, 12, 15, 11, 13, 7, 8, 7, 7},
/*rnd 3*/       {13, 15, 14, 11, 7, 7, 6, 8, 13, 14, 13, 12, 5, 5, 6, 9},
/*rnd 4*/       {14, 11, 12, 14, 8, 6, 5, 5, 15, 12, 15, 14, 9, 9, 8, 6},
/*rnd 5*/       {15, 12, 13, 13, 9, 5, 8, 6, 14, 11, 12, 11, 8, 6, 5, 5}
        };
uint32 K_l[5] = {0x0, 0x5a827999, 0x6ed9eba1, 0x8f1bbcdc, 0xa953fd4e},
                 K_r[5] = {0x50a28be6, 0x5c4dd124, 0x6d703ef3, 0x7a6d76e9, 0x0};
uint32 CV[5] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0};

uint32 f (int rnd, uint32 B, uint32 C, uint32 D)
{
        switch (rnd) {
                case 1: return B^C^D;
                case 2: return (B & C) | (~B & D);
                case 3: return (B | ~C) ^ D;
                case 4: return (B & D) | (C & ~D);
                case 5: return B ^ (C | ~D);
                }
}