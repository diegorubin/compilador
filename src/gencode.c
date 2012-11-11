#include "gencode.h"

void bsssection(void)
{
	fprintf(target,"\n\t .section .bss");
}

