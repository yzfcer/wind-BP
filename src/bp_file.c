#include "string.h"
#include "malloc.h"
#include "bp_file.h"
#include <direct.h>
#include "bp_port.h"
#include "bp_frame.h"

#define PARAM_FILE_SIZE 8192 
static char *get_file_pathname(char *module,char *name)
{
    char *ch;
    char *pathname = (char*)malloc(256);
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
            return pathname;
        }
        else
            *ch = '\0';
    }
    else
        *ch = '\0';
    strcat(pathname,"/");
    strcat(pathname,module);
    strcat(pathname,"/");
    strcat(pathname,name);
    return pathname;
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

int bp_param_read(bp_param_s *bp,char *module)
{
    int len;
    int fsize = PARAM_FILE_SIZE;
    char *buff = (char*)malloc(fsize);
    char *pathname = get_file_pathname(module,"param.txt");
    memset(buff,0,fsize);
    do
    {
        len = bp_read_file(pathname,buff,fsize);
        if(len < 0)
        {
            bp_printf("read param file failed.\r\n");
            return -1;
        }
        if(len >= 8192)
        {
            bp_printf("file is too long.\r\n");
            return -1;
        }
        return decode_param(buff,len);
    }while(0);
    free(pathname);
    free(buff);
    return 0;
}

int bp_param_write(bp_param_s *bp,char *module)
{
    int idx = 0;
    int i;
    char *buff = (char*)malloc(8192);
    char *pathname = get_file_pathname(module,"param.txt");
    memset(buff,0,8192);
    idx = bp_param_tostring(bp,buff);
    bp_write_file(pathname,buff,idx);
    free(pathname);
    return 0;
}

int bp_example_read(char *path)
{
    return 0;
}


