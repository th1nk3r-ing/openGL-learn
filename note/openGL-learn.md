# <font color=#0099ff> **openGL 学习记录** </font> 

> `@think3r` 2018-05-09 22:45:18  
> 参考: 
> 1. [OPENGL ES 3.0编程指南  原书第2版 .pdf](http://www.jb51.net/books/488955.html#download)
> 2. [OpenGL, OpenGL ES, WebGL, GLSL, GLSL ES APIs 列表](http://web.eecs.umich.edu/~sugih/courses/eecs487/common/notes/APITables-zhs.xml)

# <font color=#0099ff> **0x00 简介** </font> 

- openGL ES 当前已发布了 ~~4 个~~ 以上的版本 <u>(这个不是很准确了. 需要就诊改一下)</u> 
    - 采用固定管线的 ES 1.0, ES 1.1, 分别衍生于 openGL 1.3 和 openGL 1.5
    - ES 2.0, 采用可编程图形管线, 从 openGL 2.0 规范衍生而来;
    - ES 3.0; 从 openGL 3.3 规范衍生而来, 
- openES 3.0 实现了具有可编程着色功能的图形管线, 其由两个规范组成:
    - openGL ES 3.0 API 规范
    - openGL ES SL 3.0 着色器语言规范;
