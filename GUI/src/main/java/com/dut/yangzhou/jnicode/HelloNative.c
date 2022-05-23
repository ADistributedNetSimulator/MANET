#include "com_dut_yangzhou_jnicode_HelloNative.h"
#include <stdio.h>

JNIEXPORT jint JNICALL Java_HelloNative_greeting(JNIEnv * env, jobject obj, jint a, jint b){

    printf("Hello Native !\n");

    return a + b;
}