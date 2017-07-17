package com.mipt.artem.magicappcamera;

import android.app.Activity;
import android.content.Intent;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

import java.io.IOException;


public class CameraCaptureActivity extends AppCompatActivity implements OnSurfaceTextureReadyListener {

    private GLSurfaceView mGLView;
    private CameraRenderer mRenderer = new CameraRenderer(this);;
    private Camera mCamera;
    private SurfaceTexture.OnFrameAvailableListener frameAvailableListener = surfaceTexture -> mGLView.requestRender();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_camera_capture);
        initGlSurfaceView();
        setRenderer();
    }

    @Override
    protected void onResume() {
        super.onResume();
        openCamera();
        mGLView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        releaseCamera();
        mGLView.queueEvent(() -> mRenderer.notifyPausing());
        mGLView.onPause();
    }

    @Override
    public void onSurfaceTextureReady(SurfaceTexture surfaceTexture) {
        if(mCamera != null) {
            surfaceTexture.setOnFrameAvailableListener(frameAvailableListener);
            try {
                mCamera.setPreviewTexture(surfaceTexture);
            } catch (IOException ioe) {
                throw new IllegalStateException(ioe);
            }
            mCamera.startPreview();
        }
    }

    public static void start(Activity activity) {
        Intent intent = new Intent(activity, CameraCaptureActivity.class);
        activity.startActivity(intent);
    }

    private void initGlSurfaceView() {
        mGLView = (GLSurfaceView) findViewById(R.id.cameraPreview_surfaceView);
        mGLView.setEGLContextClientVersion(2);
    }

    protected void setRenderer() {
        mGLView.setRenderer(mRenderer);
        mGLView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
    }

    protected void openCamera() {
        if (mCamera != null) {
            throw new IllegalStateException("camera already initialized");
        }
        selectCamera();
        setCameraParams();
    }

    private void setCameraParams() {
        mCamera.setDisplayOrientation(90);
        Camera.Parameters parms = mCamera.getParameters();
        Camera.Size size = parms.getSupportedPreviewSizes().get(0);
        parms.setPreviewSize(size.width, size.height);
        mCamera.setParameters(parms);
    }

    private void selectCamera() {
        if (!tryOpenFrontalCamera()) {
            mCamera = Camera.open();
        }
        if (mCamera == null) {
            throw new IllegalStateException("Unable to open camera");
        }
    }

    private boolean tryOpenFrontalCamera() {
        Camera.CameraInfo info = new Camera.CameraInfo();
        int numCameras = Camera.getNumberOfCameras();
        for (int i = 0; i < numCameras; i++) {
            Camera.getCameraInfo(i, info);
            if (info.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
                mCamera = Camera.open(i);
                return true;
            }
        }
        return false;
    }

    private void releaseCamera() {
        if (mCamera != null) {
            mCamera.stopPreview();
            mCamera.release();
            mCamera = null;
        }
    }
}
