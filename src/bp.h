#ifndef BP_H__
#define BP_H__
#include <stdio.h>
#define bp_printf printf

typedef struct __bp_example_s bp_example_s;
typedef struct __bp_param_s bp_param_s;

struct __bp_param_s
{
    int lay_cnt;//层数
    int *node_cnt;//每层的节点数
    float err_limit;//误差阈值
    float learn_factor;//学习影子
    float **lay_value;
    float **lay_wight;//
    float **errlist;//输出误差向量
    float tot_err;//所有学习实例总误差
    float avr_err;//所有学习实例平均误差
};

//学习实例
struct __bp_example_s
{
    bp_example_s *next;
    float *in_data;
    float *out_data;
    float *errlist;//输出误差向量
    float error;//输出整体误差
};

typedef struct 
{
    int in_cnt;
    int out_cnt;
    int exa_cnt;
    bp_example_s *exam;
    char **instr;
    char **outstr;
}exaple_info_s;

#endif
