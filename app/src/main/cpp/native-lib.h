//
// Created by Artem on 17.07.17.
//

#ifndef MAGICAPPCAMERA_NATIVE_LIB_H
#define MAGICAPPCAMERA_NATIVE_LIB_H

#include <jni.h>
#include <string>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include "gl_helper.h"

extern "C" {
JNIEXPORT jint JNICALL
Java_com_mipt_artem_magicappcamera_gles_CameraRenderer_createTextureObject(JNIEnv *env, jobject instance);
};

#endif //MAGICAPPCAMERA_NATIVE_LIB_H
