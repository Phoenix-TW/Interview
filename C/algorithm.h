#include "array.h"

void *copy(void *val);
void PrintMCTS(Array * mcts);
void PrintPartMCTS(Array * mcts);

struct MCTS{
    double visit_times;
    double win_times;
    char start;
    char end;
    int parent;
    int child_from;
    int child_end;
    int is_odd;
};

struct SimulateTmp{
    char start;
    char end;
    int score;
};

int selection(Array *mcts,int mcts_index,int *leaf_node_is_odd,char *chess_copy,int *need_to_expand,int *selected_chess_color);

void expansion(Array *mcts,int* selected_index,char *chess_copy,int *leaf_node_is_odd,int need_to_expand,int selected_chess_color);

int simulation(char *chess_copy,int simulate_level,int I_simulate_first,int my_chess_color);

void backpropagation(Array *mcts,int selected_index,int win_or_lose,int is_odd);

bool IsEqual(char *a,char *b,int length);

void Negentropy(char * chess,int my_chess_color);

void *Negentropy_thread_version(void * arg);
