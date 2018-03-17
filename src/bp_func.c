#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


int get_rand(float *value,int cnt)
{
    int i;
    srand((unsigned) time(NULL));
    for(i = 0;i < cnt;i ++)
    {
    	value[i] = rand() % 100;
        value[i] = (value[i]+0.1) / 100;
    }
    return 0;
}



