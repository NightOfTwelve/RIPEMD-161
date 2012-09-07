typedef unsigned long uint32;  // "unsigned int" could appear 16-bit on some older compilers

#define BLOCK_LEN 64 // length of each block for processing in bytes
#define HASH_LEN 20 // length of hash in bytes
#define WORDS_PER_BLOCK (BLOCK_LEN / sizeof(uint32))
#define WORDS_PER_HASH (HASH_LEN / sizeof(uint32))
#define N_ROUNDS 5 // it equals to WORDS_PER_HASH, but it is a mere coincidence!
#define WORD_SIZE (sizeof(uint32))

extern int rho[WORDS_PER_BLOCK], pi[WORDS_PER_BLOCK], lsh_amt[N_ROUNDS][WORDS_PER_BLOCK];
extern uint32 K_l[WORDS_PER_HASH], K_r[WORDS_PER_HASH], CV[WORDS_PER_HASH];

uint32 f (int rnd, uint32 B, uint32 C, uint32 D);
