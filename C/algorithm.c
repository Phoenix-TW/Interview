#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "utility.h"
#include "algorithm.h"

void *copy(void *val)
{
    char *N = (char*)malloc(sizeof(char));
    *N = *((char*)val);
    return N;
}

void PrintMCTS(Array * mcts){

    struct MCTS *node;
    for(int j=0;j<array_size(mcts);j++){
        array_get_at(mcts, j,(void*)&node);

        printf("index vtimes start end parent cfrom cend odd wtimes\n");
        printf("%5d %6.2f %4d %4d %6d %5d %4d %3d %6.2f\n",j,node->visit_times,node->start,node->end,node->parent,\
               node->child_from,node->child_end,node->is_odd,node->win_times);
    }
}

void PrintPartMCTS(Array * mcts){

    struct MCTS *node;

    array_get_at(mcts, 0,(void*)&node);

    int child_from = node->child_from;
    int child_end = node->child_end;

    for(int j=child_from;j<=child_end;j++){
        array_get_at(mcts, j,(void*)&node);

        printf("index vtimes start end parent cfrom cend odd wtimes\n");
        printf("%5d %6.2f %4d %4d %6d %5d %4d %3d %6.2f\n",j,node->visit_times,node->start,node->end,node->parent,\
               node->child_from,node->child_end,node->is_odd,node->win_times);
    }
}

int selection(Array *mcts,int mcts_index,int *leaf_node_is_odd,char *chess_copy,int *need_to_expand,int *selected_chess_color){

    struct MCTS *node;

    array_get_at(mcts, mcts_index,(void*)&node);
    int start = node->child_from;
    int end = node->child_end;
    double parent_visit_times = node->visit_times;

    if(array_size(mcts)==1){
        //printf("一開始只有根節點\n");
        *need_to_expand = 1;
        return mcts_index;
    }


    if(node->child_from == -1 && node->child_end == -1 && node->visit_times != 0){
        //printf("第%d個節點的父節點之前是葉節點然後展開第%d個節點，並且這次第%d個節點為葉節點\n",mcts_index,mcts_index,mcts_index);
        *need_to_expand = 1;
        return mcts_index;
    }

    int biggest_ucb_index = -1;
    double biggest_ucb = 0;
    double ucb_temp = 0;

    if(*leaf_node_is_odd){
        for(int j=start;j<=end;j++){
            array_get_at(mcts, j,(void*)&node);

            if(node->visit_times == 0){ //為了寫code方便先new出來的點，實際上還並沒有這個點，所以現在不new，直接進expansion
                //printf("第%d個節點之前被展開但沒有拜訪過的節點，這次拜訪它\n",j);
                *need_to_expand = 0;
                return mcts_index;
            }

            ucb_temp = (node->win_times/node->visit_times) + (double)sqrt(2*log(parent_visit_times)/node->visit_times);
//printf("index=%d,ucb:%f\n",j,ucb_temp);
            if(biggest_ucb < ucb_temp){
                biggest_ucb_index = j;
                biggest_ucb = ucb_temp;
            }
        }
        *leaf_node_is_odd = 0;
    }
    else{
        for(int j=start;j<=end;j++){
            array_get_at(mcts, j,(void*)&node);

            if(node->visit_times == 0){ //為了寫code方便先new出來的點，實際上還並沒有這個點，所以現在不new，直接進expansion
                //printf("第%d個節點之前被展開但沒有拜訪過的節點，這次拜訪它\n",j);
                *need_to_expand = 0;
                return mcts_index;
            }

            ucb_temp = (1-(node->win_times/node->visit_times)) + (double)sqrt(2*log(parent_visit_times)/node->visit_times);
//printf("index=%d,ucb:%f\n",j,ucb_temp);
            if(biggest_ucb < ucb_temp){
                biggest_ucb_index = j;
                biggest_ucb = ucb_temp;
            }
        }
        *leaf_node_is_odd = 1;
    }


    array_get_at(mcts, biggest_ucb_index,(void*)&node);
    chess_copy[locatetransform(node->end)] = *selected_chess_color;
    chess_copy[locatetransform(node->start)] = 0;

    if(*selected_chess_color ==1)
        *selected_chess_color = 2;
    else
        *selected_chess_color = 1;

//printf("in selection!!\n");
    biggest_ucb_index = selection(mcts,biggest_ucb_index,leaf_node_is_odd,chess_copy,need_to_expand,selected_chess_color);
//printf("out selection!!\n");

    return biggest_ucb_index;
}

void expansion(Array *mcts,int* selected_index,char *chess_copy,int *leaf_node_is_odd,int need_to_expand,int selected_chess_color){


    struct MCTS *node;
    int expand_chess_color;

    if(selected_chess_color==1)
        expand_chess_color=1;
    else
        expand_chess_color=2;
    
    if(!need_to_expand){        //不需要expansion的話這個步驟直接改變盤面走下去即可結束expansion階段
        array_get_at(mcts, *selected_index,(void*)&node);
        int start = node->child_from;
        int end = node->child_end;

        for(int k=start;k<=end;k++){
            array_get_at(mcts, k,(void*)&node);
            if(node->visit_times == 0){
                chess_copy[locatetransform(node->end)] = expand_chess_color;
                chess_copy[locatetransform(node->start)] = 0;
                *selected_index = k;
                if(*leaf_node_is_odd ==1)
                    *leaf_node_is_odd = 0;
                else
                    *leaf_node_is_odd = 1;
                return;
            }
        }
    }
    else{   //expansion節點時，原本一次只展一個點，為了寫code方便，所以直接展完該葉節點下面所有點，但是實際上並沒有要拜訪的點的visit次數設為0
        int mychess[10];
        int j=0;
        Array *ends;
        char *element;
        for(int i =0;i<256;i++){
            if(chess_copy[i] == expand_chess_color){
                mychess[j] = (int)locatetransformrevers(i);
                j++;
            }
        }

        int child_from = array_size(mcts);
        int child_end;
        if(*leaf_node_is_odd ==1)
            *leaf_node_is_odd = 0;
        else
            *leaf_node_is_odd = 1;

        for(int i=0;i<10;i++){

            ends = ReturnPartMoves(chess_copy,mychess[i],false);

            for(int j=0;j<array_size(ends);j++){
                array_get_at(ends, j,(void*)&element);
                node = (struct MCTS *)malloc(sizeof(struct MCTS));
                node->end = *element;
                node->start = mychess[i];
                node->visit_times = 0;
                node->win_times = 0;
                node->child_end = -1;
                node->child_from = -1;
                node->parent = *selected_index;
                node->is_odd = *leaf_node_is_odd;
                array_add(mcts,(void*)node);
            }

            array_remove_all_free(ends);
            array_destroy(ends);
        }

        child_end = array_size(mcts)-1;
        array_remove_at(mcts, *selected_index,(void*)&node);
        node->child_from = child_from;
        node->child_end = child_end;
        array_add_at(mcts,(void*)node,*selected_index);

        *selected_index = array_size(mcts)-1;

        array_get_at(mcts,*selected_index,(void*)&node);

        chess_copy[locatetransform(node->end)] = expand_chess_color;
        chess_copy[locatetransform(node->start)] = 0;
    }
}

int simulation(char *chess_copy,int simulate_level,int I_simulate_first,int my_chess_color){    //奇數層往下展表示當前是我方的回合
                                                                                                //I_simulate_first = current_node_is_odd

    int mychess[10];
    int enemychess[10];

    int enemy_chess_color;
    char * my_chess_score;
    char * enemy_chess_score;

    if(my_chess_color==1){
        my_chess_score = ScoreINIT(true);
        enemy_chess_score = ScoreINIT(false);
        enemy_chess_color = 2;
    }
    if(my_chess_color==2){
        my_chess_score = ScoreINIT(false);
        enemy_chess_score = ScoreINIT(true);
        enemy_chess_color = 1;
    }

    int j=0,k=0;
    for(int i =0;i<256;i++){
        if(chess_copy[i] == my_chess_color){
            mychess[j] = (int)locatetransformrevers(i);
            j++;
        }
        if(chess_copy[i] == enemy_chess_color){
            enemychess[k] = (int)locatetransformrevers(i);
            k++;
        }
    }

    char * element;
    Array *ends;
    Array *array_tmp;
    int score_tmp;
    int my_score_increase = 0;
    int enemy_score_increase = 0;
    int move_index;

    srand(time(NULL));

    struct SimulateTmp * simulate_tmp;

    for(int k=0;k<simulate_level;k++){

//printf("simulate turn:%d\n",k);

        if(I_simulate_first){
            array_new(&array_tmp);
            for(int i =0;i<10;i++){
                ends = ReturnPartMoves(chess_copy,mychess[i],true);
                for(int j=0;j<array_size(ends);j++){
                    array_get_at(ends, j,(void*)&element);
                    score_tmp = ScoreCaculate(my_chess_score,mychess[i],*element);
                    if(score_tmp>0){
                        simulate_tmp = (struct SimulateTmp *)malloc(sizeof(struct SimulateTmp));
                        simulate_tmp->start = mychess[i];
                        simulate_tmp->end = *element;
                        simulate_tmp->score = score_tmp;
                        array_add(array_tmp,(void*)simulate_tmp);
                    }
                }
                array_remove_all_free(ends);
                array_destroy(ends);
            }

            move_index = rand()%array_size(array_tmp);
            array_get_at(array_tmp, move_index,(void*)&simulate_tmp);
            chess_copy[locatetransform(simulate_tmp->start)] = 0;
            chess_copy[locatetransform(simulate_tmp->end)] = my_chess_color;
//printf("select_move: %d -> %d\n",simulate_tmp->start,simulate_tmp->end);

            my_score_increase = my_score_increase + simulate_tmp->score;


//printf("This step's score:%d ; My score:%d\n",simulate_tmp->score,my_score_increase);

            for(int i=0;i<10;i++){
                if(mychess[i]==simulate_tmp->start){
                    mychess[i]=simulate_tmp->end;
                    break;
                }
            }

            //free(simulate_tmp);
            array_remove_all_free(array_tmp);
            array_destroy(array_tmp);
            I_simulate_first = 0;
        }
        else{
            array_new(&array_tmp);
            for(int i =0;i<10;i++){
                ends = ReturnPartMoves(chess_copy,enemychess[i],true);
                for(int j=0;j<array_size(ends);j++){
                    array_get_at(ends, j,(void*)&element);
                    score_tmp = ScoreCaculate(enemy_chess_score,enemychess[i],*element);
                    if(score_tmp>0){
                        struct SimulateTmp * simulate_tmp = (struct SimulateTmp *)malloc(sizeof(struct SimulateTmp));
                        simulate_tmp->start = enemychess[i];
                        simulate_tmp->end = *element;
                        simulate_tmp->score = score_tmp;
                        array_add(array_tmp,(void*)simulate_tmp);
                    }
                }
                array_remove_all_free(ends);
                array_destroy(ends);
            }

            move_index = rand()%array_size(array_tmp);
            array_get_at(array_tmp, move_index,(void*)&simulate_tmp);
            chess_copy[locatetransform(simulate_tmp->start)] = 0;
            chess_copy[locatetransform(simulate_tmp->end)] = enemy_chess_color;
//printf("select_move: %d -> %d\n",simulate_tmp->start,simulate_tmp->end);

            enemy_score_increase = enemy_score_increase + simulate_tmp->score;

//printf("This step's score:%d ; Enemy score:%d\n",simulate_tmp->score,enemy_score_increase);

            for(int i=0;i<10;i++){
                if(enemychess[i]==simulate_tmp->start){
                    enemychess[i]=simulate_tmp->end;
                    break;
                }
            }

            //free(simulate_tmp);
            array_remove_all_free(array_tmp);
            array_destroy(array_tmp);
            I_simulate_first = 1;
        }
//printf("\n\n");
//printfieldwithoutcls(chess_copy,108);
    }

//system("PAUSE");

    free(my_chess_score);
    free(enemy_chess_score);

    if(my_score_increase-enemy_score_increase>1)
        return 1;
    else
        return 0;
}

void backpropagation(Array *mcts,int selected_index,int win_or_lose,int is_odd){

//PrintMCTS(mcts);

    struct MCTS *node;
    struct MCTS *cur_node;
    array_remove_at(mcts, selected_index,(void*)&cur_node);

//printf("Now at index:%d is_odd:%d\n",selected_index,is_odd);

//system("PAUSE");

    if((int)(cur_node->visit_times)==0){        //向上更新時，若是第一次拜訪，就不需要與其他點比較，直接更新數據(+1)
        cur_node->win_times = cur_node->win_times + win_or_lose;
        cur_node->visit_times = cur_node->visit_times+1;
        array_add_at(mcts,(void*)cur_node,selected_index);
        if(is_odd==1)
            is_odd = 0;
        else
            is_odd = 1;
    }
    else{                                       //向上更新時，若不是第一次拜訪，就要根據該層是奇數層或偶數層來更新
        if(is_odd){                             //若要更新的節點在奇數層表示該回合是我方下，應選最佳解(我方贏率最大值)
            double biggest_win_rate =0;
            double tmp_win_rate;
            double all_visit_times_in_this_level = 0;

            array_add_at(mcts,(void*)cur_node,selected_index);

            for(int i=cur_node->child_from;i<=cur_node->child_end;i++){
                array_get_at(mcts, i,(void*)&node);
                if((int)node->visit_times!=0){
                    all_visit_times_in_this_level = all_visit_times_in_this_level + node->visit_times;
//printf("add index%d's visit times = %f\n",i,node->visit_times);
                    tmp_win_rate = node->win_times/node->visit_times;
                    if(tmp_win_rate > biggest_win_rate)
                        biggest_win_rate = tmp_win_rate;
                }
            }

            array_remove_at(mcts, selected_index,(void*)&cur_node);

            cur_node->visit_times = all_visit_times_in_this_level;
            cur_node->win_times = biggest_win_rate * all_visit_times_in_this_level;
//printf("update index%d :visit times = %f\n",selected_index,cur_node->visit_times);
            array_add_at(mcts,(void*)cur_node,selected_index);
            is_odd = 0;
        }
        else{                   //若要更新的節點在偶數層表示該回合是對方下，應選最差解(我方贏率最小值)
            double smallest_win_rate =65535;
            double tmp_win_rate;
            double all_visit_times_in_this_level = 0;

            array_add_at(mcts,(void*)cur_node,selected_index);

            for(int i=cur_node->child_from;i<=cur_node->child_end;i++){
                array_get_at(mcts, i,(void*)&node);
                if((int)node->visit_times!=0){
                    all_visit_times_in_this_level = all_visit_times_in_this_level + node->visit_times;
//printf("add index%d's visit times = %f\n",i,node->visit_times);
                    tmp_win_rate = node->win_times/node->visit_times;
                    if(tmp_win_rate < smallest_win_rate)
                        smallest_win_rate = tmp_win_rate;
                }
            }

            array_remove_at(mcts, selected_index,(void*)&cur_node);

            cur_node->visit_times = all_visit_times_in_this_level;
            cur_node->win_times = smallest_win_rate * all_visit_times_in_this_level;
//printf("update index%d :visit times = %f\n",selected_index,cur_node->visit_times);
            array_add_at(mcts,(void*)cur_node,selected_index);
            is_odd = 1;
        }
    }

    if(cur_node->parent == -1)
        return;

//printf("in backpropagation!!\n");
    backpropagation(mcts,cur_node->parent,win_or_lose,is_odd);
//printf("out backpropagation!!\n");
}

bool IsEqual(char *a,char *b,int length){

    bool equal=true;

    for(int i=0;i<length;i++){
        if(a[i]!=b[i]){
            equal = false;
            break;
        }
    }
    return equal;
}

void Negentropy(char * chess,int my_chess_color){

    clock_t start_time, end_time;
    float total_time = 0;
    start_time = clock();

    const int MCTS_times = 25600; //30000
    const int simulate_level = 10;

    Array *mcts;
    array_new(&mcts);

    struct MCTS *node;
    char * chess_copy = (char *)malloc(sizeof(char)*256);
    int win_or_lose;
    int selected_index;
    int current_node_is_odd;
    int need_to_expand = 1;

    node = (struct MCTS *)malloc(sizeof(struct MCTS));
    node->end = -127;
    node->start = -127;
    node->visit_times = 0;
    node->win_times = 0;
    node->child_end = -1;
    node->child_from = -1;
    node->parent = -1;
    node->is_odd = 1;
    array_add(mcts,(void*)node);

    int selected_chess_color;

    for(int i=0;i<MCTS_times;i++){
        memcpy(chess_copy, chess,sizeof(char)*256);
        current_node_is_odd = 1;
        selected_chess_color = my_chess_color;
//printf("\nselect前:\n");
//printfieldwithoutcls(chess_copy,108);
        selected_index = selection(mcts,0,&current_node_is_odd,chess_copy,&need_to_expand,&selected_chess_color);
//printf("\nselect後:\n");
//printfieldwithoutcls(chess_copy,108);
        expansion(mcts,&selected_index,chess_copy,&current_node_is_odd,need_to_expand,selected_chess_color);
//printf("\nexpand後:\n");
//printfieldwithoutcls(chess_copy,108);
        win_or_lose = simulation(chess_copy,simulate_level,current_node_is_odd,my_chess_color);
//win_or_lose = 1;
//printf("win_or_lose=%d\n",win_or_lose);
        backpropagation(mcts,selected_index,win_or_lose,current_node_is_odd);
//printf("\nBP後:\n");
//printfieldwithoutcls(chess_copy,108);
//PrintMCTS(mcts);
//system("PAUSE");
//system("cls");
    }

PrintPartMCTS(mcts);
//system("PAUSE");

    int best_move_start;
    int best_move_end;
    double best_win_rate = -1;
    double win_rate_tmp;

    array_get_at(mcts, 0,(void*)&node);
    int root_from = node->child_from;
    int root_end = node->child_end;

    for(int i=root_from;i<=root_end;i++){
        array_get_at(mcts, i,(void*)&node);
        if((int)node->visit_times==0)
            win_rate_tmp = 0;
        else
            win_rate_tmp = node->win_times/node->visit_times;
        if( win_rate_tmp > best_win_rate){
            best_move_start = node->start;
            best_move_end = node->end;
            best_win_rate = win_rate_tmp;
        }
    }

    chess[locatetransform(best_move_start)] = 0;
    chess[locatetransform(best_move_end)] = my_chess_color;

    array_remove_all_free(mcts);
    array_destroy(mcts);

    end_time = clock();
    total_time = (float)(end_time - start_time)/CLOCKS_PER_SEC;

    printfieldwithoutcls(chess,108);
    printf("Total Time : %f sec \n", total_time);
    system("PAUSE");
}


void *Negentropy_thread_version(void * arg){

    //const int MCTS_times = 30000;
    const int MCTS_times = 1000000;
    const int simulate_level = 50;
    Array *ends;
    Array *mcts;
    array_new(&mcts);

/*
very long code here.
*/

    for(int i=0;i<MCTS_times;i++){
    }

    pthread_exit((void *) mcts);

}

