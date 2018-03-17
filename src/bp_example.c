#include "bp_example.h"
#include <stdlib.h>
#include <string.h>
#include "bp_port.h"

exaple_info_s exam_info;

void bp_example_init(void)
{
    exaple_info_s *info = bp_example_info();
    info->in_cnt = 0;
    info->out_cnt = 0;
    info->exa_cnt = 0;
    info->instr = NULL;
    info->outstr = NULL;
}

void bp_example_reset(void)
{
    exaple_info_s *info = bp_example_info();
    bp_example_destroy(info->exam);
    bp_example_init();
    
}

static int get_char_index(char * str,int idx,char c)
{
    int i;
    int len = strlen(str);
    for(i = idx;i < len;i ++)
    {
        if(str[i] == '\0')
            return -1;
        if(str[i] == c)
            return i;
    }
    return -1;
}

static int get_input_count(char * examstr)
{
    int cnt = 1,i = 0;
    int idx = get_char_index(examstr,0,']');
    if(idx < 0)
        return -1;
    i = get_char_index(examstr,i,',');
    while(i < idx)
    {
        cnt ++;
        i = get_char_index(examstr,i+1,',');
        if(i < 0)
            break;
    }
    return cnt;
}

static int get_output_count(char * examstr)
{
    int cnt = 1,i = 0;
    int idx1,idx2;
    idx1 = get_char_index(examstr,0,']');
    idx2 = get_char_index(examstr,idx1+1,']');
    if(idx1 < 0 || idx2 < 0)
        return -1;
    i = get_char_index(examstr,idx1+1,',');
    while(i < idx2)
    {
        cnt ++;
        i = get_char_index(examstr,i+1,',');
        if(i < 0)
            break;
    }
    return cnt;
}

static int set_exaple_info(char * examstr)
{
    exaple_info_s *info = bp_example_info();
    if((info->in_cnt > 0) || (info->out_cnt > 0))
        return 0;
    info->in_cnt = get_input_count(examstr);
    info->out_cnt = get_output_count(examstr);
    if(info->in_cnt < 0 || info->out_cnt < 0)
        return -1;
    info->exa_cnt = 0;
    info->exam = NULL;
    info->instr = (char **)malloc(sizeof(char*)*info->in_cnt);
    info->outstr = (char **)malloc(sizeof(char*)*info->out_cnt);
    return 0;
}

static int check_example(char * examstr)
{
    int incnt,outcnt;
    exaple_info_s *info = bp_example_info();
    incnt = get_input_count(examstr);
    outcnt = get_output_count(examstr);
    if((incnt != info->in_cnt) || (outcnt != info->out_cnt))
        return -1;
    return 0;
}

bp_example_s *new_example(exaple_info_s *info)
{
    bp_example_s *exam = (bp_example_s *)malloc(sizeof(bp_example_s));
    exam->in_data = (float*)malloc(sizeof(float)*info->in_cnt);
    exam->out_data = (float*)malloc(sizeof(float)*info->out_cnt);
    exam->errlist = (float*)malloc(sizeof(float)*info->out_cnt);
    exam->error = 0;
    return exam;
}


static int decode_data(char * examstr,float *data,int idx1,int idx2)
{
    char *temp,*str;
    int i = 0;
    str = (char*)malloc(sizeof(char)*(idx2-idx1+2));
    memcpy(str,&examstr[idx1],idx2-idx1+1);
    str[idx2-idx1+1] = 0;
    temp = strtok(str,",");
    while(temp)
    {
        data[i++] = atof(temp);
        temp = strtok(NULL,",");
    }
    free(str);
    return 0;
}

int init_example(bp_example_s *exam,char * examstr)
{
    int i = 0;
    int idx1 = get_char_index(examstr,0,'[')+1;
    int idx2 = get_char_index(examstr,0,']')-1;
    decode_data(examstr,exam->in_data,idx1,idx2);
    idx1 = get_char_index(examstr,idx2+2,'[') + 1;
    idx2 = get_char_index(examstr,idx2+2,']') - 1;
    decode_data(examstr,exam->out_data,idx1,idx2);
    return 0;
}

static void insert_example(bp_example_s *exam)
{
    bp_example_s *ex;
    exaple_info_s *info = bp_example_info();
    exam->next = NULL;
    if(info->exam == NULL)
    {
        info->exam = exam;
        info->exa_cnt ++;
        return;
    }
    ex = info->exam;
    while(ex->next != NULL)
        ex = ex->next;
    ex->next = exam;
    info->exa_cnt ++;
}

int bp_example_create(char * examstr)
{
    int ret = 0;
    bp_example_s *exam;
    exaple_info_s *info = bp_example_info();
    if((examstr == NULL) || (examstr[0] == '\0'))
        return -1;
    ret = set_exaple_info(examstr);
    if(ret < 0)
    {
        bp_printf("[ERROR] set example info failed: %s\r\n",examstr);
        return -1;
    }
    ret = check_example(examstr);
    if(ret < 0)
    {
        bp_printf("[ERROR] invalid example: %s\r\n",examstr);
        return -1;
    }
    exam = new_example(info);
    ret = init_example(exam,examstr);
    if(ret < 0)
    {
        bp_printf("[ERROR] init example failed: %s\r\n",examstr);
        return -1;
    }
    insert_example(exam);
    return 0;
}

static void free_example(bp_example_s *exam)
{
    if(exam == NULL)
        return;
    if(exam->in_data != NULL)
        free(exam->in_data);
    if(exam->out_data != NULL)
        free(exam->out_data);
    if(exam->out_data != NULL)
        free(exam->out_data);
    free(exam);
}

void bp_example_destroy(bp_example_s *exams)
{
    bp_example_s *exam = exams,*ex;
    while(exam != NULL)
    {
        ex = exam->next;
        free_example(exam);
        exam = ex;
    }
}
void bp_example_print_one(bp_example_s *exam)
{
    int i,idx;
    char *examstr = (char *)malloc(256);
    exaple_info_s *info = bp_example_info();
    idx = 0;
    memset(examstr,0,256);
    idx += sprintf(&examstr[idx],"[");
    for(i = 0;i < info->in_cnt;i ++)
    {
        idx += sprintf(&examstr[idx],"%.0f,",exam->in_data[i]);
    }
    idx --;
    idx += sprintf(&examstr[idx],"]->[");
    for(i = 0;i < info->out_cnt;i ++)
    {
        idx += sprintf(&examstr[idx],"%.0f,",exam->out_data[i]);
    }
    idx --;
    idx += sprintf(&examstr[idx],"]\r\n");
    bp_printf("%s",examstr);
    free(examstr);
    
}

void bp_example_print(void)
{
    bp_example_s *exam;
    exaple_info_s *info = bp_example_info();
    if(info->exa_cnt <= 0)
    {
        bp_printf("ERROR:has no examples\r\n");
        return;
    }
    exam = info->exam;
    while(exam)
    {
        bp_example_print_one(exam);
        exam = exam->next;
    }
}

bp_example_s * bp_example_list(void)
{
    exaple_info_s *info = bp_example_info();
    return info->exam;
}

exaple_info_s * bp_example_info(void)
{
    exaple_info_s *info = &exam_info;
    return info;
}
