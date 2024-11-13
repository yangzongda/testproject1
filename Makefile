
# ָ�����������࣬��δ�����������·��������������������ʹ�þ���·��
# �ҵĽ�����빤������·����/usr/local/arm/arm-2009q3/binĿ¼�У�������·��
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


# export�����ı����Ǹ���Ŀ¼�µ�Makefileʹ�õ�
export AS LD CC CPP AR NM STRIP OBJCOPY OBJDUMP


# �������ڱ���ʱ�Ĳ�������
CFLAGS := -Wall -O2 -g -DDEBUG
# ���ͷ�ļ�·����ָ��������ͷ�ļ�����Ŀ¼������ӵĻ�includeĿ¼�µ�ͷ�ļ�����ʱ�Ҳ���
# ͷ�ļ�һ�������λ�ã�.c�ļ���ǰ�ļ�Ŀ¼�£���includeĿ¼��
CFLAGS += -I $(shell pwd)/include


# �����������Ӳ�������
LDFLAGS :=


export CFLAGS LDFLAGS

# ��õ�ǰ����Ķ���Ŀ¼
TOPDIR := $(shell pwd)
export TOPDIR


# ���彫���������ɵĿ�ִ�г��������
TARGET := image_player


# ��֤��������ں�
# ��Ӷ���Ŀ¼�µ�.c�ļ���Ӧ��.o�ļ�
obj-y += main.o# ��Ӧ����Ŀ¼�µ�main.c

# ��Ӷ���Ŀ¼�µ����ļ��У�ע��Ŀ¼�������һ��/�������ļ����е����ļ��������ļ���Makefile����ӣ�
obj-y += display/


# all��Ĭ��make�Ķ���built-in.o�ǵ�һ���������ɵģ���Makefile.build�й�
all: 
	make -C ./ -f $(TOPDIR)/Makefile.build
	$(CC) $(LDFLAGS) -o $(TARGET) built-in.o

# ͨ��nfs�����������������ĿĿ¼���Ƶ����ļ�ϵͳĿ¼
cp:
	cp ../testproject1/ /home/aston/rootfs/rootfs/ -rf

clean:
	rm -f $(shell find -name "*.o")
	rm -f $(TARGET)

# .d�ļ��Ǳ�����������ɵ������ļ�
distclean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.d")
	rm -f $(TARGET)
	