#ifndef __BMP_H__
#define __BMP_H__



// BMP文件头结构,bf means bmpfile
// 使用__attribute__((packed))让结构体成员紧密排列。因为bmp的文件头数据本身是紧密排列的，所以不需要编译器帮我们对齐。
typedef struct __attribute__((packed)) tagBITMAPFILEHEADER
{
	unsigned short      bfType;   //文件的标识，值必须是0x4D42（也就是“BM”两个字符，注意是Little-Endian小端）
	unsigned long       bfSize; //是整个文件的大小
	unsigned short      bfReserved1;    //bfReserved1和bfReserved2必须是0.
	unsigned short      bfReserved2;    //bfReserved1和bfReserved2必须是0.
	unsigned long       bfOffBits;      //偏移量，即 位图文件头+位图信息头+调色板 的大小
} BITMAP_FILE_HEADER, *PBITMAPFILEHEADER;

// BMP信息头结构,bi means bmpinfo
typedef struct tagBITMAPINFOHEADER
{
	unsigned long       biSize; //BITMAPINFOHEADER这个结构体的大小。必须为40（字节
	long                biWidth;    //BMP位图的宽度，单位是像素
	long                biHeight;   //BMP位图的高度，单位是像素（有符号数，即可能为负
	unsigned short      biPlanes;   //位图的“位面数”。这个值必须为1（2字节
	unsigned short      biBitCount; //位图的“颜色位数”，颜色位数为1表示黑白，为2表示4色，为4表示16种颜色，为8表示有256种颜色。
	unsigned long       biCompression;  //位图的压缩格式
	unsigned long       biSizeImage;    //位图数据块的大小。以字节为单位。如果你的位图没有经过压缩，这个值可以是0.
	long                biXPelsPerMeter;    //表示横向的每米的像素数。可以为0.
	long                biYPelsPerMeter;    //表示纵向的每米的像素数。可以为0.
	unsigned long       biClrUsed;  //位图实际使用过的调色板的颜色数。如果这个值为0，表示这个位图使用了整个调色板。只有8位以及8位以下的索引颜色位图才需要考虑这个值。
	unsigned long       biClrImportant; //指定重要的颜色数
} BITMAP_INFO_HEADER, *PBITMAPINFOHEADER;


#endif










