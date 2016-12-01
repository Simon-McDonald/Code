################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Camera.cpp \
../src/CollidableObject.cpp \
../src/GeometryShader.cpp \
../src/LoadShaders.cpp \
../src/ObjLoader.cpp \
../src/OpenGLController.cpp \
../src/Player.cpp \
../src/RenderableObject.cpp \
../src/Renderer.cpp \
../src/Sphere.cpp \
../src/StopWatchOld.cpp \
../src/UserInputController.cpp \
../src/WindowManager.cpp \
../src/WorldObject.cpp \
../src/WorldPosition.cpp \
../src/controls.cpp \
../src/main.cpp \
../src/temp.cpp 

OBJS += \
./src/Camera.o \
./src/CollidableObject.o \
./src/GeometryShader.o \
./src/LoadShaders.o \
./src/ObjLoader.o \
./src/OpenGLController.o \
./src/Player.o \
./src/RenderableObject.o \
./src/Renderer.o \
./src/Sphere.o \
./src/StopWatchOld.o \
./src/UserInputController.o \
./src/WindowManager.o \
./src/WorldObject.o \
./src/WorldPosition.o \
./src/controls.o \
./src/main.o \
./src/temp.o 

CPP_DEPS += \
./src/Camera.d \
./src/CollidableObject.d \
./src/GeometryShader.d \
./src/LoadShaders.d \
./src/ObjLoader.d \
./src/OpenGLController.d \
./src/Player.d \
./src/RenderableObject.d \
./src/Renderer.d \
./src/Sphere.d \
./src/StopWatchOld.d \
./src/UserInputController.d \
./src/WindowManager.d \
./src/WorldObject.d \
./src/WorldPosition.d \
./src/controls.d \
./src/main.d \
./src/temp.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I"C:\MinGW\external\glew-1.13.0\include" -I"D:\Simon\Dropbox\Code\C++\SimonCppLibrary\include" -I"C:\MinGW\external\SDL2-2.0.3\include\SDL2" -I"C:\MinGW\external\SOIL\src" -I"C:\MinGW\external\glm" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


