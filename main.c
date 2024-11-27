#include <stdio.h>

#include <fb.h>


int main(void)
{
    int ret = -1;
	pic_info pictrue;
    
    printf("image decode player.......\n");
    
    ret = fb_open();
	if(ret<0)
	{
		DBG("fb_open error.\n");
		return -1;
	}

	draw_background(WIDTH,HEIGHT,WHITE);
	
	//draw_bmp_test1(-50, HEIGHT - HEIGHT_BMP_test1 +50, pBmp);// 从左下角超出屏幕

	//bmp_analyze("bmp1.bmp");

	pictrue.pathname = "bmp1.bmp";
	is_bmp(&pictrue);

	fb_close();
    
    return 0;
}


