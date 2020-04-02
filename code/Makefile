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

Include_Dir :=  -I./inc/ \
				-I../file/Mali_OpenGL_ES_Emulator-v3.0.4-2-g8d905-Windows-64bit/include/

# 编译选项
C_Flags += -fdiagnostics-color 
C_Flags += -g -Wall -Wno-unused -Wconversion -Wno-uninitialized -Wno-sign-conversion
C_Flags += $(Include_Dir)
C_Flags += 

# 源文件
C_SrcFiles = ./src/build_time.c \
			 ./src/win32_platform.c \
			 ./src/OGL_common.c 

ifeq ($(MAKECMDGOALS), helloTriangle)
	C_SrcFiles += ./hello_Triangle/hello_Triangle.c
else ifeq ($(MAKECMDGOALS), triangle2)
	C_SrcFiles += ./triangle2/triangle2.c
endif


#Cxx_SrcFiles = 
#C_SrcFiles += $(patsubst %.c,%.o,$(wildcard ./*.c))

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
	$(CXX) $(C_Flags) -c $< -o $@

helloTriangle: $(Target) 
triangle2: $(Target)

.PHONY : clean 
	
clean :
	-rm -rf $(Target) $(Objs) $(Target).exe
