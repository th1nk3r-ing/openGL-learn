# <font color=#0099ff> **openGL 学习笔记** </font>

> `@think3r` 2018-05-09 22:56:27
> 1. [OpenGL学习路径](https://cstsinghua.github.io/2018/07/12/openGL%E5%AD%A6%E4%B9%A0%E8%B7%AF%E5%BE%84/)
> 2. [Android 开发者官方开发者指南](https://developer.android.google.cn/guide?hl=zh_cn)
> 3. [learnopengl](https://learnopengl-cn.github.io/) 
> 4. [OpenGL-wikipedia](https://zh.wikipedia.org/wiki/OpenGL)
> 5. [OPENGL ES 3.0编程指南  原书第2版 --- github](https://github.com/danginsburg/OpenGL-ES3-book)
> 6. [https://github.com/KhronosGroup](https://github.com/KhronosGroup)
> 7. [khronos 官网](https://www.khronos.org/registry/EGL/)
> 8. [OpenGL-man-Page](https://www.khronos.org/registry/EGL/sdk/docs/man/)
> 9. [OpenGL-Refpages---> github](https://github.com/KhronosGroup/OpenGL-Refpages)

  
## <font color=#009A000> 0x01 参考链接 </font>

主要记录自己在学习 openGL 中的一些笔记 :

1. [openGL 概述](./note/openGL概述.md)
2. [EGL](./note/EGL.md)
3. [EGL 之外的东西](./note/EGL之外的东西.md)
4. [OpenGL 和 OpenGL-ES](./note/openGL和openGL-ES.md)
5. [GLSL](./note/GLSL.md)

## <font color=#009A000> 0x02 学习环境搭建 </font>

### <font color=#FF4500> **windwos 下 OpenGL 环境搭建** </font>

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
    - wsl 远程时执行窗口相关的命令时, 容易出现 `GFW` 等相关的错误, 可使用 `export DISPLAY=:1` 来搞定;

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

### <font color=#FF4500> **Windows 下 OpenGL-Es 模拟环境的搭建** </font>

略; 同上述 Linux ;

关于 cmd 下 `printf()` 颜色的支持 :

- [C Programming - using ANSI escape codes on Windows, macOS and Linux terminals](https://solarianprogrammer.com/2019/04/08/c-programming-ansi-escape-codes-windows-macos-linux-terminals/)
- [ANSI escape code](https://en.wikipedia.org/wiki/ANSI_escape_code#DOS_and_Windows)
