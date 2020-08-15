#include "toyplane.h"

ToyPlane::ToyPlane(const QVector3D& position, Qt3DCore::QEntity *parent)
: Qt3DCore::QEntity( parent )
{
  Qt3DRender::QMesh *mesh = new Qt3DRender::QMesh( this );
  mesh->setSource( QUrl( "qrc:/toyplane.obj" ) );

  Qt3DCore::QTransform *transform = new Qt3DCore::QTransform;
//  transform->setTranslation( QVector3D(10.0f, 5.0f, 10.0f) );
  transform->setTranslation( position );
  transform->setScale( 0.1f );

  addComponent(mesh);
  addComponent(transform);
}

void ToyPlane::setMaterial( Qt3DRender::QMaterial *material ) {
  mMaterial = material;
  addComponent(mMaterial);
}
