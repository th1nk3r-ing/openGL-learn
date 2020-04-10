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

ifeq ($(MAKECMDGOALS), 1.helloTriangle)
	C_SrcFiles += ./example/hello_Triangle/hello_Triangle.c
else ifeq ($(MAKECMDGOALS), 2.triangleIndxed)
	C_SrcFiles += ./example/triangle_indxed/triangle_indxed.c
else ifeq ($(MAKECMDGOALS), 3.triangle2)
	C_SrcFiles += ./example/triangle2/triangle2.c
else ifeq ($(MAKECMDGOALS), 4.shader_glsl)
	C_SrcFiles += ./example/shader_glsl/shader_glsl.c
else ifeq ($(MAKECMDGOALS), 5.shader_glsl2)
	C_SrcFiles += ./example/shader_glsl2/shader_glsl2.c
else ifeq ($(MAKECMDGOALS), 6.textureCombined)
	C_SrcFiles += ./example/textureCombined/textureCombined.c
else ifeq ($(MAKECMDGOALS), 7.texture2)
	C_SrcFiles += ./example/texture2/texture2.c	
else ifeq ($(MAKECMDGOALS), 8.textureMode)
	C_SrcFiles += ./example/textureMode/textureMode.c		
else ifeq ($(MAKECMDGOALS), 9.transFormations)
	C_SrcFiles += ./example/transFormations/transFormations.c	
else ifeq ($(MAKECMDGOALS), 10.coordinateSystem)
	C_SrcFiles += ./example/coordinateSystem/coordinateSystem.c	
else ifeq ($(MAKECMDGOALS), 11.camera)
	C_SrcFiles += ./example/camera/camera.c		
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
	$(CXX) $(C_Flags) -o $(Target) $(Objs) $(LibPath) $(StaticLibs) $(ShardLibs) 
	@echo -e $(prt_Green) "\n****** make OK! *****\n" $(prt_End)

# 编译源文件
$(C_Objs) : %.o : %.c
	$(CC) $(C_Flags) -c $< -o $@
$(Cxx_Objs) : %.o : %.cpp
	$(CXX) $(C_Flags) -c $< -o $@

1.helloTriangle: $(Target)
2.triangleIndxed: $(Target)
3.triangle2: $(Target)
4.shader_glsl: $(Target)
5.shader_glsl2: $(Target)
6.textureCombined: $(Target)
7.texture2: $(Target)
8.textureMode: $(Target)
9.transFormations: $(Target)
10.coordinateSystem: $(Target)
11.camera: $(Target)

.PHONY : clean 
	
clean :
	-rm -rf $(Target) $(Objs) *.o $(Target).exe
	-find ./ -name "*.o" | xargs rm -rf
