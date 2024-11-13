 /*
 * 	创建一个新文件夹/display，将fb.c放入该文件夹
 * 	在此子文件夹中添加子Makefile
 * 
 * fb.c 操作framebuffer的基础代码，包括fb的打开、ioctl获取信息
 *      基本的测试fb显示代码
 * 
 * ******************************************************
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "fb.h"		//自行创建



// 定义全局变量
unsigned int *pfb = NULL;	//用于保存申请到的内存空间的首地址
					//全局变量不初始化值为零，所以此处不初始化没关系
int fbfd = -1;		//设成全局变量，因为fb_open和fb_close都要用到
unsigned int screenlength = 0;


int fb_open(void)
{
	int ret = -1;
	
	//{0}是个初始化列表，用于初始化结构体、数组等复合类型。成员都为0
	struct fb_fix_screeninfo finfo = {0};//用于获取framebuffer不可变信息
	struct fb_var_screeninfo vinfo = {0};//用于获取framebuffer可变信息
	

	//framebuffer的操作通常包括以下4步。标准化操作，框架可直接借用再自行修改
	// 第1步：打开设备
	fbfd = open(FBDEVICE, O_RDWR);
	if (fbfd < 0)
	{
		perror("open /dev/fb0");
		return -1;
	}
	printf("open %s success.\n", FBDEVICE);
	
	
	// 第2步：获取设备的硬件信息
	
	//字符驱动通常的功能有开，关，读，写。但是
	//需要扩展新的功能，一般以增设ioctl()命令的方式实现
	
	//FBIOGET_FSCREENINFO是用于获取帧缓冲设备的ioctl请求代码，
	//它定义在Linux头文件中。即驱动层定义了命令，应用层ioctl进行调用
	ret = ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo);//获取不可变信息
	if (ret < 0)
	{
		perror("ioctl");//输出错误消息到stderr，该消息包含最近一次系统调用失败的原因
		return -1;
	}
	//打印的结构体信息就是ioctl从驱动层获取到的
	printf("smem_start = 0x%lx, smem_len = %u.\n", finfo.smem_start, finfo.smem_len);
	
	ret = ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo);//获取可变信息
	if (ret < 0)
	{
		perror("ioctl");
		return -1;
	}
	printf("xres = %u, yres = %u.\n", vinfo.xres, vinfo.yres);
	printf("xres_virtual = %u, yres_virtual = %u.\n", vinfo.xres_virtual, vinfo.yres_virtual);
	printf("bpp = %u.\n", vinfo.bits_per_pixel);

	
	// 第3步：进行mmap内存映射
	// 根据第二步获取到的设备信息，计算需要申请的内存大小 =长*宽*像素占几个字节
	screenlength = vinfo.xres_virtual * vinfo.yres_virtual * vinfo.bits_per_pixel / 8;
	printf("screenlength = %d\n", screenlength);
	//mmap将帧缓冲设备的内存映射到应用层的地址空间，无需通过系统调用进行数据读写
	//应用层可以直接通过指针进行操作，像操作普通内存一样，而不需要关心底层i/o操作
	pfb = mmap(NULL, screenlength, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if (NULL == pfb)
	{
		perror("mmap failed.");
		return -1;
	}
	printf("pfb = %p.\n", pfb);
	
	//第四步：填充framebuffer，使LCD显示画面
	draw_background(WIDTH, HEIGHT, WHITE);
	draw_line(BLACK);

	
	return 0;
}


void fb_close(void)
{
	//第五步：清理资源
	if(munmap(pfb, screenlength) == -1){
		perror("munmap failed.");
	}
	close(fbfd);
	printf("close %s success.\n", FBDEVICE);
}



void draw_background(unsigned int width, unsigned int height, unsigned int color)
{
	unsigned int x, y;
	
	for (y=0; y<height; y++)
	{
		for (x=0; x<width; x++)
		{
			*(pfb + y * WIDTH + x) = color;
		}
	}
}

void draw_line(unsigned int color)
{
	unsigned int x;
	
	for (x=50; x<600; x++)
	{
		*(pfb + 200 * WIDTH + x) = color;
	}
}














