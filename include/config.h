#ifndef __CONFIG_H__
#define __CONFIG_H__

// debug宏的定义
//#define DEBUG   // 想关闭就注释掉。此处注释掉是因为Makefile编译参数中有设置 -DDEBUG
#ifdef DEBUG
#define DBG(...)							\
	do{										\
	fprintf(stderr, " DBG(%s, %s(), %d): ",	\
	__FILE__, __FUNCTION__, __LINE__);		\
    fprintf(stderr, __VA_ARGS__);			\
	}while(0)
#else

#define DBG(...)
#endif


// 用于封装一张图片的相关信息
typedef struct
{
	const char *pathname;     //图片在文件系统中的路径+文件名
	long width; //图片分辨率之宽
	long height;//图片分辨率之高
	unsigned short bpp;   //图片的位图深度bpp
	unsigned char *pData;		//指向存储图片有效数据的buf
}pic_info;


#endif




