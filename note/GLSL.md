# <font color=#0099ff> **GLSL** </font>

> `@think3r` 2020-04-04 16:01:03 <br>
> 参考链接:
> 1. [learnopengl-CN 你好，三角形](https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/) 
> 2. [Getting started with glsl](https://riptutorial.com/glsl)
> 3. [GLSL Versions和GLSL ES Versions 对比](https://www.cnblogs.com/beeasy/p/6339313.html)
> 4. [GLSL 中文手册](https://github.com/wshxbqq/GLSL-Card)
> 5. [GLSL 详解（基础篇）](https://colin1994.github.io/2017/11/11/OpenGLES-Lesson04/)
> 6. [GLSL 详解（高级篇）](https://colin1994.github.io/2017/11/12/OpenGLES-Lesson05/#7-_%E9%A2%84%E5%A4%84%E7%90%86)
> 7. [OpenGL ES 2.0升级到3.0配置win32环境以及编译所遇bug](https://blog.csdn.net/lb377463323/article/details/77047221)

---
<br />

![../image/GLSL_version](../image/GLSL_version.bmp)

## <font color=#009A000> ES-GLSL 版本区别 </font>

- 对于 `GLSL` :
  > As of GLSL `130+`, `in` and `out` are used instead of `attribute` and `varying`. GLSL `330+` includes other features like `layout` qualifiers and changes `texture2D` to `texture`.
- 对于 `ES-GLSL` :
  - `1.00 ES-GLSL` 中的 `attribute` 和 `varying` 同样在 `3.00 ES-GLSL` 被 `in` 和 `out` 取代, 同样增加了 `layout` ;
  - `1.00 ES GLSL` 中的 `texture2D, texture3D` 在 `3.00 ES GLSL` 中被 `texture` 取代;
    - opengl-ES 中是没有 `texture1D` 的, 请使用高度为 1 的 `texture2D`
  - `1.00 ES GLSL` 的 `gl_FragColor` 和 `gl_FragData` 在 `3.00 ES GLSL` 中取消掉了，需要自己定义 out 变量作为片段着色器的输出颜色，如 `out vec4 fragColor` ;
  - 参考 `opengles20-reference-card.pdf` 和 `opengles32-quick-reference-card.pdf`;
- 变量名不能以 `gl_` 作为前缀;
- 切记 OpenGL 的矩阵是**主列存储**的，和 DirectX 以及大学线代学习的行存储矩阵有本质不同！
  - > 使用矩阵构造器时, 参数按列填充矩阵. By <<OpenGL ES 3.0 编程指南>>

| 限定符 ES-GLSL 1.00 | 描述 |
| --- | --- |
| < `none`: default > | 局部可读写变量，或者函数的参数 |
| `const` | 编译时常量，或只读的函数参数 |
| `attribute` |	由应用程序传输给顶点着色器的逐顶点的数据 |
| `uniform` | 在图元处理过程中其值保持不变，由应用程序传输给着色器 |
| `varying` | 由顶点着色器传输给片段着色器中的插值数据 |

| 限定符 GL-GLSL 3.00 | 描述 |
| --- | --- |
| `none` | (默认)本地读写内存, 或者输入参数 |
| `const` | 编译时的常量, 或者只读函数参数 |
| `in` / `centroid in` | 从前一阶段链接到一个着色器 |
| `out` / `centroid out` | 从着色器连接到下一个阶段 |
| `uniform` | 在图元处理中值不改变, 统一变量组成了着色器、 OpenGL ES 和应用程序的链接 |

## <font color=#009A000> shader 概述 </font>

- Shaders 在现代 OpenGL 中是个很重要的概念。应用程序离不开它，除非你理解了，否则这些代码也没有任何意义。
- Shaders 是一段 GLSL 小程序，运行在 GPU 上而非 CPU。它们使用 OpenGL Shading Language (GLSL) 语言编写，看上去像 C 或 C++，但却是另外一种不同的语言。使用 shader 就像你写个普通程序一样：写代码，编译，最后链接在一起才生成最终的程序。
- **Shaders 并不是个很好的名字，因为它不仅仅只做着色。只要记得它们是个用不同的语言写的，运行在显卡上的小程序就行。**
- 在旧版本固定管线的 OpenGL 中，shaders 是可选的。在现代 OpenGL 中，为了能在屏幕上显示出物体，shaders 是必须的。
- 一般有如下两种不同的着色器:
  - 在你的场景中，每个 **顶点** 都需要调用的程序，称为 **顶点着色器(Vertex shaders)** 。
    - 假如你在渲染一个简单的场景：一个长方形，每个角只有一个顶点。于是 vertex shader 会被调用四次。
    - 它负责执行：诸如灯光、几何变换等等的计算。得出最终的顶点位置后，为下面的片段着色器提供必须的数据。
  - 在你的场景中，大概每个像素都会调用的程序，称 **片段着色器(Fragment shaders)** 。
    - 在一个简单的场景，也是刚刚说到的长方形。这个长方形所覆盖到的每一个像素，都会调用一次 fragment shader。
    - 片段着色器的责任是计算灯光，以及更重要的是计算出每个像素的最终颜色。

## <font color=#009A000> 顶点着色器 </font>

- 顶点着色器的每个输入变量也叫 **顶点属性(Vertex Attribute)**.
- 我们能声明的顶点属性是有上限的，它一般由硬件来决定。OpenGL 确保至少有 16 个包含 4 分量的顶点属性可用 .

### <font color=#FF4500> 着色器基础 </font>

- 虽然着色器是各自独立的小程序，但是它们都是一个整体的一部分，出于这样的原因，我们希望每个着色器都有输入和输出，这样才能进行数据交流和传递。
- 为了在 `顶点着色器` 中管理 `顶点数据` , 我们使用 `location` 这一元数据指定输入变量，这样我们才可以在 CPU 上配置顶点属性。
  - 你也可以忽略 `layout (location = 0)` 标识符，通过在 OpenGL 代码中使用 `glGetAttribLocation` 查询属性位置值 (Location)，但是我更喜欢在着色器中设置它们，这样会更容易理解而且节省你（和 OpenGL）的工作量。
- `片段着色器` : 它需要一个 `vec4` 颜色输出变量，因为片段着色器需要生成一个最终输出的颜色。
  - 如果你在片段着色器没有定义输出颜色，OpenGL 会把你的物体渲染为黑色（或白色）。
- 如果打算从一个着色器向另一个着色器发送数据，我们必须在发送方着色器中声明一个输出，在接收方着色器中声明一个类似的输入。当类型和名字都一样的时候，OpenGL 就会把两个变量 `链接` 到一起，它们之间就能发送数据了（这是在链接程序对象时完成的）。
  - GLSL 定义了 `in` 和 `out` 关键字专门来实现这个目的。每个着色器使用这两个关键字设定输入和输出，只要一个输出变量与下一个着色器阶段的输入匹配，它就会传递下去。
  - 但在顶点和片段着色器中会有点不同。
    - 顶点着色器应该接收的是一种特殊形式的输入，否则就会效率低下。顶点着色器的输入特殊在，它从顶点数据中直接接收输入。
    - 片段着色器则需要一个 vec4 颜色输出变量，因为片段着色器需要生成一个最终输出的颜色。如果你在片段着色器没有定义输出颜色，OpenGL 会把你的物体渲染为黑色（或白色）。
- `uniform` 是一种从 CPU 中的应用向 GPU 中的着色器发送数据的方式，
  - 但 `uniform` 和顶点属性有些不同。首先，uniform 是全局的 (Global)。全局意味着 uniform 变量必须在每个着色器程序对象中都是**独一无二**的，而且它**可以被着色器程序的任意着色器在任意阶段访问**。
  - 第二，无论你把 uniform 值设置成什么，*uniform 会一直保存它们的数据，直到它们被重置或更新*。
  - *注意 :* 如果你声明了一个 uniform 却在 GLSL 代码中没用过，编译器会静默移除这个变量，导致最后编译出的版本中并不会包含它，这可能导致几个非常麻烦的错误，记住这点！
  - 使用:
    - 首先需要找到 `着色器中` uniform 属性的 `索引/位置值` 。
    - 当我们得到 uniform 的索引/位置值后，我们就可以更新它的值了。
    - *注意 :* 查询 uniform 地址不要求你之前使用过着色器程序，但是更新一个 uniform 之前你必须先使用程序（调用 `glUseProgram` )，因为它是在当前激活的着色器程序中设置 uniform 的。
  - [统一变量缓冲对象](https://zhuanlan.zhihu.com/p/33093968)
    - > 统一变量缓冲对象 -- Uniform buffer object
- `vecn.xyzw`
- 向量重组: `vec4 differentVec = someVec.wxyx;`

## <font color=#009A000> [shader 加密混淆处理](./shader%E5%8A%A0%E5%AF%86%E5%A4%84%E7%90%86.md) </font>
