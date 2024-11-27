#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <bmp.h>
#include <fb.h>

// 规定能使用的图片的最大分辨率
// 规定bpp最大为24（8bit*3，RGB888）
#define BMP_MAX_RESOLUTION (1920*1080)
#define BMP_BUF_SIZE (BMP_MAX_RESOLUTION*3)

// 存储bmp图片的数据
unsigned char bmp_buf[BMP_BUF_SIZE];


// 用结构体的方式分析bmp图片
// 传入的参数是自行封装的结构体，结构体为了储存图片信息。以结构体作为函数的参数
// 传入的结构体中只含有图片的路径，其他信息在本函数中获取
int is_bmp(pic_info *pPic)
{
    BITMAP_FILE_HEADER fHeader;
    BITMAP_INFO_HEADER iHeader;
    ssize_t fd = -1, ret = -1;
    size_t len = 0;


    fd = open(pPic->pathname,O_RDONLY);
    if(fd < 0)
    {
        fprintf(stderr,"open %s failed\n",pPic->pathname);
        close(fd);
        return -1;
    }

    //  第二步：读取bmp文件头部信息，得到该BMP图片的详细信息
	ret = read(fd, &fHeader, sizeof(fHeader));// 读取文件头
    if(-1 == ret)
    {
        fprintf(stderr,"read %s fHeader failed\n",pPic->pathname);
        close(fd);
        return -1;
    }
    DBG("bfType = %x.\n", fHeader.bfType);
	DBG("bfSize = %ld.\n", fHeader.bfSize);
	DBG("bfOffBits = %ld.\n", fHeader.bfOffBits);

    ret = read(fd, &iHeader, sizeof(iHeader));// 读取信息头
    if(-1 == ret)
    {
        fprintf(stderr,"read %s iHeader failed\n",pPic->pathname);
        close(fd);
        return -1;
    }
	DBG("bmp resolution = %ld*%ld.\n", iHeader.biHeight,iHeader.biWidth);
	DBG("bmp bpp = %d.\n", iHeader.biBitCount);
    pPic->width = iHeader.biWidth;// 将这一步获取到的数据录入封装的结构体中
    pPic->height = iHeader.biHeight;
    pPic->bpp = iHeader.biBitCount;

    // 第三步：读取图片数据
    // 跳过文件头和信息头，文件指针跳到图片数据开始的地方，读取图片数据
    lseek(fd, fHeader.bfOffBits, SEEK_SET);
    len = iHeader.biWidth * iHeader.biHeight * (iHeader.biBitCount / 8);
    ret = read(fd, bmp_buf, len);
    if(-1 == ret)
    {
        fprintf(stderr,"read %s bmp_buf failed\n",pPic->pathname);
        close(fd);
        return -1;
    }
    pPic->pData = bmp_buf;

    // 第四步：将得到的有效数据丢到fb中去显示
    draw_bmp_test2(700, 100, pPic);

    close(fd);

    return 0;
}



// 用数组的方式分析bmp图片
// 这种方式只是为了实验探索bmp图片的格式
// 不推荐使用这种分析方式。需要提前获取文件头每位偏移量对应的信息
int bmp_analyze(const char *path)
{
    int fd = -1;
    unsigned char buf[54] = {0};// bmp的文件头占54个字节

    // 打开bmp文件
    fd = open(path,O_RDONLY);
    if(fd < 0)
    {
        fprintf(stderr,"open %s failed\n",path);
        close(fd);
        return -1;
    }

    // 读取文件头，判断图片格式是否为bmp
    read(fd,buf,54);

    if(buf[0] != 'B' || buf[1] != 'M')
    {
        fprintf(stderr,"%s is not a bmp file\n",path);
        close(fd);
        return -1;
    }
    
    // 打印bmp图片的宽高，每个像素的位数
    DBG("width:%d\n",*(int *)(buf+18));//0012h
    DBG("height:%d\n",*(int *)(buf+22));//0016h
    DBG("bit per pixel:%d\n",*(short *)(buf+28));//001Ch

    // 将得到的有效数据丢到fb中去显示

    // 释放资源
    close(fd);
    
    return 0;
}








