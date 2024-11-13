#include <stdio.h>

#include "fb.h"

int main(void)
{
    int ret = -1;
    
    printf("image decode player.......\n");
    
    ret = fb_open();
	if(ret<0)
	{
		printf("fb_open error.\n");
		return -1;
	}

	draw_background(1024,600,RED);

	fb_close();
    
    return 0;
}


