// LED-Interactive-Desk
// Based on STC15F408AD
// Designed by xiao3d and blanboom

#include <intrins.h>
#include <stdio.h>
#include "stc15.h"
#include "app.h"

int main()
{
	uint8 counter;
	
	init();
	
	while(1)
	{
		keyProcess();
		for(counter = 0; counter < 8; counter++)
		{
			lightUp(counter);
			ledProcess(counter);
		}
	}
	return 0;
}
