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
    int node_cnt[4] = {2,5,3,2};
    bp = (bp_param_s*)malloc(sizeof(bp_param_s));
    bp_example_read(module);
    bp_example_print();
    info = bp_example_info();
    
    bp_create(bp,4,node_cnt);
    bp_set_err_limit(bp,0.000002f);
    bp_set_learn_factor(bp,0.002f);
    bp_param_print(bp);
    
    bp_learn(bp,info->exam,info->exa_cnt);
    bp_param_print(bp);
    bp_param_write(bp,module);
}

int bp_calc_module(char *module)
{
    bp_param_s *bp;
    bp_example_s *exam;
    exaple_info_s *info;
    bp = (bp_param_s *)malloc(sizeof(bp_param_s));
    bp_param_read(bp,module);
    info = bp_example_info();
    info->in_cnt = bp->input_cnt;
    info->out_cnt = bp->output_cnt;
    exam = new_example(bp->input_cnt,bp->output_cnt);
    init_example(exam,"[8,6]->[0,1]");
    bp_calc_example(bp,exam);

}

int main(int argc,char **argv)
{
    char *module = NULL;
    char *examstr;
    if(argc >= 3)
    {
        module = argv[1];
        if(strcmp(argv[2],"learn") == 0)
            bp_learn_module(module);
        if(strcmp(argv[2],"calc") == 0)
        {
            if(argc == 4)
            {
                examstr = argv[3];
            }
            bp_calc_module(module);
        }
        system("pause");
        return 0;
    }
    

    module = "test";
    bp_learn_module(module);
    bp_calc_module(module);
	system("pause");
}
