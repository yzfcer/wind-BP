#ifndef BP_FRAME_H__
#define BP_FRAME_H__
#include <stdio.h>
#include "bp.h"

bp_param_s *bp_get_param(void);
int  bp_create(bp_param_s *bp,int lay_cnt,int *node_cnt);
void bp_calc_layer_and_node(bp_param_s *bp,int in_cnt,int out_cnt);
void bp_set_learn_factor(bp_param_s *bp,float learn_factor);
void bp_set_err_limit(bp_param_s *bp,float err_limit);
void bp_set_lay_wight(bp_param_s *bp,int layer,float *wights);
int bp_param_tostring(bp_param_s *bp,char *buff);
void bp_param_print(bp_param_s *bp);
int bp_learn(bp_param_s *bp,bp_example_s *example,int count);
void bp_calc_example(bp_param_s *bp,bp_example_s *example);

#endif
