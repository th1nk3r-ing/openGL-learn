/*
 学习 openGL 时刻记住的几大法则 :
  1. GPU 的数据存放在显存, 需要显示的发送才能使用;
  2. GPU 程序就是 shader 代码;
  3. openGL 是一个状态机;
  4. 如果你设计师, 你需要设计什么杨的概念来怎样实现 openGL ?
  5. 根据现有的 openGL 概念, 抽象出 openGL 的主要数据结构;
  6. 程序是由数据结构和算法(控制组成的), 数据结构在第四点抽象出来了, 那么是不是控制流上也由限制呢?
*/

typedf enum buffer_binding_target
{
	GL_ARRAY_BUFFER,  /* Vertex attributes, 即 VBO */
	GL_ATOMIC_COUNTER_BUFFER,
	GL_COPY_READ_BUFFER,
	GL_COPY_WRITE_BUFFER,
	GL_DISPATCH_INDIRECT_BUFFER,
	GL_DRAW_INDIRECT_BUFFER,
	GL_ELEMENT_ARRAY_BUFFER,	/* Vertex array indices, 即 EBO, IBO */
	GL_PIXEL_PACK_BUFFER,
	GL_PIXEL_UNPACK_BUFFER,
	GL_QUERY_BUFFER,
	GL_SHADER_STORAGE_BUFFER,
	GL_TEXTURE_BUFFER,
	GL_TRANSFORM_FEEDBACK_BUFFER,
	GL_UNIFORM_BUFFER,
}BufBindTarget;

/* 缓冲对象 : GPU 端的一块内存 */
struct BO {
	int8_t data[n];	/* 数据 */
};

/* 顶点缓冲对象：Vertex Buffer Object */
struct VBO {
	struct BO 			*pstBo;
	enum BufBindTarget  eTarget = GL_ARRAY_BUFFER;
};

/* 顶点索引缓冲对象：Element Buffer Object; 
	Vertex array indices 顶点数组索引
	使用 EBO 时的绘制函数为 : glDrawElements()	*/
typedef struct EBO {
	struct BO 			*pstBo;
	enum BufBindTarget  eTarget = GL_ELEMENT_ARRAY_BUFFER;
}IBO; /* 或 Index Buffer Object，IBO */

/* glVertexAttribPointer() 函数的抽象结构体,
	*/ 
struct VAP {
	uint32_t u32GLSL_vertexLocation;  /* 要传递给 GLSL 中顶点变量的位置 */
	int32_t  s32VertexSize;	   /* GLSL 中顶点变量的大小 : vect3 ? vect4 ?  3 个还是 4 个*/
	enum     eVertexType;      /* GLSL 中顶点变量的类型 : int, float, uint, double */
	bool     bBeNormalize;     /* 是否将数据映射至 0~1 */
	uint32_t u32BoDataStride;  /* BO 中数据的 stride, 紧密排列为 0 */	
	
	const void * pointer;	       /* 指向 GPU 端存放的数据, 通常是 VBO 
									pointer == NULL 则使用当前状态机中已绑定至 GL_ARRAY_BUFFER 中的数据; 
									pointer != NULL 则使用 pointer 指定的 CPU 内存中的数据 */
};

/* 顶点数组对象：Vertex Array Object */
struct VAO {
	uint32_t   u32VaildCnt;		/* 启用的个数, 待核对 */
	uint32_t   pu32GLSLVertexLocation[16];	/* GLSL 中顶点的 layOut / 位置 */
	struct VAP pstVAP[16];  	/* 一系列的 VAP ; 例如 :
									pstVAP[0] 图元的顶点属性
									pstVAP[1] 通过顶点着色器传递的颜色值
									pstVAP[2] 与图元顶点对应的纹理顶点属性 */
	struct VBO pstVBO[16];		/* VAP 对应的 VBO */
	
	struct EBO *pstEBO;			/* 指向 EBO/IBO */
};






