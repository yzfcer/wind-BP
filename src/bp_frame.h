#ifndef BP_FRAME_H__
#define BP_FRAME_H__
#include <stdio.h>
#include "bp.h"

int bp_create(bp_param_s *bp,int lay_cnt,int *node_cnt);
void bp_calc_layer_and_node(int in_cnt,int out_cnt);
void bp_set_learn_factor(bp_param_s *bp,float learn_factor);
void bp_set_lay_wight(bp_param_s *bp,int layer,float *wights);
void bp_set_err_limit(bp_param_s *bp,float err_limit);
void bp_translate_learn_layer(float *wight,float *value_front,
                    float *value_behind,int count_front,
                    int count_behind,int func_idx);
int bp_translate_learn_example(bp_param_s *bp,bp_example_s *example);
void bp_param_print(bp_param_s *bp);
int bp_learn(bp_param_s *bp,bp_example_s *example,int count);
#endif
