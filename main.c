#include <stdio.h>

#include <fb.h>


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

	draw_background(WIDTH,HEIGHT,BLACK);
	
	draw_bmp_test1(-50, HEIGHT - HEIGHT_BMP_test1 +50, pBmp);// 从左下角超出屏幕

	fb_close();
    
    return 0;
}


