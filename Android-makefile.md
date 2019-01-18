# <font color=#0099ff> **安卓编译相关** </font> 

> `@think3r` 2019-01-18 22:02:05  

## <font color=#009A000> 参考链接: </font> 

- [linux和Android的Makefile和android.mk](https://www.cnblogs.com/maogefff/p/7910441.html#_label1)

## <font color=#009A000> 0x00 概述 </font> 

- 安卓下的 Makefile 可以生成如下四种代码成果物:
    1. `JAVA`  代码的 `JAR 包` ;
    2. `JAVA`  代码的 `APK 应用程序` ;
    3. `C/C++` 代码的 `二进制可执行文件 ELF(BUILD_EXECUTABLE)` ;
    4. `C/C++` 代码的库文件:
        - 静态库 `*.so (BUILD_SHARED_LIBRARY)`
        - 静态库  `*.a   (BULID_STATIC_LIBRARY)`

## <font color=#009A000> 0x01 *.mk </font> 

- Android.mk 是一个向 Android NDK 构建系统描述 NDK 项目的 GUN Makefile 片段。它是每一个 NDK 项目的必备组件。构建系统希望它出现在 jni 子目录中。下面是 hello-jni 项目中 Android.mk 文件的内容。
    - Application.mk 用来描述你的工程下的 native 模块, 一般放在 $PROJECT/jni/Application.mk, 其中 $PROJECT 表示你的工程目录，这样就可以被 ndk-build 脚本文件找到. (注：在这种方式下，进入 jni 目录，即 $PROJECT/jni/,然后执行 ndk-build，就可以直接编译 jni 生成 .so 文件了
    - Android.mk 文件语法允许我们将 Source 打包成一个"modules". modules 可以是：静态库 或者 动态库。
- Android.mk 文件首先必须要指定 LOCAL_PATH 变量，用于查找源文件。由于一般情况下 Android.mk 和需要编译的源文件在同一目录下，所以定义成如下形式：
    - `LOCAL_PATH := $(call my-dir)` 将 LOCAL_PATH 变量定义成本文件所在目录路径;
    - `parent-makefile` 返回调用树中父 Makefile 路径。即包含当前Makefile的Makefile 路径
    
- Android.mk 中可以定义多个编译模块，每个编译模块都是 
    - 以 `include $(CLEAR_VARS)` 开始，
        -CLEAR_VARS:指向一个编译脚本
        - `CLEAR_VARS` 由编译系统提供，指定让 GNU MAKEFILE 为你清除除 LOCAL_PATH 以外的所有 LOCAL_XXX 变量，清楚它们可以避免冲突.
    - 以 `include $(BUILD_XXX)` 结束
        - `BUILD_EXECUTABLE` 生成 C/C++ 可执行文件;

- `LOCAL_C_INCLUDES` : 可选变量，表示头文件的搜索路径
    - 默认的头文件的搜索路径是 LOCAL_PATH 目录。     
- `LOCAL_CFLAGS`
    - `LOCAL_CFLAGS-D`， 增加全局宏定义。
    - `LOCAL_CFLAGS := -DUSE_JSC` 相当于在所有源文件中增加一个#define USE_JSC
    - `LOCAL_CFLAGS := -DUSE_COPY_BIT=1` 相当于在所有源文件中增加一个#define USE_COPY_BIT 1
- `LOCAL_MODULE` 变量用来给这些模块设定一个唯一的名称(编译生成的目标名称):
    - `LOCAL_MODULE := hello-jni`
- `LOCAL_SRC_FILES` 变量定义用来建立和组装这个模块的源文件列表，
    - `LOCAL_SRC_FILES := hello-jni.c`
- `LOCAL_LDLIBS` : 编译模块时要使用的附加的链接器选项。这对于使用 `-l` 前缀传递指定库的名字是有用的。 
    - `LOCAL_LDLIBS := -lz` 表示告诉链接器生成的模块要在加载时刻链接到 `/system/lib/libz.so`
- `LOCAL_SHARED_LIBRARIES` 表示模块在运行时要依赖的共享库（动态库），在链接时就需要，以便在生成文件时嵌入其相应的信息。 
- `LOCAL_STATIC_LIBRARIES` 表示该模块需要使用哪些静态库，以便在编译时进行链接。 
    - 只是加载静态库中用到的函数
- `LOCAL_WHOLE_STATIC_LIBRARIES` 会加载整个静态库.
- `LOCAL_JNI_SHARED_LIBRARIES` 定义了要包含的 so 库文件的名字，如果程序没有采用 jni，不需要;
    - `LOCAL_JNI_SHARED_LIBRARIES := libxxx` 这样在编译的时候，NDK 自动会把这个 libxxx 打包进 apk； 放在 `your-apk/lib/` 目录下
    
## <font color=#009A000> Android Make </font> 

- 对于 android 而言，android 使用的是 GNU 的 make，因此它的 makefile 格式也是 GNU 的 makefile 格式。现在网络上关于 makefile 最好的文档就是陈皓的《跟我一起写makefile》，这份文档对 makefile 进行了详细的介绍，因此推荐大家先看这份文档


- android make 系统总共分为四层
    - arch
    - board
    - device
    - product
    
- 在一个 makefile 文件中，可以一次性的 make 出多个目标！    
    - 每个编译模块都是以include $(CLEAR_VARS)开始，以include $(BUILD_XXX)结束。
- 常用的 mk 分类:
    - Android.mk 用来编译模块或者 apk 的，module 对应 native code，package 对应于 java
    - AndroidProducts.mk 设置product，设置系统包含了那些应用
    - target_<os>-<arch>.mk，host_<os>-<arch>.mk，<os>-<arch>.mk 针对不同的系统和CPU架框进行设置
    - BoardConfig.mk 设置主板用的，比如driver的选择
    - ./build/core/Makefile 定义了image是如何生成的
    - 

## <font color=#009A000> 0x03 深入理解 安卓 bulid 系统 </font> 

> [理解 Android Build 系统](https://www.ibm.com/developerworks/cn/opensource/os-cn-android-build/)

### <font color=#FF4500> 简介概述 </font> 

-   Android Build 系统用来编译 Android 系统，Android SDK 以及相关文档。该系统主要由 Make 文件，Shell 脚本以及 Python 脚本组成，其中最主要的是 Make 文件。众所周知，Android 是一个开源的操作系统。Android 的源码中包含了大量的开源项目以及许多的模块。不同产商的不同设备对于 Android 系统的定制都是不一样的。如何将这些项目和模块的编译统一管理起来，如何能够在不同的操作系统上进行编译，如何在编译时能够支持面向不同的硬件设备，不同的编译类型，且还要提供面向各个产商的定制扩展，是非常有难度的。但 Android Build 系统很好的解决了这些问题，这里面有很多值得我们开发人员学习的地方。

- Build 系统中最主要的处理逻辑都在 Make 文件中，而其他的脚本文件只是起到一些辅助作用，由于篇幅所限，本文只探讨 Make 文件中的内容。

### <font color=#FF4500> makefile 分类 </font> 

整个 Build 系统中的 Make 文件可以分为三类：

1. 第一类是 Build 系统核心文件，此类文件定义了整个 Build 系统的框架，而其他所有 Make 文件都是在这个框架的基础上编写出来的。Build 系统核心文件全部位于 `/build/core` （本文所提到的所有路径都是以 Android 源码树作为背景的，`/` 指的是源码树的根目录，与文件系统无关）目录下。
2. 第二类是针对某个产品（一个产品可能是某个型号的手机或者平板电脑）的 Make 文件，这些文件通常位于 device 目录下，该目录下又以公司名以及产品名分为两级目录. 对于一个产品的定义通常需要一组文件，这些文件共同构成了对于这个产品的定义。例如，`/device/sony/it26` 目录下的文件共同构成了对于 Sony LT26 型号手机的定义。
3. 第三类是针对某个模块（关于模块后文会详细讨论）的 Make 文件。整个系统中，包含了大量的模块，每个模块都有一个专门的 Make 文件，这类文件的名称统一为 `Android.mk` ，该文件中定义了如何编译当前模块。Build 系统会在整个源码树中扫描名称为 `Android.mk` 的文件并根据其中的内容执行模块的编译。

## <font color=#009A000> 编译 Android 系统 </font> 

- Android 系统的编译环境目前只支持 Ubuntu 以及 Mac OS 两种操作系统。关于编译环境的构建方法请参见以下路径：http://source.android.com/source/initializing.html

- 获取到安卓源码后, 在其目录下执行如下的三条命令即可:
    ````sh
    source build/envsetup.sh 
    lunch full-eng 
    make -j8
    ````
    - 第一行命令 `source build/envsetup.sh` 引入了 `build/envsetup.sh` 脚本。该脚本的作用是初始化编译环境，并引入一些辅助的 Shell 函数，这其中就包括第二步使用 lunch 函数。
        - `croot` 切换到源码树的根目录;
        - `m`   在源码树的根目录下执行 `make`;
        - `mm` build 当前目录下的模块;
        - `mmm` bulid 指定目录下的模块;
        - 等等......
    - 第二行命令 `lunch full-eng` 是调用 lunch 函数，并指定参数为 `full-eng` 。lunch 函数的参数用来指定此次编译的目标设备以及编译类型。在这里，这两个值分别是 “full”和“eng”。  
        - “full”是 Android 源码中已经定义好的一种产品，是为模拟器而设置的。
        - 如果调用 lunch 函数的时候没有指定参数，那么该函数将输出列表以供选择（列表的内容会根据当前 Build 系统中包含的产品配置而不同，具体参见后文“添加新的产品”），此时可以通过输入编号或者名称进行选择。
    - 第三行命令 `make -j8` 才真正开始执行编译。make 的参数 `-j` 指定了同时编译的 Job 数量，这是个整数，该值通常是编译主机 CPU 支持的并发线程总数的 1 倍或 2 倍（例如：在一个 4 核，每个核支持两个线程的 CPU 上，可以使用 make -j8 或 make -j16）。在调用 make 命令时，如果没有指定任何目标，则将使用 **默认** 的名称为 `droid` 目标，该目标会编译出完整的 Android 系统镜像。

## <font color=#009A000> 编译结果路径说明 </font> 

- Build 结果的目录结构如下:
    - 所有的编译产物都将位于 /out 目录下，该目录下主要有以下几个子目录：
        - `/out/host/` ：该目录下包含了 **针对主机的 Android 开发工具的产物** 。即 SDK 中的各种工具，例如：emulator，adb，aapt 等。
        - `/out/target/common/` ：该目录下包含了针对设备的共通的编译产物，主要是 Java 应用代码和 Java 库。
        - `/out/target/product/<product_name>/` ：包含了针对特定设备的编译结果以及平台相关的 `C/C++` 库和二进制文件。其中，<product_name>是具体目标设备的名称。
        - `/out/dist/` ：包含了为多种分发而准备的包，通过 `make disttarget` 将文件拷贝到该目录，默认的编译目标不会产生该目录。
- Build 生成的镜像文件:
    - Build 的产物中最重要的是三个镜像文件，它们都位于 `/out/target/product/<product_name>/` 目录下。
        - `system.img` ：包含了 Android OS 的系统文件，库，可执行文件以及预置的应用程序，将被挂载为根分区。

        - `ramdisk.img` ：在启动时将被 Linux 内核挂载为只读分区，它包含了 /init 文件和一些配置文件。它用来挂载其他系统镜像并启动 init 进程。
        - `userdata.img` ：将被挂载为 /data，包含了应用程序相关的数据以及和用户相关的数据。

## <font color=#009A000> make 文件说明 </font> 

- 整个 Build 系统的入口文件是源码树根目录下名称为“Makefile”的文件，当在源代码根目录上调用 make 命令时，make 命令首先将读取该文件。Makefile 文件的内容只有一行：“include build/core/main.mk”。该行代码的作用很明显：包含 build/core/main.mk 文件。在 main.mk 文件中又会包含其他的文件，其他文件中又会包含更多的文件，这样就引入了整个 Build 系统。
    - 此处略去, 不再此细讲;

- Android 源码中包含了许多的模块，模块的类型有很多种，例如：Java 库，C/C++ 库，APK 应用，以及可执行文件等。 并且，Java 或者 C/C++ 库还可以分为静态的或者动态的，库或可执行文件既可能是针对设备（本文的 “设备” 指的是 Android 系统将被安装的设备，例如某个型号的手机或平板）的也可能是针对主机（本文的“主机”指的是开发 Android 系统的机器，例如装有 Ubuntu 操作系统的 PC 机或装有 MacOS 的 iMac 或 Macbook）的。不同类型的模块的编译步骤和方法是不一样，为了能够一致且方便的执行各种类型模块的编译，在 `config.mk` 中定义了许多的常量，这其中的每个常量描述了一种类型模块的编译方式，这些常量有：
    - BUILD_HOST_STATIC_LIBRARY
    - BUILD_HOST_SHARED_LIBRARY
    - BUILD_STATIC_LIBRARY
    - BUILD_SHARED_LIBRARY
    - BUILD_EXECUTABLE
    - BUILD_HOST_EXECUTABLE
    - BUILD_PACKAGE
    - BUILD_PREBUILT
    - BUILD_JAVA_LIBRARY
    - BUILD_STATIC_JAVA_LIBRARY
    - BUILD_HOST_JAVA_LIBRARY
- 这些常量的值都是另外一个 Make 文件的路径，详细的编译方式都是在对应的 Make 文件中定义的。这些常量和 Make 文件的是一一对应的，对应规则也很简单：常量的名称是 Make 文件的文件名除去后缀全部改为大写然后加上“BUILD_”作为前缀。例如常量 BUILD_HOST_PREBUILT 的值对应的文件就是 host_prebuilt.mk。

## <font color=#009A000> 添加新的模块 </font> 

- 在源码树中，一个模块的所有文件通常都位于同一个文件夹中。为了将当前模块添加到整个 Build 系统中，每个模块都需要一个专门的 Make 文件，该文件的名称为“Android.mk”。Build 系统会扫描名称为“Android.mk”的文件，并根据该文件中内容编译出相应的产物。

- 需要注意的是：在 Android Build 系统中，编译是以模块（而不是文件）作为单位的，每个模块都有一个唯一的名称，一个模块的依赖对象只能是另外一个模块，而不能是其他类型的对象。对于已经编译好的二进制库，如果要用来被当作是依赖对象，那么应当将这些已经编译好的库作为单独的模块。对于这些已经编译好的库使用 BUILD_PREBUILT 或 BUILD_MULTI_PREBUILT。例如：当编译某个 Java 库需要依赖一些 Jar 包时，并不能直接指定 Jar 包的路径作为依赖，而必须首先将这些 Jar 包定义为一个模块，然后在编译 Java 库的时候通过模块的名称来依赖这些 Jar 包。


