prt_Green="\e[1;32m"
prt_Yellow="\e[1;33m"
prt_Red="\e[1;31m"
prt_End="\e[0m"

# 编译器 
#ToolChains_PreFix=/mnt/c/MinGw64/bin/
CC=$(ToolChains_PreFix)gcc.exe
CXX=$(ToolChains_PreFix)g++.exe

# 启动多核编译
MAKEFLAGS = -j

Target = testDemo

Include_Dir :=  -I./common/inc/ \
				-I../file/Mali_OpenGL_ES_Emulator-v3.0.4-2-g8d905-Windows-64bit/include/ \
				-I./third/inc/ 

# 编译选项
C_Flags += -fdiagnostics-color 
C_Flags += -g -Wall -Wunused -Wconversion -Wno-uninitialized -Wno-sign-conversion
C_Flags += $(Include_Dir)

# 源文件
C_SrcFiles = ./common/src/build_time.c \
			 ./common/src/win32_platform.c \
			 ./common/src/OGL_common.c \
			 ./common/src/my_ResourceLoader.c
		
Cxx_SrcFiles = ./common/src/my_TransFormations.cpp

# 利用模式变量和 shell 命令, 去除 -Wconversion, 以关闭 stb_image.h 中的警告;
./common/src/my_ResourceLoader.o : C_Flags:=$(shell echo $(C_Flags)|sed -s 's/-Wconversion//g')

ifeq ($(MAKECMDGOALS), helloTriangle)
	C_SrcFiles += ./example/hello_Triangle/hello_Triangle.c
else ifeq ($(MAKECMDGOALS), triangleIndxed)
	C_SrcFiles += ./example/triangle_indxed/triangle_indxed.c
else ifeq ($(MAKECMDGOALS), triangle2)
	C_SrcFiles += ./example/triangle2/triangle2.c
else ifeq ($(MAKECMDGOALS), shader_glsl)
	C_SrcFiles += ./example/shader_glsl/shader_glsl.c
else ifeq ($(MAKECMDGOALS), shader_glsl2)
	C_SrcFiles += ./example/shader_glsl2/shader_glsl2.c
else ifeq ($(MAKECMDGOALS), textureCombined)
	C_SrcFiles += ./example/textureCombined/textureCombined.c
else ifeq ($(MAKECMDGOALS), texture2)
	C_SrcFiles += ./example/texture2/texture2.c	
else ifeq ($(MAKECMDGOALS), textureMode)
	C_SrcFiles += ./example/textureMode/textureMode.c		
else ifeq ($(MAKECMDGOALS), transFormations)
	C_SrcFiles += ./example/transFormations/transFormations.c	
else ifeq ($(MAKECMDGOALS), coordinateSystem)
	C_SrcFiles += ./example/coordinateSystem/coordinateSystem.c	
endif


#Object File
C_Objs := $(C_SrcFiles:.c=.o)
Cxx_Objs := $(Cxx_SrcFiles:%.cpp=%.o)

Objs = $(C_Objs) $(Cxx_Objs)

#Lib
StaticLibs := -L../file/Mali_OpenGL_ES_Emulator-v3.0.4-2-g8d905-Windows-64bit/
			 
#LibPath := ../SDL/lib
ShardLibs := -lm \
			 -lpthread \
			 -lEGL \
			 -lGLESv2 \
			 -lgdi32

# 链接输出
$(Target) : $(Objs)
	$(CC) $(C_Flags) -o $(Target) $(Objs) $(LibPath) $(StaticLibs) $(ShardLibs) 
	@echo -e $(prt_Green) "\n****** make OK! *****\n" $(prt_End)

# 编译源文件
$(C_Objs) : %.o : %.c
	$(CC) $(C_Flags) -c $< -o $@
$(Cxx_Objs) : %.o : %.cpp
	$(CC) $(C_Flags) -c $< -o $@

helloTriangle: $(Target)
triangleIndxed: $(Target)
triangle2: $(Target)
shader_glsl: $(Target)
shader_glsl2: $(Target)
textureCombined: $(Target)
texture2: $(Target)
textureMode: $(Target)
transFormations: $(Target)
coordinateSystem: $(Target)

.PHONY : clean 
	
clean :
	-rm -rf $(Target) $(Objs) *.o $(Target).exe
	-find ./ -name "*.o" | xargs rm -rf
