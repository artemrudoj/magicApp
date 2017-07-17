package com.mipt.artem.magicappcamera;

import android.graphics.SurfaceTexture;
import android.opengl.GLSurfaceView;

import com.mipt.artem.magicappcamera.OnSurfaceTextureReadyListener;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class CameraRenderer implements GLSurfaceView.Renderer {
    private final float[] mSTMatrix = new float[16];
    private final OnSurfaceTextureReadyListener onSurfaceTextureReadyListener;
    private SurfaceTexture mSurfaceTexture;

    static {
        System.loadLibrary("native-lib");
    }

    public CameraRenderer(OnSurfaceTextureReadyListener onSurfaceTextureReadyListener) {
        this.onSurfaceTextureReadyListener = onSurfaceTextureReadyListener;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        init();
        onSurfaceTextureReadyListener.onSurfaceTextureReady(createSurfaceTexture());
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        //empty
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        mSurfaceTexture.updateTexImage();
        mSurfaceTexture.getTransformMatrix(mSTMatrix);
        drawNative(mSTMatrix);
    }

    public void notifyPausing() {
        release();
        releaseSurface();
    }

    public native void release();

    private native void init();

    private native int createTextureObject();

    private native void drawNative(float[] texMatrix);

    private void releaseSurface() {
        if (mSurfaceTexture != null) {
            mSurfaceTexture.release();
            mSurfaceTexture = null;
        }
    }

    private SurfaceTexture createSurfaceTexture() {
        mSurfaceTexture = new SurfaceTexture(createTextureObject());
        return mSurfaceTexture;
    }
}
