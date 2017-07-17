#include <jni.h>
#include <string>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include "gl_helper.h"

auto gVertexShader =
        "uniform mat4 uMVPMatrix;\n"
        "uniform mat4 uTexMatrix;\n"
        "attribute vec4 aPosition;\n"
        "attribute vec4 aTextureCoord;\n"
        "varying vec2 vTextureCoord;\n"
        "void main() {\n"
        "    gl_Position = uMVPMatrix * aPosition;\n"
        "    vTextureCoord = (uTexMatrix * aTextureCoord).xy;\n"
        "}\n";

auto gFragmentShader =
        "#extension GL_OES_EGL_image_external : require\n"
        "precision mediump float;\n"
        "varying vec2 vTextureCoord;\n"
        "uniform samplerExternalOES sTexture;\n"
        "void main() {\n"
        "    vec4 tc = texture2D(sTexture, vTextureCoord);\n"
        "    float color = tc.r * 0.4 + tc.g * 0.49 + tc.b * 0.11;\n"
        "    gl_FragColor = vec4(color, color, color, 1.0);\n"
        "}\n";

float FULL_RECTANGLE_COORDS[] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f,  1.0f,
        1.0f,  1.0f,
};

float IDENTITY_MATRIX[] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
};
float FULL_RECTANGLE_TEX_COORDS[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f
};

int coordsPerVertex = 2;
int vertexStride = coordsPerVertex * sizeof(GLfloat);
int vertexCount = 4;
int texCoordStride = 8;
int programHandle;
int textureId;
int uMVPMatrixLoc;
int uTexMatrixLoc;
int aPositionLoc;
int aTextureCoordLoc;

extern "C" {
    JNIEXPORT jint JNICALL
    Java_com_mipt_artem_magicappcamera_CameraRenderer_createTextureObject(JNIEnv *env, jobject instance) {
            GLuint textures;
            glGenTextures(1, &textures);
            glBindTexture(GL_TEXTURE_EXTERNAL_OES, textures);
            textureId = textures;
            return textures;

        }

    JNIEXPORT void JNICALL
    Java_com_mipt_artem_magicappcamera_CameraRenderer_init(JNIEnv *env, jobject instance) {
        programHandle = createProgram(gVertexShader, gFragmentShader);
        aPositionLoc = glGetAttribLocation(programHandle, "aPosition");
        aTextureCoordLoc = glGetAttribLocation(programHandle, "aTextureCoord");
        uMVPMatrixLoc = glGetUniformLocation(programHandle, "uMVPMatrix");
        uTexMatrixLoc = glGetUniformLocation(programHandle, "uTexMatrix");
    }

    JNIEXPORT void JNICALL
    Java_com_mipt_artem_magicappcamera_CameraRenderer_release(JNIEnv *env, jobject instance) {
        glDeleteProgram(programHandle);
    }

    JNIEXPORT void JNICALL
    Java_com_mipt_artem_magicappcamera_CameraRenderer_drawNative(JNIEnv *env, jobject instance,
                                                                        jfloatArray texMatrix_) {
        jfloat *texMatrix = env->GetFloatArrayElements(texMatrix_, NULL);

        glUseProgram(programHandle);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_EXTERNAL_OES, textureId);
        glUniformMatrix4fv(uMVPMatrixLoc, 1, false, IDENTITY_MATRIX);
        glUniformMatrix4fv(uTexMatrixLoc, 1, false, texMatrix);
        glEnableVertexAttribArray(aPositionLoc);
        glVertexAttribPointer(aPositionLoc, coordsPerVertex, GL_FLOAT, false, vertexStride, FULL_RECTANGLE_COORDS);
        glEnableVertexAttribArray(aTextureCoordLoc);
        glVertexAttribPointer(aTextureCoordLoc, 2, GL_FLOAT, false, texCoordStride, FULL_RECTANGLE_TEX_COORDS);
        glDrawArrays(GL_TRIANGLE_STRIP, 0,  vertexCount);
        glDisableVertexAttribArray(aPositionLoc);
        glDisableVertexAttribArray(aTextureCoordLoc);
        glBindTexture(GL_TEXTURE_EXTERNAL_OES, 0);
        glUseProgram(0);

        env->ReleaseFloatArrayElements(texMatrix_, texMatrix, 0);
    }
}
