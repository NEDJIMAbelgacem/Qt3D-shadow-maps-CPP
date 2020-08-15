#ifndef SHADOWMAPFRAMEGRAPH_H
#define SHADOWMAPFRAMEGRAPH_H

#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DExtras>

#include "shadowmaplight.h"

class ShadowMapFrameGraph
{
  public:
    ShadowMapFrameGraph(QWindow *window, Qt3DRender::QCamera *viewCamera, ShadowMapLight *light);
    Qt3DRender::QTexture2D* shadowDepthTexture() { return mShadowDepthTexture; }
    Qt3DRender::QTexture2D* viewColorTexture() { return mViewColorTexture; }
    Qt3DRender::QTexture2D* viewDepthTexture() { return mViewDepthTexture; }
    Qt3DRender::QRenderSettings *renderSettings() { return mRenderSettings; }
    Qt3DRender::QFrameGraphNode *frameGraph() { return mFrameGraph; }
    Qt3DRender::QLayer *previewLayer() { return mPreviewLayer; }
    Qt3DRender::QLayer *postprocessingLayer() { return mPostprocessingLayer; }
    Qt3DRender::QCamera *viewCamera() { return mViewCamera; }
    Qt3DRender::QCamera *lightCamera() { return mLightCamera; }
    ShadowMapLight *light() { return mLight; }
  private:
    Qt3DRender::QCamera *mViewCamera = nullptr;
    Qt3DRender::QCamera *mLightCamera = nullptr;
    Qt3DRender::QTexture2D *mShadowDepthTexture = nullptr;
    Qt3DRender::QTexture2D *mViewColorTexture = nullptr;
    Qt3DRender::QTexture2D *mViewDepthTexture = nullptr;
    Qt3DRender::QRenderSettings *mRenderSettings = nullptr;
    Qt3DRender::QFrameGraphNode* mFrameGraph = nullptr;
    Qt3DRender::QLayer *mNoQuadRender = nullptr;
    Qt3DRender::QLayer *mPreviewLayer = nullptr;
    Qt3DRender::QLayer *mPostprocessingLayer = nullptr;
    ShadowMapLight* mLight = nullptr;
    Qt3DRender::QRenderTarget *mShadowRenderTarget = nullptr;
    Qt3DRender::QRenderTargetOutput *mShadowRenderTargetOutput = nullptr;
    Qt3DRender::QRenderTarget *mForwardRenderTarget = nullptr;
    Qt3DRender::QRenderTargetOutput *mForwardRenderTargetColorOutput = nullptr;
    Qt3DRender::QRenderTargetOutput *mForwardRenderTargetDepthOutput = nullptr;

    Qt3DRender::QFrameGraphNode *constructPreviewSubTree();
    Qt3DRender::QFrameGraphNode *constructPostProcessSubTree();
    Qt3DRender::QFrameGraphNode *constructShadowRenderPass();
    Qt3DRender::QFrameGraphNode *constructForwardRenderPass();
};

#endif // SHADOWMAPFRAMEGRAPH_H
