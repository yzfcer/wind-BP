#ifndef BP_H__
#define BP_H__
#include <stdio.h>
#define bp_printf printf

typedef struct __bp_example_s bp_example_s;
typedef struct __bp_param_s bp_param_s;

struct __bp_param_s
{
    int lay_cnt;//����
    int *node_cnt;//ÿ��Ľڵ���
    float err_limit;//�����ֵ
    float learn_factor;//ѧϰӰ��
    float **lay_value;
    float **lay_wight;//
    float **errlist;//����������
    float tot_err;//����ѧϰʵ�������
    float avr_err;//����ѧϰʵ��ƽ�����
};

//ѧϰʵ��
struct __bp_example_s
{
    bp_example_s *next;
    float *in_data;
    float *out_data;
    float *errlist;//����������
    float error;//����������
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
