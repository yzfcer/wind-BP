#include "bp_frame.h"
#include "math.h"
#include <malloc.h>
#include <string.h>
#include "bp_func.h"
typedef float (*FUNC_EXT)(float t);
float ext_func1(float t){return 1/(1+exp(-t));}
float ext_func2(float t){return (exp(t)-exp(-t))/(exp(t)+exp(-t));}
float ext_func3(float t){return atan(t);}
FUNC_EXT func_ext[3] = {ext_func1,ext_func2,ext_func3};

static void print_float_arr(char *title,float *data,int cnt)
{
    int i;
    bp_printf("%s:",title);
    for(i = 0;i < cnt;i ++)
        bp_printf("%f ",data[i]);
    bp_printf("\r\n");
}

//为节点数据和节点权重矩阵分配空间,初始化权重矩阵
int bp_create(bp_param_s *bp,int lay_cnt,int *node_cnt)
{
    int i,j;
    int cnt;
    bp->node_cnt = (int*)malloc(lay_cnt*sizeof(int));
    memset(bp->node_cnt,0,lay_cnt*sizeof(int));
    bp->lay_value = (float**)malloc(lay_cnt*sizeof(float *));
    memset(bp->lay_value,0,lay_cnt*sizeof(float *));
    bp->errlist = (float**)malloc(lay_cnt*sizeof(float *));
    memset(bp->errlist,0,lay_cnt*sizeof(float *));
    bp->lay_wight = (float**)malloc((lay_cnt-1)*sizeof(float *));
    memset(bp->lay_wight,0,(lay_cnt-1)*sizeof(float *));
    for(i = 0;i < lay_cnt;i ++)
    {
        bp->node_cnt[i] = node_cnt[i];
        bp->lay_value[i] = (float*)malloc(node_cnt[i]*sizeof(float));
        memset(bp->lay_value[i],0,node_cnt[i]*sizeof(float));
        bp->errlist[i] = (float*)malloc(node_cnt[i]*sizeof(float));
        memset(bp->errlist[i],0,node_cnt[i]*sizeof(float));
    }
    for(i = 0;i < lay_cnt-1;i ++)
    {
        cnt = node_cnt[i]*node_cnt[i+1];
        bp->lay_wight[i] = (float*)malloc(cnt*sizeof(float));
        get_rand(bp->lay_wight[i],cnt);
    }
    bp->lay_cnt = lay_cnt;
    
    return 0;
}


//根据输入和输出节点数来设定层数
int bp_calc_layer_count(int in_cnt,int out_cnt)
{
    int lay = (in_cnt * out_cnt) / 5;
    lay = lay < 3?3:lay;
    lay = lay > 6?6:lay;
    return lay;
}

//根据输入和输出节点数来设定各层节点数
void bp_calc_node_count(int in_cnt,int out_cnt,int lay_cnt,int* node_cnt)
{
    node_cnt[0] = in_cnt;
    node_cnt[lay_cnt-1] = out_cnt;
    
}


void bp_set_learn_factor(bp_param_s *bp,float learn_factor)
{
    bp->learn_factor = learn_factor;
}

void bp_set_lay_wight(bp_param_s *bp,int layer,float *wights)
{
    int i;
    int cnt = bp->node_cnt[layer]*bp->node_cnt[layer+1];
    for(i = 0;i < cnt;i ++)
        bp->lay_wight[layer][i] = wights[i];
}

void bp_set_err_limit(bp_param_s *bp,float err_limit)
{
    bp->err_limit = err_limit;
}

//层间学习正向传递
void bp_translate_learn_layer(float *wight,float *lay1_value,
                    float *lay2_value,int lay1_cnt,
                    int lay2_cnt,int func_idx)
{
    int i,j;
    float *wgt=wight,t;
    for(i=0;i<lay2_cnt;i++,wgt=wight+i)
    {
        for(j=0,t=0;j<lay1_cnt;j++)
            t += wight[j*lay2_cnt+i]*lay1_value[j];
        *(lay2_value+i)= func_ext[func_idx](t);
    }
}

//计算输出误差
float bp_calc_out_error(bp_param_s *bp,bp_example_s *example)
{
    int i;
    int err_cnt = bp->node_cnt[bp->lay_cnt-1];
    example->error = 0.0f;
    for(i=0;i < err_cnt;i++)
    {
        example->errlist[i] = bp->lay_value[bp->lay_cnt-1][i] - example->out_data[i];
        bp->errlist[bp->lay_cnt-1][i] = example->errlist[i];
        example->error += example->errlist[i] * example->errlist[i];
    }
    example->error = sqrt(example->error);
    return example->error;
}

void bp_modify_wight(float *errlist1,float *errlist2,int lay1_cnt,int lay2_cnt,
    float learn_factor,float *wight,float *lay1_value,int func_idx)
{
    int i,j;
    int idx1,idx;
    float t,*w=wight,f1;
    for(i=0;i<lay1_cnt;i++)
    {
        idx1 = i*lay2_cnt;
        for(j=0,t=0;j<lay2_cnt;j++,w=w+1)
        {
            idx = idx1 + j;
            t = t+wight[idx]*errlist2[j];
            wight[idx] = wight[idx] - learn_factor * errlist2[j]*lay1_value[i];
        }
        switch(func_idx)
        {
            case 0:f1=*(lay1_value+i)*(1-*(lay1_value+i));break;
            case 1:f1=1-(*(lay1_value+i))*(*(lay1_value+i));break;
            case 2:f1=1/(1+(*(lay1_value+i))*(*(lay1_value+i)));break;
            default:printf("* parameter error:tranerr.\n");
                    break;
        }
        errlist1[i]=f1 * t;
    }
}

void bp_calc_example(bp_param_s *bp,bp_example_s *example)
{
    int i;
    int ok_cnt = 0;
    
    for(i = 0;i < bp->node_cnt[0];i ++)
        bp->lay_value[0][i] = example->in_data[i];
    for(i = 0;i < bp->lay_cnt-1;i ++)
    {
        bp_translate_learn_layer(bp->lay_wight[i],bp->lay_value[i],
                                bp->lay_value[i+1],bp->node_cnt[i],
                                bp->node_cnt[i+1],0);
            
    }
    print_float_arr("result",bp->lay_value[bp->lay_cnt-1],bp->node_cnt[bp->lay_cnt-1]);
}
//一个实例的完整学习流程
int bp_translate_learn_example(bp_param_s *bp,bp_example_s *example)
{
    int i;
    int ok_cnt = 0;
    
    for(i = 0;i < bp->node_cnt[0];i ++)
        bp->lay_value[0][i] = example->in_data[i];
    //print_float_arr("in",bp->lay_value[0],bp->node_cnt[0]);

    for(i = 0;i < bp->lay_cnt-1;i ++)
    {
        bp_translate_learn_layer(bp->lay_wight[i],bp->lay_value[i],
                                bp->lay_value[i+1],bp->node_cnt[i],
                                bp->node_cnt[i+1],0);
            
    }
    bp_calc_out_error(bp,example);
    //print_float_arr("errlist",example->errlist,bp->node_cnt[bp->lay_cnt-1]);
    if(example->error < bp->err_limit)
        return 0;
    
    for(i = bp->lay_cnt - 1;i > 0;i --)
    {
        bp_modify_wight(bp->errlist[i-1],bp->errlist[i],
            bp->node_cnt[i-1],bp->node_cnt[i],bp->learn_factor,
            bp->lay_wight[i-1],bp->lay_value[i-1],0);
        //print_float_arr("lay error",bp->errlist[i-1],bp->node_cnt[i-1]);
        
    }
    return -1;
}

int bp_learn_flow(bp_param_s *bp,bp_example_s *example,int count)
{
    int cnt = 0;
    int i;
    bp_example_s *exa = example;
    exaple_info_s *info = bp_example_info();
    bp->tot_err = 0.0F;
    for(i = 0;i < count;i ++)
    {
        bp_translate_learn_example(bp,exa);
        if(exa->error < bp->err_limit)
            cnt ++;
        bp->tot_err += exa->error;
        exa = exa->next;
    }
	bp->avr_err = bp->tot_err / count;
    if(cnt >= count)
    {
        exa = example;
        for(i = 0;i < count;i ++)
        {
            print_float_arr("err",exa->errlist,info->out_cnt);
            exa = exa->next;
        }
        return 0;
    }
        
    return -1;
}
#include <windows.h>

int bp_learn(bp_param_s *bp,bp_example_s *example,int count)
{
    int ret = -1,I;
    int calc_cnt = 0;
	float err = 100000;
	int stop_cnt = 0;
    while(ret != 0)
    {
        calc_cnt ++;
        ret = bp_learn_flow(bp,example,count);
        if(!(calc_cnt&0xffff) || (ret == 0))
        {
            bp_printf("calc_cnt:%d,average error:%f\r\n",calc_cnt,bp->avr_err);
        }
        if(bp->tot_err >= err)
			stop_cnt ++;
		else
		{
			stop_cnt = 0;
			err = bp->tot_err;
		}
			
#if 0
		if((stop_cnt > 2000)&&(bp->learn_factor > 0.000002))
		{
			bp->learn_factor /= 2;
			stop_cnt = 0;
			bp_printf("modify learn_factor:%f\r\n",bp->learn_factor);
		}
#endif
		if(stop_cnt > 10000)
		{
			bp_printf("error is stable,avrerage error:%f\r\n",bp->avr_err);
			bp_printf("calc_cnt:%d,average error:%f\r\n\r\n\r\n",calc_cnt,bp->avr_err);
			return 0;
		}

        //bp_param_print(bp);
        //Sleep(1000);
    }
    return 0;
}

static void print_wight(float *wight,int lay_cnt1,int lay_cnt2)
{
    int i,j,k = 0;
    for(i = 0;i < lay_cnt1;i ++)
    {
        for(j = 0;j < lay_cnt2;j ++)
            bp_printf("%f ",wight[k++]);
        bp_printf("\r\n");
    }
    bp_printf("\r\n");
}

void bp_param_print(bp_param_s *bp)
{
    int i;
    if(bp == NULL)
    {
        bp_printf("NULL bp_param pointer\r\n");
        return;
    }
    bp_printf("lay_cnt:%d\r\n",bp->lay_cnt);
    bp_printf("learn_factor:%f\r\n",bp->learn_factor);
    bp_printf("err_limit:%f\r\n",bp->err_limit);
    bp_printf("layer wight:\r\n");
    for(i = 0;i < bp->lay_cnt-1;i ++)
    {
        
        bp_printf("layer[%d] =\r\n",i+1);
        print_wight(bp->lay_wight[i],bp->node_cnt[i],bp->node_cnt[i+1]);
    }
    
}


