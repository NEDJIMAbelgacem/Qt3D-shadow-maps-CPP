#include "shadowmapframegraph.h"

Qt3DRender::QFrameGraphNode *ShadowMapFrameGraph::constructPreviewSubTree() {
  // Frame graph subtree:
  // layerFilter
  //   viewport
  //     debugOverlay
  Qt3DRender::QLayerFilter *layerFilter = new Qt3DRender::QLayerFilter;
  layerFilter->setObjectName("preview");
  Qt3DRender::QViewport *viewport = new Qt3DRender::QViewport(layerFilter);
  Qt3DRender::QDebugOverlay *debugOverlay = new Qt3DRender::QDebugOverlay(viewport);

  layerFilter->addLayer(mPreviewLayer);
  layerFilter->setFilterMode(Qt3DRender::QLayerFilter::FilterMode::AcceptAnyMatchingLayers);

  viewport->setNormalizedRect(QRectF(0.7f, 0.0f, 0.3f, 0.3f));

  return layerFilter;
}

Qt3DRender::QFrameGraphNode *ShadowMapFrameGraph::constructPostProcessSubTree() {

  Qt3DRender::QLayerFilter *layerFilter = new Qt3DRender::QLayerFilter;
  layerFilter->setObjectName("postprocess");
  Qt3DRender::QRenderPassFilter *renderPassFilter = new Qt3DRender::QRenderPassFilter(layerFilter);
  Qt3DRender::QClearBuffers *clearBuffers = new Qt3DRender::QClearBuffers(renderPassFilter);
  layerFilter->addLayer(mPostprocessingLayer);
  layerFilter->setFilterMode(Qt3DRender::QLayerFilter::FilterMode::AcceptAnyMatchingLayers);

  clearBuffers->setClearColor(QColor::fromRgbF(0.0f, 0.0f, 0.0f));
  clearBuffers->setBuffers(Qt3DRender::QClearBuffers::ColorDepthBuffer);

  Qt3DRender::QCameraSelector *lightCameraSelector = new Qt3DRender::QCameraSelector(clearBuffers);
  lightCameraSelector->setObjectName("select light camera");
  lightCameraSelector->setCamera(mLightCamera);

  return layerFilter;
}

Qt3DRender::QFrameGraphNode *ShadowMapFrameGraph::constructShadowRenderPass() {
  Qt3DRender::QRenderPassFilter *shadowRenderPassFilter = new Qt3DRender::QRenderPassFilter();
  shadowRenderPassFilter->setObjectName("shadow pass");

  Qt3DRender::QRenderTargetSelector *shadowRenderTargetSelector = new Qt3DRender::QRenderTargetSelector(shadowRenderPassFilter);
  shadowRenderTargetSelector->setObjectName("shadow target selector");
  shadowRenderTargetSelector->setTarget(mShadowRenderTarget);

  Qt3DRender::QClearBuffers *shadowClearDepthBuffer = new Qt3DRender::QClearBuffers(shadowRenderTargetSelector);
  shadowClearDepthBuffer ->setBuffers( Qt3DRender::QClearBuffers::BufferType::DepthBuffer );

  Qt3DRender::QCameraSelector *lightCameraSelector = new Qt3DRender::QCameraSelector(shadowClearDepthBuffer);
  lightCameraSelector->setCamera( mLightCamera );

  return shadowRenderPassFilter;
}

Qt3DRender::QFrameGraphNode *ShadowMapFrameGraph::constructForwardRenderPass() {
  Qt3DRender::QRenderPassFilter *forwardRenderPassFilter = new Qt3DRender::QRenderPassFilter;
  forwardRenderPassFilter->setObjectName("forward pass");

  Qt3DRender::QRenderTargetSelector *forwardRenderingTargetSelector = new Qt3DRender::QRenderTargetSelector(forwardRenderPassFilter);
  forwardRenderingTargetSelector->setObjectName("forward target select");
  forwardRenderingTargetSelector->setTarget(mForwardRenderTarget);

  Qt3DRender::QClearBuffers *forwardClearBuffers = new Qt3DRender::QClearBuffers(forwardRenderingTargetSelector);
  forwardClearBuffers->setClearColor( QColor::fromRgbF(0.0, 0.0, 0.0, 1.0) );
  forwardClearBuffers->setBuffers(Qt3DRender::QClearBuffers::ColorDepthBuffer);

  return forwardRenderPassFilter;
}

ShadowMapFrameGraph::ShadowMapFrameGraph(QWindow *window, Qt3DRender::QCamera *viewCamera, ShadowMapLight *light)
{
  mLight = light;
  mViewCamera = viewCamera;
  mLightCamera = light->lightCamera();

  mRenderSettings = new Qt3DRender::QRenderSettings;
//  mRenderSettings->setRenderPolicy(Qt3DRender::QRenderSettings::OnDemand);

  mShadowDepthTexture = new Qt3DRender::QTexture2D;
  mShadowDepthTexture->setWidth(1024);
  mShadowDepthTexture->setHeight(1024);
  mShadowDepthTexture->setFormat( Qt3DRender::QTexture2D::TextureFormat::DepthFormat );
  mShadowDepthTexture->setGenerateMipMaps( false );
  mShadowDepthTexture->setMagnificationFilter( Qt3DRender::QTexture2D::Linear );
  mShadowDepthTexture->setMinificationFilter( Qt3DRender::QTexture2D::Linear );
  mShadowDepthTexture->wrapMode()->setX( Qt3DRender::QTextureWrapMode::ClampToEdge );
  mShadowDepthTexture->wrapMode()->setY( Qt3DRender::QTextureWrapMode::ClampToEdge );
  mShadowDepthTexture->setComparisonFunction( Qt3DRender::QTexture2D::ComparisonFunction::CompareLessEqual );
  mShadowDepthTexture->setComparisonMode( Qt3DRender::QTexture2D::ComparisonMode::CompareRefToTexture );

  mViewColorTexture = new Qt3DRender::QTexture2D;
  mViewColorTexture->setWidth(1024);
  mViewColorTexture->setHeight(1024);
  mViewColorTexture->setFormat( Qt3DRender::QTexture2D::TextureFormat::RGBA16F );
  mViewColorTexture->setGenerateMipMaps( false );
  mViewColorTexture->setMagnificationFilter( Qt3DRender::QTexture2D::Linear );
  mViewColorTexture->setMinificationFilter( Qt3DRender::QTexture2D::Linear );
  mViewColorTexture->wrapMode()->setX( Qt3DRender::QTextureWrapMode::ClampToEdge );
  mViewColorTexture->wrapMode()->setY( Qt3DRender::QTextureWrapMode::ClampToEdge );

  mViewDepthTexture = new Qt3DRender::QTexture2D;
  mViewDepthTexture->setWidth(1024);
  mViewDepthTexture->setHeight(1024);
  mViewDepthTexture->setFormat( Qt3DRender::QTexture2D::TextureFormat::DepthFormat );
  mViewDepthTexture->setGenerateMipMaps( false );
  mViewDepthTexture->setMagnificationFilter( Qt3DRender::QTexture2D::Linear );
  mViewDepthTexture->setMinificationFilter( Qt3DRender::QTexture2D::Linear );
  mViewDepthTexture->wrapMode()->setX( Qt3DRender::QTextureWrapMode::ClampToEdge );
  mViewDepthTexture->wrapMode()->setY( Qt3DRender::QTextureWrapMode::ClampToEdge );
  mViewDepthTexture->setComparisonFunction( Qt3DRender::QTexture2D::ComparisonFunction::CompareLessEqual );
  mViewDepthTexture->setComparisonMode( Qt3DRender::QTexture2D::ComparisonMode::CompareRefToTexture );

  mPreviewLayer = new Qt3DRender::QLayer;
  mPreviewLayer->setRecursive(true);

  mPostprocessingLayer = new Qt3DRender::QLayer;
  mPostprocessingLayer->setRecursive(true);

  mShadowRenderTarget = new Qt3DRender::QRenderTarget;
  mShadowRenderTargetOutput = new Qt3DRender::QRenderTargetOutput;
  mShadowRenderTargetOutput->setAttachmentPoint( Qt3DRender::QRenderTargetOutput::Depth );
  mShadowRenderTargetOutput->setTexture(mShadowDepthTexture);
  mShadowRenderTarget->addOutput(mShadowRenderTargetOutput);

  mForwardRenderTarget = new Qt3DRender::QRenderTarget;
  mForwardRenderTargetDepthOutput = new Qt3DRender::QRenderTargetOutput;
  mForwardRenderTargetDepthOutput->setAttachmentPoint( Qt3DRender::QRenderTargetOutput::Depth );
  mForwardRenderTargetDepthOutput->setTexture(mViewDepthTexture);
  mForwardRenderTarget->addOutput(mForwardRenderTargetDepthOutput);
  mForwardRenderTargetColorOutput = new Qt3DRender::QRenderTargetOutput;
  mForwardRenderTargetColorOutput->setAttachmentPoint( Qt3DRender::QRenderTargetOutput::Color0 );
  mForwardRenderTargetColorOutput->setTexture(mViewColorTexture);
  mForwardRenderTarget->addOutput(mForwardRenderTargetColorOutput);

// renderSurfaceSelector
//   viewport
//     viewCameraSelector
//       noQuadLayerFilter
//         shadowRenderPass
//         forwardRenderPass
//       PostprocessingNode
//       PreviewNode

  Qt3DRender::QRenderSurfaceSelector *renderSurfaceSelector = new Qt3DRender::QRenderSurfaceSelector;
  renderSurfaceSelector->setSurface(window);

  Qt3DRender::QViewport *viewport = new Qt3DRender::QViewport( renderSurfaceSelector );
  viewport->setObjectName("main viewport");
  viewport->setNormalizedRect(QRectF(0.0f, 0.0f, 1.0f, 1.0f));

  Qt3DRender::QCameraSelector *viewCameraSelector = new Qt3DRender::QCameraSelector(viewport);
  viewCameraSelector->setObjectName("select view camera");
  viewCameraSelector->setCamera(mViewCamera);

  Qt3DRender::QLayerFilter *noQuadLayerFilter = new Qt3DRender::QLayerFilter(viewCameraSelector);
  noQuadLayerFilter->setObjectName("exclude preview & postprocess quads");
  noQuadLayerFilter->addLayer(mPreviewLayer);
  noQuadLayerFilter->addLayer(mPostprocessingLayer);
  noQuadLayerFilter->setFilterMode(Qt3DRender::QLayerFilter::FilterMode::DiscardAnyMatchingLayers);

  Qt3DRender::QFrameGraphNode *shadowRenderPass = constructShadowRenderPass();
  shadowRenderPass->setParent(noQuadLayerFilter);

  Qt3DRender::QFrameGraphNode *forwardRenderPass = constructForwardRenderPass();
  forwardRenderPass->setParent(noQuadLayerFilter);

  Qt3DRender::QFrameGraphNode *postprocessingNode = constructPostProcessSubTree();
  postprocessingNode->setParent(viewCameraSelector);

  Qt3DRender::QFrameGraphNode *previewNode = constructPreviewSubTree();
  previewNode->setParent(viewCameraSelector);

  mRenderSettings->setActiveFrameGraph(renderSurfaceSelector);
  mFrameGraph = renderSurfaceSelector;
}
