#ifndef BP_FILE_H__
#define BP_FILE_H__
#include "bp.h"
int bp_param_read(bp_param_s *bp,char *module);
int bp_param_write(bp_param_s *bp,char *module);
int bp_example_read(char *path);
int bp_example_write(char *path);
#endif
