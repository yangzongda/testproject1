 /*
 * 	����һ�����ļ���/display����fb.c������ļ���
 * 	�ڴ����ļ����������Makefile
 * 
 * fb.c ����framebuffer�Ļ������룬����fb�Ĵ򿪡�ioctl��ȡ��Ϣ
 *      �����Ĳ���fb��ʾ����
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

#include "fb.h"		//���д���



// ����ȫ�ֱ���
unsigned int *pfb = NULL;	//���ڱ������뵽���ڴ�ռ���׵�ַ
					//ȫ�ֱ�������ʼ��ֵΪ�㣬���Դ˴�����ʼ��û��ϵ
int fbfd = -1;		//���ȫ�ֱ�������Ϊfb_open��fb_close��Ҫ�õ�
unsigned int screenlength = 0;


int fb_open(void)
{
	int ret = -1;
	
	//{0}�Ǹ���ʼ���б����ڳ�ʼ���ṹ�塢����ȸ������͡���Ա��Ϊ0
	struct fb_fix_screeninfo finfo = {0};//���ڻ�ȡframebuffer���ɱ���Ϣ
	struct fb_var_screeninfo vinfo = {0};//���ڻ�ȡframebuffer�ɱ���Ϣ
	

	//framebuffer�Ĳ���ͨ����������4������׼����������ܿ�ֱ�ӽ����������޸�
	// ��1�������豸
	fbfd = open(FBDEVICE, O_RDWR);
	if (fbfd < 0)
	{
		perror("open /dev/fb0");
		return -1;
	}
	printf("open %s success.\n", FBDEVICE);
	
	
	// ��2������ȡ�豸��Ӳ����Ϣ
	
	//�ַ�����ͨ���Ĺ����п����أ�����д������
	//��Ҫ��չ�µĹ��ܣ�һ��������ioctl()����ķ�ʽʵ��
	
	//FBIOGET_FSCREENINFO�����ڻ�ȡ֡�����豸��ioctl������룬
	//��������Linuxͷ�ļ��С��������㶨�������Ӧ�ò�ioctl���е���
	ret = ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo);//��ȡ���ɱ���Ϣ
	if (ret < 0)
	{
		perror("ioctl");//���������Ϣ��stderr������Ϣ�������һ��ϵͳ����ʧ�ܵ�ԭ��
		return -1;
	}
	//��ӡ�Ľṹ����Ϣ����ioctl���������ȡ����
	printf("smem_start = 0x%lx, smem_len = %u.\n", finfo.smem_start, finfo.smem_len);
	
	ret = ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo);//��ȡ�ɱ���Ϣ
	if (ret < 0)
	{
		perror("ioctl");
		return -1;
	}
	printf("xres = %u, yres = %u.\n", vinfo.xres, vinfo.yres);
	printf("xres_virtual = %u, yres_virtual = %u.\n", vinfo.xres_virtual, vinfo.yres_virtual);
	printf("bpp = %u.\n", vinfo.bits_per_pixel);

	
	// ��3��������mmap�ڴ�ӳ��
	// ���ݵڶ�����ȡ�����豸��Ϣ��������Ҫ������ڴ��С =��*��*����ռ�����ֽ�
	screenlength = vinfo.xres_virtual * vinfo.yres_virtual * vinfo.bits_per_pixel / 8;
	printf("screenlength = %d\n", screenlength);
	//mmap��֡�����豸���ڴ�ӳ�䵽Ӧ�ò�ĵ�ַ�ռ䣬����ͨ��ϵͳ���ý������ݶ�д
	//Ӧ�ò����ֱ��ͨ��ָ����в������������ͨ�ڴ�һ����������Ҫ���ĵײ�i/o����
	pfb = mmap(NULL, screenlength, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if (NULL == pfb)
	{
		perror("mmap failed.");
		return -1;
	}
	printf("pfb = %p.\n", pfb);
	
	//���Ĳ������framebuffer��ʹLCD��ʾ����
	draw_background(WIDTH, HEIGHT, WHITE);
	draw_line(BLACK);

	
	return 0;
}


void fb_close(void)
{
	//���岽��������Դ
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














