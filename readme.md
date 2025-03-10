# <font color=#0099ff> **openGL 学习笔记** </font>

> `@think3r` 2018-05-09 22:56:27 <br>
> 参考链接 :
> 1. [OpenGL学习路径](https://cstsinghua.github.io/2018/07/12/openGL%E5%AD%A6%E4%B9%A0%E8%B7%AF%E5%BE%84/)
> 2. [Android 开发者官方开发者指南](https://developer.android.google.cn/guide?hl=zh_cn)
> 3. [learnopengl](https://learnopengl-cn.github.io/) 
> 4. [OpenGL-wikipedia](https://zh.wikipedia.org/wiki/OpenGL)
> 5. [OPENGL ES 3.0编程指南  原书第2版 --- github](https://github.com/danginsburg/OpenGL-ES3-book)
> 6. [https://github.com/KhronosGroup](https://github.com/KhronosGroup)
> 7. [khronos 官网](https://www.khronos.org/registry/EGL/)
> 8. [OpenGL-man-Page](https://www.khronos.org/registry/EGL/sdk/docs/man/)
> 9. [OpenGL-Refpages---> github](https://github.com/KhronosGroup/OpenGL-Refpages)
> 10. [**杀不死的 GPU 系列 --> 知乎:环太平洋**](https://zhuanlan.zhihu.com/p/43573883)

## <u>**现已不再维护此项目中的代码, 仅更新 OpenGL 笔记, 具体代码及 demo 正在移植到 Android 平台, 参考 [https://gitee.com/think3r/Android_openGLES](https://gitee.com/think3r/Android_openGLES)**</u>
   - 该项目采用 Android studio + NDK 来开发, 相应的 OGL-ES demo 渲染均在 native 层, 采用 C++ 编写;

1. 记录 OGL 学习笔记;
2. 参照 learnopengl, 在 windwos 上移植的 OGL-ES demo;

demo 主要架构:

- MFC 创建窗口与交互处理;
- Mali OGL-ES Emulator;
- 编译平台 MinGW64,  编译工具 WSL ;
- ~~docker 支持?(代做)~~

---

## <font color=#009A000> 0x00 学习笔记 </font>

1. [**openGL 概述**](./note/openGL概述.md)
2. [**EGL**](./note/EGL.md)
3. [**EGL 之外的东西**](./note/EGL之外的东西.md)
4. [**OpenGL 和 OpenGL-ES**](./note/openGL和openGL-ES.md)
   - [VO, VBO, EBO, VAO概念梳理](./note/openGL_vbo_ebo_vao.md)
   - [纹理](./note/纹理.md)
5. [**GLSL**](./note/GLSL.md)
   - [shader-加密处理](./note/shader%E5%8A%A0%E5%AF%86%E5%A4%84%E7%90%86.md) 
6. ~~[**坐标变换**](./note/坐标变换.md)~~

---

## <font color=#009A000> 0x01 学习环境搭建 </font>

### <font color=#FF4500> ~~**windwos 下 OpenGL-ES 模拟环境的搭建**~~(已不在维护) </font>

> 参考链接:
> 1. [vs2017下搭建opengl es 3.0开发环境](https://blog.csdn.net/brahmsjiang/article/details/78572141)
> 2. [OpenGL ES Emulator](https://developer.arm.com/tools-and-software/graphics-and-gaming/opengl-es-emulator/downloads)
> 3. [虚拟机中使用OpenGL遇到的错误总结 ](https://blog.csdn.net/qq_37996632/article/details/100329045)

1. 打算使用 OpenGL Es 实现一遍 learnopengl 之中的例程(仅仅实现最简单的和最复杂的两个).
2. 环境则使用 ~~Ubuntu~~ windows mingw 和 Arm 的 OpenGL-ES 模拟器来搭建;
   - 预留 Android 下的 surface 传递接口, 以 so 的形式来调用.
   - 预留 surface 配置的接口.
   - EGL 后续可能使用 Java 实现, 固将 EGL 和 GL 分别封装成两个库;
   - 预留键盘/鼠标操作的统一接口;
3. 开发环境 : source Insight + make;
   - ~~wsl 远程时执行窗口相关的命令时, 容易出现 `GFW` 等相关的错误, 可使用 `export DISPLAY=:1` 来搞定;~~ (在 1903 下的 wsl 中 wsl 以不能直接运行...)
   - 需在 windows 环境变量 `Path` 中添加 `../file/Mali_OpenGL_ES_Emulator-v3.0.4-2-g8d905-Windows-64bit` 或者直接拷贝输出成果物至上述路径下;
     - 至于 `-Wl,-rpath=` 则因为 mingw 不支持而作罢, 可参考链接 : [MinGW-w64 - for 32 and 64 bit Windows -- A complete runtime environment for gcc](https://sourceforge.net/p/mingw-w64/support-requests/153/)
     > rpath is not used on windows. DLLs are loaded as described https://docs.microsoft.com/en-us/windows/win32/dlls/dynamic-link-library-search-order 
   - 而静态库链接一直有问题... 暂且停滞...
4. 关于 cmd 下 `printf()` 颜色的支持 :
   - [C Programming - using ANSI escape codes on Windows, macOS and Linux terminals](https://solarianprogrammer.com/2019/04/08/c-programming-ansi-escape-codes-windows-macos-linux-terminals/)
   - [ANSI escape code](https://en.wikipedia.org/wiki/ANSI_escape_code#DOS_and_Windows)

---

### <font color=#FF4500> ~~ubuntu 下的 OpenGL Es 模拟限制~~ </font>

1. [can't run mail-cube with emulator on ubuntu 16.04](https://community.arm.com/developer/tools-software/graphics/f/discussions/6875/can-t-run-mail-cube-with-emulator-on-ubuntu-16-04)
2. [OpenGl 4 support for VMWare Workstation Pro](https://communities.vmware.com/thread/553334)
3. [How to update to latest OpenGL version on Virtualbox Ubuntu Linux machine?](https://askubuntu.com/questions/858407/how-to-update-to-latest-opengl-version-on-virtualbox-ubuntu-linux-machine)

根据 Arm 的文档, 我们得到了如下的问题描述 :

```text
In general, the Mali OpenGL ES Emulator will work on any system that supports at least:
• OpenGL 3.2 – when OpenGL ES 2.0 contexts are used1
• OpenGL 3.3 – when OpenGL ES 3.0 contexts are used
• OpenGL 4.3 – when OpenGL ES 3.1 or 3.2 contexts are used
```

 According to the Mali OpenGL ES Emulator documentation:
>The emulator requires a graphics card that supports at least OpenGL `4.3`.
而 VmWare 下的模拟器, 则只支持到 `OpenGL 3.0`; VirtualBox 更是只有 `2.0` ; <u>**因此将环境先迁移 windows 下;**</u>

## <font color=#FF4500> **Windows 下 OpenGL-Es 模拟环境的搭建** </font>


---

## <font color=#009A000> 0x02 demo on Windows-OpenGL-ES </font>

0. Note :
   - ~~由于 `deInif()` 崩溃, 固暂时不对 OGL 资源做完全的释放~~
   - Mali OGL-ES 模拟器的 GLSL 编译有 bug, 相同类型的不同变量只能写在同一行, 否则会报错;
1. `make 1.helloTriangle`
   - 环境测试 && 最初入门;
2. `make 2.triangleIndxed`
   - 全局变量 `u32DrawMode` 绘制模式, 可为 : `GL_TRIANGLES`, `GL_POINTS`, `GL_LINE_LOOP` 等;
   - `-D USE_VAO` 使用 <u>BO + VBO + VAO + EBO</u> 绘制四边形;
     - hello_triangle_indexed
   - `-D USE_VBO` 使用 <u>BO + VBO</u>  绘制四边形;
   - `-D USE_EBO` 使用 <u>BO + VBO + EBO</u> 绘制四边形;
   - `-D USE_TRANGLES` 6 顶点直接绘制四边形;
   - `-D USE_TRANGLES_STRIP` 4 顶点直接绘制四边形;
3. `make 3.triangle2`
   - `-D ONE_VBO` 使用 <u>1 VBO + 2 EBO + 2 VAO + 2 program</u> 绘制颜色不同的四边形;
   - `-D TWO_VBO` 使用 <u>2 VBO + 2 VAO + 2 program</u> 绘制颜色不同的四边形;
4. `make 4.shader_glsl`
   - `-D TWO_VERTEX_ATTRI_POINTER` 1 VBO + 1 VAO + 双顶点数组指针, 绘制调色板三角形;
   - `-D UINFORM_TEST` 使用 GLSL 中的 uniform 绘制变色三角形;
5. `make 5.shader_glsl2`
   - shaderToy 着色器代码移植, 炫酷的动态调色盘;
6. `make 6.textureCombined`
   - 纹理初步使用 + 颜色混合(使用了 VAO);
7. `make 7.textureCombined`
   - 纹理 x 2 混合 + 颜色混合;
8. `make 8.textureMode`
   - 纹理缩放和渲染模式;
   - <u>note: glsl 中的两个纹理, 必须放到一行, 否则会报错!</u>
9. `make 9.transFormations` 平面矩形
   - 对应 `transformations_exercise2`;
10. `make 10.coordinateSystem` 立方体
    - 无宏定义, 对应 `6.2.coordinate_systems_depth`
    - `-D MULTIPLE_CUBE` 对应 `6.3.coordinate_systems_multiple`
11. `make 11.camera` 摄像机 :
    - 增加了 MFC 鼠标支持 :
      - 方向键/WASD -------> 前进, 后退, 左, 右 ;
      - 鼠标滚轮 ------------> 放大 ;
      - `ctrl` + 鼠标移动 ----> 旋转 ;
    - [WASD -- 百度百科](https://baike.baidu.com/item/WASD/105282)
      - WASD ，位于键盘的左手面。这四个按键在第一人称射击游戏上通常作为控制玩家角色的移动。W 和 S 键用来控制角色向前或向后移动， A 和 D 键则用来控制角色左右转身。这组按键还有另一个变化 W-A-X-D，用以迁就一些习惯使用数字键盘的方向按键（8-4-6-2）的使用者。
