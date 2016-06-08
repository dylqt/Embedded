
#include "stc15_clk.h"

void WeakUpDownMode(unsigned int time) 
{
	unsigned char a = time & 0xff;
	unsigned char b = (time >> 8) & 0xff;
	b |= 0x80;
	WKTCL = a;
	WKTCH = b;
	//OledWriteWordByHex(1, 0, a);
	//OledWriteWordByHex(1, 16, b);
}


