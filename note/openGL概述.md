# <font color=#0099ff> **OpenGL 基础知识** </font>

> `@think3r` 2020-04-03 22:54:03 

> 1. [知乎 :  如何理解 OpenGL 中着色器、渲染管线、光栅化等概念？](https://www.zhihu.com/question/29163054/answer/296309838)  
> 2. [官方网站与 API 文档](https://www.khronos.org/)
>    - [openGL](https://www.khronos.org/opengl)
>    - [openGL-ES](https://www.khronos.org/opengles)
>    - [EGL](https://www.khronos.org/egl)
> 3. [OpenGL 与 OpenGL ES2 之间有何区别？](https://www.zhihu.com/question/25078532)

## <font color=#009A000> 0x00 openGL, openMax, openCV </font>

- openGL 是用来做图形的, 其更像是一个 `api` ，和 openMax 等一样。
- openCV 是用来做视觉的, 实际上，openCV 是一个算法库，集成了很多视觉算法; 
- openMax 是用来做多媒体(音视频)的, 其全名为开放多媒体加速层（英语：Open Media Acceleration，缩写为 OpenMAX, OMAX）. 
  - 一个不需要授权、跨平台的软件抽象层，以 C 语言实现的软件接口，用来处理多媒体。它是由 Khronos Group 提出的标准，也由他们来维持，目标在于创造一个统一的接口，加速大量多媒体资料的处理。
  - *该标准针对嵌入式设备或/和移动设备的多媒体软件架构*。
  - 在架构底层上为多媒体的 codec 和数据处理定义了一套统一的编程接口 (OpenMAX IL API) ,对多媒体数据的处理功能进行系统级抽象,为用户屏蔽了底层的细节。因此, 多媒体应用程序和多媒体框架通过 OpenMAX IL 可以以一种统一的方式来使用 codec 和其他多媒体数据处理功能,具有了跨越软硬件平台的移植性;

## <font color=#009A000> 0x01 openGL -- Open Graphics Library </font>

> [OpenGL是什么?](https://www.zhihu.com/question/51867884)
- OpenGL API 是一套接口，学习这套接口，就可以在那些支持 OpenGL 的机器上正常使用这些接口，在屏幕上看到绘制的结果。
- 这套接口是 Khronos 这个组织在维护，怎么维护呢? 就是写一个说明书，指导各个 GPU 厂家，如果他们要支持 OpenGL 的话，要怎么实现一个具体的 OpenGL 库。Khronos 说要实现 `glDrawArray()` 这个接口，那么厂家就得在他的库里实现这个接口。如果不实现，那么就不算支持 OpenGL。也有一些接口不一定要实现, 是可选的。
- Khronos 每隔一段时间发布一个新的 OpenGL API 版本，就是发布这个说明书。一般两个 : 一个是 `core profile`，一定要实现的 ; 还有另一个是为了兼容旧版本接口的说明书。
  - 厂家实现的 OpenGL 库的内容其实就是厂家自己的团队整合自己的图形知识以及 GPU 硬件的指令。硬件厂家实现的 OpenGL 库一般在他们的硬件的驱动里。当然，如果硬件开发商的某款显卡无法在硬件上支持 OpenGL 所定义的所有必须功能，那么硬件开发商就必须通过软渲染的方式提供这种功能。
  - OpenGL 的实现也没说一定要有 GPU 的，像 mesa 3d 可以在 *纯软件* 的环境下运行。
  - 当前显卡对 openGL 的支持程度, 可以用 AIDIA-64 软件查看( AIDIA-64 同时支持了 windwos, Android, ios 等设备)
  - 主机电脑的具体查看路径为: `显示设备 --> OpenGL`
- OpenGL 的 Open 指的是 **接口是开放的，而具体实现的库一般都是不公开的 .**
  - 如 : mesa 3d 是一个开源的实现。

---

- 像 AMD、Nvidia 等图形硬件厂家还得支持微软的 Direct3D，所以他们还得按照微软的接口说明书，整合 Direct3D 的库给 Windows 使用。
  - 在 Windows 上，还有一种情况，就是这个 OpenGL 库可能是 Direct3D 接口的一个封装(Wrapper)。表面上调用 OpenGL 接口，实际上里面的实现是间接调用了 Direct3D 接口。
  - PASS : 这个 Wrapper 方法同样可以在电脑上提供 OpenGL-ES 的模拟环境, 如 `Mali OpenGL ES Emulator` ;

---

## <font color=#009A000> 0x02 openGL, openEGL, openGL-ES, GLFW, glut 等概念的区分 </font>

### <font color=#FF4500> 绘制环境 </font>

- **OpenGL 只是关注于怎么调用接口实现绘画**, 因此其在使用时，需要与一个实际的窗口系统关联起来。
- 在不同平台上有不同的机制以关联窗口系统 :
  - 在 Windows 上是 WGL;
  - 在 Linux 上是 GLX (X-Window);
  - 在 Apple OS 上是 AGL ;
  - 在嵌入式设备上则是 EGL 。
  - 很遗憾, 这部分都 **没有实现跨平台** 。
- 这也是为什么存在 glut（已经不更新了，代替者 freeglut，不支持 macOS）、glfw 这些“**半**”跨平台的框架, 这些框架帮助你轻松地在各平台下使用 OpenGL。
  - glut 是基本的窗口界面，是独立于 gl 和 glu 的. 如果不喜欢用 glut 可以用 MFC 和 Win32 窗口等代替，但是 glut 是跨平台的，这就保证了我们编出的程序是跨平台的，如果用 MFC 或者 Win32 只能在 windows 操作系统上使用。选择 OpenGL 的一个很大原因就是因为它的跨平台性，所以我们可以尽量的使用 glut 库。

---

### <font color=#FF4500> GL 和 GL-ES </font>

- 在电脑主机上使用的是 OpenGL, 而在**嵌入式设备**上、手机还有浏览器里的 OpenGL 实现使用的是 `OpenGL ES`。
  - 注1 : 浏览器的 WebGL 是对 OpenGL ES 的 js 封装。
  - 注2 : 嵌入式设备 **建立环境多用的是 EGL 库**。
- GL 是一个非常大的 API 集合，既有老的特性，也有厂商特定的 API，也有新的 API。规模一大，驱动的质量就不好控制了，用户用起来也不方便。实现一个功能要写十行代码来检测这个功能能不能起作用。所以 Khronos 接手后， 就考虑把 GL 那些边边角角的 API 统统都干掉，只保留最常用、最必要的那些 API，<u>同时也是为了匹配移动芯片的特性，就诞生了 OpenGL ES。</u>
- 所以基本上 GL-ES 算是 GL 的子集（当然区别也有不少），如果你想学 OpenGL ES，你可以考虑从 OpenGL Programming Guide 或者是从 OpenGL Bible 入手（当然要最新版的，同时这两本书也基本上只介绍了比较核心的 API），然后再辅以一两本 ES 的图书即可。

---

- 既然 OpenGL ES 只是一组函数接口，那么如何使用呢？ 我们肯定首先要去实现这些函数接口，而 android 提供了两种类型的实现：软件实现，硬件实现。
  - 硬件实现 : 前面提到这组函数接口主要是为了和 GPU 这个硬件进行打交道的。所以各个硬件厂商会提供相关的实现，例如高通平台的 adreno 解决方案, arm 的 Mali 方案;
  - 软件实现 : android 也提供了一套 OpenGL ES 的软件实现，就是说不用 GPU 了，完全用软件实现画图的相关功能，也就是 libagl，软件实现最终编译完保存   `system\lib\egl\libGLES_android.so`

## <font color=#009A000> 0x03 EGL 官方解释 </font>

- <u>**如下为 EGL 的官方的简介与翻译 :**</u>

> EGL (Embedded Graphics Library) - Native Platform Interface  (原生平台接口)
> EGL is an interface between Khronos rendering APIs  such as OpenGL ES or OpenVG and the underlying native platform window system.
> - EGL 是 Khronos 渲染 API（如 OpenGL ES 或 OpenVG）与底层本机平台窗口系统之间的接口. EGL 是 OpenGL ES 和底层的 native window system 之间的接口，承上启下。

> It handles graphics context management, surface/buffer binding, and rendering synchronization and enables high-performance, accelerated, mixed-mode 2D and 3D rendering using other Khronos APIs.
> - 它处理图形上下文管理，表面/缓冲区绑定和渲染同步，并通过使用其他 Khronos API 实现高性能，加速，混合模式 2D 和 3D 渲染。

> EGL is a complement to OpenGL ES. EGL is used for getting surfaces to render to using functions like eglCreateWindowSurface, and you can then draw to that surface with OpenGL ES. Its role is similar to GLX/WGL/CGL.
> - EGL 是 OpenGL ES 的补充。 EGL 用于使表面渲染为使用 eglCreateWindowSurface 等函数，然后您可以使用 OpenGL ES 绘制该表面。 它的作用类似于 GLX / WGL / CGL。

> Whether or not EGL can give you a context that supports OpenGL ES 2.0 may vary by platform, but if the Android device supports ES 2.0 and EGL, you should be able to get such a context from EGL. Take a look at the EGL_RENDERABLE_TYPE attribute and the EGL_OPENGL_ES2_BIT when requesting an EGLConfig.
> - EGL 是否可以为您提供支持 OpenGL ES 2.0 的环境可能因平台而异，但如果 Android 设备支持 ES 2.0 和 EGL，则应该能够从 EGL 获取这样的环境。 请求 EGLConfig 时，请查看 EGL_RENDERABLE_TYPE 属性和 EGL_OPENGL_ES2_BIT。

---

- EGL 则是 OpenGL ES 在嵌入式平台上 WGL, GLX, AGL 等 的等价物。EGL 假设 OS 会提供窗口系统，但 EGL 与平台无关，并不局限于任何特定的窗口系统，所有用到本地窗口系统的地方都用屏蔽指针来处理。我觉得这就是它易于移植的关键. 
- 一个 window 恰如你在计算机中看到的一个 window。它拥有唯一一个用以绘制自己的内容的 surface。应用通过 Window Manager 创建一个 window，Window Manager 为每一个 window 创建一个 surface，并把该 surface 传递给应用以便应用在上面绘制。应用可以在 surface 上任意进行绘制。对于 Window Manager 来说，surface 就是一个不透明的矩形而已。
- 由于 OpenGL ES 是跨平台的，引入 EGL 就是为了屏蔽不同平台上的区别。本地窗口相关的 API 提供了访问本地窗口系统的接口，EGL 提供了创建渲染表面，接下来 OpenGL ES 就可以在这个渲染表面上绘制，同时提供了图形上下文(context)，用来进行状态管理。
