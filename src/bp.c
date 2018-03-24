#include <stdio.h>
#include <stdlib.h>
#include<stdlib.h>
#include "bp_frame.h"
#include "bp_example.h"
#include "bp_file.h"

int bp_learn_module(char *module)
{
    bp_param_s *bp;
    exaple_info_s *info;
    bp_example_s *exam;
    int node_cnt[4] = {2,4,4,2};
    bp = (bp_param_s*)malloc(sizeof(bp_param_s));
    //if(argc > 1)
    //    bp_param_read(bp,argv[1]);
#if 1
    bp_example_create("[7,7]->[0,1]");
    bp_example_create("[5,8]->[0,1]");
    bp_example_create("[5,6]->[0,1]");
    bp_example_create("[7,9]->[0,1]");
    bp_example_create("[9,5]->[0,1]");
    bp_example_create("[8,6]->[0,1]");
#endif
#if 1
    bp_example_create("[2,3]->[1,0]");
    bp_example_create("[1.3,4]->[1,0]");
    bp_example_create("[1.8,3.2]->[1,0]");
    bp_example_create("[1,4.3]->[1,0]");
    bp_example_create("[2,2]->[1,0]");
    bp_example_create("[3,4]->[1,0]");
#endif
    bp_example_print();
    
    bp_create(bp,4,node_cnt);
    bp_set_err_limit(bp,0.000002f);
    bp_set_learn_factor(bp,0.001f);
    bp_param_print(bp);
    info = bp_example_info();
    bp_learn(bp,info->exam,info->exa_cnt);
    bp_param_print(bp);
    bp_param_write(bp,module);
}

int bp_calc_module(char *module)
{
    bp_param_s *bp;
    exaple_info_s *info;
    bp_example_s *exam;
    bp = bp_get_param();
    bp = (bp_param_s *)malloc(sizeof(bp_param_s));
    bp_param_read(bp,module);
    info = bp_example_info();
    exam = new_example(info);
    init_example(exam,"[8,6]->[0,1]");
    bp_calc_example(bp,exam);
    exam = new_example(info);
    init_example(exam,"[7.4,6.7]->[0,0]");
    bp_calc_example(bp,exam);
    exam = new_example(info);
    init_example(exam,"[2.1,4.7]->[0,0]");
    bp_calc_example(bp,exam);
    exam = new_example(info);
    init_example(exam,"[1.3,4]->[1,0]");
    bp_calc_example(bp,exam);
}

int main(int argc,char **argv)
{
    char *module = NULL;
    if(argc >= 3)
    {
        module = argv[1];
        if(strcmp(module,"learn") == 0)
            bp_learn_module(module);
        else if(strcmp(module,"calc") == 0)
            bp_calc_module(module);
        system("pause");
        return 0;
    }
    module = "test";
    bp_learn_module(module);
    bp_calc_module(module);
	system("pause");
}
