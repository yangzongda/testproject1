// 防止头文件被多次包含
#ifndef __FB_H__
#define __FB_H__

#include <config.h>

//将头文件都添加到include文件夹下面
//把fb.c中的宏定义和函数声明都移到此头文件中

#define FBDEVICE	"/dev/fb0"  //framebuffer

#define WIDTH		1024	//对应实际屏幕大小，所以不是普适性程序，只适应固定分辨率
#define HEIGHT		600     //但嵌入式程序一般与硬件高度适配，对普适性的需求较弱

#define WHITE		0xffffffff			//颜色测试
#define BLACK		0x00000000
#define RED			0xffff0000
#define GREEN		0xff00ff00			
#define BLUE		0xff0000ff			

#define GREENP		0x0000ff00			// 一样，说明前2个ff透明位不起作用


// 声明这些变量，否则main.c中无法调用(头文件中不可定义)
extern const unsigned char *pBmp;
extern const unsigned int WIDTH_BMP1;
extern const unsigned int HEIGHT_BMP1;
//extern const int x_gap_bmp_test1;
//extern const int y_gap_bmp_test1;

// 函数声明
// 画背景色和划线的测试函数
void draw_background(unsigned int width, unsigned int height, unsigned int color);
void draw_line(unsigned int color);
void draw_bmp_test1(int x, int y,const unsigned char *filename);
void draw_bmp_test2(int x_gap, int y_gap, pic_info *pPic);
int bmp_analyze(const char *path);
int is_bmp(pic_info *pPic);

int fb_open(void);
void fb_close(void);


#endif








