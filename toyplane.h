#ifndef TOYPLANE_H
#define TOYPLANE_H

#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DExtras>

class ToyPlane : public Qt3DCore::QEntity
{
  public:
    ToyPlane(const QVector3D& position, Qt3DCore::QEntity *parent = nullptr);

    void setMaterial( Qt3DRender::QMaterial *material );

  private:
    Qt3DRender::QMaterial *mMaterial = nullptr;
};

#endif // TOYPLANE_H
