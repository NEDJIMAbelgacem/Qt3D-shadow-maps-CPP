#include "trefoil.h"

Trefoil::Trefoil(const QVector3D &position, Qt3DCore::QEntity *parent)
: Qt3DCore::QEntity(parent)
{
  Qt3DRender::QMesh *mesh = new Qt3DRender::QMesh( this );
  mesh->setSource( QUrl( "qrc:/trefoil.obj" ) );

  Qt3DCore::QTransform *transform = new Qt3DCore::QTransform;
//  transform->setTranslation(QVector3D(0.0f, 5.0f, 0.0f));
  transform->setTranslation(position);

  addComponent(mesh);
  addComponent(transform);
}

void Trefoil::setMaterial( Qt3DRender::QMaterial *material ) {
  mMaterial = material;
  addComponent(mMaterial);
}
