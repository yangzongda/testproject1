 /* *****************************************************
 *
 * 	创建一个新文件夹/display，将fb.c放入该文件夹
 * 	在此子文件夹中添加子Makefile
 * 
 * fb.c 操作framebuffer的基础代码，包括fb的打开、ioctl获取信息
 *      基本的测试fb显示代码
 * 
 * ******************************************************
 */

#include <stdio.h>
#include <sys/types.h>	// 文件类型
#include <sys/stat.h>	// 文件状态控制
#include <fcntl.h>		// 文件控制，如O_RDONLY
#include <unistd.h>		// 文件控制，如close()
#include <linux/fb.h>	// 内核源码kernel/include/linux/fb.h。获取设备信息
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <fb.h>		// 自行创建
#include "bmp_test1.c"	// 引入bmp图片


// 定义全局变量
unsigned int *pfb = NULL;	//用于保存申请到的内存空间的首地址
					//全局变量不初始化值为零，所以此处不初始化没关系
int fbfd = -1;		//设成全局变量，因为fb_open和fb_close都要用到
unsigned int screenlength = 0;

// 定义一个全局变量，用于保存bmp图片
const unsigned char *pBmp = gImage_bmp_test1;

const unsigned int WIDTH_BMP1 = 500;
const unsigned int HEIGHT_BMP1 = 500;
//const int x_gap_bmp_test1 = 262;
//const int y_gap_bmp_test1 = 50;

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
	DBG("open %s success.\n", FBDEVICE);
	
	
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
	DBG("smem_start = 0x%lx, smem_len = %u.\n", finfo.smem_start, finfo.smem_len);
	
	ret = ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo);//获取可变信息
	if (ret < 0)
	{
		perror("ioctl");
		return -1;
	}
	DBG("xres = %u, yres = %u.\n", vinfo.xres, vinfo.yres);
	DBG("xres_virtual = %u, yres_virtual = %u.\n", vinfo.xres_virtual, vinfo.yres_virtual);
	DBG("bpp = %u.\n", vinfo.bits_per_pixel);

	
	// 第3步：进行mmap内存映射
	// 根据第二步获取到的设备信息，计算需要申请的内存大小 =长*宽*像素占几个字节
	screenlength = vinfo.xres_virtual * vinfo.yres_virtual * vinfo.bits_per_pixel / 8;
	DBG("screenlength = %d\n", screenlength);
	//mmap将帧缓冲设备的内存映射到应用层的地址空间，无需通过系统调用进行数据读写
	//应用层可以直接通过指针进行操作，像操作普通内存一样，而不需要关心底层i/o操作
	pfb = mmap(NULL, screenlength, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if (NULL == pfb)
	{
		perror("mmap failed.");
		return -1;
	}
	DBG("pfb = %p.\n", pfb);
	
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
	DBG("close %s success.\n", FBDEVICE);
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



void draw_bmp_test1(int x_gap, int y_gap, const unsigned char *pBmp)
{
	unsigned int x, y, color, p = 0;
	
	for(y = 0; y < HEIGHT_BMP1; y++)
	{
		if(y_gap + y > HEIGHT)
		{
			break;// 纵向超出屏幕，退出
		}
		for(x = 0; x < WIDTH_BMP1; x++)
		{
			if(x_gap + x > WIDTH)
			{
				p += 3;// 每次循环指向像素的p都要+3
				continue;// 横向超出屏幕，跳过下面部分(即不显示)直接开始下一次循环
			}
			// 这个图像是RGB888格式，每个像素点有三字节数据。
			// 循环的方式与图像生成方式相关，生成时从左上沿每行到右下。
			// 通过两层外循环确定像素点，将三字节颜色值按RGB顺序写入内存
			// 颜色不对就手动调，总之就是三个颜色的排列组合
			color = (pBmp[p+2] << 0) | (pBmp[p+1] << 8) | (pBmp[p+0] <<16);
			*(pfb + (y_gap + y) * WIDTH + (x_gap + x)) = color;
			p += 3;// 3字节颜色数据对应一个像素点
		}
	}

	DBG("draw_bmp_test1 ending.\n");
}


// 该函数与draw_bmp_test1()函数功能相同，但方向不同。
// draw_bmp_test1()针对的是Img2Lcd得到的.c文件，本函数针对的是真实.bmp文件
// 因为该BMP图片数据的存储方式是自底至上，自左向右，与之前使用软件得到的自上至底的数据是不同的
// 故显示函数也需要做出一定的改变
//void draw_bmp_test2(int x_gap, int y_gap, const unsigned char *pbmp_Data, long bmp_width, long bmp_height)
void draw_bmp_test2(int x_gap, int y_gap, pic_info *pPic)
{
	unsigned char *pbmp_Data = pPic->pData; // 指向图像数据
	unsigned int color, p = 0;
	int x, y;

	p = pPic->width * 3 * pPic->height -3;

	for(y = y_gap; y <(pPic->height + y_gap); y++)
	{
		//printf("y = %d ", y);
		if(y > HEIGHT)
		{
			break;// 纵向超出屏幕，结束
		}

		/*
		for(x = 499; x>=0; x--)
		{
			//printf("x = %d ", x);
			color = (pbmp_Data[p+2] << 16) | (pbmp_Data[p+1] << 8) | (pbmp_Data[p+0] <<0);
			*(pfb + y * WIDTH + x) = color;
			p -= 3;// 3字节颜色数据对应一个像素点
		}
		*/
	

		for(x =(pPic->width + x_gap -1); x >= x_gap; x--)
		{
			if(x > WIDTH)
			{
				p -= 3;// 每次循环指向像素的p都要-3
				continue;// 横向超出屏幕，跳过下面部分(即不显示)直接开始下一次循环
			}
			// 这个图像是RGB888格式，每个像素点有三字节数据。
			// 循环的方式与图像生成方式相关，生成时从左上沿每行到右下。
			// 通过两层外循环确定像素点，将三字节颜色值按RGB顺序写入内存
			// 颜色不对就手动调，总之就是三个颜色的排列组合
			color = (pbmp_Data[p+2] << 16) | (pbmp_Data[p+1] << 8) | (pbmp_Data[p+0] <<0);
			*(pfb + y * WIDTH + x) = color;
			p -= 3;// 3字节颜色数据对应一个像素点
		}

	}

	DBG("draw_bmp_test2 ending.\n");
}














