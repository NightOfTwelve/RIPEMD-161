// Andrew Goebel - atggg3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ripemd.h"

#define MAX_LEN 32000 // maximal allowed length of input, may be increased up to 8 megabytes

typedef unsigned char uchar;

void calc_hash(uchar* message, uchar* result);
void print_hash(uchar* hash);
void input_string(uchar* s);

// auxiliary arrays for precalculated amounts of bit positions to shift calculation results
int rhoL[N_ROUNDS][WORDS_PER_BLOCK], rhoR[N_ROUNDS][WORDS_PER_BLOCK]; 

int main(void) {
    uchar* buffer = malloc(MAX_LEN);
    uchar hash[HASH_LEN];

    input_string(buffer);
    calc_hash(buffer, hash);
    print_hash(hash);
    
    free(buffer);
}

void input_string(uchar* s) {
    int i;
    
    fgets(s, MAX_LEN, stdin);
    
    // trim all carriage returns, line feeds and other invisible control characters at the end of string.
    for (i = strlen(s) - 1; i >= 0 && s[i] < ' '; i--) {
        s[i] = 0;
    }

}

// adds padding to message, returns number of blocks
int padding(uchar* message, int len) {
    int i;
    unsigned long bitLen = len * 8L; // length of message in bits
    
    message[len++] = 0x80; // padding has at least one byte
        
    // filling with zeroes upto nearest proper block border minus size of field for length (8 bytes)
    while ((len % BLOCK_LEN) != BLOCK_LEN - 8) {
        message[len++] = 0x00;
    }
    
    // filling length field
    for (i = 0; i < 8; i++) {
        message[len++] = (uchar) (bitLen & 0xFF);
        bitLen >>= 8;
    }
    
    return len / 64;
}

// cyclic rotation of the uint32 to the left, by specified number of bit positions
// e.g. rol(0x80000008, 2) = 0x00000022
uint32 rol(uint32 x, int shift) {
    return (x << shift) | (x >> (32 - shift));
}

// precalculation of values RHO used as amount of bit positions for shifting
// these values are the same for each of blocks so it is good to calculate them before evaluation
void rho_precalc(void) {
    int i, j;
    for (i = 0; i < WORDS_PER_BLOCK; i++) {
        rhoL[0][i] = i;
        rhoR[0][i] = pi[i];
        for (j = 1; j < N_ROUNDS; j++) {
            rhoL[j][i] = rho[rhoL[j - 1][i]];
            rhoR[j][i] = rho[rhoR[j - 1][i]];
        }
    }
}

// calculates hash of "message" formatted as ASCII-Z string
// 20 bytes of result are written to "result" variable
void calc_hash(uchar* message, uchar* result) {
    int len = strlen(message);
    int blocks;
    int i, j, k;
    uint32 abcdeL[WORDS_PER_HASH]; // variables Al, Bl, Cl, Dl, El are placed here
    uint32 abcdeR[WORDS_PER_HASH]; // variables Ar, Br, Cr, Dr, Er are placed here
    uint32 T;
    uint32 words[WORDS_PER_BLOCK]; // words of current block
    
    rho_precalc();
    
    blocks = padding(message, len);
    
    for (k = 0; k < blocks; k++) {
        
        // endianness-independent translation from byte*message to uint32*words
        for (i = 0; i < WORDS_PER_BLOCK; i++) {
            words[i] = 0;
            for (j = 0; j < WORD_SIZE; j++) {
                words[i] |= message[k * BLOCK_LEN + i * WORD_SIZE + j] << (j * 8);
            }
        }
        
        for (j = 0; j < WORDS_PER_HASH; j++) {
            abcdeL[j] = CV[j];
            abcdeR[j] = CV[j];
        }
        
        for (j = 0; j < N_ROUNDS; j++) {
            for (i = 0; i < WORDS_PER_BLOCK; i++) {
                T = abcdeL[0];
                T += f(j + 1, abcdeL[1], abcdeL[2], abcdeL[3]);
                T += words[rhoL[j][i]];
                T += K_l[j];
                T = rol(T, lsh_amt[j][rhoL[j][i]]);
                T += abcdeL[4];
                abcdeL[0] = abcdeL[4];
                abcdeL[4] = abcdeL[3];
                abcdeL[3] = rol(abcdeL[2], 10);
                abcdeL[2] = abcdeL[1];
                abcdeL[1] = T;
                T = abcdeR[0];
                T += f(5 - j, abcdeR[1], abcdeR[2], abcdeR[3]);
                T += words[rhoR[j][i]];
                T += K_r[j];
                T = rol(T, lsh_amt[j][rhoR[j][i]]);
                T += abcdeR[4];
                abcdeR[0] = abcdeR[4];
                abcdeR[4] = abcdeR[3];
                abcdeR[3] = rol(abcdeR[2], 10);
                abcdeR[2] = abcdeR[1];
                abcdeR[1] = T;
            }
        }
        
        T = CV[1] + abcdeL[2] + abcdeR[3];
        for (i = 1; i < WORDS_PER_HASH; i++) {
            CV[i] = CV[(i + 1) % WORDS_PER_HASH] + abcdeL[(i + 2) % WORDS_PER_HASH] + abcdeR[(i + 3) % WORDS_PER_HASH];
        }
        CV[0] = T;
    }
    
    // endianness-independent translation from uint32*hash to uchar*result
    for (i = 0; i < WORDS_PER_HASH; i++) {
        for (j = 0; j < WORD_SIZE; j++) {
            result[i * WORD_SIZE + j] = (uchar) (CV[i] & 0xFF);
            CV[i] >>= 8;
        }
    }
}

void print_hash(uchar* hash) {
    int i;
    for (i = 0; i < HASH_LEN; i++) {
        printf("%02X", hash[i]);
    }
    printf("\n");
}

