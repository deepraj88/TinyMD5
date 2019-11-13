#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const int M = 0b01011101111011;
const int K = 0b10011011101110;
#define STAGE 14
//const int M = 0x0DBA;
//const int K = 0x059B;

typedef struct _state {
    char a;
    char b;
    char c;
    char d;
}TINY_MD5_STATE;

const TINY_MD5_STATE initial_state = {
    1,
    1,
    0,
    1
};

char function_f(TINY_MD5_STATE state)
{
    return 0x1 & ((state.b & state.c) | (~state.b & state.d));
}

char function_g(TINY_MD5_STATE state)
{
    return 0x1 & ((state.b & state.d) | (state.c & ~state.d));
}

char function_h(TINY_MD5_STATE state)
{
    return 0x1 & (state.b ^ state.c ^ state.d);
}

char function_i(TINY_MD5_STATE state)
{
    return 0x1 & (state.c ^ (state.b | ~state.d));
}

TINY_MD5_STATE calculate_round(TINY_MD5_STATE state, char Mi, char Ki, unsigned char round)
{
    TINY_MD5_STATE nextState;
    char f_result;

    nextState.c = state.b;
    nextState.d = state.c;
    nextState.a = state.d;

    char(* function_scheduler[4])(TINY_MD5_STATE state) = {
        function_f,
        function_g,
        function_h,
        function_i
    };

    f_result = function_scheduler[round%4](state); 
    nextState.b = f_result ^ state.a;
    nextState.b ^= Mi;
    nextState.b ^= Ki;
    nextState.b = ~nextState.b & 0x1;
    nextState.b = ~(nextState.b^state.b) & 0x1;

    return nextState;
}


TINY_MD5_STATE tinyMD5(){
    TINY_MD5_STATE current_state = {
        initial_state.a,
        initial_state.b,
        initial_state.c,
        initial_state.d,
    };

    int i;
    char Mi, Ki;

        printf("Stage  Mi Ki  Current_state\n");
    for (i = 0; i < STAGE; i ++){
        
        Mi = (M >> (STAGE-1-i)) & 0x1;
        Ki = (K >> (STAGE-1-i)) & 0x1;

        printf("%2d     [%hhd, %hhd]    %hhd%hhd%hhd%hhd\n", i, Mi, Ki, current_state.a,
                current_state.b, current_state.c, current_state.d);
        current_state = calculate_round(current_state, Mi, Ki, i);

    }
    
    printf("Result           %d%d%d%d\n", current_state.a, current_state.b,
            current_state.c, current_state.d);

    return current_state;
}

int main(void) 
{
    tinyMD5();
}
