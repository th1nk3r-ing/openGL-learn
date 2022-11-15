# <font color=#0099ff> **shader 的加密混淆处理** </font>

> `@think3r` 2022-11-15 21:47:18  

## <font color=#009A000> 0x00 GLSL 源码编译问题 </font>

### <font color=#FF4500> GNU 解决方案 </font>

> 参考链接 :
> 1. [objcopy命令介绍](https://blog.csdn.net/weixin_34236497/article/details/91508488)
> 2. [GNU Objcopy语法和使用](https://blog.csdn.net/linux12121/article/details/82932535)
> 3. [如何利用objcopy选项来填充生成的bin文件](https://my.oschina.net/hechunc/blog/3020177)
> 4. [GNU 工具 nm](http://blog.sina.com.cn/s/blog_5ec5eb330101d5k5.html)
> 5. [objcopy-gnu-doc](https://sourceware.org/binutils/docs/binutils/objcopy.html)

- 由于 GLSL 代码是在运行时编译的, 而输入编译器的是一个 GLSL 字符串源码 那么通常就有如下两种方式来完成:
  1. 将 GLSL 源码放置到单独的文件中, 使用时 `fread()` 读入内存中再编译,  
      - 优点: GLSL 源码可以进行高亮的操作, 便于编程;
      - 缺点: GLSL 源码会随着程序一块打包发布, 容易泄露程序逻辑;
  2. 将 GLSL 直接以字符串的方式放置到源码中, 可直接进行编译;
      - 优点: GLSL 源码不进行单独发布, 泄露风险稍小;
      - 缺点: GLSL 源码因为是字符串, 开发可能不大方便(高亮啥的没有);

- 于是我就想能不能将上述两个方法的优点都结合起来? 后来发现, GNU 的 `objcopy` 工具就能很好的解决这点;
  - 下方 `makefile` 中的三个选项可生成平台对应的 `.o` 文件;
    - `-I` 表示输入文件的格式;
    - `-O` 表示输出文件的格式, 这个执行 `objcopy` 查看其支持的格式;
    - `-B` --binary-architecture `<arch>`
  - `--rename-section .data=.rodata,alloc,load,readonly,data,contents` 将 GLSL 代码配置为只读;
    - 当加入上述只读选项时, 建议配合 `const` 修饰来保证安全, 否则 GLSL 代码被放置到了只读段, 而使用的指针地址则直接指向了只读段, 修改只读段中的数据会造成程序崩溃. 加入 const 后, 编译器可额外检查代码中静态的修改错误;
    - linux 中，gnu 工具 `nm` 用来列出目标文件的符号清单, 
      - `A` 该符号的值是绝对的，在以后的链接过程中，不允许进行改变。这样的符号值，常常出现在中断向量表中，例如用符号来表示各个中断向量函数在中断向量表中的位置;
      - `D` 该符号位于初始话数据段中。一般来说，分配到 data section 中。
      - `R` 该符号位于只读数据区。~~例如定义全局 `const int test[] = {123, 123}` ;则 `test` 就是一个只读数据区的符号。(存疑)~~
    - 当未加入上述配置选项时, nm 如下:

    ```log
    000000000000015f D _binary_src_vertShader_txt_end
    000000000000015f A _binary_src_vertShader_txt_size
    0000000000000000 D _binary_src_vertShader_txt_start
    ```

    - 当加入时, nm 如下:

    ```log
    000000000000015f R _binary_src_vertShader_txt_end
    000000000000015f A _binary_src_vertShader_txt_size
    0000000000000000 R _binary_src_vertShader_txt_start
    ```

  - `--pad-to` 和 `--gap-fill` 则分别表示对齐和对齐的填充
    - **GLSL 代码因为是字符串, 所以将对齐放大到了 `文件大小+1`, 且将最后一个字符填充 0, 来拟合字符串.** 否则会出现 fragShader 和 vertShader 混合到一个字符串的情况, 切记!
  - 注意: arm 平台下的 size 可能不能使用(引起崩溃), 但可用 `start - end` 来替代;

- `makefile` 局部:

  ```Makefile
  OBJCOPY=objcopy

  GLSL_Files = ./src/fragShader.txt  ./src/vertShader.txt
  GLSL_Objs := $(GLSL_Files:.txt=.o)

  GLSL_Flags := -I binary -O elf64-littleaarch64 -B aarch64
  GLSL_Flags += --rename-section .data=.rodata,alloc,load,readonly,data,contents

  # expr \( `stat --format=%s $<` + 4 \) / 4 \* 4 是为了计算文件大小, 并向上 4 对齐;

  $(GLSL_Objs) : %o : %txt
    #echo size is $(shell expr `stat --format=%s $<`)
    $(OBJCOPY) $(GLSL_Flags) --pad-to=$(shell expr \( `stat --format=%s $<` + 4 \) / 4 \* 4 ) --gap-fill=0x00 $< $@

  $(Target) : $(Objs) $(GLSL_Objs)
    $(CC) $(C_Flags) -o $(Target) $(Objs) $(GLSL_Objs) $(LIBS_WithPath)
  ```

- 程序中的使用:

  ```c
  extern const char const _binary_src_vertShader_txt_start[];
  extern const char const _binary_src_vertShader_txt_end[];
  extern const char const _binary_src_fragShader_txt_start[];
  extern const char const _binary_src_fragShader_txt_end[];

  uint32_t size = _binary_src_fragShader_txt_end -_binary_src_vertShader_txt_start;
  for(uint32_t i = 0; i < size; i++)
  {
      printf("[%d] ", _binary_src_fragShader_txt_start[i]);
  }
  printf("\n");
  ```

- TODO : 加上自定义的混淆和解密工具就 OK 了

## <font color=#009A000> 0x01 一种基于 C++ 的 shader 加密方案 </font>

### <font color=#FF4500> 参考 </font>

- [Shader文件的保护方法及装置](https://xueshu.baidu.com/usercenter/paper/show?paperid=1d700tm0w1250ay06a7g0jc09c470334)
- [c/c++字符串混淆方案总结](https://blog.csdn.net/wangbaochu/article/details/45370397)
- [Encrypting / obfuscating a string literal at compile-time](https://stackoverflow.com/questions/6934217/encrypting-obfuscating-a-string-literal-at-compile-time)
  - ADVobfuscator
  - Obfuscate
- shader 的 format-debug : `clang-format -style=file yuvRender.glsl > shader.c`

### <font color=#FF4500> 简介 </font>

- 查看 `shader` 的命令 :
  - `grep -rs -E "vec3|vec4" -A 15 -B 30 ./classes*  --text > out.txt`
  - `find . -name "*.so" | xargs grep --text -rs -E "vec3|vec4" -A 15 -B 30`

### <font color=#FF4500> ADVobfuscator </font>

- 其特点是在 栈空间上进行的操作, 因此会有栈溢出风险, 但兼容 `C++11` & `C++14`;
- > `ADVobfuscator/Indexes.h:32:45: fatal error: recursive template instantiation exceeded maximum depth of 1024`
  - 解决方法 : `-ftemplate-depth=35536`
- 但脚本可编译通过, Android Studio 编译失败;

    ```log
    clang++: error: unable to execute command: Illegal instruction: 4
    clang++: error: clang frontend command failed due to signal (use -v to see invocation)
    Android (6317467 based on r365631c1) clang version 9.0.8 (https://android.googlesource.com/toolchain/llvm-project e0caee08e5f09b374a27a676d04978c81fcb1928) (based on LLVM 9.0.8svn)
    Target: aarch64-none-linux-android29
    Thread model: posix
    InstalledDir: /Users/think3r/Library/Android/sdk/ndk/21.1.6352462/toolchains/llvm/prebuilt/darwin-x86_64/bin
    clang++: note: diagnostic msg: PLEASE submit a bug report to https://github.com/android-ndk/ndk/issues and include the crash backtrace, preprocessed source, and associated run script.
    clang++: note: diagnostic msg:
    ```

### <font color=#FF4500> Obfuscate </font>

- 需使用 C++14
- 不会有堆栈溢出的问题;
- 跟编译器优化无关?? `-O0` 也看不到相关字符串;
