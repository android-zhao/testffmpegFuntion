//
// Created by zhf on 2021/6/6.
//
#include<android/log.h>
#include <jni.h>
#include <string>

extern "C" {
#include <libavutil/log.h>
#include <libavformat/avformat.h>
#include <libavutil/dict.h>
#include <libavcodec/avcodec.h>
#include <libavutil/timestamp.h>
}

const char *TAG_ZHF = "zhf";
#define  INBUF_SIZE_NEW  4096


#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, TAG_ZHF, __VA_ARGS__)

static void decode(AVCodecContext *avCodecContext, AVFrame *avFrame, AVPacket *pkt,
                   const char *filename);

static void yuv_save(AVFrame *avFrame, char *filename);

extern "C"
JNIEXPORT void JNICALL
Java_com_example_helloffmpeg_newffmpeg_FFmpegUtils_helloFFmpeg_1new(JNIEnv *env, jclass clazz) {
    // TODO: implement helloFFmpeg_new()
    __android_log_print(ANDROID_LOG_ERROR, TAG_ZHF, "current FFmpeg version: %s",
                        av_version_info());
    LOGE("test ffmpeg and jni");
}extern "C"
JNIEXPORT jint JNICALL
Java_com_example_helloffmpeg_newffmpeg_FFmpegUtils_decodeVideo(JNIEnv *env, jobject thiz,
                                                               jstring file_path,
                                                               jstring dst_file_path) {
    // TODO: implement decodeVideo()
}extern "C"
JNIEXPORT void JNICALL
Java_com_example_helloffmpeg_newffmpeg_FFmpegUtils_printFileInfo_1new(JNIEnv *env, jclass clazz,
                                                                      jstring file_path) {
    // TODO: implement printFileInfo_new()

    const char *path = env->GetStringUTFChars(file_path, nullptr);
    __android_log_write(ANDROID_LOG_ERROR, TAG_ZHF, path);

    AVFormatContext *fmt_ctx = nullptr;
    AVDictionaryEntry *dictionaryEntry = nullptr;
    int ret;

    //注册
    //此方法已被移除，没有替代方法
    //已经不需要调用此方法
    //av_register_all();

    //创建AVFormatContext
    if ((ret = avformat_open_input(&fmt_ctx, path, nullptr, nullptr)) < 0) {
        __android_log_write(ANDROID_LOG_ERROR, TAG_ZHF, av_err2str(ret));
        goto end;
    }

    //找到流信息
    if ((ret = avformat_find_stream_info(fmt_ctx, nullptr)) < 0) {
        __android_log_write(ANDROID_LOG_ERROR, TAG_ZHF, av_err2str(ret));
        goto end;
    }

    //解析metadata
    while ((dictionaryEntry = av_dict_get(fmt_ctx->metadata, "", dictionaryEntry,
                                          AV_DICT_IGNORE_SUFFIX))) {
        __android_log_print(ANDROID_LOG_ERROR, TAG_ZHF, "%s=%s\n", dictionaryEntry->key,
                            dictionaryEntry->value);
    }

    //释放资源
    end:
    env->ReleaseStringUTFChars(file_path, path);
    if (fmt_ctx) avformat_close_input(&fmt_ctx);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_helloffmpeg_newffmpeg_FFmpegUtils_decodeVideo2yuv(JNIEnv *env, jobject thiz,
                                                                   jstring file_path,
                                                                   jstring dst_file_path) {
    // TODO: implement decodeVideo2yuv()

    const char *src_file = env->GetStringUTFChars(file_path, nullptr);
    const char *des_file = env->GetStringUTFChars(dst_file_path, nullptr);
    LOGE("SRC file:%s", src_file);
    LOGE("DES file :%s", des_file);

    AVCodecParserContext *avCodecParserContext;
    AVCodecContext *avCodecContext;
    AVCodec *avCodec;
    FILE *file = nullptr;
    AVPacket *avPacket;
    AVFrame *avFrame;
    uint8_t input[INBUF_SIZE_NEW + AV_INPUT_BUFFER_PADDING_SIZE];

    uint8_t *data = nullptr;
    size_t data_size;
    int result = 0;
    avPacket = av_packet_alloc();
    if (!avPacket) {
        LOGE("AVPACKET open failed");
        goto end;
    }
    memset(input + INBUF_SIZE_NEW, 0, AV_INPUT_BUFFER_PADDING_SIZE);
    avCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!avCodec) {
        LOGE("avcodec_find_decoder  open failed");
        goto end;
    }
    LOGE("avcodec_find_decoder  open sucess");
    avCodecParserContext = av_parser_init(avCodec->id);
    if (!avCodecParserContext) {
        LOGE("avCodecParserContext  init failed");
        goto end;
    }
    LOGE("av_parser_init  open sucess");
    avCodecContext = avcodec_alloc_context3(avCodec);
//    av_find_best_stream(avFormatContext,AVMEDIA_TYPE_VIDEO,)

    if (!avCodecContext) {
        LOGE("avcodec_alloc_context3 failed");
        goto end;
    }
    LOGE("avcodec_alloc_context3  open sucess");
    result = avcodec_open2(avCodecContext, avCodec, nullptr);
    if (result < 0) {
        LOGE("avcodec_open2 failed");
        goto end;
    }
    LOGE("avcodec_open2  open sucess");
    file = fopen(src_file, "rbe");
    if (!file) {
        LOGE("open file failed, error is %s:", strerror(errno));
    }

    LOGE("fopen  open sucess");
    avFrame = av_frame_alloc();
    if (!avFrame) {
        LOGE("avframe alloc failed");
        goto end;
    }
    LOGE("av_frame_alloc  open sucess");
    while (!feof(file)) {
        data_size = fread(input, 1, INBUF_SIZE_NEW, file);
        if (!data_size) {
            break;
        }
        LOGE("fread  sucess :%d ", data_size);
        data = input;
        while (data_size > 0) {
            result = av_parser_parse2(avCodecParserContext, avCodecContext, &avPacket->data,
                                      &avPacket->size, data, data_size, AV_NOPTS_VALUE,
                                      AV_NOPTS_VALUE, 0);
            LOGE("av_parser_parse2  open sucess");
            if (result < 0) {
                LOGE("av_parser_parse2 failed");
                goto end;;
            }
            data += result;
            data_size -= result;

            if (avPacket->size) {
                decode(avCodecContext, avFrame, avPacket, des_file);
            }
            LOGE("decode   sucess");
        }
    }
    decode(avCodecContext, avFrame, nullptr, des_file);

    end:
    env->ReleaseStringUTFChars(file_path, src_file);
    env->ReleaseStringUTFChars(dst_file_path, des_file);
    if (avCodecContext) {
        avcodec_free_context(&avCodecContext);
    }
    if (avCodecParserContext) {
        av_parser_close(avCodecParserContext);
    }
    if (!file) {
        fclose(file);
    }
}

static void decode(AVCodecContext *avCodecContext, AVFrame *avFrame, AVPacket *pkt,
                   const char *filename) {
    char buf[1024];
    int ret;

    //将packet发送给codec
    ret = avcodec_send_packet(avCodecContext, pkt);
    if (ret < 0) {
        __android_log_print(ANDROID_LOG_ERROR, TAG_ZHF, "Error sending a packet for decoding: %s\n",
                            av_err2str(ret));
        return;
    }

    while (ret >= 0) {
        //解码出frame并存入avFrame参数
        ret = avcodec_receive_frame(avCodecContext, avFrame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            return;
        } else if (ret < 0) {
            __android_log_write(ANDROID_LOG_ERROR, TAG_ZHF, "Error during decoding\n");
            return;
        }

        //为防止文件太多观察不便，每20个avFrame中抽取一个并保存为文件
        if (avCodecContext->frame_number % 20 == 0) {
            __android_log_print(ANDROID_LOG_ERROR, TAG_ZHF, "saving avFrame %3d\n",
                                avCodecContext->frame_number);

            /* the picture is allocated by the decoder. no need to
               free it */
            //拼接文件名
            //C库函数:int snprintf(char *str, size_t size, const char *format, ...),将可变参数(...)按照format格式化成字符串，
            //并将字符串复制到str中，size为要写入的字符的最大数目，超过size会被截断。
            snprintf(buf, sizeof(buf), "%s-%d.yuv", filename, avCodecContext->frame_number);
            yuv_save(avFrame, buf);
        }
    }
}

//将avFrame保存为yuv文件
static void yuv_save(AVFrame *avFrame, char *filename) {
    FILE *file;

    file = fopen(filename, "we");
    if (!file) {
        __android_log_write(ANDROID_LOG_ERROR, TAG_ZHF, "Could not open out file\n");
        __android_log_write(ANDROID_LOG_ERROR, TAG_ZHF, strerror(errno));
        return;
    }

    //这段代码的原理参考《YUV与FFmpeg.md》
    int width = avFrame->width;
    int height = avFrame->height;
    for (int i = 0; i < height; i++)
        fwrite(avFrame->data[0] + i * avFrame->linesize[0], 1, width, file);
    for (int j = 0; j < height / 2; j++)
        fwrite(avFrame->data[1] + j * avFrame->linesize[1], 1, width / 2, file);
    for (int k = 0; k < height / 2; k++)
        fwrite(avFrame->data[2] + k * avFrame->linesize[2], 1, width / 2, file);

    fclose(file);
}
