#include <intrins.h>
#include <stdio.h>
#include "stc15.h"
#include "app.h"

int main()
{
	uint8 i;
	
	init();
	
	while(1)
	{
		keyProcess();
		
		for(i=0;i<8;i++)
		{
			lightUp(i);
			ledProcess(i);
		}
	}
	return 0;
}
