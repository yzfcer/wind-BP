#include "string.h"
#include "malloc.h"
#include "bp_file.h"
#include <direct.h>
#include "bp_port.h"
#include "bp_frame.h"
#include "cJSON.h"
#define PARAM_FILE_SIZE 8192 
static int get_file_pathname(char *pathname,char *module,char *filename)
{
    char *ch;
    memset(pathname,0,256);
    getcwd(pathname,256);
    ch = strrchr(pathname,'\\');
    if(ch == NULL)
    {
        ch = strrchr(pathname,'/');
        if(ch == NULL)
        {
            bp_printf("path error:%s\r\n",pathname);
            memset(pathname,0,256);
            return -1;
        }
        else
            *ch = '\0';
    }
    else
        *ch = '\0';
    strcat(pathname,"/");
    strcat(pathname,module);
    strcat(pathname,"/");
    strcat(pathname,filename);
    return 0;
}


static int get_line(char *buff,int idx,int end)
{
    int i;
    int len;
    if(idx >= end)
        return 0;
    for(i = idx;i < end;i ++)
    {
        if(buff[i] == '\r' || buff[i] == '\n')
        {
            buff[i] = '\0';
            len = strlen(&buff[idx]);
            return len+1;
        }
    }
    buff[end-1] = '\0';
    len = strlen(&buff[idx]);
    return len+1;
}

static int decode_param(char *buff,int filelen)
{
    int idx = 0,len;
    float learn_factor = -1.0F;
    int lay_cnt = -1;
    int *node_cnt = NULL;
    do
    {
        len = get_line(buff,idx,filelen);
        bp_printf("%s\r\n",&buff[idx]);
        idx += len;
    }while(len > 0);
    return 0;
    
}


static cJSON *bp_format_JSON_str(bp_param_s *bp)
{
    int i;
    char *buff;
    cJSON *arr;
    char wight[12];
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root,"lay_cnt",bp->lay_cnt);
    cJSON_AddNumberToObject(root,"learn_factor",bp->learn_factor);
    cJSON_AddNumberToObject(root,"err_limit",bp->err_limit);
    cJSON_AddNumberToObject(root,"tot_err",bp->tot_err);
    cJSON_AddNumberToObject(root,"avr_err",bp->avr_err);
    arr = cJSON_CreateIntArray(bp->node_cnt,bp->lay_cnt);
    cJSON_AddItemToObject(root,"node_cnt",arr);

    for(i = 0;i < bp->lay_cnt-1;i ++)
    {
        memset(wight,0,sizeof(wight));
        sprintf(wight,"lay_wight%d",i+1);
        arr = cJSON_CreateFloatArray(bp->lay_wight[i],(bp->node_cnt[i]*bp->node_cnt[i+1]));
        cJSON_AddItemToObject(root,wight,arr);
    }
    return root;
}

static int bp_from_JSON_str(bp_param_s *bp,char *JSONstr)
{
    int i,j,cnt;
    cJSON *obj,*arr;
    char wight[12];
    cJSON *root = cJSON_Parse(JSONstr);
    obj = cJSON_GetObjectItem(root,"lay_cnt");
    bp->lay_cnt = obj->valueint;
    obj = cJSON_GetObjectItem(root,"learn_factor");
    bp->learn_factor = (float)obj->valuedouble;
    obj = cJSON_GetObjectItem(root,"err_limit");
    bp->err_limit= (float)obj->valuedouble;
    obj = cJSON_GetObjectItem(root,"tot_err");
    bp->tot_err = (float)obj->valuedouble;
    obj = cJSON_GetObjectItem(root,"avr_err");
    bp->avr_err = (float)obj->valuedouble;
    //obj = cJSON_GetArrayItem(cJSON * array,int item)
    bp->node_cnt = (int*)malloc(bp->lay_cnt*sizeof(int));
    memset(bp->node_cnt,0,bp->lay_cnt*sizeof(int));
    
    bp->lay_value = (float**)malloc(bp->lay_cnt*sizeof(float *));
    memset(bp->lay_value,0,bp->lay_cnt*sizeof(float *));
    bp->errlist = (float**)malloc(bp->lay_cnt*sizeof(float *));
    memset(bp->errlist,0,bp->lay_cnt*sizeof(float *));
    bp->lay_wight = (float**)malloc((bp->lay_cnt-1)*sizeof(float *));
    memset(bp->lay_wight,0,(bp->lay_cnt-1)*sizeof(float *));

    obj = cJSON_GetObjectItem(root,"node_cnt");
    for(i = 0;i < bp->lay_cnt;i ++)
    {
        arr = cJSON_GetArrayItem(obj,i);
        bp->node_cnt[i] = arr->valueint;
        bp->lay_value[i] = (float*)malloc(bp->node_cnt[i]*sizeof(float));
        memset(bp->lay_value[i],0,bp->node_cnt[i]*sizeof(float));
        bp->errlist[i] = (float*)malloc(bp->node_cnt[i]*sizeof(float));
        memset(bp->errlist[i],0,bp->node_cnt[i]*sizeof(float));
    }
    for(i = 0;i < bp->lay_cnt-1;i ++)
    {
        cnt = bp->node_cnt[i]*bp->node_cnt[i+1];
        bp->lay_wight[i] = (float*)malloc(cnt*sizeof(float));
        memset(wight,0,sizeof(wight));
        sprintf(wight,"lay_wight%d",i+1);
        obj = cJSON_GetObjectItem(root,wight);
        
        for(j = 0;j < cnt;j ++)
        {
            arr = cJSON_GetArrayItem(obj,j);
            bp->lay_wight[i][j] = arr->valuedouble;
        }
        
    }
    return 0;
}

int bp_param_read(bp_param_s *bp,char *module)
{
    int idx = 0;
    int ret;
    char *pathname = (char*)malloc(256);
    char *buff = (char*)malloc(8192);
    ret = get_file_pathname(pathname,module,"param.txt");
    memset(buff,0,8192);
    bp_read_file(pathname,buff,8192);
    bp_from_JSON_str(bp,buff);
    bp_printf("read param file\r\n");
    bp_param_print(bp);
    free(pathname);
    free(buff);
    
    return 0;
}

int bp_param_write(bp_param_s *bp,char *module)
{
    int idx = 0;
    int ret;
    cJSON *root;
    char *pathname = (char*)malloc(256);
    char *buff = (char*)malloc(8192);
    ret = get_file_pathname(pathname,module,"param.txt");
    memset(buff,0,8192);
    root = bp_format_JSON_str(bp);
    buff = cJSON_Print(root);
    
    bp_printf("write param file\r\n");
    bp_printf("%s\r\n",buff);
    bp_write_file(pathname,buff,strlen(buff));
    free(pathname);
    free(buff);
    return 0;
}

int bp_example_read(bp_example_s *exam,char *module)
{
    int idx = 0;
    int ret;
    char *pathname = (char*)malloc(256);
    char *buff = (char*)malloc(8192);
    ret = get_file_pathname(pathname,module,"param.txt");
    memset(buff,0,8192);
    bp_read_file(pathname,buff,8192);
    free(pathname);
    free(buff);
    return 0;
}

int bp_example_write(bp_example_s *exam,char *module)
{
    int idx = 0;
    int ret;
    char *pathname = (char*)malloc(256);
    char *buff = (char*)malloc(8192);
    ret = get_file_pathname(pathname,module,"param.txt");
    memset(buff,0,8192);
    bp_read_file(pathname,buff,8192);
    bp_write_file(pathname,buff,idx);
    free(pathname);
    free(buff);
    return 0;
}


