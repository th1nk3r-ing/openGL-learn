/*
 学习 openGL 时刻记住的几大法则 :
  1. GPU 的数据存放在显存, 需要显示的发送才能使用;
  2. GPU 程序就是 shader 代码;
  3. openGL 是一个状态机;
  4. 如果你设计师, 你需要设计什么杨的概念来怎样实现 openGL ?
  5. 根据现有的 openGL 概念, 抽象出 openGL 的主要数据结构;
  6. 程序是由数据结构和算法(控制组成的), 数据结构在第四点抽象出来了, 那么是不是控制流上也由限制呢?
*/

/* 缓冲对象 : GPU 端的一块内存 */
struct BO {
	int8_t data[n];
};

/* 顶点缓冲对象：Vertex Buffer Object */
struct VBO {
	struct BO Vertex_stBo;
	enum   GL_ARRAY_BUFFER;
};

/* 索引缓冲对象：Element Buffer Object */
typedef struct EBO {
	struct BO Index_stBo;
	enum   GL_ELEMENT_ARRAY_BUFFER;
}IBO; /* 或 Index Buffer Object，IBO */

/* glVertexAttribPointer() 函数的抽象结构体 */
struct VAP {
	uint32_t u32GLSL_vertexLocation;  /* 要传递给 GLSL 中顶点变量的位置 */
	int32_t  s32VertexSize;	   /* GLSL 中顶点变量的大小 : vect3 ? vect4 ?  3 个还是 4 个*/
	enum     eVertexType;      /* GLSL 中顶点变量的类型 : int, float, uint, double */
	bool     bBeNormalize;     /* 是否将数据映射至 0~1 */
	uint32_t u32BoDataStride;  /* BO 中数据的 stride, 紧密排列为 0 */
	const void * pBo;	       /* 指向 GPU 端存放的数据 
									pBO == NULL 则使用状态机中已绑定 VBO 中的数据; 
									pBO != NULL 则使用 pBo 指定的 CPU 内存中的数据 */
};

/* 顶点数组对象：Vertex Array Object */
struct VAO {
	struct VAP *pstVAp;  /* 指向一系列的 VAP  此处可能有误 !!!!!!!!!!!!!!!!!!!!!!!! */
};