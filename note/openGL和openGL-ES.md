# <font color=#0099ff> **OGL 和 OGL-ES** </font>

> `@think3r` 2020-04-04 15:54:14 <br>
> 参考链接: <br>
> 1. [【Android】Android SurfaceFlinger之OpenGL-ES](https://blog.csdn.net/iEearth/article/details/54729848?locationNum=7&fps=1)
> 2. [Android OpenGL-ES 1.x 教程的Native实现](http://www.cnblogs.com/bpasser/archive/2011/09/25/2189941.html)
> 3. [最简单的视音频播放示例5：OpenGL播放RGB/YUV](https://blog.csdn.net/leixiaohua1020/article/details/40333583)

## <font color=#009A000> OGL 一般使用流程 </font>

使用 OpenGL 播放视频最简单的情况下需要如下步骤：

- **初始化 :**
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
         - 初始化纹理;
- 循环显示画面
  - 调整显示位置，图像大小/  设置纹理;
  - 画图
  - 显示

PASS : 如果仅仅是向绘制三角形, 那么上述步骤中 texture 相关的部分都可以略去.

## <font color=#009A000> **openGL 构建原理** </font>

<u>**OpenGL的一大特性就是对扩展(Extension)的支持，**</u> 当一个显卡公司提出一个新特性或者渲染上的大优化，通常会以扩展的方式在驱动中实现。如果一个程序在支持这个扩展的显卡上运行，开发者可以使用这个扩展提供的一些更先进更有效的图形功能。通过这种方式，开发者不必等待一个新的 OpenGL 规范面世，就可以使用这些新的渲染特性了，只需要简单地检查一下显卡是否支持此扩展。通常，当一个扩展非常流行或者非常有用的时候，它将最终成为未来的 OpenGL 规范的一部分。

在 OpenGL 中，任何事物都在 3D 空间中，而屏幕和窗口却是 2D 像素数组，这导致 OpenGL 的大部分工作都是关于把 3D 坐标转变为适应你屏幕的 2D 像素。3D 坐标转为 2D 坐标的处理过程是由 OpenGL 的图形渲染管线（Graphics Pipeline，大多译为管线，实际上指的是一堆原始图形数据途经一个输送管道，期间经过各种变化处理最终出现在屏幕的过程）管理的。

图形渲染管线接受一组 3D 坐标，然后把它们转变为你屏幕上的有色 2D 像素输出。

- 图形渲染管线可以被粗划分为两个主要部分：
  - 第一部分把你的3D坐标转换为2D坐标，
  - 第二部分是把2D坐标转变为实际的有颜色的像素。
  - note: **2D 坐标和像素** 也是不同的，2D 坐标精确表示一个点在 2D 空间中的位置，而 2D 像素是这个点的近似值，2D 像素受到你的屏幕/窗口分辨率的限制。
- 图形渲染管线可以被细划分为几个阶段，每个阶段将会把前一个阶段的输出作为输入。所有这些阶段都是高度专门化的（它们都有一个特定的函数），并且很容易并行执行。正是由于它们具有并行执行的特性，当今大多数显卡都有成千上万的小处理核心，它们在 GPU 上为每一个（渲染管线）阶段运行各自的小程序，从而在图形渲染管线中快速处理你的数据。这些小程序叫做着色器(Shader)。

有些着色器允许开发者自己配置，这就允许我们用自己写的着色器来替换默认的。这样我们就可以更细致地控制图形渲染管线中的特定部分了，而且因为它们运行在 GPU 上，所以它们可以给我们节约宝贵的 CPU 时间。OpenGL 着色器是用 OpenGL 着色器语言(OpeGL Shading Language, GLSL)写成的，在下一节中我们再花更多时间研究它。

![openGL图形渲染管线-各阶段.jpg](../image/openGL图形渲染管线-各阶段.jpg)

- 要注意蓝色部分代表的是我们可以注入自定义的着色器的部分。

1. 图形渲染管线的第一个部分是 **顶点着色器(Vertex Shader)**，它把一个单独的顶点作为输入。顶点着色器主要的目的是把 3D 坐标转为另一种 3D 坐标（后面会解释），同时顶点着色器允许我们对顶点属性进行一些基本处理。
2. **图元装配(Primitive Assembly)** 阶段将顶点着色器输出的所有顶点作为输入（如果是 `GL_POINTS`，那么就是一个顶点），并所有的点装配成指定图元的形状；本节例子中是一个三角形。图元装配阶段的输出会传递给几何着色器(Geometry Shader)。
   - 为了让 OpenGL 知道我们的坐标和颜色值构成的到底是什么，OpenGL 需要你去指定这些数据所表示的渲染类型。我们是希望把这些数据渲染成一个点？一个三角形？还是仅仅是一个长长的线？
   - 做出的这些提示叫做 图元(Primitive)，任何一个绘制指令的调用都将把图元传递给 OpenGL。这是其中的几个：`GL_POINTS、GL_TRIANGLES、GL_LINE_STRIP`。
3. **几何着色器** 几何着色器把图元形式的一系列顶点的集合作为输入，它可以通过产生新顶点构造出新的（或是其它的）图元来生成其他形状。其输出会被传入光栅化阶段 (Rasterization Stage). 
   - 这里几何着色器会把图元映射为最终屏幕上相应的像素，生成供片段着色器(Fragment Shader)使用的片段(Fragment)。
   - 在片段着色器运行之前会执行*裁切(Clipping)*。裁切会丢弃超出你的视图以外的所有像素，用来提升执行效率。
   - <u>**注 : OpenGL 中的一个片段是 OpenGL渲染一个像素所需的所有数据。**</u>
4. **片段着色器** 的主要目的是计算一个像素的最终颜色，这也是所有 OpenGL 高级效果产生的地方。通常，片段着色器包含 3D 场景的数据（比如光照、阴影、光的颜色等等），这些数据可以被用来计算最终像素的颜色。
5. 在所有对应颜色值确定以后，最终的对象将会被传到最后一个阶段，我们叫做 Alpha 测试和混合(Blending)阶段。这个阶段检测片段的对应的深度（和模板(Stencil)）值（后面会讲），用它们来判断这个像素是其它物体的前面还是后面，决定是否应该丢弃。这个阶段也会检查 alpha 值（alpha 值定义了一个物体的透明度）并对物体进行混合(Blend)。所以，即使在片段着色器中计算出来了一个像素输出的颜色，在渲染多个三角形的时候最后的像素颜色也可能完全不同。

可以看到，图形渲染管线非常复杂，它包含很多可配置的部分。然而，对于大多数场合，我们只需要配置顶点和片段着色器就行了。几何着色器是可选的，通常使用它默认的着色器就行了。

在现代 OpenGL 中，我们必须定义至少一个顶点着色器和一个片段着色器（因为 GPU 中没有默认的顶点/片段着色器）。出于这个原因，刚开始学习现代 OpenGL 的时候可能会非常困难，因为在你能够渲染自己的第一个三角形之前已经需要了解一大堆知识了。在本节结束你最终渲染出你的三角形的时候，你也会了解到非常多的图形编程知识。

---

## <font color=#009A000> [**点击此处阅读 shader 相关知识**](./GLSL.md) </font>

---

## <font color=#009A000> **理解 OpenGL 的客户端和服务端** </font>

> [OpenGL-客户端和服务器模式](https://www.jianshu.com/p/a9ffde5dd0b4)

任何一种 OpenGL 程序, 本质上都可以分为两部分:

- CPU 端运行的部分, 采用 C++ 之类的语言编写;
- GPU 端运行的部分, 使用 GLSL 语言编写;

首先明确下当今图像系统的构成:

- CPU + DDR; CPU 串行处理数据.
  - 一般的集显和 arm-soc 都采用上述这种形式;
- GPU + GPU_DDR(显存); GPU 并行处理数据, 有时候 GPU 还带有自己独立的显存!

OpenGL 是按照 `客户机->服务器` 模式设计的:

- 客户端：
  - 负责发送 OpenGL 命令;
  - CPU 上面存储的代码，比如 OpenGLApi，C 和 C++ 代码;
- 服务端：
  - 负责接收 OpenGL 命令并执行相应的操作;
  - 调用的是 GPU 芯片;

客户端是存储在 `CPU` 储存器中的，并且在 `应用程序` 中执行或者在主系统内存中驱动程序中执行。 `驱动程序` 会将 `渲染命令` 和 `数组` 组合起来，发送给 `服务器` 执行！

`服务器` 和 `客户端` 在功能上也是异步的。客户端不断的把数据块和命令块组合在一起输送到缓冲区，然后缓冲区就会发送到服务器执行.

<u>**即 :**</u>

- 客户端：CPU、内存等硬件，以及用户编写的 OpenGL 程序
- 服务端：OpenGL 驱动程序、显示设备（实际上就是图形加速卡上的硬件和内存）

<font color=#EA00DA> <u>**注意 : 此部分内容非常重要, 一定要有自己的理解 !!! 否则后续的很多东西都你将会知道怎么用, 但不知道为什么这么设计, 为什么这么用 !**</u></font>

---

<div STYLE="page-break-after: always;"></div><!------------------分页符------------------->

### <font color=#FF4500> **BO, VBO, VAO 概念详解** </font>

0. Buffer Object, `**BO**` :
   - OpenGL 有很多缓冲对象类型，缓冲的意思是将 CPU 端的数据传送到 GPU(通过 `glBufferData` 传递), 然后可以通过一个 ID 来进行管理和使用;
     - 使用这些缓冲对象的好处是我们可以一次性的发送一大批数据到显卡上，而不是每个顶点发送一次。从 CPU 把数据发送到显卡相对较慢，所以只要可能我们都要尝试尽量一次性发送尽可能多的数据。当数据发送至显卡的内存中后，顶点着色器几乎能立即访问顶点，这是个非常快的过程。
        - 不明白的话, 滚回去接着看上面的 openGL 客户端和服务端模式.
   - 虽然 BO 能将数据送到 GPU 端, 但 GPU 怎么使用(是颜色? 是顶点? 还是其它的索引数据?), 这些 GPU 都不知道.
1. 顶点缓冲对象：Vertex Buffer Object，`**VBO**`
   - 通过绑定 BO 至 `GL_ARRAY_BUFFER` 我们得到了 VBO, <u>VBO 确定了这是顶点数据(供顶点着色器使用). </u>
     - 但 VBO 没解释这是什么的顶点(点? 线? 三角形? 四边形?), 也说不了, 仅此而已.
       - OpenGL 需要你去指定这些数据所表示的渲染类型。我们是希望把这些数据渲染成一系列的点？一系列的三角形？还是仅仅是一个长长的线？做出的这些提示叫做**图元(Primitive)**，任何一个绘制指令的调用都将把图元传递给 OpenGL。这是其中的几个：`GL_POINTS、GL_TRIANGLES、GL_LINE_STRIP`。 *这些都是后话了.*
     - 而且 VBO 本身也没有并没有说明该如何解释(`glVertexAttribPointer()`) BO 中的数据; 即: **VBO 仅仅是 BO 的一个更具体的子类**, 仅仅表示该 BO 是 vertex 的 BO;
2. VBO 的属性指针 ( 通过 `glVertexAttribPointer()` 函数指定):
   - 一个 VBO 的属性指针包含 :
     - 数据类型 int? float?
     - 每个位置数据的个数, 3 个一组? 还是 4 个一组?
     - 数据的 stride . 
     - 数据的起始地址等等;
   - 一个包含了属性的 VBO 可表示成下图所示 :
   ![VBO](./../image/VBO.png)
3. 顶点数组对象：Vertex Array Object，`**VAO**`, 又是对 VBO 的一次封装 ---> **( VBO 属性指针 + 配置的 VBO 的状态 )**;
   - 顶点数组对象(Vertex Array Object, VAO)可以像顶点缓冲对象那样被绑定，任何随后的顶点属性调用都会储存在这个 VAO 中。
   - 这样的**好处**就是，当配置顶点属性指针时，你只需要将那些调用执行一次，之后再绘制物体的时候只需要绑定相应的 VAO 就行了。 这使在不同顶点数据和属性配置之间切换变得非常简单，只需要绑定不同的 VAO 就行了。刚刚设置的所有状态都将存储在 VAO 中.
   ![VAO](./../image/VAO.png)
   - 一个顶点数组对象会储存以下这些内容：
      - `glEnableVertexAttribArray()` 时的启用的是哪个定点(属性);
      - 通过 `glVertexAttribPointer()` 设置的顶点属性配置。
      - 通过 `glVertexAttribPointer()` 调用与顶点属性关联的顶点缓冲对象。
      - 配置的 EBO 及其状态;
4. 索引缓冲对象：Element Buffer Object，`**EBO**` 或 Index Buffer Object，`**IBO**`
   - EBO 是减少 VBO 中顶点数量的一次变种, 同时也可以被 VAO 管理.

---

**总结一下 (用结构体实现的方式去理解上述概念):**

1. BO 是 GPU 端的一个内存, 里面存放了数据;
2. VBO 将 BO 绑定至 vertex, 即 : 指明了那块内存数据是 vertex 顶点数据;
     - `glVertexAttribPointer()` 顶点属性指针, 则用来解释 VBO 中数据类型和到底该怎么划分使用;
         - <u>**配置完顶点属性指针后, 就可以直接去绘制了,**</u> 下方的 EBO, VAO, 都是附加项;
3. EBO 是也是一个 BO, 存有数据, 只不过其数据是 VBO 中顶点数据的索引, 其目的是用来减少 VBO 中数据的个数的;
4. VAO 则是一系列的指针, 这些指针分别都指向了 2 中已可以使用的顶点, 其目的是一次配置到处使用;

---

### <font color=#FF4500> BO,VAO,VBO,EBO 使用详解 </font>

定义顶点数据以后，我们会把它作为输入发送给图形渲染管线的第一个处理阶段：顶点着色器。

- BO 会在 **GPU** 上创建内存用于储存我们的数据 ;
  - 就像 OpenGL 中的其它对象一样，这个缓冲有一个独一无二的 ID，所以我们可以使用 `glGenBuffers()` 函数和一个缓冲 ID 生成一个对象的 ID (<u>即：向状态机获取一个独一无二的 ID</u>) ：
    - `void glGenBuffers(GLsizei n,GLuint * buffers);`
- OpenGL 有很多缓冲对象类型 `GL_ARRAY_BUFFER / GL_ELEMENT_ARRAY_BUFFER / GL_PIXEL_PACK_BUFFER` 等，顶点缓冲对象的缓冲类型是 `GL_ARRAY_BUFFER`。
  - 我们可以使用 `glBindBuffer()` 函数把新创建的缓冲绑定到 `GL_ARRAY_BUFFER` 目标上(VBO) (<u>即 ：向状态机声明刚刚指定的 ID 的类型</u>) ：
    - `void glBindBuffer(GLenum target,GLuint buffer);`
  - 我们通过 **顶点缓冲对象(Vertex Buffer Objects, VBO)** <u>*管理其申请配置的内存*</u>，它会在  <u>GPU 内存（通常被称为显存）</u>中储存大量顶点。从这一刻起，我们使用的任何（在 `GL_ARRAY_BUFFER` 目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)。
  - 然后我们可以调用 `glBufferData()` 函数，它会把之前定义的顶点数据复制到 GPU 端缓冲的内存中：`void glBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);`
    - 本函数用法的另一种解释: 在服务端内存中分配 size 个存储单元, 用于存储数据或者索引; 然后将数据从应用程序的数组中拷贝到 OpenGL 服务端的内存中.
    - 参数介绍:
      - 第四个参数指定了我们希望显卡如何管理给定的数据。它有三种形式：
        - `GL_STATIC_DRAW` ：数据不会或几乎不会改变。
        - `GL_DYNAMIC_DRAW`：数据会被改变很多。
        - `GL_STREAM_DRAW` ：数据每次绘制时都会改变。
      - 比如说一个缓冲中的数据将频繁被改变，那么使用的类型就是 `GL_DYNAMIC_DRAW` 或 `GL_STREAM_DRAW`，这样就能确保显卡把数据放在能够高速写入的内存部分。
  - > `void glBindBuffer(	GLenum target, GLuint buffer);`  
    - 参考链接: [glGenBuffers与glBindBuffer理解](https://blog.csdn.net/qq_36383623/article/details/85123077)
    - OpenGL 允许我们同时绑定多个缓冲，只要它们是不同的缓冲类型。**换句话说，同一时间，不能绑定两个相同类型的缓冲对象.** 比如顶点缓冲类型 `GL_ARRAY_BUFFER`，`glBufferData()` 是通过指定目标缓冲类型来进行数据传输的，而每一个目标缓冲类型再使用前要提前绑定一个缓冲对象，从而赋予这个缓冲对象一个类型的意义，如果绑定了两个相同类型的目标缓冲，数据的配置肯定就会出错。（可以这样想一下，我要把数据存入顶点缓冲区，但是顶点缓冲区可以有很多缓冲对象，我需要传入哪个呢，于是我就要提前绑定一个，之后，我只要向顶点缓冲区内传入数据，这个数据就会自动进入被绑定的那个对象里面）.
- 还要配置 OpenGL 如何解释这些内存，即: 手动指定输入数据的哪一个部分对应 *顶点着色器* 的哪一个顶点属性:
  - `void glVertexAttribPointer（GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer）;`
    - 第一个参数指定我们要配置的顶点属性。还记得我们在**顶点着色器**中使用 `layout(location = 0)` 定义了 position 顶点属性的位置值(Location)吗？它可以把顶点属性的位置值设置为 0。因为我们希望把数据传递到这一个顶点属性中，所以这里我们传入 `0`。
    - 第二个参数指定顶点属性的大小。顶点属性是一个 `vec3`，它由 3 个值组成，所以大小是 3。
    - 第三个参数指定数据的类型，这里是 `GL_FLOAT` (**GLSL 中 vec\* 都是由浮点数值组成的**)。
    - 下个参数定义我们是否希望数据被标准化(Normalize)。如果我们设置为 `GL_TRUE`，所有数据都会被映射到 0（对于有符号型 signed 数据是 -1）到 1 之间。我们把它设置为 `GL_FALSE`。
    - 第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。
      - 由于下个组位置数据在3 个 float 之后，我们把步长设置为 `3 * sizeof(float)` 。
      - 要注意的是由于我们知道这个数组是紧密排列的（在两个顶点属性之间没有空隙）我们也可以设置为 `0` 来让 OpenGL 决定具体步长是多少（只有当数值是紧密排列时才可用）。
      - 一旦我们有更多的顶点属性，我们就必须更小心地定义每个顶点属性之间的间隔，我们在后面会看到更多的例子（译注: 这个参数的意思简单说就是从这个属性第二次出现的地方到整个数组0位置之间有多少字节）。
    - 最后一个参数的类型是 `void *` ，所以需要我们进行这个奇怪的强制类型转换。它表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是 0。我们会在后面详细解释这个参数。
      - 使用 VBO 时, 每个顶点属性从一个 VBO 管理的内存中获得它的数据，而具体是从哪个 VBO（程序中可以有多个 VBO）获取则是通过在调用 `glVertexAttribPointer()` 时绑定到 `GL_ARRAY_BUFFER` 的 VBO 决定的。由于在调用 `glVertexAttribPointer` 之前绑定的是先前定义的 VBO 对象，顶点属性 0 现在会链接到它的顶点数据。
      - > [GLES2.0中文API-glBindBuffer](https://blog.csdn.net/flycatdeng/article/details/82664520) 
      - > When vertex array pointer state is changed by a call to glVertexAttribPointer, the current buffer object binding (GL_ARRAY_BUFFER_BINDING) is copied into the corresponding client state for the vertex attrib array being changed, one of the indexed GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDINGs. While a non-zero buffer object is bound to the GL_ARRAY_BUFFER target, the vertex array pointer parameter that is traditionally interpreted as a pointer to <u>client-side memory</u> is instead interpreted as an offset within the buffer object measured in basic machine units.
      - > 即 `glVertexAttribPointer()` 原本的最后一个参数是顶点属性数组的指针. 但, 当绑定 VBO 后，最后一个参数就是指向所绑定的 VBO 的一个地址了
      - 具体的例子 : [demo-triangle-indexed](../code/triangle_indxed/triangle_indxed.c)
- 并且指定其如何发送给显卡。
- 使能服务端对该数据的访问 `glEnableVertexAttribArray()` :
  > [OpenGL-ES 3.0 glEnableVertexAttribArray的作用](https://www.jianshu.com/p/b05884a0ff8e/)
  - 对于 OpenGL-ES 3.0，可使用至少 16 个顶点属性, 默认情况下，出于性能考虑，所有顶点着色器的属性（Attribute）变量都是关闭的，意味着数据在着色器端是不可见的，哪怕数据已经上传到 GPU.
  - 数据在 GPU 端是否可见，即，着色器能否读取到数据，由是否启用了对应的属性决定，这就是 `glEnableVertexAttribArray()` 的功能，允许顶点着色器读取 GPU（服务器端）数据. 由 `glEnableVertexAttribArray()` 启用指定属性，才可在顶点着色器中访问逐顶点的属性数据。
  - 那么，`glEnableVertexAttribArray()` 应该在 `glVertexAttribPointer()` 之前还是之后调用？答案是都可以，只要在绘图调用（ `glDraw*` 系列函数）前调用即可。

---

**线框模式(Wireframe Mode)**

要想用线框模式绘制你的三角形，你可以通过 `glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)` 函数配置 OpenGL 如何绘制图元。默认为： 直到我们用 `glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)` 将其设置回默认模式。

- 第一个参数表示我们打算将其应用到所有的三角形的正面和背面，
- 第二个参数告诉我们用线来绘制， 之后的绘制调用会一直以线框模式绘制三角形。

### <font color=#FF4500> VAO 和 IBO </font>

顶点数组对象(Vertex Array Object, VAO)可以像顶点缓冲对象那样被绑定，任何随后的顶点属性调用都会储存在这个 VAO 中。这样的好处就是，当配置顶点属性指针时，你只需要将那些调用执行一次，之后再绘制物体的时候只需要绑定相应的 VAO 就行了。这使在不同顶点数据和属性配置之间切换变得非常简单，只需要绑定不同的 VAO 就行了。**刚刚设置的所有状态都将存储在 VAO 中.**

在渲染顶点这一话题上我们还有最后一个需要讨论的东西——索引缓冲对象(Element Buffer Object，EBO，也叫Index Buffer Object，IBO)。要解释索引缓冲对象的工作方式最好还是举个例子：假设我们不再绘制一个三角形而是绘制一个矩形。我们可以绘制两个三角形来组成一个矩形（OpenGL主要处理三角形）。这会生成下面的顶点的集合：

- 其余： 略； 参考 :
  > [learnopengl-CN 你好，三角形](https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/)

<div STYLE="page-break-after: always;"></div><!------------------分页符------------------->

### <font color=#FF4500> 关于状态机 </font>

- openGL 是一个 **状态机** , 尤其是如果你使用固定功能的管线.
  - 你可以设置它的各种状态,  然后让这些状态一直生效, 直到再次修改他们.
  - 许多表示模式的状态变量可以使用 `glENable()` 和 `glDisable()` 函数进行启用和禁用.
- openGL 的状态机思想无处不在, 在 `hello_triangle_exercise1.cpp` 中, 我们使用了如下的 方式, 可以帮助你更好的理解状态机的思想:

```cpp
float vertices[] = {
    // first triangle
    -0.9f, -0.5f, 0.0f,  // left 
    -0.0f, -0.5f, 0.0f,  // right
    -0.45f, 0.5f, 0.0f,  // top 
    // second triangle
      0.0f, -0.5f, 0.0f,  // left
      0.9f, -0.5f, 0.0f,  // right
      0.45f, 0.5f, 0.0f   // top 
}; 

unsigned int VBO, VAO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);
// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
glBindVertexArray(VAO);

glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
glBindBuffer(GL_ARRAY_BUFFER, 0); /* 恢复默认的绑定状态, 即: 解绑 glBindBuffer(GL_ARRAY_BUFFER, VBO); */

// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
glBindVertexArray(0); /* 解绑 VAO */

/* 有了上方的解绑, 所以使用时, 才在下方再次绑定了 VAO */
glUseProgram(shaderProgram);
glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
glDrawArrays(GL_TRIANGLES, 0, 6); // set the count to 6 since we're drawing 6 vertices now (2 triangles); not 3!
```

---

## <font color=#009A000> **纹理** </font>

纹理的由来 : 我们可以为每个顶点添加颜色来增加图形的细节，从而创建出有趣的图像。但是，<u>**如果想让图形看起来更真实，我们就必须有足够多的顶点，从而指定足够多的颜色。这将会产生很多额外开销，因为每个模型都会需求更多的顶点，每个顶点又需求一个颜色属性。**</u> 艺术家和程序员更喜欢使用纹理(Texture)。

**为了能够把纹理映射(Map)到三角形上，我们需要指定三角形的每个顶点各自对应纹理的哪个部分。这样每个顶点就会关联着一个纹理坐标(Texture Coordinate)，用来标明该从纹理图像的哪个部分采样（译注：采集片段颜色）。之后在图形的其它片段上进行片段插值(Fragment Interpolation)。**

纹理渲染首先都是需要在图元绘制的基础上运行的.(基本的图元: 点, 线, 三角形!)这个可以由纹理的定义来看出: 

- 纹理(Texture) ：一种**包裹着物体**的特殊类型图像，给物体精细的视觉效果

所以, 对于 YUV 格式的处理, 我们首先需要进行图元的绘制, 然后再把纹理贴图到图元上, 没有图元, 纹理无从谈起!(实际中, 我们的代码顺序刚好相反, 先配置纹理贴图数据, 然后再绘制图元: 由两个三角形组成的四边形图元)

```c
glBindTexture(GL_TEXTURE_2D, texture);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
```

<u>**纹理(texture) 坐标(coords)** 起始于(0, 0)，也就是纹理图片的左下角，终始于(1, 1)，即纹理图片的右上角。</u>

```c
// 1. 生成/创建纹理, 并得到对应的 ID.
void glGenTextures( 
        GLsizei n,           /* 指定要生成的纹理 ID 的数量。*/
        GLuint * textures);  /* 指定存储生成的纹理 ID 的数组。 */

// 2. 绑定纹理, 解释纹理是一维, 二维, 还是其它
void glBindTexture(
        GLenum target,  /*     指定之前激活了的纹理要绑定到的一个目标。
                        必须是 GL_TEXTURE_2D 或 GL_TEXTURE_CUBE_MAP。 */
        GLuint texture); /*     指定纹理 ID。 */

// 3. 使用 glUniform1i 设置每个采样器的方式告诉 OpenGL 每个着色器采样器属于哪个纹理单元。
glUniform1i(glGetUniformLocation(program, "texture0"), 0); /* GL_TEXTURE0 */
glUniform1i(glGetUniformLocation(program, "texture1"), 1); /* GL_TEXTURE1 */

// 4. 为当前绑定的纹理对象设置环绕、过滤方式
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

// 5. 加载纹理的内容, 加载后可删除源数据 [OpenGL-ES2.0/3.0 glTexImage2D 函数详解](https://blog.csdn.net/jeffasd/article/details/78135588)
void glTexImage2D(
       GLenum target,   /* 维度 */
　　　　GLint level,    /* 理指定多级渐远纹理的级别，一般为 0 */
　　　　GLint internalFormat,   /* 指的是纹理数据在 OpenGL 中是如何表示的， 如 GL_RGB 
                                  就表示纹理的像素在 OpenGL 里面以红绿蓝三个分量表示，
                                  在纹理图像加载过程中不支持格式间的转换。 */
　　　　GLsizei height,
　　　　GLint border,   /* 应该总是被设为 0（历史遗留的问题） */
　　　　GLenum format,  /* 载入纹理的格式，它告诉 OpenGL
                          外部数据是如何存储每个像素数据的 */
　　　　GLenum type,    /* 组成图像的数据是无符号字节类型 */
　　　　const GLvoid * data); /* 图像数据 */
// data可能是一个空指针。在这种情况下，会分配纹理内存以适应宽度width和高度的纹理height。 ???????????????

// 6. 现在只剩下在调用 glDrawElements 之前绑定纹理了，它会自动把纹理赋值给片段着色器的采样器：
/* 使用第一个纹理 */
glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元
glBindTexture(GL_TEXTURE_2D, texture0); // texture0 <--> GL_TEXTURE0
/* 使用第二个纹理*/
glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D, texture1); // texture1 <--> GL_TEXTURE1
```

我们能够在一个片段着色器中设置多个纹理。OpenGL 至少保证有 16 个纹理单元供你使用，也就是说你可以激活从 `GL_TEXTURE0` 到 `GL_TEXTRUE15`。它们都是按顺序定义的，所以我们也可以通过 `GL_TEXTURE0 + 8` 的方式获得 `GL_TEXTURE8` ，这在当我们需要循环一些纹理单元的时候会很有用。

一个纹理的位置值通常称为一个纹理单元(Texture Unit)。一个纹理的默认纹理单元是 `0` (`GL_TEXTURE0`)，它是默认的激活纹理单元. 纹理是全局的, 在 shaders 中定义了纹理后, 需要对纹理单元进行排序赋值, 这样使用的时候就能使用 GL_TEXTURE0 之类的了. 而这个绑定, 只需要一次.

<u>*这里主要有两个纹理参数:*</u>

1. 一个是 `glGenTextures()` 生成的纹理 `ID(texture0)`. 此 `ID` 主要用于 openGL 程序中对纹理进行参数配置等, 可以理解为 openGL 状态机中的纹理 `ID`.
2. 第二个为 `GL_TEXTURE0`, 这个是 GLSL 着色器程序中的纹理名字. 
3. 在通过第一个 `ID` 配置好纹理的参数后, 需要传递到 GLSL 的中使用, 两者的对照绑定如上步骤 6 所示.

### <font color=#FF4500> 纹理参数 </font>

> 1. [OpenGL纹理详解（上）](https://www.jianshu.com/p/1829b4acc58d)
> 2. [OpenGL--------纹理处理](https://www.cnblogs.com/Anita9002/p/4410235.html)

通过纹理坐标获取像素颜色信息的过程称为采样，而采样的结果会根据纹理参数设置的不同而千差万别。OpenGL 中设置纹理参数的 API 接口为 `glTextureParameter()`，我们所有的纹理参数都由这个接口设置，下面我们介绍几种常用的纹理参数的配置。

1. **warpping:** 纹理坐标的范围与 OpenGL 的屏幕坐标范围一样，是 0-1。超出这一范围的坐标将被 OpenGL 根据 GL_TEXTURE_WRAP 参数的值进行处理:
   - `GL_TEXTURE_WRAP_S` 横轴 x 方向, `GL_TEXTURE_WRAP_T` 纵轴 y 方向;
     - `GL_REPEAT` 犹如字面意思那样会重复，当几何纹理坐标大于 1.0 的时候，他取的纹理坐标的值位于纹理坐标减去 1.0 的位置，如纹理坐标是 1.1 的时候，他取的颜色的值是 0.1。所以 1.0 和 2.0 一样，1.1 和 0.1 一样，所以就会重复，就向字面意思那样。
     - `GL_MIRRORED_REPEAT`: 超出纹理范围的坐标整数部分被忽略，但当整数部分为奇数时进行取反，形成镜像效果。
     - `GL_CLAMP_TO_EDGE` :超出纹理范围的坐标被截取成0和1，形成纹理边缘延伸的效果。
     - `GL_CLAMP_TO_BORDER` : 超出纹理范围的部分被设置为边缘色。
       - 如果我们将 wrapping 模式设置为 `GL_CLAMP_TO_BORDER`，我们需要单独设置另一属性----边界颜色, 如下：
        ````c 
        float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        glTextureParameter(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color)
        ````

![纹理warp](../image/纹理warp.jpg)

2. **过滤**:   由于纹理坐标和我们当前的屏幕分辨率是无关的，所以当我们为一个模型贴纹理时，往往会遇到纹理尺寸与模型尺寸不符的情况，这时，纹理会因为缩放而失真。处理这一失真的过程我们称为过滤，在OpenGL中我们有如下几种常用的过滤手段：
     - `GL_TEXTURE_MAG_FILTER`：指当纹理图象被使用到一个大于它的形状上时（即：有可能纹理图象中的一个像素会被应用到实际绘制时的多个像素。例如将一幅 `256*256` 的纹理图象应用到一个`512*512` 的正方形），应该如何处理。
     - `GL_TEXTURE_MAG_FILTER`：指当纹理图象被使用到一个大于它的形状上时（即：有可能纹理图象中的一个像素会被应用到实际绘制时的多个像素。例如将一幅 `256*256` 的纹理图象应用到一个`512*512` 的正方形），应该如何处理。
     - 可选择的设置有:
       - `GL_NEAREST`: 最临近过滤，获得最靠近纹理坐标点的像素。
       - `GL_LINEAR`: 线性插值过滤，获取坐标点附近 4 个像素的加权平均值。
       - 以及用于 mipmap 的四个参数: `GL_NEAREST_MIPMAP_NEAREST` ,  `GL_LINEAR_MIPMAP_NEAREST`, `GL_NEAREST_MIPMAP_LINEAR`, `GL_LINEAR_MIPMAP_LINEAR` ;

![纹理过滤](../image/纹理过滤.jpg)

---

## <font color=#009A000> 显存/内存占用 </font>

> 1. [OpenGL 使用 PBO 高速复制屏幕图像到内存或者纹理中](https://www.cnblogs.com/crsky/p/7870835.html)
> 2. [OpenGL深入探索——像素缓冲区对象 （PBO）](https://blog.csdn.net/panda1234lee/article/details/51546502)
> 3. [Android使用Direct Textures提高glReadPixels、glTexImage2D性能](https://www.jianshu.com/p/1fa36461fc6f)

openGL 需要将所有的数据都保存到缓存对象(buffer object)中, 它相当于由 OpenGL 维护的一块内存区域.

openGL 的绘制通常就是将顶点数据传输到 OpenGL 服务端.

---

## <font color=#009A000> 坐标变换 </font>

> `@think3r` 2020-04-04 16:44:35

将物体的坐标变换到几个过渡坐标系(Intermediate Coordinate System)的优点在于，**在这些特定的坐标系统中，一些操作或运算更加方便和容易，这一点很快就会变得很明显**。

OpenGL 希望在每次顶点着色器运行后，我们可见的所有顶点都为标准化设备坐标(Normalized Device Coordinate, NDC)。

- 也就是说，每个顶点的 x，y，z 坐标都应该在 -1.0 到 1.0 之间，超出这个坐标范围的顶点都将不可见。
- 我们通常会自己设定一个坐标的范围，之后再在顶点着色器中将这些坐标变换为标准化设备坐标。
- 然后将这些标准化设备坐标传入光栅器(Rasterizer)，将它们变换为屏幕上的二维坐标或像素。

对我们来说比较重要的总共有5个不同的坐标系统：

![coordinate_systems.png](../image/coordinate_systems.png)

1. 局部空间(Local Space，或者称为物体空间(Object Space))
    - 局部坐标是对象相对于局部原点的坐标，也是物体起始的坐标。或者换个说法, 局部空间是指物体所在的坐标空间，即对象最开始所在的地方。
    - 想象你在一个建模软件（比如说Blender）中创建了一个立方体。你创建的立方体的原点有可能位于(0, 0, 0)，即便它有可能最后在程序中处于完全不同的位置。甚至有可能你创建的所有模型都以(0, 0, 0)为初始位置（译注：然而它们会最终出现在世界的不同位置）。所以，你的模型的所有顶点都是在局部空间中：它们相对于你的物体来说都是局部的。
2. 世界空间(World Space)
    - 世界空间坐标是处于一个更大的空间范围的。这些坐标相对于世界的全局原点，它们会和其它物体一起相对于世界的原点进行摆放。
    - 如果我们将我们所有的物体导入到程序当中，它们有可能会全挤在世界的原点(0, 0, 0)上，这并不是我们想要的结果。我们想为每一个物体定义一个位置，从而能在更大的世界当中放置它们。
    - 世界空间中的坐标正如其名：是指顶点相对于（游戏）世界的坐标。如果你希望将物体分散在世界上摆放（特别是非常真实的那样），这就是你希望物体变换到的空间。
    - 物体的坐标将会从局部变换到世界空间；该变换是由 `模型矩阵(Model Matrix)` 实现的。模型矩阵是一种变换矩阵，它能通过对物体进行 `位移、缩放、旋转` 来将它置于它本应该在的位置或朝向。
      - 你可以将它想像为变换一个房子，你需要先将它缩小（它在局部空间中太大了），并将其位移至郊区的一个小镇，然后在y轴上往左旋转一点以搭配附近的房子。
3. 观察空间(View Space，或者称为视觉空间(Eye Space))
    - 观察空间的每个坐标都是从摄像机或者说观察者的角度进行观察的。观察空间经常被人们称之 `OpenGL的摄像机(Camera)` （所以有时也称为摄像机空间(Camera Space)或视觉空间(Eye Space)）。
    - 观察空间是将世界空间坐标转化为用户视野前方的坐标而产生的结果。因此观察空间就是从摄像机的视角所观察到的空间。
    - 而这通常是由一系列的位移和旋转的组合来完成，平移/旋转场景从而使得特定的对象被变换到摄像机的前方。这些组合在一起的变换通常存储在一个 `观察矩阵(View Matrix)` 里，它被用来将世界坐标变换到观察空间。在下一节中我们将深入讨论如何创建一个这样的观察矩阵来模拟一个摄像机。
4. 裁剪空间(Clip Space)
    - **在一个 <u>顶点着色器运行的最后，</u> OpenGL 期望所有的坐标都能落在一个特定的范围内，且任何在这个范围之外的点都应该被裁剪掉(Clipped)。** 被裁剪掉的坐标就会被忽略，所以剩下的坐标就将变为屏幕上可见的片段。这也就是裁剪空间(Clip Space)名字的由来。
    - 坐标到达观察空间之后，我们需要将其投影到裁剪坐标。裁剪坐标会被处理至 -1.0 到 1.0 的范围内，并判断哪些顶点将会出现在屏幕上。
    - 因为将所有可见的坐标都指定在 -1.0 到 1.0 的范围内不是很直观，所以我们会指定自己的坐标集(Coordinate Set)并将它变换回标准化设备坐标系，就像 OpenGL 期望的那样。
    - 为了将顶点坐标从观察变换到裁剪空间，我们需要定义一个 `投影矩阵(Projection Matrix)，`
      - 它指定了一个范围的坐标，比如在每个维度上的 -1000 到 1000。投影矩阵接着会将在这个指定的范围内的坐标变换为标准化设备坐标的范围(-1.0, 1.0)。所有在范围外的坐标不会被映射到在 -1.0 到 1.0 的范围之间，所以会被裁剪掉。
      - 如果只是图元(Primitive)，例如三角形，的一部分超出了裁剪体积(Clipping Volume)，则 OpenGL 会重新构建这个三角形为一个或多个三角形让其能够适合这个裁剪范围。
    - **由投影矩阵创建的观察箱(Viewing Box)被称为平截头体(Frustum)，每个出现在平截头体范围内的坐标都会最终出现在用户的屏幕上。** 将特定范围内的坐标转化到标准化设备坐标系的过程（而且它很容易被映射到 2D 观察空间坐标）被称之为投影(Projection)，因为使用投影矩阵能将 3D 坐标投影(Project)到很容易映射到 2D 的标准化设备坐标系中。
    - 将观察坐标变换为裁剪坐标的投影矩阵可以为两种不同的形式，每种形式都定义了不同的平截头体。我们可以选择创建一个正射投影矩阵(Orthographic Projection Matrix)或一个透视投影矩阵(Perspective Projection Matrix)。
      - 正射投影;
      - 透视投影;

- 我们为上述的每一个步骤都创建了一个变换矩阵：模型矩阵、观察矩阵和投影矩阵。一个顶点坐标将会根据以下过程被变换到裁剪坐标：
![calc](../image/calc.jpg)
  - 注意矩阵运算的顺序是相反的（记住我们需要从右往左阅读矩阵的乘法）。
  - 最后的顶点应该被赋值到顶点着色器中的 `gl_Position` ，OpenGL将会自动进行透视除法和裁剪。
- **顶点着色器的输出要求所有的顶点都在裁剪空间内，这正是我们刚才使用变换矩阵所做的。** OpenGL 然后对裁剪坐标执行透视除法从而将它们变换到标准化设备坐标。OpenGL 会使用 `glViewPort()` 内部的参数来将标准化设备坐标映射到屏幕坐标，每个坐标都关联了一个屏幕上的点。这个过程称为`视口变换`.

5. 屏幕空间(Screen Space)
    - 最后，我们将裁剪坐标变换为屏幕坐标，我们将使用一个叫做视口变换(Viewport Transform)的过程。视口变换将位于 -1.0 到 1.0 范围的坐标变换到由 `glViewport()` 函数所定义的坐标范围内。最后变换出来的坐标将会送到光栅器，将其转化为片段。 

---

## <font color=#009A000> 自定义摄像机类 </font>

- OpenGL 本身没有摄像机(Camera)的概念，但我们可以通过把场景中的所有物体往相反方向移动的方式来模拟出摄像机，产生一种我们在移动的感觉，而不是场景在移动。
- 本节我们将会讨论如何在 OpenGL 中配置一个摄像机，并且将会讨论 FPS 风格的摄像机，让你能够在 3D 场景中自由移动。我们也会讨论键盘和鼠标输入，最终完成一个自定义的摄像机类。

---

## <font color=#009A000> 多物体绘制 </font>

- 比如, 你现在的代码已经可以绘制 1 个 房子的模型了, 但是你想要一下子绘制多个出来, 这时候怎么办呢?
  - 很好办, 再次调用图元绘制命令就行了, 但是如果你这么直接做了, 那看到的可能还是一个 三角形, 为什么呢?
  - 因为两次绘制的房子模型重叠了, 这时候, 你需要将第二次绘制的房子模型采用不同变换矩阵去映射到世界坐标系中即可.
- 单个的房子模型包含了其图形布局, 顶点间的相对位置等等. 第二个房子是在第一个房子的基础上进行了一次世界坐标系的复制和位置变换.


## <font color=#009A000> 颜色与光照 </font>

- 光照源其实是一个物体, 拥有光线颜色 `lightColor ` 和在三维空间中的位置 `lightPos`;
- 物体反射颜色的系数 `toyColor` 与光源颜色数值相乘就能得到这个物体本来该有的颜色.
- 基于去哦们对光物理特性的理解, 和有限的运算能力, 我们一般采用 `冯氏光照模型` 来进行光学模拟. 其主要有三个部分组成:
  - `环境(Ambient)光照` : 即使在黑暗光照的情况下, 世界上也通常会有一些光亮(月亮, 远处的光等), 所以物体基本上不会是完全黑暗的.
    - 一个单独的系数,
  - `漫反射光照(Diffuse Lighting)` : 模拟光源对物体方向性涌向(Directional Impact);
    - 法向量(Normal Vector): 一个垂直于顶点表面的向量。
    - 定向的光线：作为光源的位置与片段的位置之间向量差的方向向量。 为了计算这个光线，我们需要: 
      - 光的位置向量
      - 片段的位置向量
  - `镜面光照(Specular Lighting)` : 模拟有光泽物体上面出现的亮点。
