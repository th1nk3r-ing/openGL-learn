# <font color=#0099ff> **openGL ES** </font>
> `@think3r`  2018-05-10 22:4:56

官方 github 地址:  [https://github.com/KhronosGroup](https://github.com/KhronosGroup)

## <font color=#009A000> 0x00 EGL </font>

- [官网](https://www.khronos.org/registry/EGL/)

- 当前版本的EGL是EGL 1.5, 更新 于 2014年8月27日. 下面是 EGL 1.5 的头文件:
    - <a href="https://www.khronos.org/registry/EGL/api/EGL/egl.h" target="_blank">egl.h</a>
    - <a href="https://www.khronos.org/registry/EGL/api/EGL/eglext.h" target="_blank">eglext.h</a>
    - <a href="https://www.khronos.org/registry/EGL/api/EGL/eglplatform.h" target="_blank">eglplatform.h</a>
    - <a href="https://www.khronos.org/registry/EGL/api/KHR/khrplatform.h" target="_blank">khrplatform.h"</a>
        - <KHR/khrplatform.h> (required by the current EGL and OpenGL ES headers).
- [eglspec.1.5.withchanges.pdf](https://www.khronos.org/registry/EGL/specs/eglspec.1.5.withchanges.pdf)

- [man-Page](https://www.khronos.org/registry/EGL/sdk/docs/man/)
- [OpenGL-Refpages---> github](https://github.com/KhronosGroup/OpenGL-Refpages)
    - 可用浏览器打开 make 之后的 xml 文件, 作为 man Page 查看;
    - make 之前的命令:
        ```sh
        sudo apt-get install xsltproc
        sudo apt-get install docbook-xsl
        sudo apt-get install docbook-xsl-ns
        sudo apt-get install w3-dtd-mathml
        sudo apt-get install docbook-mathml
        ```
    - 也可以直接下载 make 后打包好的文件;
- 遇到的一个问题: [texture vs texture2D in GLSL](https://stackoverflow.com/questions/12307278/texture-vs-texture2d-in-glsl)
