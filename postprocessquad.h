#ifndef POSTPROCESSQUAD_H
#define POSTPROCESSQUAD_H

#include <Qt3DRender>
#include <Qt3DCore>

#include "shadowmapframegraph.h"

class PostprocessQuadMaterial
{
  public:
    PostprocessQuadMaterial(ShadowMapFrameGraph *frameGraph, const QString& vertexShaderPath, const QString& fragmentShaderPath);
    Qt3DRender::QMaterial *material() { return mMaterial; }
  private:
    Qt3DRender::QMaterial *mMaterial = nullptr;
    Qt3DRender::QParameter *mColorTexturePrameter = nullptr;
    Qt3DRender::QParameter *mDepthTexturePrameter = nullptr;
    Qt3DRender::QParameter *mShadowTextureParameter = nullptr;
    Qt3DRender::QParameter *lightProjParameter = nullptr;
    Qt3DRender::QParameter *cameraProjParameter = nullptr;
    Qt3DRender::QParameter *lightViewParameter = nullptr;
    Qt3DRender::QParameter *cameraViewParameter = nullptr;
    Qt3DRender::QCamera *mViewCamera = nullptr;
    Qt3DRender::QCamera *mLightCamera = nullptr;
};

class PostprocessQuad : public Qt3DCore::QEntity
{
  public:
    PostprocessQuad(ShadowMapFrameGraph *frameGraph, const QString& vertexShaderPath, const QString& fragmentShaderPath, Qt3DCore::QEntity *parent = nullptr);
  private:
    PostprocessQuadMaterial *materialContainer = nullptr;
};

#endif // POSTPROCESSQUAD_H
