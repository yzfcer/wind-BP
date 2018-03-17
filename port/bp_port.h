#ifndef BP_PORT_H__
#define BP_PORT_H__
#define bp_printf printf

int bp_read_file(char *pathname,char *buff,int size);
int bp_write_file(char *pathname,char *buff,int size);
int bp_get_pwd(char *buff,int len);
#endif
