
# 指定编译器种类，若未将交叉编译链路径导出环境变量，则需使用绝对路径
# 我的交叉编译工具链的路径在/usr/local/arm/arm-2009q3/bin目录中，即绝对路径
#CROSS_COMPILE = /usr/local/arm/arm-2009q3/bin/arm-linux-
CROSS_COMPILE = /usr/local/arm/arm-2009q3/bin/arm-none-linux-gnueabi-

AS			= $(CROSS_COMPILE)as
LD			= $(CROSS_COMPILE)ld
CC			= $(CROSS_COMPILE)gcc
CPP			= $(CC) -E
AR			= $(CROSS_COMPILE)ar
NM			= $(CROSS_COMPILE)nm
STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump


# export导出的变量是给子目录下的Makefile使用的
export AS LD CC CPP AR NM STRIP OBJCOPY OBJDUMP


# 编译器在编译时的参数设置
CFLAGS := -Wall -O2 -g -DDEBUG
# 添加头文件路径，指定编译器头文件所在目录，不添加的话include目录下的头文件编译时找不到
# 头文件一般放两个位置，.c文件当前文件目录下，或include目录中
CFLAGS += -I $(shell pwd)/include


# 链接器的链接参数设置
LDFLAGS :=


export CFLAGS LDFLAGS

# 获得当前程序的顶层目录
TOPDIR := $(shell pwd)
export TOPDIR


# 定义将来编译生成的可执行程序的名字
TARGET := image_player


# 保证被编译进内核
# 添加顶层目录下的.c文件对应的.o文件
obj-y += main.o# 对应顶层目录下的main.c

# 添加顶层目录下的子文件夹（注意目录名后面加一个/）（子文件夹中的子文件夹在子文件的Makefile中添加）
obj-y += display/


# all是默认make的对象。built-in.o是第一步编译生成的，和Makefile.build有关
all: 
	make -C ./ -f $(TOPDIR)/Makefile.build
	$(CC) $(LDFLAGS) -o $(TARGET) built-in.o

# 通过nfs拷贝到开发板里，从项目目录复制到根文件系统目录
cp:
	cp ../testproject1/ /home/aston/rootfs/rootfs/ -rf

clean:
	rm -f $(shell find -name "*.o")
	rm -f $(TARGET)

# .d文件是编译过程中生成的依赖文件
distclean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.d")
	rm -f $(TARGET)
	