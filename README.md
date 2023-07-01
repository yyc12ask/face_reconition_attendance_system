# 人脸识别考勤机

## 问题总结

常用解决问题办法：按照报错信息添加依赖等、仔细查看项目课件、百度报错信息

**编译报错**：/usr/bin/ld: /tmp/ccOYY87W.o: undefined reference to symbol '_ZN2cv6String10deallocateEv'
//usr/lib/x86_64-linux-gnu/libopencv_core.so.3.2: 无法添加符号: DSO missing from command line
collect2: error: ld returned 1 exit status

- 解决：编译c++文件时缺少必要的依赖，g++编译时需要添加两个库opencv_videoio 和lopencv_core：` g++ text.cpp -o text -l opencv_videoio -lopencv_core`

多打log（printf函数）便于调试，从log分析

## 项目主体

环境搭建：VirtualBox虚拟机＋Ubuntu18.04

语言：c＋＋

项目框架：opencv

硬件:摄像头、电脑

开发流程：

- 图像采集（拍照片），需要程序驱动摄像头去采集照片
- Linux人脸检测，将脸截图出来，使用OpenCV库，opencv只负责人脸检测，照片那块区域是人脸，无法完成人脸识别，人脸识别借助了百度智能云
- opencv将照片上的人脸截图，然后发送给百度智能云，百度智能云完成人脸对比工作，百度智能云先分析人脸照片的特征，然后和人脸库中的人脸做对比，并找出人脸是哪个人的脸，然后百度智能云通过互联网发送出对比结果

OpenCV

- OpenCV是一个开源的计算机视觉和机器学习软件库其使用一系列C语言函数和少量C++类实现，内部实现了很多图像处理和计算机视觉的通用算法；OpenCV可以运行在Linux系统上，且其轻量、高效所以在嵌入式领域得到广泛的应用；

- 在线安装：sudo apt-get install libopencv-dev
  - 注意：执行此命令前先执行apt-get update。apt-get下载某个包中它的所有依赖项都必须存在，这就是为什么我们每次执行apt-get的时候都需要先apt-get update的更新软件包的原因。
- 查看安装状态：dpkg -s libopencv-dev
- 安装版本：3.2.0

- 在线文档：https://opencv.org/

virtualbox接摄像头

- 乌邦图在能接到摄像头前需要在virtualbox中安装插件，根据virtualbox版本可以在这里找自己需要的驱动插件：https://www.virtualbox.org/wiki/Download_Old_Builds
- 点击virtualBox中：管理 –> 全局设定 –> 扩展 –> 添加新包
- 选择刚下载好的驱动安装，安装成功则提示成功安装（如果不成功需要看版本是否匹配）
- 接下来在 虚拟机设备-摄像头-Integrated Camera勾选即可在虚拟机内使用摄像头了

查询Linux下摄像头编号：

- 打开Linux终端，使用命令`ls /dev/video*`即可查询摄像头编号

连同opencv库共同编译c++文件命令：` g++ text.cpp -o text -l opencv_videoio -lopencv_core`

​	解释：opencv是第三方库，编译器不能从系统中找到该库，代码中使用了VideoCapture，VideoCapture处于opencv_videoio库中，所以编译时需要用-l来指定链接用到的opencv_videoio库

g++是gcc默认语言设为c++的一个特殊版本，连接时自动使用c++标准库而不使用c标准库，如果使用gcc编译c++程序可以使用命令 `gcc  text.cpp -o text -l opencv_videoio -lopencv_core -lstdc++`，选项 -l (ell) 通过添加前缀 lib 和后缀 .a 将跟随它的名字变换为库的名字 libstdc++.a。而后它在标准库路径中查找该库。gcc 的编译过程和输出文件与 g++ 是完全相同的。

执行编译后的文件：`./text`

### 图像采集

一秒切换24帧就可以实现图片转成图像，这里延时40ms

控制台退出正在执行的进程：CTRL＋c

连同opencv库共同编译c++文件命令：` g++ text.cpp -o text -l opencv_videoio -lopencv_core -lopencv_highgui `

### 图像处理

- 彩色照片信息量过大，需要转化为黑白照片以提高准确度并降低难度

- 灰度图人脸轮廓不够清晰

解决方法：直方图均匀化，通过调节照片亮度和对比度来区分脸和背景，使脸更清晰

编译命令： `g++ text.cpp -o text -l opencv_videoio -lopencv_core -lopencv_highgui -lopencv_imgproc`

### 人脸检测

这里使用现成的模型来让opencv进行学习人脸

已经训练好的模型路径： `/usr/share/opencv/haarcascades`

编译命令：`g++ text.cpp -o text -l opencv_videoio -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect`

### 人脸截取

- 截取人脸

- 转图片为统一格式

编译命令： `g++ text.cpp -o text -l opencv_videoio -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -lopencv_imgcodecs`

### 人脸识别

- 通过百度智能云创建人脸库

- 搭建人脸识别SDK环境

  - ### 安装人脸识别 C++ SDK

    **人脸识别 C++ SDK目录结构**

    ```text
    ├── base
    │  ├── base.h                                // 请求客户端基类
    │  ├── base64.h                              // base64加密相关类
    │  ├── http.h                                // http请求封装类
    │  └── utils.h                               // 工具类
    └── face.h                             // 人脸识别 交互类
    ```

    **最低支持 C++ 11+**

    **直接使用开发包步骤如下**：

    1.在[官方网站](http://ai.baidu.com/sdk)下载C++ SDK压缩包。

    2.将下载的`aip-cpp-sdk-version.zip`解压, 其中文件为包含实现代码的头文件。

    3.安装依赖库libcurl4-openssl-dev（需要支持https）、openssl 、libjsoncpp-dev(>1.6.2版本，0.x版本将不被支持)、libssl-dev。

    4.编译工程时添加 C++11 支持 (gcc/clang 添加编译参数 -std=c++11), 添加第三方库链接参数 lcurl, lcrypto, ljsoncpp。

    5.在源码中include face.h ，引入压缩包中的头文件以使用aip命名空间下的类和方法。

    ### 新建client

    client是人脸识别的C++客户端，为使用人脸识别的开发人员提供了一系列的交互方法。当您引入了相应头文件后就可以新建一个client对象

    用户可以参考如下代码新建一个client：

    

    ```c++
        #include "face.h"
    
        // 设置APPID/AK/SK
        std::string app_id = "你的 App ID";
        std::string api_key = "你的 Api key";
        std::string secret_key = "你的 Secret Key";
    
        aip::Face client(app_id, api_key, secret_key);
    ```

    在上面代码中，常量`APP_ID`在百度云控制台中创建，常量`API_KEY`与`SECRET_KEY`是在创建完毕应用后，系统分配给用户的，均为字符串，用于标识用户，为访问做签名验证，可在AI服务控制台中的**应用列表**中查看。

    **注意**：如您以前是百度云的老用户，其中`API_KEY`对应百度云的“Access Key ID”，`SECRET_KEY`对应百度云的“Access Key Secret”。

    

    注意：百度sdk里面base目录下的base和http文件中代码需要修改，将两者里面的 `#include <json/json.h>`改成 `#include <jsoncpp/json/json.h>`即可

  - 编译时需要将文件放在一起，并使用命令： `g++ text.cpp -o text -l opencv_videoio -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -lopencv_imgcodecs -std=c++11 -lcurl -lcrypto -ljsoncpp`

    编译通过后表明环境搭建完成
  
- 涉及到的问题：

  - 调用人脸搜索时没有成员变量search。解决方式：百度提交工单有百度工程师进行解答，SDK版本下载需要为最新版本4.16.5的c http sdk，打开face.h文档里面定义的方法版本为face_search_v3，需要传递四个参数，最后一个参数先定义一个变量  Json::Value options然后传进来就行了

识别结果打印

```
识别成功：
{
	"cached" : 0,
	"error_code" : 0,
	"error_msg" : "SUCCESS",
	"log_id" : 860269363,
	"result" : 
	{
		"face_token" : "96b935e845a87ad85aec4c14628cd8cc",
		"user_list" : 
		[
			{
				"group_id" : "testing",
				"score" : 93.500564575195,
				"user_id" : "ycc",
				"user_info" : ""
			}
		]
	},
	"timestamp" : 1688127260
}
识别失败：
{
	"cached" : 0,
	"error_code" : 222202,
	"error_msg" : "pic not has face",
	"log_id" : 2483567221,
	"result" : null,
	"timestamp" : 1688128883
}

```

### 数据处理

- 解析json数据
  - 程序不显示没有脸的结果信息
  - 识别相似度低于80%的结果直接放弃，高于80则认为是正确的结果
  - 只显示需要的数据

- 记录系统时间
  - time方法获取的时间是从1970年1月1日0时0分0秒到现在的秒数，使用ctime可以将总秒数换算成具体的时间
- 记录考勤信息到文件中，这里使用输出重定向 `./App >> log.txt`



## 移植开发板

在移植前需要让开发板能运行c++程序，先使用交叉编译测试c++程序是否能在开发板上运行

经过测试乌邦图上没有交叉编译c++的工具链，在网上查找乌邦图基于c++的交叉编译工具链应该为arm-linux-gnueabihf或者arm-linux-gnueabi，参考链接http://t.csdn.cn/EcFVl

没有移植的教程和基础，以后再尝试（还要在开发板上控制摄像头等外设，肯定需要其他软件或者需要增加驱动的代码）

![2023-06-30 231456](https://cdn.staticaly.com/gh/yyc12ask/typora_img@main/img/2023-06-30 231456.png)



