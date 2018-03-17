#include "stdio.h"
#include "bp_port.h"

int bp_read_file(char *pathname,char *buff,int size)
{
    FILE *fp;
    int len;
    fp=fopen(pathname,"r");
    if(fp==NULL)  
    {
        bp_printf("can not open file:%s\r\n",pathname);  
        return -1;
    }
    len = fread(buff,size,1,fp);
    fclose(fp);
    return len;
}

int bp_write_file(char *pathname,char *buff,int size)
{
    FILE *fp;
    int len;
    fp=fopen(pathname,"w");
    if(fp==NULL)  
    {
        bp_printf("can not open file:%s\r\n",pathname);  
        return -1;
    }
    len = fwrite(buff,size,1,fp);
    fclose(fp);
    return len;
}

int bp_get_pwd(char *buff,int len)
{

}
