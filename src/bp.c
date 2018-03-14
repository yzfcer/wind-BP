#include <stdio.h>
#include <stdlib.h>
#include<stdlib.h>
#include "bp_frame.h"
#include "bp_example.h"

int main(int argc,char **argv)
{
    int i;
    bp_param_s *bp;
    exaple_info_s *info;
    bp_example_s *exam;
    int node_cnt[4] = {2,4,4,2};
	float tt = (float)atof("gdgdggd");
    if(argc > 1)
    {
        bp_printf("%s\r\n",argv[1]);
    }


	bp_example_create("[7,7]->[1,0]");
    bp_example_create("[5,8]->[1,0]");
    bp_example_create("[5,6]->[1,0]");
    bp_example_create("[7,9]->[1,0]");
    bp_example_create("[10,5]->[1,0]");
    bp_example_create("[8,6]->[1,0]");

    bp_example_create("[2,3]->[0,1]");
    bp_example_create("[1.3,4]->[0,1]");
    bp_example_create("[1.8,3.2]->[0,1]");
    bp_example_create("[1,5]->[0,1]");
    bp_example_create("[2,2]->[0,1]");
    bp_example_create("[3,4]->[0,1]");
 


    bp_example_print();
    bp = (bp_param_s*)malloc(sizeof(bp_param_s));
    bp_create(bp,4,node_cnt);
    bp_set_err_limit(bp,0.01f);
    bp_set_learn_factor(bp,0.0002f);
    bp_param_print(bp);
    info = bp_example_info();
    bp_learn(bp,info->exam,info->exa_cnt);
    bp_example_create("[2.4,3.7]->[0,1]");
    exam = new_example(info);
    init_example(exam,"[2.4,3.7]->[0,1]");
    bp_calc_example(bp,exam);
    //bp_param_print(bp);
	system("pause");
}
