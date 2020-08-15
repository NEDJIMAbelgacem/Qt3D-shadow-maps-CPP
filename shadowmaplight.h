#ifndef SHADOWMAPLIGHT_H
#define SHADOWMAPLIGHT_H

#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DExtras>

class ShadowMapLight : public Qt3DCore::QEntity
{
  public:
    ShadowMapLight(const QVector3D &position, Qt3DCore::QEntity *parent = nullptr);

    Qt3DRender::QCamera *lightCamera() { return mLightCamera; }

    QMatrix4x4 lightViewMatrix()
    {
      return mTransform->matrix() * mLightCamera->viewMatrix();
//      QMatrix4x4 m;
//      m.setRow(0, QVector4D(2.0f / 40.0f, 0.0f, 0.0f, 0.0f ));
//      m.setRow(1, QVector4D(0.0f, 2.0f / 40.0f, 0.0f, 0.0f ));
//      m.setRow(2, QVector4D(0.0f, 0.0f, -2.0f / (100.0f - 0.1f), - (100.0f + 0.1f) / (100.0f - 0.1f) ));
//      m.setRow(3, QVector4D(0.0f, 0.0f, 0.0f, 1.0f));
//      return m;
//      return  mLightCamera->viewMatrix() * mTransform->matrix();
//      return mLightCamera->viewMatrix();
//      return mTransform->matrix();
    }

    QMatrix4x4 lightProjectionMatrix()
    {
      return mLightCamera->projectionMatrix();
    }

  private:
    Qt3DRender::QCamera *mLightCamera = nullptr;
    Qt3DCore::QTransform *mTransform = nullptr;
};

#endif // SHADOWMAPLIGHT_H
