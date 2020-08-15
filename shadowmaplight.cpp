#include "shadowmaplight.h"

ShadowMapLight::ShadowMapLight(const QVector3D &position, Qt3DCore::QEntity *parent)
: Qt3DCore::QEntity( parent )
{
  mLightCamera = new Qt3DRender::QCamera(this);
  mLightCamera->setObjectName("lightCameraLens");
//  mLightCamera->setProjectionType( Qt3DRender::QCameraLens::ProjectionType::OrthographicProjection);
//  mLightCamera->lens()->setOrthographicProjection(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.f);
//  mLightCamera->setPosition(position);
//  mLightCamera->setViewCenter(QVector3D(0.0f, 0.0f, 0.0f));
//  mLightCamera->viewAll();
//  mLightCamera->setUpVector(QVector3D(0.0f, 1.0f, 0.0f));


  mLightCamera->lens()->setPerspectiveProjection(45.0f, 1.0f, 0.1f, 100.0f);
  mLightCamera->setPosition(QVector3D(position));
  mLightCamera->setViewCenter(QVector3D(0.0f, -10.0f, 0.0f));
  mLightCamera->setUpVector(QVector3D(0.0f, 1.0f, 0.0f));
  mLightCamera->setProjectionType( Qt3DRender::QCameraLens::ProjectionType::PerspectiveProjection );

  Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight;
  mTransform= new Qt3DCore::QTransform;
  mTransform->setTranslation(position);
  addComponent(light);
  addComponent(mTransform);
}
