
#include "stc15_clk.h"

void WeakUpDownMode(unsigned char time) 
{
	WKTCL = time;  
	WKTCH = 0x80;
}


