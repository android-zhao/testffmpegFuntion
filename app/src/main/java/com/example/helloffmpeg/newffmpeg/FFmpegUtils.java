package com.example.helloffmpeg.newffmpeg;

import java.io.File;

class FFmpegUtils {
    static {
        System.loadLibrary("native-lib");
    }

    // 连接jni 确保jni函数可用 打印ffmpeg 版本
    public native static void helloFFmpeg_new();

    public native static void printFileInfo_new(String filePath);
    // 解码视频
    public native int decodeVideo(String filePath, String dstFilePath);

    //解码视频成yuv
    public static  native void decodeVideo2yuv(String filePath, String dstFilePath);
}
