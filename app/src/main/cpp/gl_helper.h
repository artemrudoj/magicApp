//
// Created by Artem on 15.07.17.
//

#ifndef MAGICAPPCAMERA_GL_HELPER_H
#define MAGICAPPCAMERA_GL_HELPER_H

#include <EGL/egl.h> // requires ndk r5 or newer
#include <GLES2/gl2.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <android/log.h>
#include <stdlib.h>

#define  LOG_TAG    "native-lib"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

GLuint loadShader(GLenum shaderType, const char* pSource);
void checkGlError(const char* op);
GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);



#endif //MAGICAPPCAMERA_GL_HELPER_H
