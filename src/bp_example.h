#ifndef BP_EXAPLE_H__
#define BP_EXAPLE_H__
#include "bp.h"


void bp_example_init(void);
void bp_example_reset(void);
int bp_example_create(char * exam);
void bp_example_destroy(bp_example_s *exam);
void bp_example_print(void);
bp_example_s * bp_example_list(void);
exaple_info_s * bp_example_info(void);

#endif
