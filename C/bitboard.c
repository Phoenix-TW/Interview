#include <immintrin.h>

struct bitboard_status{
    __m256i board;
    __m256i a_block_connect[10];
    __m256i b_block_connect[10];
    __m256i c_block_connect[10];
    __m256i d_block_connect[10];
    short int a;
    short int b;
    short int c;
    short int d;
    __m256i wall;
};

void init_bitboard(struct bitboard_status *bitboard){

    __m256i my_board = _mm256_setr_epi64x(9305070702895824896ULL,524804ULL,0ULL,0ULL);
    __m256i enemy_board = _mm256_setr_epi64x(0ULL,0ULL,4647749999818440704ULL,4328818946ULL);
    bitboard->board = _mm256_or_si256(my_board,enemy_board);

    for(int i=0;i<10;i++){
        bitboard->a_block_connect[i] = _mm256_setzero_si256();
        bitboard->b_block_connect[i] = _mm256_setzero_si256();
        bitboard->c_block_connect[i] = _mm256_setzero_si256();
        bitboard->d_block_connect[i] = _mm256_setzero_si256();
    }

    bitboard->a = 0;
    bitboard->b = 0;
    bitboard->c = 0;
    bitboard->d = 0;

    bitboard->wall = _mm256_setr_epi64x(7825354404200972287ULL,577028689559345458ULL,11040095757133709344ULL,18446726459863675500ULL);
}
