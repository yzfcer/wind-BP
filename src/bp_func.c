#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


int get_rand(float *value,int cnt)
{
    int i;
    unsigned tt;// = x((unsigned)time(NULL)); 
    srand((unsigned) time(NULL));
    for(i = 0;i < cnt;i ++)
    {
    	value[i] = rand() % 200 - 100;
        //printf("%0.f ",value[i]);
        value[i] = (value[i]+0.1) / 200;
		value[i] += 0.5;
        //printf("%f ",value[i]);
    
    }
    return 0;
}



