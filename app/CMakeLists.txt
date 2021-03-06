cmake_minimum_required(VERSION 3.10.2)

# 从系统路径中找到log和android这两个库
find_library(log-lib log)
find_library(android-lib android)

# 编译native-lib动态库
add_library(native-lib
        SHARED
        src/main/cpp/native-lib.cpp
        src/main/cpp/native-lib2.cpp
        src/main/cpp/native-lib3.cpp
        src/main/cpp/native-lib4.cpp
        src/main/cpp/native-lib5.cpp
        src/main/cpp/native_new_lib.cpp
        )

# 创建变量JNI_LIBS_DIR
set(JNI_LIBS_DIR ${CMAKE_SOURCE_DIR}/src/main/jniLibs)

# 导入avutil库
# set_target_properties指定了从哪里导入
add_library(avutil
        SHARED
        IMPORTED)
set_target_properties(avutil
        PROPERTIES IMPORTED_LOCATION
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libavutil.so)

add_library(swresample
        SHARED
        IMPORTED)
set_target_properties(swresample
        PROPERTIES IMPORTED_LOCATION
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libswresample.so)

add_library(swscale
        SHARED
        IMPORTED)
set_target_properties(swscale
        PROPERTIES IMPORTED_LOCATION
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libswscale.so)

add_library(avcodec
        SHARED
        IMPORTED)
set_target_properties(avcodec
        PROPERTIES IMPORTED_LOCATION
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libavcodec.so)

add_library(avformat
        SHARED
        IMPORTED)
set_target_properties(avformat
        PROPERTIES IMPORTED_LOCATION
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libavformat.so)

add_library(avfilter
        SHARED
        IMPORTED)
set_target_properties(avfilter
        PROPERTIES IMPORTED_LOCATION
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libavfilter.so)

add_library(avdevice
        SHARED
        IMPORTED)
set_target_properties(avdevice
        PROPERTIES IMPORTED_LOCATION
        ${JNI_LIBS_DIR}/${ANDROID_ABI}/libavdevice.so)

# 指定头文件的位置
include_directories(${JNI_LIBS_DIR}/includes)

# 将avutil等库与native-lib库连接起来，这样我们就可以在native-lib库的代码中使用avutil等库了
target_link_libraries(native-lib
        avutil swresample swscale avcodec avformat avfilter avdevice
        ${log-lib} ${android-lib})