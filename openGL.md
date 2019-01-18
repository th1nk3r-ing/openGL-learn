# <font color=#0099ff> **OpenGL 学习** </font> 

> `@think3r` 
> 1. [知乎 :  如何理解 OpenGL 中着色器、渲染管线、光栅化等概念？](https://www.zhihu.com/question/29163054/answer/296309838)  

<!-- TOC -->

- [<font color=#0099ff> **OpenGL 学习** </font>](#font-color0099ff-opengl-学习-font)
    - [<font color=#009A000> 0x00 openGL, openMax, openCV </font>](#font-color009a000-0x00-opengl-openmax-opencv-font)
        - [<font color=#FF4500> openGL -- Open Graphics Library </font>](#font-colorff4500-opengl----open-graphics-library-font)
        - [<font color=#FF4500> 有关 Andoid, openGL openEGL, openGL ES 等概念的区分</font>](#font-colorff4500-有关-andoid-opengl-openegl-opengl-es-等概念的区分font)
            - [openEGL (Embedded Graphics Library) :](#openegl-embedded-graphics-library-)
        - [<font color=#FF4500> openMax </font>](#font-colorff4500-openmax-font)
    - [<font color=#009A000> Shaders </font>](#font-color009a000-shaders-font)
    - [<font color=#009A000> openGL  </font>](#font-color009a000-opengl--font)
    - [<font color=#009A000> 0x00 windows 下, openGL 显示 YUV 图像 </font>](#font-color009a000-0x00-windows-下-opengl-显示-yuv-图像-font)

<!-- /TOC -->

## <font color=#009A000> 0x00 openGL, openMax, openCV </font> 

- openaGL 是用来做图形的，openCV 是用来做视觉的; 实际上，openCV 是一个算法库，集成了很多视觉算法; 而 openGL 更像是一个 api，和 openMax 等一样。

### <font color=#FF4500> openGL -- Open Graphics Library </font> 

> [OpenGL是什么?](https://www.zhihu.com/question/51867884)

- OpenGL API 是一套接口，学习这套接口，就可以在那些支持 OpenGL 的机器上正常使用这些接口，在屏幕上看到绘制的结果。

- 这套接口是 Khronos 这个组织在维护，怎么维护呢? 就是写一个说明书，指导各个 GPU 厂家，如果他们要支持 OpenGL 的话，要怎么实现一个具体的 OpenGL 库。Khronos 说要实现 glDrawArray 这个接口，那么厂家就得在他的库里实现这个接口。如果不实现，那么就不算支持 OpenGL。也有一些接口不一定要实现。

- Khronos 每隔一段时间发布一个新的 OpenGL API 版本，就是发布这个说明书。一般两个，一个是 core profile，一定要实现的，还有另一个是为了兼容旧版本接口的说明书。
    - 厂家实现的 OpenGL 库的内容其实就是厂家自己的团队整合自己的图形知识以及 GPU 硬件的指令。硬件厂家实现的 OpenGL 库一般在他们的硬件的驱动里。当然，如果硬件开发商的某款显卡无法在硬件上支持 OpenGL 所定义的所有必须功能，那么硬件开发商就必须通过软渲染的方式提供这种功能。
    - OpenGL 的实现也没说一定要有 GPU 的，像 mesa 3d 可以在纯软件的环境下运行。
    - 当前显卡对 openGL 的支持程度, 可以用 AIDIA-64 软件查看, 具体路径为: `显示设备 --> OpenGL`
- OpenGL 的 Open 指的是 **接口是开放的，而具体实现的库一般都是不公开的.** 目前据我所知， mesa 3d 是一个开源的实现。

- 像 AMD、Nvidia 等图形硬件厂家还得支持 Direct3D，所以他们还得按照微软的接口说明书，整合 Direct3D 的库给 Windows 使用。
    - 在 Windows上，还有一种情况，就是这个 OpenGL 库可能是 Direct3D 接口的一个封装(Wrapper)。表面上调用 OpenGL 接口，实际上里面的实现是间接调用了 Direct3D 接口。

- 另外 OpenGL 只是关注于怎么调用接口实现绘画。为了使用 OpenGL，还得初始化一个 OpenGL 环境。这个环境需要 CGL、WGL、GLX 这些库里的接口来建立。这几个库分别对应 苹果系统、Windows、还有 Linux 的 X-Window，**没有实现跨平台** 。这也是为什么存在 glut（已经不更新了，代替者 freeglut，不支持 macOS）、glfw 这些“半”跨平台的框架帮助你轻松地在各平台下使用 OpenGL。
    - glut 是基本的窗口界面，是独立于 gl 和 glu 的，如果不喜欢用 glut 可以用 MFC 和 Win32 窗口等代替，但是 glut 是跨平台的，这就保证了我们编出的程序是跨平台的，如果用 MFC 或者 Win32 只能在 windows 操作系统上使用。选择 OpenGL 的一个很大原因就是因为它的跨平台性，所以我们可以尽量的使用 glut 库。

### <font color=#FF4500> 有关 Andoid, openGL openEGL, openGL ES 等概念的区分</font> 

- 在嵌入式设备上、手机还有浏览器里的 OpenGL 实现使用的是 OpenGL ES 的说明书。浏览器的 WebGL 是对 OpenGL ES 的 js 封装。建立环境用的是 EGL 库。
    - 既然 OpenGL ES 只是一组函数接口，那么如何使用呢？ 我们肯定首先要去实现这些函数接口，而android 提供了两种类型的实现：软件实现，硬件实现。
        -  硬件实现，前面提到这组函数接口主要是为了和 GPU这个硬件进行打交道的。所以各个硬件厂商会提供相关的实现，例如高通平台的 adreno 解决方案
        - 软件实现，android 也提供了一套 OpenGL ES 的软件实现，就是说不用 GPU 了，完全用软件实现画图的相关功能，也就是 libagl，软件实现最终编译完保存   `system\lib\egl\libGLES_android.so`
    
#### openEGL (Embedded Graphics Library) :

> EGL - Native Platform Interface  (原生平台接口)
> EGL is an interface between Khronos rendering APIs  such as OpenGL ES or OpenVG and the underlying native platform window system.     
- EGL 是 Khronos 渲染 API（如 OpenGL ES 或 OpenVG）与底层本机平台窗口系统之间的接口. EGL 是 OpenGL ES 和底层的 native window system 之间的接口，承上启下。             
> It handles graphics context management, surface/buffer binding, and rendering synchronization and enables high-performance, accelerated, mixed-mode 2D and 3D rendering using other Khronos APIs.
- 它处理图形上下文管理，表面/缓冲区绑定和渲染同步，并通过使用其他 Khronos API 实现高性能，加速，混合模式 2D 和 3D 渲染。
> EGL is a complement to OpenGL ES. EGL is used for getting surfaces to render to using functions like eglCreateWindowSurface, and you can then draw to that surface with OpenGL ES. Its role is similar to GLX/WGL/CGL.
- EGL 是 OpenGL ES 的补充。 EGL 用于使表面渲染为使用 eglCreateWindowSurface 等函数，然后您可以使用 OpenGL ES 绘制该表面。 它的作用类似于 GLX / WGL / CGL。
> Whether or not EGL can give you a context that supports OpenGL ES 2.0 may vary by platform, but if the Android device supports ES 2.0 and EGL, you should be able to get such a context from EGL. Take a look at the EGL_RENDERABLE_TYPE attribute and the EGL_OPENGL_ES2_BIT when requesting an EGLConfig.
- EGL 是否可以为您提供支持 OpenGL ES 2.0 的环境可能因平台而异，但如果 Android 设备支持 ES 2.0 和 EGL，则应该能够从 EGL 获取这样的环境。 请求 EGLConfig 时，请查看 EGL_RENDERABLE_TYPE 属性和 EGL_OPENGL_ES2_BIT。
- 在 Android上，EGL 完善了 OpenGL ES。利用类似 eglCreateWindowSurface 的 EGL 函数可以创建 surface 用来 render ，有了这个 surface 你就能往这个 surface 中利用 OpenGL ES 函数去画图了.

- OpenGL 在使用时，需要与一个实际的窗口系统关联起来。在不同平台上有不同的机制以关联窗口系统，在 Windows 上是 WGL，在 Linux 上是 GLX，在 Apple OS 上是 AGL 等。
    - EGL 则是 OpenGL ES 在嵌入式平台上（WGL,GLX,AGL）的等价物。EGL 假设 OS 会提供窗口系统，但 EGL 与平台无关，并不局限于任何特定的窗口系统，所有用到本地窗口系统的地方都用屏蔽指针来处理。我觉得这就是它易于移植的关键. 
- 由于 OpenGL ES 是跨平台的，引入 EGL 就是为了屏蔽不同平台上的区别。本地窗口相关的 API 提供了访问本地窗口系统的接口，EGL 提供了创建渲染表面，接下来 OpenGL ES 就可以在这个渲染表面上绘制，同时提供了图形上下文，用来进行状态管理。

> [OpenGL 与 OpenGL ES2 之间有何区别？](https://www.zhihu.com/question/25078532)

- GL 是一个非常大的 API 集合，既有老的特性，也有厂商特定的 API，也有新的 API。规模一大，驱动的质量就不好控制了，用户用起来也不方便。实现一个功能要写十行代码来检测这个功能能不能其作用。所以 Khronos 接手后， 就考虑把 GL 那些边边角角的 API 统统都干掉，只保留最常用、最必要的那些 API，同时也是为了匹配移动芯片的特性，就诞生了 OpenGL ES。

- 所以基本上 GL ES 算是 GL 的子集（当然区别也有不少），如果你想学 OpenGL ES，你可以考虑从 OpenGL Programming Guide 或者是从 OpenGL Bible 入手（当然要最新版的，同时这两本书也基本上只介绍了比较核心的 API），然后再辅以一两本 ES 的图书即可。

- opengl 是各个平台的统称，
    - 移动端的是 opengles，
    - web 端的是 webgl;
    - opengl es 是 opengl 的 subset， 没有 glu， glut 库。 针对手机做了优化等

### <font color=#FF4500> openMax </font> 

- openMax 开放多媒体加速层（英语：Open Media Acceleration，缩写为 OpenMAX, OMAX），一个不需要授权、跨平台的软件抽象层，以 C 语言实现的软件接口，用来处理多媒体。它是由 Khronos Group 提出的标准，也由他们来维持，目标在于创造一个统一的接口，加速大量多媒体资料的处理。

- 该标准针对嵌入式设备或/和移动设备的多媒体软件架构 。在架构底层上为多媒体的 codec 和数据处理定义了一套统一的编程接口 (OpenMAX IL API) ,对多媒体数据的处理功能进行系统级抽象,为用户屏蔽了底层的细节。因此,  多媒体应用程序和多媒体框架通过 OpenMAX IL 可以以一种统一的方式来使用  codec 和其他多媒体数据处理功能,具有了跨越软硬件平台的移植性;


<div STYLE="page-break-after: always;"></div><!------------------分页符------------------->

- 在 OpenGL 中，任何事物都在 3D 空间中，而屏幕和窗口却是 2D 像素数组，这导致 OpenGL 的大部分工作都是关于把 3D 坐标转变为适应你屏幕的 2D 像素。
- 3D 坐标转为 2D 坐标的处理过程是由 OpenGL 的图形渲染管线（Graphics Pipeline，大多译为管线，实际上指的是一堆原始图形数据途经一个输送管道，期间经过各种变化处理最终出现在屏幕的过程）管理的。
    - 图形渲染管线可以被划分为两个主要部分：
        - 第一部分把你的3D坐标转换为2D坐标，
        - 第二部分是把2D坐标转变为实际的有颜色的像素。
        - note: 2D 坐标和像素也是不同的，2D 坐标精确表示一个点在 2D 空间中的位置，而 2D 像素是这个点的近似值，2D 像素受到你的屏幕/窗口分辨率的限制。
- 图形渲染管线接受一组 3D 坐标，然后把它们转变为你屏幕上的有色 2D 像素输出。图形渲染管线可以被划分为几个阶段，每个阶段将会把前一个阶段的输出作为输入。所有这些阶段都是高度专门化的（它们都有一个特定的函数），并且很容易并行执行。正是由于它们具有并行执行的特性，当今大多数显卡都有成千上万的小处理核心，它们在 GPU 上为每一个（渲染管线）阶段运行各自的小程序，从而在图形渲染管线中快速处理你的数据。这些小程序叫做着色器 (Shader)。
- 有些着色器允许开发者自己配置，这就允许我们用自己写的着色器来替换默认的。这样我们就可以更细致地控制图形渲染管线中的特定部分了，而且因为它们运行在 GPU 上，所以它们可以给我们节约宝贵的 CPU 时间。 OpenGL 着色器是用 OpenGL 着色器语言(OpenGL Shading Language, GLSL)写成的。

- 下面，你会看到一个图形渲染管线的每个阶段的抽象展示。要注意蓝色部分代表的是我们可以注入自定义的着色器的部分。

![openGL图形渲染管线.jpg](./image/openGL图形渲染管线-各阶段.jpg)

如上图，图形渲染管线包含很多部分，每个部分都将在转换顶点数据到最终像素这一过程中处理各自特定的阶段。下边将概括性地解释一下渲染管线的每个部分，让你对图形渲染管线的工作方式有个大概了解。

- 首先，我们以数组的形式传递3个3D坐标作为图形渲染管线的输入，用来表示一个三角形，这个数组叫做 **顶点数据(Vertex Data)** ；顶点数据是一系列顶点的集合。
    - 一个顶点 (Vertex) 是一个 3D 坐标的数据的集合。而顶点数据是用顶点属性 (Vertex Attribute) 表示的，它可以包含任何我们想用的数据，但是简单起见，我们还是假定每个顶点只由一个 3D 位置(当我们谈论一个“位置”的时候，它代表在一个“空间”中所处地点的这个特殊属性；同时“空间”代表着任何一种坐标系，比如x、y、z三维坐标系，x、y二维坐标系，或者一条直线上的x和y的线性关系，只不过二维坐标系是一个扁扁的平面空间，而一条直线是一个很瘦的长长的空间)和一些颜色值组成的吧。
    - 为了让 OpenGL 知道我们的坐标和颜色值构成的到底是什么，OpenGL 需要你去指定这些数据所表示的渲染类型。我们是希望把这些数据渲染成一个点？一个三角形？还是仅仅是一个长长的线？
        - 做出的这些提示叫做 图元(Primitive)，任何一个绘制指令的调用都将把图元传递给 OpenGL。这是其中的几个：GL_POINTS、GL_TRIANGLES、GL_LINE_STRIP。
    - 图形渲染管线的第一个部分是顶点着色器(Vertex Shader)，它把一个单独的顶点作为输入。顶点着色器主要的目的是把 3D 坐标转为另一种 3D 坐标（后面会解释），同时顶点着色器允许我们对顶点属性进行一些基本处理。
- **图元装配(Primitive Assembly)** 阶段将顶点着色器输出的所有顶点作为输入（如果是 GL_POINTS，那么就是一个顶点），并所有的点装配成指定图元的形状；比如本例子是一个三角形。
    - 图元装配阶段的输出会传递给几何着色器(Geometry Shader)。几何着色器把图元形式的一系列顶点的集合作为输入，它可以通过产生新顶点构造出新的（或是其它的）图元来生成其他形状。例子中，它生成了另一个三角形。
- 几何着色器的输出会被传入 **光栅化阶段 (Rasterization Stage)** ，这里它会把图元映射为最终屏幕上相应的像素，生成供片段着色器 (Fragment Shader) 使用的片段(Fragment)。
    - 在片段着色器运行之前会执行裁切(Clipping)。裁切会丢弃超出你的视图以外的所有像素，用来提升执行效率。
- OpenGL中的一个**片段** 是 OpenGL 渲染一个像素所需的所有数据。
    - 片段着色器的主要目的是计算一个像素的最终颜色，这也是所有 OpenGL 高级效果产生的地方。通常，片段着色器包含 3D 场景的数据（比如光照、阴影、光的颜色等等），这些数据可以被用来计算最终像素的颜色。
- 在所有对应颜色值确定以后，最终的对象将会被传到最后一个阶段，我们叫做 **Alpha 测试和混合 (Blending) 阶段** 。这个阶段检测片段的对应的深度（和模板 (Stencil) ）值，用它们来判断这个像素是其它物体的前面还是后面，决定是否应该丢弃。这个阶段也会检查 alpha 值（alpha值定义了一个物体的透明度）并对物体进行混合 (Blend)。所以，即使在片段着色器中计算出来了一个像素输出的颜色，在渲染多个三角形的时候最后的像素颜色也可能完全不同。

- 可以看到，图形渲染管线非常复杂，它包含很多可配置的部分。然而，对于大多数场合，我们只需要配置顶点和片段着色器就行了。几何着色器是可选的，通常使用它默认的着色器就行了。**在 OpenGL 中，我们必须定义至少一个顶点着色器和一个片段着色器（因为 GPU 中没有默认的顶点/片段着色器）**。 出于这个原因，刚开始学习现代 OpenGL 的时候可能会非常困难，因为在你能够渲染自己的第一个三角形之前已经需要了解一大堆知识了。

## <font color=#009A000> Shaders </font> 

- Shaders 在现代 OpenGL 中是个很重要的概念。应用程序离不开它，除非你理解了，否则这些代码也没有任何意义。Shaders 是一段 GLSL 小程序，运行在 GPU 上而非 CPU。它们使用 OpenGL Shading Language (GLSL) 语言编写，看上去像 C 或 C++，但却是另外一种不同的语言。使用 shader 就像你写个普通程序一样：写代码，编译，最后链接在一起才生成最终的程序。
- **Shaders 并不是个很好的名字，因为它不仅仅只做着色。只要记得它们是个用不同的语言写的，运行在显卡上的小程序就行。**
- 在旧版本的 OpenGL 中，shaders 是可选的。在现代 OpenGL 中，为了能在屏幕上显示出物体，shaders 是必须的。
- 一般有如下两种不同的着色器:

    - 在你的场景中，每个 **顶点** 都需要调用的程序，称为 **顶点着色器(Vertex shaders)** 。
        - 假如你在渲染一个简单的场景：一个长方形，每个角只有一个顶点。于是 vertex shader 会被调用四次。
        - 它负责执行：诸如灯光、几何变换等等的计算。得出最终的顶点位置后，为下面的片段着色器提供必须的数据。
    - 在你的场景中，大概每个像素都会调用的程序，称 **片段着色器(Fragment shaders)** 。
        - 在一个简单的场景，也是刚刚说到的长方形。这个长方形所覆盖到的每一个像素，都会调用一次 fragment shader。
        - 片段着色器的责任是计算灯光，以及更重要的是计算出每个像素的最终颜色。 

## <font color=#009A000> openGL  </font> 

- openGL 是一个 **状态机** , 尤其是如果你使用固定功能的管线.
    - 可以设置它的各种状态,  然后让这些状态一直生效, 知道再次修改他们.
    - 许多表示模式的状态变量可以使用 `glENable()` 和 `glDisable()` 函数进行启用和禁用.
- OpenGL 主要包括三个函数库，它们是核心库、实用函数库和编程辅助库。
    - **核心库** 中包含了 OpenGL 最基本的命令函数。核心库提供了一百多个函数，这些函数都以 `gl` 为前缀，用来建立各种各样的几何模型、进行坐标变换、产生光照效果、进行纹理映射、产生雾化效果等所有的二维和三维图形操作。
    - **实用函数库** 是比核心库更高一层的函数库，它提供四十多个函数，这些函数都以 `glu` 为前缀。由于 OpenGL 是一个图形标准，是独立于任何窗口系统或操作系统的，在 `OpenGL` 中没有提供窗口管理和消息事件响应的函数，也没有鼠标和键盘读取事件的功能，所以在编程辅助库提供了一些基本的窗口管理函数、事件处理函数和简单的事件函数。这类函数以 `aux` 作为前缀。
    - 值得一提的是，目前 **AUX 编程辅助库** 已经很大程度上被 GLUT 库取代了。以下介绍以 GLUT 库为例。


- openGL 包含了很多渲染函数, 但它们的设计目的是独立于任何窗口系统或者操作系统. 因而, 他们并没有包含打开窗口, 或者从键盘, 鼠标读取事件的函数. 而且遗憾的是, 如果连最基本的打开窗口的功能都没有, 编写一个完整的图形程序几乎是不可能的. 并且, 绝大多数有趣的程序都需要一些用户输入, 或者需要操作系统和窗口系统的其它服务. 绝大多数情况下, 只有完整的程序才能形成有趣的示例程序吗因此本书使用 `GLUT` 来简化打开窗口, 检测输入等任务.
    - GLUT: `OpenGL 应用工具包 (OpenGL Utility Toolkit)`, 是一个与窗口系统无关的工具包。它作为 AUX 库的功能更强的替代品，用于隐藏不同窗口系统 API 的复杂性。GLUT 的子程序的前缀使用 `glut` ;
    - GLUT 是专为构建中小型 OpenGL 程序。虽然 GLUT 是适合学习 OpenGL 和开发简单的 OpenGL 应用程序。 GLUT 并不是一个功能全面的工具包所以大型应用程序需要复杂的用户界面最好使用本机窗口系统工具包。所以 GLUT 是简单的、容易的、小的。
    > - `GLUT library available on Android?`    
     >   - Why do you want to use GLUT on Android? You know that GLUT never was meant for real-world applications, just for small demos and tutorials;
    - Android 沿用了 J2ME 的 openGL ES API. 相比 C 版本的 OpenGL, Opengl ES 没有 glu 和 glut 库，~~而且只能画三角形(多边形需要三角化)???~~;

- 既然OpenGL ES只是一组函数接口，那么如何使用呢？我们肯定首先要去实现这些函数接口，而android提供了两种类型的实现：软件实现，硬件实现。


## <font color=#009A000> 0x00 windows 下, openGL 显示 YUV 图像 </font> 

> [最简单的视音频播放示例5：OpenGL播放RGB/YUV](https://blog.csdn.net/leixiaohua1020/article/details/40333583)

- 使用 OpenGL 播放视频最简单的情况下需要如下步骤：
    - 初始化
        - 初始化
        - 创建窗口
        - 设置绘图函数
        - 设置定时器
        - 初始化 Shader, 初始化 Shader 的步骤比较多，主要可以分为 3 步：创建 Shader，创建 Program，初始化 Texture。
            1. 创建一个 Shader 对象, Shader 有点类似于一个程序的编译器
                - 编写 Vertex Shader 和 Fragment Shader 源码。
                    - 在这里用到了一种新的语言：OpenGL Shader Language, 简称 GLSL。它是一种类似于C语言的专门为 GPU 设计的语言，它可以放在 GPU 里面被并行运行。
                - 创建两个 shader 实例 。
                - 给 Shader实例指定源码。
                - 在线编译 shaer 源码。
            2. 创建一个 Program 对象, Program 有点类似于一个程序的链接器。
                - 创建 program。
                - 绑定 shader 到 program。
                - 链接 program。
                - 使用 porgram。
            3. 初始化 Texture。可以分为以下步骤。
                - 定义定点数组
                - 设置顶点数组
                - 初始化纹理 
    - 循环显示画面
        - 调整显示位置，图像大小/  设置纹理;
            -
        - 画图
        - 显示

- 再说一点自己学习 OpenGL 时候的一个明显的感觉：OpenGL 的函数好多啊。OpenGL 的函数的特点是数量多，但是每个函数的参数少。而 Direct 3D 的特点和它正好反过来，函数少，但是每个函数的参数多。
- 刚开始学习 Direct3D 显示视频技术的人一定会有一个疑问：“像 GDI 那样直接指定一下像素数据，然后画在窗口上不就行了？为什么又是渲染又是纹理，搞得这么复杂？”。确实，相比于 GDI，Direct3D 的入门的代码要复杂很多。其实 Ditect3D 中的很多概念并不是出自于视频领域，而是出自于 3D 制作。下面简单记录一下 Direct3D 这些概念的意义。
    - 纹理（Texture）
        - 纹理实际上就是一张图片。个人感觉这个词的英文 Texture 其实也可以翻译成 “材质” （“纹理”总给人一种有很多花纹的感觉 =_=）。在 3D 制作过程中，如果单靠计算机绘制生成 3D 模型，往往达不到很真实的效果。如果可以把一张 2D 图片 “贴” 到 3D 模型的表面上，则不但节约了计算机绘图的计算量，而且也能达到更真实的效果。纹理就是这张 “贴图”。
            - 例如，下面这张图就是把一张 “木箱表面” 的纹理贴在了一个正六面体的六个面上，从而达到了一个 “木箱” 的效果（还挺像CS里面的木箱的）。
    - 渲染（Render）
        - 渲染就是从模型生成图像的过程，通常是 3D 制作的最后一步。
            - 例如上图的那个木箱，在经过纹理贴图之后，形成了一个 “木箱” 模型。但是只有把它作为 2D 图像输出到屏幕上之后，它才能被我们的看见。这个输出的过程就是渲染。我们也可以调整这个 “木箱模型” 的参数，达到不同的渲染结果。比如说改变观察角度等等。

