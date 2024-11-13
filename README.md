# testproject1


**项目**：图片显示


## 引言

在开发板上实现BMP格式的图片的显示
本项目中会考虑平台可移植性，即可通过简单配置方便移植到各种开发板上（指不同大小尺寸的LCD，不同类型的触摸屏）
对应6.06-6.10。


## 开发环境

- **硬件**：开发板(S5PV210 + LCD:1024*600)
- **操作系统**：PC主机Win10 + 虚拟机Ubuntu14.04.1(基于Linux API)
- **开发工具**：Visual Studio Code + SourceInsight3.5 + vim7.4.52
- **编译器**：gcc4.8.2


## 必要的基础环境
1. 开发板uboot(uboot在iNand中)
2. 开发板kernel(使用开发板厂商提供的源码包自行修改编译得到zImage,zImage在tftp服务器中)
3. 自己制作的根文件系统rootfs
4. PC的ubuntu中配置好的tftp服务器
5. PC的ubuntu中配置好的nfs服务器


## 其他
1. 代码管理：Makefile + GitHub
2. 调试流程：Windows共享文件夹编辑、虚拟机Ubuntu中编译、make cp到nfs方式的rootfs中在开发板上运行
3. [代码位置](https://github.com/yangzongda/testproject1)


---

