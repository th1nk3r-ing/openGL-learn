# <font color=#0099ff> **iOS openGL** </font>

> `@think3r` 2023-01-17 10:25:54
> 1. [OPenGL ES 摘要 - iOS开发](https://www.jianshu.com/p/190e28fd173b)
> 2. [layer 的一些知识](https://www.cnblogs.com/wlsxmhz/p/5520048.html)
> 3. [【iOS】菜鸟进阶:浅谈Layer两个属性position和anchorPoint](https://www.jianshu.com/p/7703e6fc6191)

## <font color=#009A000> 0x00 UI 基础 </font>

- `view` 和 `layer` :
  - View 负责处理事务(可以响应事件, 如: 触摸事件)，layer 用于显示.
  - 一个 view 有一个 underlying layer, 它是这个 view 所有的 draw 的实现者, 可通过 view 的 layer property 获得, 但是它没有相应的 view property, 相反的, view 是这个 layer 的 delegate.
  - 如果想改变 view 的 underlying layer, 需实现 view 的 `+(class) layerClass{}` 函数,返回自定义的 layer 类.
    - 往往这个类不负责具体的画图,只是负责组织多个子 layer 的显示.
  - `layerClass` :
    - 每一个 UIView 都是寄宿在一个 CALayer 的示例上。
    - 这个图层是由视图自动创建和管理的，那我们可以用别的图层类型替代它么？一旦被创建，我们就无法代替这个图层了。
    - 但是如果我们继承了 UIView，那我们就可以重写 `+layerClass` 方法使得在创建的时候能返回一个不同的图层子类。UIView 会在初始化的时候调用 `+layerClass` 方法，然后用它的返回类型来创建宿主图层;
- `- (void) viewDidLoad{}` 和 `loadView` 的区别
  - 上述的 3 个方法，都是 `UIViewController` 的方法，跟 `UIViewController` 的 `view` 属性的生命周期息息相关。
  - `loadView` :
    - 用来负责创建 UIViewController 的 view,
    - 每次访问 UIViewController 的 view(比如controller.view、self.view)而且 view 为 nil，loadView方法就会被调用。
  - `viewDidLoad` :
    - view 创建完毕后，最终都会调用 viewDidLoad 方法
- layer 的位置, 控制 layer 的位置使用两个 property :
  - position 一个 superlayer 坐标系下的坐标
    - 用来设置 CALayer 在父层中的位置
    - 以父层的左上角为原点(0, 0)
  - anchorpoint 一个本身坐标系下的坐标;
    - 称为“定位点”、“锚点”
    - 决定着CALayer身上的哪个点会在position属性所指的位置
    - 以自己的左上角为原点(0, 0)
    - 它的x、y取值范围都是0~1，默认值为（0.5, 0.5）
  - 如同属性名称一样,就像用图钉在墙上(父视图)订一个图片, position 表示的是图钉在墙上(父视图)坐标, anchorpoint 表示是图钉在图片上坐标.

## <font color=#009A000> 0x01 iOS OpenGL-ES 摘要 </font>

- 在 iOS 中使用 EAGL 提供的 EAGLContext 类来实现和提供一个呈现环境，用来保持 OpenGLES 使用到的硬件状态。
- 在 iOS 中，可以使用 GLKView 将 OpenGL ES 绘制的内容渲染到屏幕上，还可以使用 CAEAGLLayer 图层将动画与视图相结合。
  - EAGL 是一个 Objective-C API，提供使 OpenGL ES 与 Core Animation 和 UIKIT 集成的接口。
- iOS  平台有一个比较特殊的地方就是如果 APP 进入后台之后, 就不能再进行 OpenGL-ES 的渲染操作, 否则可能会 crash,
- 在 iOS 平台上不允许开发者使用 OpenGL-Es 直接渲染屏幕, 必须使用 FrameBuffer 和 RenderBuffer 来进行渲染.
  - OpenGL 渲染到 RenderBuffer
  - RenderBuffer 则需要绑定到一个 `CAEAGLLayer` 上去.
  - 最后调用 EAGLContext 的 `presentRenderBuffer` 方法将渲染结果上屏;
    - 类似于 `swapBuffer` 过程, EAGL 将 RenderBuffer 的数据更新到物理屏幕(View 的 layer)
- OpenGL ES 中需要先创建一个 `EAGLContext` 的渲染上下文对象。而且每个线程都只能对应一个上下文 ????
  - 在同一个线程中切换不同的上下文时，需要对上下文进行强引用以防止其被释放，并且在切换之前应屌用 `glFlush()` 函数将当前的上下文提交的指令传到徒刑硬件中去。

## <font color=#009A000> 0x02 iOS OpenGL-Es layer 创建 </font>

### <font color=#FF4500> 1. `CAEAGLLayer` </font>

1. 显示类继承自 `UIView` , 重写父类 `UIView` 的 `layerClass` 方法, 并返回 `CAEAGLLayer` 类型.
2. 然后在该 View 的 initWithFrame 方法中, 获得 layer 并强制类型转换为 CAEAGLLayer.

    ```objective-c
    @interface MGSDLGLView : UIView
    @end

    @implementation MGSDLGLView
    + (Class) layerClass {
        return [CAEAGLLayer class];
    }

    CAEAGLLayer *eaglLayer = (CAEAGLLayer *) [self layer];
    @end
    ```
### <font color=#FF4500> 2. `GLKView` </font>

- GLKView 类为渲染提供了一个显示视图，
  - 可以手动创建 : `- (instancetype)initWithFrame:(CGRect)frame context:(EAGLContext *)context;`
  - 也可以将某个 UIView 强制转换成 GLKView

    ```objective-c
    GLKView *view = （GLKView *）self.view;
    view.context =[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    view.srawableColorFormat = GLKViewDrawableColorFormatRGBA8888;
    ```

  - 当创建了 GLKView 之后，就可以通过重写 drawRect：方法来进行绘制 : `- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect;`

## <font color=#009A000> 0x01 </font>

- OpenGL ES中需要先创建一个EAGLContext的渲染上下文对象。而且每个线程都只能对应一个上下文，在同一个线程中切换不同的上下文时，需要对上下文进行强引用以防止其被释放，并且在切换之前应屌用glFlush()函数将当前的上下文提交的指令传到徒刑硬件中去。有两个重要的函数设置和获取当前的上下文：
